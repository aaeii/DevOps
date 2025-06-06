#ifndef S21_CAT_H_
#define S21_CAT_H_

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
typedef struct {
  bool b;
  bool e;
  bool n;
  bool s;
  bool t;
  bool v;
  bool error;
} Flags;
Flags read_of_flags_cat(int argc, char *argv[], int *index_file);
void processing_file(char *f, Flags flags);
void line_number(int *lineNumber);
void print_tab(int line);
void print_non_printable(int line);

#endif
