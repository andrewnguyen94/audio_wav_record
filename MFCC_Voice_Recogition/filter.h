#ifndef FILTER_H

/* include -------------------------------------
------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "utils.h"

/* define, constant ----------------------------------------------
------------------------------------------------------------------*/

/*functions
-------------------------------------------------
-------------------------------------------------*/

float *butterworth_bandpass(int order, float *signal, int size, int sample_rate, float high_freq_cutoff, float low_freq_cutoff);
float *butterworth_lowpass(int order, float *signal, int size, int sample_rate, float cutoff_freq);

#endif // !FILTER_H