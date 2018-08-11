#include <stdio.h>
#include "record.h"
#include "utils.h"

int main(int argc, char **argv)
{
	int size;
	SAMPLE *audio_signal = get_audio_signal_from_source(&size);

	SIGNAL a = setSignal(audio_signal,size);
	

	hyper_vector frames = getFrames(a);

	COMPLEX *fft = DFT(frames, 512);

	for (int i = 0; i < frames.row; i++) {
		for (int j = 0; j < 257; j++)
			printf("%f + i * %f   ", fft[i * 257 + j].real, fft[i * 257 + j].img);
	}
	
	getch();
	return 1;

}