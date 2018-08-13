#include "utils.h"

float mel2hz(float mel)
{
	return (700 * (exp(mel / 1125) - 1));
}

float hz2mel(float hz)
{
	return (2595 * log10(1 + hz / 700));
}

COMPLEX * DFT(hyper_vector frame, int pointFFT)
{
	float temp, real = 0, img = 0;

	COMPLEX *fft = malloc(sizeof(COMPLEX) * frame.row * (pointFFT / 2 + 1));

	system("cls");
	for (int i = 0; i < frame.row; i++) {
		for (int k = 0; k < pointFFT / 2 + 1; k++)
		{
			for (int n = 0; n < frame.col; n++)
			{
				float term = -2 * PI * (k + 1) * (n + 1) / frame.col;
				temp = frame.data[i * frame.col + n] * HammingWindow(n, frame.col);
				real += temp * cos(term);
				img += temp * sin(term);
			}
			fft[i* pointFFT / 2 + k].real = real;
			fft[i* pointFFT / 2 + k].img = img;

			real = img = 0;
		}

	}
	//de-allocate
	free(frame.data);

	return fft;
}

float magnitude(float real, float img)
{
	return sqrt(real*real + img * img);
}

filter_bank filterbank(int nfilt, int NFFT)
{
	int lowfreq_mel = 0;                    //cận dưới thang Mel
	float highfreq_mel = (float)hz2mel(SAMPLE_RATE / 2);   //Cận trên
	float *melpoint = (float*)malloc(sizeof(float)*nfilt + 2);
	float *hzpoint = (float*)malloc(sizeof(float)*nfilt + 2);
	float *bin = (float*)malloc(sizeof(float)*nfilt + 2);           //FFT bins được tính theo công thức (NFFT + 1) * hzpoints / SAMPLE_RATE

	float step = (highfreq_mel - lowfreq_mel) / (nfilt + 2);       //bước chuyển tuyến tính thang Mel
	for (int i = 0; i < nfilt + 2; i++)
	{
		if (i == 0)
		{
			melpoint[i] = lowfreq_mel;
			hzpoint[i] = (float)mel2hz(melpoint[i]);
			bin[i] = floor(((NFFT + 1) * (float)hzpoint[i]) / SAMPLE_RATE);
			continue;
		}
		if (i == nfilt + 1)
		{
			melpoint[i] = highfreq_mel;
			hzpoint[i] = SAMPLE_RATE / 2;
			bin[i] = floor(((NFFT + 1) * (float)hzpoint[i]) / SAMPLE_RATE);
			break;
		}
		melpoint[i] = melpoint[i - 1] + step;
		hzpoint[i] = (float)mel2hz(melpoint[i]);
		bin[i] = floor(((NFFT + 1) * (float)hzpoint[i]) / SAMPLE_RATE);
	}

	int a = (int)floor((1.0 * NFFT / 2) + 1);
	float *fbank = (float*)calloc(nfilt* a, sizeof(float));       //26 filter, moi filter chi co 1 diem co gia tri bang 1 (chinh la tan so dc chia theo thang tuyen tinh)           


																  //tính filterbanks theo công thức.
	for (int m = 1; m < nfilt + 1; m++)
	{
		int f_m_minus_1 = (int)bin[m - 1];      //điểm bắt đầu
		int f_m = (int)bin[m];                  //đạt cực trị
		int f_m_plus_1 = (int)bin[m + 1];       //kết thúc của 1 filter
		for (int k = f_m_minus_1; k < f_m; k++)     //len dan cho den khi dat cuc dai fbank (0 - 1)
		{
			fbank[m - 1 * a + k] = (k - bin[m - 1]) / (bin[m] - bin[m - 1]);     //chia nguyen -> chi bang 1 tai diem f_m (1 so tan so xac dinh)
		}
		for (int k = f_m; k < f_m_plus_1; k++)      //chu y de toi uu
		{
			fbank[m - 1 * a + k] = (bin[m + 1] - k) / (bin[m + 1] - bin[m]);
		}
	}

	return getFBank(fbank,nfilt,a);
}

float HammingWindow(float a, int frameLength)
{
	return 0.54 - 0.46 * cos(2 * PI * a / (frameLength - 1));
}

int getLength(SAMPLE * a)
{
	return sizeof(a) / sizeof(a[0]);
}

filter_bank getFBank(float *fbank, int nfilt, int filt_len) {
	filter_bank temp;
	temp.data = fbank;
	temp.nfilt = nfilt;
	temp.filt_len = filt_len;
	return temp;
}


SIGNAL setSignal(SAMPLE * a, int size)
{
	SIGNAL temp;
	temp.raw_signal = a;
	temp.frame_length = SAMPLE_RATE * 0.025;
	temp.step_lengh = SAMPLE_RATE * 0.01;
	temp.signal_length = size;
	return temp;
}

hyper_vector setHVector(SAMPLE * a, int col, int row, int dim)
{
	hyper_vector temp_vector;
	temp_vector.col = col;
	temp_vector.row = row;
	temp_vector.dim = dim;
	temp_vector.data = a;
	return temp_vector;
}

hyper_vector getFrames(SIGNAL a)
{
	int signal_len = a.signal_length;
	int frame_len = a.frame_length;
	int frame_step = a.step_lengh;

	if (signal_len <= frame_len)        //số mẫu toàn tín hiệu nhỏ hơn độ rộng khung
		a.num_frame = 1;
	else
	{
		float num_additional_frames = (float)(signal_len - frame_len) / frame_step;   //so frame khong tinh frame cuoi
		num_additional_frames = (ceil(num_additional_frames));
		a.num_frame = 1 + (int)num_additional_frames;                             //numframes (frame cuoi day du hoac khong)
	}

	int padsignal_len = (a.num_frame - 1) * frame_step + frame_len;      //do dai chuoi tin hieu neu cac frame day du
	int zeros = padsignal_len - signal_len;                //Do dai chuoi Zeros can pad.

	realloc(a.raw_signal, padsignal_len);
	for (int i = signal_len; i < signal_len + zeros; i++)         //chen them 0 vao frame cuoi.
	{
		a.raw_signal[i] = 0;
	}


	// thuc hien chia frame (0:0->framelen, 1:framestep->(framelen + framestep),...
	int index = 0;
	int dem1 = 0, dem2 = 0;
	int temp = frame_step;

	SAMPLE *frames = (SAMPLE*)malloc(sizeof(SAMPLE) * a.num_frame * a.frame_length);

	for (int i = 0; i < a.num_frame; i++) {
		for (int j = 0; j < frame_len; j++)
			frames[i * frame_len + j] = 0;
	}
	/*system("cls");
	float temp12;*/
	while (index < a.num_frame)
	{
		if (index == 0)                 //frame dau tien
			for (int i = 0; i < frame_len; i++)
			{
				frames[index * frame_len + i] = a.raw_signal[i];
				/*printf("%f  ", frames[index * frame_len + i]);*/
			}
		else                          //cac frames con lai, framestep->(framelen + framestep)...
		{
			for (int i = temp; i < temp + frame_len; i++)
			{

				frames[index * frame_len + dem1++] = a.raw_signal[i];
				/*printf("%f  ", temp12);*/
			}
			temp += frame_step;
			dem1 = 0;
		}
		index++;
	}


	return setHVector(frames, frame_len, a.num_frame, frame_len*a.num_frame);
}

void error(char *err)
{
	fprintf(stderr, "error : ", err);
	exit(0);
}