#include <stdio.h>
#include "record.h"
#include "mfcc.h"
#include "utils.h"


void writeDBFS(SAMPLE* raw_signal, int trim_ms, int signal_len) {

	FILE *f = fopen("file.txt", "w");
	if (f == NULL)
	{
		printf("Error opening file!\n");
		exit(1);
	}
	for (int i = 0; i < signal_len; i++) {
		if (i < 360) {
			raw_signal[i] = 0;
			fprintf(f, "%.8f\n", raw_signal);
			continue;
		}
		fprintf(f, "%.8f\n", raw_signal[i]);
	}
	fclose(f);

	FILE *f2 = fopen("file2.txt", "w");
	int chunk_size = 160;
	float sum = 0;
	while (trim_ms < signal_len)
	{
		sum = 0;
		for (int i = trim_ms; i < trim_ms + chunk_size; i++) {
			sum += raw_signal[i] * raw_signal[i];
		}
		sum = sqrt(sum / (chunk_size));
		sum = 20 * log10(sum);
		fprintf(f2, "%f\n", sum);
		trim_ms += chunk_size;
	}
	fclose(f2);
}

void testCovm(float a[]) {
	int col = 5;
	int row = 5;
	int i, j, k;
	float sum;
	float mean[5];
	for (i = 0; i < col; i++) {
		sum = 0;
		for (j = 0; j < row; j++) {
			sum += a[j*col + i];
		}
		mean[i] = sum / row;
	}

	float *cov = (float*)malloc(sizeof(float)*col*col);
	sum = 0;
	for (j = 0; j < col; j++) {
		for (k = 0; k < col; k++) {
			for (i = 0; i < row; i++) {
				sum += (a[i*col + j] - mean[j])*(a[i*col + k] - mean[k]);
			}
			cov[j*col + k] = sum / (row - 1);
			printf("%f ", cov[j*col + k]);
			sum = 0;
		}
		printf("\n");
	}


	float size = col * (col + 1) / 2;
	float *final_mfcc = (float*)malloc(sizeof(float)*size);


	int mark = col + 1;
	int col_temp = col;
	int dem = 0, dem2 = 1;
	i = j = dem2 = dem;
	while (i < size)
	{
		if (i == 0) {
			final_mfcc[i] = cov[i];
			dem2++;
		}
		else {
			if (dem2 < col_temp) {
				j += mark;
				final_mfcc[i] = cov[j];
				dem2++;
			}
			else {
				col_temp--;
				dem++;
				j = dem;
				final_mfcc[i] = cov[j];
				dem2 = 1;
				printf("%f ", final_mfcc[i]);

				i++;
				continue;
			}
		}
		printf("%f ", final_mfcc[i]);
		i++;
	}

}

int main(int argc, char **argv)
{
	FILE *f = fopen("db.txt", "w");
	if (f == NULL)
	{
		printf("Error opening file!\n");
		exit(1);
	}

	int dem = 0;
	char c, c1;
	while (1) {
		int size;
		/*______________________get_raw_signal____________________________________________________________*/
		SAMPLE *audio_signal = get_audio_signal_from_source(&size);

		/*______________________get_pre_emphasized_signal_________________________________________________*/
		SIGNAL a = setSignal(audio_signal, size);

		/*______________________get_silence_free_signal_________________________________________________*/
		writeDBFS(a.raw_signal, 0, size);
		SIGNAL temp = silence_trim(a);
		free(a.raw_signal);
		/*______________________get_Frames________________________________________________________________*/
		hyper_vector frames = getFrames(temp);
		free(temp.raw_signal);
		/*______________________compute_DFT_and_Power_spectrum____________________________________________*/
		hyper_vector power_spec = DFT_PowerSpectrum(frames, 512);
		/*______________________get_filterbanks___________________________________________________________*/
		filter_bank fbanks = filterbank(26, 512);
		/*______________________apply_filterBanks_________________________________________________________*/
		hyper_vector transpose_param = setHVector(fbanks.data, fbanks.filt_len, fbanks.nfilt, 1);
		hyper_vector tmp = transpose(transpose_param);
		free(transpose_param.data);
		hyper_vector apply = multiply(power_spec, tmp);
		free(tmp.data);
		/*______________________get_more_compact_output_by_performing_DCT_conversion_______________________*/
		hyper_vector test = DCT(apply, 13);
		free(apply.data);
		/*______________________append_frame_energy_into_mfcc_vectors______________________________________*/
		append_energy(test, power_spec);
		free(power_spec.data);
		/*______________________final_feature_vector_size_1x91_____________________________________________*/
		hyper_vector final_feats = cov(test);
		free(test.data);

		fprintf(f, "%d: ",dem);
		for (int i = 0; i<final_feats.col; i++) {
			printf("%f ", final_feats.data[i]);
			fprintf(f,"%f ", final_feats.data[i]);
		}

		free(final_feats.data);
		fprintf(f, "\n");
		printf("\nDONE!");
		fflush(stdin);

		c = getch();
		c1 = getch();
		if (c == 'y' || c == 'Y' || c1 == 'y') {
			system("cls");
			dem++;
		}

		printf("%d\n", dem);
		if (dem == 100) {
			fclose(f);
			return 1;
		}
	}
}