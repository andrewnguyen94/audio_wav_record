#include <stdio.h>
#include "record.h"
#include "utils.h"

int main(int argc, char **argv)
{
	int size;
	SAMPLE *audio_signal = get_audio_signal_from_source(&size);

	struct SIGNAL a = setSignal(audio_signal,size);
	

	SAMPLE **frames = getFrames(a);

	return 1;

}