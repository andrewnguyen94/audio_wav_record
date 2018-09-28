#include <stdio.h>
#include "record.h"
#include "mfcc.h"
#include "utils.h"
#ifdef __cplusplus
extern "C" {
#endif
#include "svm2.h"
#ifdef __cplusplus
}
#endif // __cplusplus

void normalize_test(char *filename, int row, int col) {
	int dem = 0, i = 0, j = 0;
	float * raw_training = (float*)malloc(sizeof(float) * (row+1) * col);
	int label;

	FILE *f = fopen(filename, "r");

	if (f == NULL)
	{
		printf("Error opening file!\n");
		exit(1);
	}
	for (i = 0; i < row; i++) {
		dem = 0;
		fscanf(f, "%d", &label);
		for (j = 0; j < col; j++) {
			fscanf(f, "%f", &raw_training[i * col + j]);
		}
	}
	fclose(f);

	FILE *fdb = fopen("db2.txt", "r");
	if (fdb == NULL)
	{
		printf("Error opening file!\n");
		exit(1);
	}
	for (i = 0; i < 10; i++) {
		fscanf(fdb, "%d", &label);
		for (j = 0; j < col; j++) {
			fscanf(fdb, "%f", &raw_training[(row) * col + j]);
			printf("%f ", raw_training[(row)* col + j]);
		}
		Get_normalize(label,raw_training, row, col);
	}
	fclose(fdb);
	free(raw_training);
}

int main(int argc, char **argv)
{
	int current_max_index = 0;
	int is_record = find_args(argc, argv, "-record");
	int is_create_database = find_args(argc, argv, "-createdb");
	int is_normalize_db = find_args(argc, argv, "-normlizedb");
	int is_train = find_args(argc, argv, "-training");
	int is_predict = find_args(argc, argv, "-predicting");
	if (is_record) {
		int current_index;
		char *path = argv[2];
		size_t len_path_folder = strlen(path);
		char *path_config = (char *)malloc(sizeof(char) *(len_path_folder + 10));
		strcpy(path_config, path);
		strcat(path_config, "config.txt");
		record_audio_to_database(path, &current_index);
		FILE *config;
		if (!cfileexists(path_config)) {
			FILE *config_first_write = fopen(path_config, "w");
			current_max_index = current_index;
			fprintf(config_first_write, "%d", current_index);
			fclose(config_first_write);
		}
		else {
			config = fopen(path_config, "r");
			fscanf(config, "%d", &current_max_index);
			if (current_index > current_max_index) {
				fclose(config);
				FILE *config_write = fopen(path_config, "w");
				current_max_index = current_index;
				fprintf(config_write, "%d", current_max_index);
				fclose(config_write);
			}
			else {
				fclose(config);
			}
		}
	}
	if (is_create_database) {
		char *path = argv[2];
		if (path == NULL) {
			fprintf(stderr, "Path no exist!");
			exit(1);
		}
		size_t len_path_folder = strlen(path);
		char *path_config = (char *)malloc(sizeof(char) *(len_path_folder + 10));
		strcpy(path_config, path);
		strcat(path_config, "config.txt");
		FILE *config = fopen(path_config, "r");
		if (config == NULL) {
			fprintf(stderr, "Config file no exist!");
			exit(1);
		}
		fscanf(config, "%d", &current_max_index);
		create_database(path, current_max_index);
	}
	if (is_normalize_db) {
		char *path = argv[2];
		if (path == NULL) {
			fprintf(stderr, "Path no exist!");
			exit(1);
		}
		size_t len_path_folder = strlen(path);
		char *path_config = (char *)malloc(sizeof(char) *(len_path_folder + 10));
		char *path_db = (char *)malloc(sizeof(char) * (len_path_folder + 6));
		char *path_info = (char *)malloc(sizeof(char) * (len_path_folder + 8));
		char *path_meaning = (char *)malloc(sizeof(char) * (len_path_folder + 8));
		char *path_nor = (char *)malloc(sizeof(char) * (len_path_folder + 14));
		strcpy(path_config, path);
		strcat(path_config, "config.txt");
		strcpy(path_db, path);
		strcat(path_db, "db.txt");
		strcpy(path_info, path);
		strcat(path_info, "info.txt");
		strcpy(path_meaning, path);
		strcat(path_meaning, "mean.txt");
		strcpy(path_nor, path);
		strcat(path_nor, "normalized.txt");
		FILE *config = fopen(path_config, "r");
		if (config == NULL) {
			fprintf(stderr, "Config file no exist!");
			exit(1);
		}
		fscanf(config, "%d", &current_max_index);
		normalize_db(path_nor, path_meaning, path_db, path_info, current_max_index);
	}
	if (is_train) {
		char *path = argv[argc - 1];
		training_normalize(path, argc, argv);
	}
	if (is_predict) {
		char *path = argv[argc - 1];
		int predict_probability;
		if (argv[2][0] != '-' && argv[2][1] != 'b') {
			predict_probability = 0;
		}
		else {
			predict_probability = 1;
		}
		predict_test_one_time(path, predict_probability);
	}
}