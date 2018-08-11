#include "utils.h"

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
				double term = -2 * PI * (k + 1) * (n + 1) / frame.col;
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

float HammingWindow(float a, int frameLength)
{
	return 0.54 - 0.46 * cos(2 * PI * a / (frameLength - 1));
}

int getLength(SAMPLE * a)
{
	return sizeof(a) / sizeof(a[0]);
}

SIGNAL setSignal(SAMPLE * a,int size)
{
	SIGNAL temp;
	temp.raw_signal = a;
	temp.frame_length = SAMPLE_RATE*0.025;
	temp.step_lengh = SAMPLE_RATE*0.01;
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

	for (int i = 0; i < a.num_frame; i++){
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

	
	return setHVector(frames,frame_len,a.num_frame,frame_len*a.num_frame);
}

void error(char *err)
{
	fprintf(stderr, "error : ", err);
	exit(0);
}

int find_arg(int argc, char* argv[], char *arg)
{
	int i;
	for (i = 0; i < argc; ++i) {
		if (!argv[i]) continue;
		if (0 == strcmp(argv[i], arg)) {
			return 1;
		}
	}
	return 0;
}
