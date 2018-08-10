#include "utils.h"

struct COMPLEX ** DFT(SAMPLE **frames,int num_frame, int frame_length, int pointFFT)
{
	float temp, real = 0, img = 0;
	
	struct COMPLEX ** fft = malloc(sizeof(struct COMPLEX*) * num_frame);
	for (int i = 0; i < pointFFT / 2 + 1; i++)
		fft[i] = malloc(sizeof(struct COMPLEX*)*(pointFFT / 2 + 1));


	for (int i = 0; i < num_frame; i++) {
		for (int k = 0; k < pointFFT / 2 + 1; k++)
		{
			for (int n = 0; n < frame_length; n++)
			{
				double term = -2 * PI * (k + 1) * (n + 1) / frame_length;
				temp = frames[i][n] * HammingWindow(n, frame_length);
				real += temp * cos(term);
				img += temp * sin(term);
			}
			fft[i][k].real = real;
			fft[i][k].img = img;
			real = img = 0;
		}

		//de-allocate
		for (int i = 0; i < frame_length; i++)
			free(frames[i]);
		free(frames);
		return fft;
	}
}

float HammingWindow(float a, int frameLength)
{
	return 0.54 - 0.46 * cos(2 * PI * a / (frameLength - 1));
}

int getLength(SAMPLE * a)
{
	return sizeof(a) / sizeof(a[0]);
}

SAMPLE ** getFrames(struct SIGNAL a)
{
	int signal_len = getLength(a.raw_signal);
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
	
	realloc(a.raw_signal,zeros);
	for (int i = signal_len; i < signal_len + zeros; i++)         //chen them 0 vao frame cuoi.
	{
		a.raw_signal[i] = 0;
	}


	// thuc hien chia frame (0:0->framelen, 1:framestep->(framelen + framestep),...
	int index = 0;
	int dem1 = 0, dem2 = 0;
	int temp = frame_step;
	SAMPLE **frames = malloc(sizeof(SAMPLE*) * a.num_frame);
	for (int i = 0; i < a.frame_length + 1; i++){
		frames[i] = malloc(sizeof(struct COMPLEX*)*(a.frame_length));
		frames[i] = 0;
	}
	while (index < a.num_frame)
	{
		if (index == 0)                 //frame dau tien
			for (int i = 0; i < frame_len; i++)
			{
				frames[index][i] = a.raw_signal[i];
			}
		else                          //cac frames con lai, framestep->(framelen + framestep)...
		{
			for (int i = temp; i < temp + frame_len; i++)
			{
				frames[index][dem1++] = a.raw_signal[i];
			}
			temp += frame_step;
			dem1 = 0;
		}
		index++;
	}
	return frames;
}

void error(char *err)
{
	fprintf(stderr, "error : ", err);
	exit(0);
}
