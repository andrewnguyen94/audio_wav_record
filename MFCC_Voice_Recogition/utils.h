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

struct SIGNAL {
	SAMPLE *raw_signal;
	int frame_length;		//so sample trong 1 frame
	int step_lengh;		//do dai bc nhay
	int num_frame;			//so frame trong 1 audio signal
	int signal_length;
};

struct COMPLEX {
	float img;
	float real;
	float magnitude;	
};

/*functions
------------------------------------------
------------------------------------------*/
int getLength(SAMPLE *a);
struct SIGNAL setSignal(SAMPLE *a,int size);
SAMPLE **getFrames(struct SIGNAL a);
struct COMPLEX ** DFT(SAMPLE **signal, int num_frame, int frame_length, int pointFFT);
float HammingWindow(float a, int frameLength);
void error(char *err);
#endif