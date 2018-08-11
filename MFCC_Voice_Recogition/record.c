#include "record.h"

SAMPLE * get_audio_signal_from_source(int *size)
{
	PaStreamParameters inputParameters, outputParameters;
	PaStream *stream;
	PaError err;
	SAMPLE *recordedSamples;
	int i;
	int totalFrames;
	int numSamples;
	size_t numBytes;
	SAMPLE max, average, val;

	printf("patest_read_record.c\n"); fflush(stdout);

	totalFrames = NUM_SECONDS * SAMPLE_RATE; /* Record for a few seconds. */
	numSamples = totalFrames * NUM_CHANNELS;

	numBytes = numSamples * sizeof(SAMPLE);
	*size = numSamples;
	recordedSamples = (SAMPLE *) calloc(numSamples, sizeof(SAMPLE));
	if (!recordedSamples)
	{
		printf("Could not allocate record array.\n");
		exit(1);
	}
	for (i = 0; i<numSamples; i++) *(recordedSamples + i) = 0;

	err = Pa_Initialize();
	if (err != paNoError) goto error;

	inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
	if (inputParameters.device == paNoDevice) {
		fprintf(stderr, "Error: No default input device.\n");
		goto error;
	}
	inputParameters.channelCount = NUM_CHANNELS;
	inputParameters.sampleFormat = PA_SAMPLE_TYPE;
	inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
	inputParameters.hostApiSpecificStreamInfo = NULL;

	/* Record some audio. -------------------------------------------- */
	err = Pa_OpenStream(
		&stream,
		&inputParameters,
		NULL,                  /* &outputParameters, */
		SAMPLE_RATE,
		FRAMES_PER_BUFFER,
		paClipOff,      /* we won't output out of range samples so don't bother clipping them */
		NULL, /* no callback, use blocking API */
		NULL); /* no callback, so no callback userData */
	if (err != paNoError) goto error;

	err = Pa_StartStream(stream);
	if (err != paNoError) goto error;
	printf("Now recording!!\n"); fflush(stdout);

	err = Pa_ReadStream(stream, recordedSamples, totalFrames);
	if (err != paNoError) goto error;

	err = Pa_CloseStream(stream);
	if (err != paNoError) goto error;

	//for (int i = 0; i < numSamples; i++) {
	//	printf(" %f ", recordedSamples[i]);
	//}
	return recordedSamples;
error:
	Pa_Terminate();
	fprintf(stderr, "An error occured while using the portaudio stream\n");
	fprintf(stderr, "Error number: %d\n", err);
	fprintf(stderr, "Error message: %s\n", Pa_GetErrorText(err));
	return -1;
	return recordedSamples;
}

void record_audio_to_database()
{

	//SAMPLE *audio_signal = get_audio_signal_from_source();
	
}

char * get_name_of_new_file()
{
	
	return NULL;
}

KEYWORDS get_key_word(int key)
{
	if (key == 0) return TU;
	else if (key == 1) return TRUNG_ANH;
	else if (key == 2) return TRUNG;
	else {
		exit(0);
	}
}
