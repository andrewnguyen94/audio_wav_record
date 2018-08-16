#include <stdio.h>
#include "record.h"
#include "utils.h"

int main(int argc, char **argv)
{
	int size;
	SAMPLE *audio_signal = get_audio_signal_from_source(&size);

	SIGNAL a = setSignal(audio_signal,size);
	
	hyper_vector frames = getFrames(a);

	hyper_vector power_spec = DFT_PowerSpectrum(frames, 512);

	//for (int i = 0; i < frames.row; i++) {
	//	for (int j = 0; j < 257; j++)
	//		printf("pow: %.9f\n", power_spec.data[i*257 +j]);
	//}

	filter_bank fbanks = filterbank(26, 512);

	//float temp[9] = { 2,3,4,5,6,7,8,9,10 };
	//hyper_vector a;
	//a.data = temp;
	//a.col = 3;
	//a.row = 3;
	//multiply(a,transpose(a));
	//getch();
	multiply(power_spec, transpose(setHVector(fbanks.data, fbanks.filt_len,fbanks.nfilt,2)));
	getch();
	//int record = find_arg(argc, argv, "-record");
	//if (record) {
	//	record_audio_to_database();
	//}
	//return 1;
}