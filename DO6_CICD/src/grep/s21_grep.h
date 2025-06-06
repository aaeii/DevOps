#ifndef S21_GREP_H_
#define S21_GREP_H_

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 10000

typedef struct {
  int e;
  int f;
  int i;
  int s;
  int v;
  int n;
  int h;
  int o;
  int l;
  int c;
  int many_flags;
  char str[SIZE];
  char str_o[SIZE];
  char filename[SIZE];
  char argument_str[SIZE];
} Flags;

void read_of_flags_grep(int argc, char *argv[], Flags *opts);
void processing_file(char *argv[], Flags *flag);
void f_file(char *argv[], Flags *flag);
void produces_text_line_number(int *number, Flags *flag);
void ignores_case(int *fl_i);
void inverts_meaning_of_searches(int *match);
void print_grep(Flags *flag);
void flags_c(Flags *flag, int *number, int count_identical_lines, char *argv[]);
void only_matching(Flags *flag, regmatch_t *structt, regex_t *reg);

#endif