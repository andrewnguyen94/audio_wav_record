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

void error(char *err)
{
	fprintf(stderr, "error : ", err);
	exit(0);
}
