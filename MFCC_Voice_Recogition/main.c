#include <stdio.h>
#include "record.h"
#include "mfcc.h"

int main(int argc, char **argv)
{
	//int is_record = find_args(argc, argv, "-record");
	/*if (is_record) {
		record_audio_to_database();
	}*/
	int size;
	SAMPLE *audio_signal = get_audio_signal_from_source(&size);

	SIGNAL a = setSignal(audio_signal,size);
	a = silence_trim(a);

	hyper_vector frames = getFrames(a);

	hyper_vector power_spec = DFT_PowerSpectrum(frames, 512);

	//for (int i = 0; i < frames.row; i++) {
	//	for (int j = 0; j < 257; j++)
	//		printf("pow: %.9f\n", power_spec.data[i*257 +j]);
	//}

	filter_bank fbanks = filterbank(26, 512);

	//
	//hyper_vector a;
	//a.data = temp;
	//a.col = 9;
	//a.row = 0;

	hyper_vector apply = multiply(power_spec, transpose(setHVector(fbanks.data, fbanks.filt_len,fbanks.nfilt,2)));
	system("cls");
	apply = DCT(apply, 13);
	getch();

}