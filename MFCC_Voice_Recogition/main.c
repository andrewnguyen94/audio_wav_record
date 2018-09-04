#include <stdio.h>
#include "record.h"
#include "mfcc.h"
#include "utils.h"

void normalize_from_file(char *filename, int row, int col) {
	int dem = 0, i = 0, j = 0;
	float * raw_training = (float*)malloc(sizeof(float) * row * col);
	int* label = (int*)malloc(sizeof(int)*row);

	FILE *f = fopen(filename, "r");

	if (f == NULL)
	{
		printf("Error opening file!\n");
		exit(1);
	}
	for (i = 0; i < row; i++) {
		dem = 0;
		fscanf(f, "%d", &label[i]);
		for (j = 0; j < col; j++) {
			fscanf(f, "%f", &raw_training[i * col + j]);
			printf("%f ", raw_training[i * col + j]);
		}
		printf("\n");
	}
	fclose(f);
	normalize(label, raw_training, row, col);
	free(label);
	free(raw_training);
}



int main(int argc, char **argv)
{
	FILE *f;
	int  option, dem = 0;
	printf("1. Extract DB info.\n2. Record to DB.\n3. Record to DB Ver.2\n4. Create DB.\n5. Extract from DB.\nOption: ");
	scanf("%d", &option);
	switch (option) {
	case 1:
//		int  info[5];
//		/*char *label = (char*)malloc(sizeof(char));
//		int size = get_number_of_sample_in_record();
//		const char *default_ext = ".txt";
//		const char *default_path = "./data/";
//*/
//		FILE * fdb = fopen("db.txt", "r");
//		if (fdb == NULL) {
//			fprintf(stderr, "file no exist!!! \n");
//			exit(1);
//		}
//		dem = 0;
//
//		FILE *finf = fopen("info.txt", "w");
//		if (finf == NULL) {
//			fprintf(stderr, "file no exist!!! \n");
//			exit(1);
//		}
//
//		while (label1 < 4)
//		{
//			while (true) {
//				SAMPLE* audio_signal = read_audio_signal_from_file(path);
//				hyper_vector feature_vector_all_frame = get_feature_vector_from_signal(audio_signal, size);
//				int size_feature_vector = feature_vector_all_frame.col * feature_vector_all_frame.row * feature_vector_all_frame.dim;
//				printf("size : %d \n", size_feature_vector);
//				fprintf(fdb, "%d ", label1 + 1);
//				for (int j = 0; j < size_feature_vector; ++j) {
//					fprintf(fdb, "%f ", feature_vector_all_frame.data[j]);
//					printf("%f ", feature_vector_all_frame.data[j]);
//				}
//				fprintf(fdb, "\n");
//				free(path);
//				i++;
//			}
//
//			fprintf(finf, "%d ", i);
//
//			++label1;
//			i = 0;
//		}
//		fprintf(finf, "%d", dem);
//		fclose(finf);
//		fclose(fdb);
		break;
	case 2:
		f = fopen("db.txt", "w");
		if (f == NULL)
		{
			printf("Error opening file!\n");
			exit(1);
		}
		dem = 0;
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

			fprintf(f, "%d: ", dem);
			for (int i = 0; i < final_feats.col; i++) {
				printf("%f ", final_feats.data[i]);
				fprintf(f, "%f ", final_feats.data[i]);
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
		break;
	case 3:
		int is_record = find_args(argc, argv, "-record");
		if (is_record) {
			int is_training = find_args(argc, argv, "-training");
			int is_testing = find_args(argc, argv, "-testing");
			record_audio_to_database(is_training, is_testing);
		}
	case 4:
		int  label1 = 0, i = 0;
		char *label = (char*)malloc(sizeof(char));
		int size = get_number_of_sample_in_record();
		const char *default_ext = ".txt";
		const char *default_path = "./data/";

		FILE * fdb = fopen("db.txt", "w");
		if (fdb == NULL) {
			fprintf(stderr, "file no exist!!! \n");
			exit(1);
		}
		dem = 0;

		FILE *finf = fopen("info.txt", "w");
		if (finf == NULL) {
			fprintf(stderr, "file no exist!!! \n");
			exit(1);
		}

		while (label1 < 4)
		{
			sprintf(label, "%d", label1);
			char *temp = (char*)malloc(sizeof(char) * 9);
			strcpy(temp, default_path);
			strcat(temp, label);
			strcat(temp, "_");

			while (true) {
				char *path;
				char *index;
				if (i < 10) { index = (char *)malloc(sizeof(char)); }
				else if (i >= 10 && i < 100) {
					index = (char *)malloc(sizeof(char) * 2);
				}
				else {
					index = (char *)malloc(sizeof(char) * 3);
				}
				sprintf(index, "%d", i);
				size_t len = 9 + 4 + strlen(index) + 1;
				path = (char *)malloc(len * sizeof(char));
				strcpy(path, temp);
				strcat(path, index);
				strcat(path, default_ext);
				printf("path : %s \n", path);
				if (check_path(path)) {
					free(path);
					break;
				}
				dem++;
				SAMPLE* audio_signal = read_audio_signal_from_file(path);
				hyper_vector feature_vector_all_frame = get_feature_vector_from_signal(audio_signal, size);
				int size_feature_vector = feature_vector_all_frame.col * feature_vector_all_frame.row * feature_vector_all_frame.dim;
				printf("size : %d \n", size_feature_vector);
				fprintf(fdb, "%d ", label1 + 1);
				for (int j = 0; j < size_feature_vector; ++j) {
					fprintf(fdb, "%f ", feature_vector_all_frame.data[j]);
					printf("%f ", feature_vector_all_frame.data[j]);
				}
				fprintf(fdb, "\n");
				free(path);
				i++;
			}

			fprintf(finf, "%d ", i);

			++label1;
			i = 0;
		}
		fprintf(finf, "%d", dem);
		fclose(finf);
		fclose(fdb);
		break;

	case 5:
		int option;
		int info[5];

		FILE *f = fopen("info2.txt", "r");
		for (int i = 0; i < 5; i++) {
			fscanf(f, "%d", &info[i]);
		}
		fclose(f);

		system("cls");
		printf("Create Normalized DB for:\n5.1 All Classes.5.2 Separated Classes.\nOption (1/2): ");
		scanf("%d", &option);
		switch (option) {
		case 1:
			normalize_from_file("db.txt", info[4], 91);
			break;

		case 2:
			FILE * fdb = fopen("db.txt", "r");
			int temp = 1;
			int label = 1;
			dem = 0;
			while (label <4) {
				SAMPLE *classes = (SAMPLE*)malloc(sizeof(SAMPLE)*info[dem] * 91);
				system("cls");
				for (int i = 0; i < info[dem]; i++)
				{
					fscanf(fdb, "%d", &label);
					for (int j = 0; j < 91; ++j) {
						fscanf(fdb, "%f", &classes[i * 91 + j]);
						printf("%f ", classes[i * 91 + j]);
					}
					printf("\n");

				}
				normalize2(label, classes, info[dem], 91);
				free(classes);
				dem++;
			}
			fclose(fdb);
			break;
		}
		printf("\nDONE!!");
		getch();
	}
	return 1;
}