#include <stdio.h>
#include "record.h"
#include "mfcc.h"
#include "utils.h"

int main(int argc, char **argv)
{
	int is_record = find_args(argc, argv, "-record");
	if (is_record) {
		int is_training = find_args(argc, argv, "-training");
		int is_testing = find_args(argc, argv, "-testing");
		record_audio_to_database(is_training, is_testing);
	}
	int size = get_number_of_sample_in_record();
	char *path = (char *)"./data/0_2.txt";
	SAMPLE* audio_signal = get_audio_signal_from_source(path);
	hyper_vector feature_vector = get_feature_vector_from_signal(audio_signal, size);


	return 1;
}