#ifndef UTILS_H
#define UTILS_H

/*includes
------------------------------------------
------------------------------------------*/
#include"record.h"

/*defines, constants
------------------------------------------
------------------------------------------*/

/*functions
------------------------------------------
------------------------------------------*/

void error(char *err);
int find_args(int argc, char* argv[], char *arg);
char *get_name_file_from_path(char *path);
int cfileexists(const char * filename);
#endif