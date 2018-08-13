#ifndef UTILS_H
#define UTILS_H

/*includes
------------------------------------------
------------------------------------------*/
#include "record.h"
#include <math.h>

/*defines, constants
------------------------------------------
------------------------------------------*/

#define PI 3.14159265359

typedef struct SIGNAL {
	SAMPLE *raw_signal;
	int frame_length;		//so sample trong 1 frame
	int step_lengh;		//do dai bc nhay
	int num_frame;			//so frame trong 1 audio signal
	int signal_length;
}SIGNAL;

typedef struct COMPLEX {
	float img;
	float real;
	float magnitude;
}COMPLEX;

typedef struct hyper_vector {
	int row;
	int col;
	int dim;
	SAMPLE *data;
}hyper_vector;

typedef struct filter_bank {
	int data;
	int nfilt;
	int filt_len;
}filter_bank;
/*functions
------------------------------------------
------------------------------------------*/

filter_bank getFBank(float *fbank, int nfilt, int filt_len);
int getLength(SAMPLE *a);
SIGNAL setSignal(SAMPLE *a, int size);
hyper_vector setHVector(SAMPLE *a, int col, int row, int dim);

hyper_vector getFrames(struct SIGNAL a);
COMPLEX *DFT(hyper_vector a, int pointFFT);
float magnitude(float real, float img);
filter_bank filterbank(int nfilt, int NFFT);
float HammingWindow(float a, int frameLength);
void error(char *err);

float hz2mel(float hz);
float mel2hz(float hz);

#endif