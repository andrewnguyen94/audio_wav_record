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
struct SIGNAL {
	SAMPLE *raw_signal;
	const float frame_length;		//so sample trong 1 frame
	const float step_lengh;		//do dai bc nhay
	const int num_frame;			//so frame trong 1 audio signal
};

struct COMPLEX {
	float img;
	float real;
	float magnitude;	
};

#define PI 3.14159265359

/*functions
------------------------------------------
------------------------------------------*/
struct COMPLEX ** DFT(SAMPLE *signal, int num_frame, int frame_length, int pointFFT);
float HammingWindow(float a, int frameLength);
void error(char *err);

#endif