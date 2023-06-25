#ifndef SRC_CAT_S21_
#define SRC_CAT_S21_

#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define BUFFER_SIZE 4096
typedef struct {
  int b_flag;
  int e_flag;
  int n_flag;
  int s_flag;
  int t_flag;
  int v_flag;
  int error_flag;
} flags;

int parser(int argc, char *argv[], flags *flag);
void cat(char *path, flags flag);

#endif
