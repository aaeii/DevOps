#include "s21_cat.h"
int main(int argc, char *argv[]) {
  int index_file;
  Flags flags = read_of_flags_cat(argc, argv, &index_file);
  // printf("%d,%d,%d", index_file,optind,argc);
  if (!flags.error) {
    for (int i = index_file; i < argc; i++) {
      processing_file(argv[i], flags);
    }
  } else {
    printf("usage: s21_cat [-bnesvtET] [file ...]");
  }

  return 0;
}

Flags read_of_flags_cat(int argc, char *argv[], int *index_file) {
  struct option longOptions[] = {{"number-nonblank", 0, NULL, 'b'},
                                 {"number", 0, NULL, 'n'},
                                 {"squeeze-blank", 0, NULL, 's'},
                                 {NULL, 0, NULL, 0}};

  int currentFlag;
  Flags flags = {false, false, false, false, false, false, false};

  while ((currentFlag =
              getopt_long(argc, argv, "bnesvtET", longOptions, NULL)) != -1) {
    switch (currentFlag) {
    case 'b':
      flags.b = true;
      break;
    case 'n':
      flags.n = true;
      break;
    case 's':
      flags.s = true;
      break;
    case 't':
      flags.t = true;
      flags.v = true;
      break;
    case 'e':
      flags.e = true;
      flags.v = true;
      break;
    case 'T':
      flags.t = true;
      break;
    case 'E':
      flags.e = true;
      break;
    case 'v':
      flags.v = true;
      break;
    default:
      flags.error = true;
      break;
    }
  }
  *index_file = optind;
  return flags;
}
void processing_file(char *f, Flags flags) {
  FILE *file = fopen(f, "r");
  if (file != NULL) {
    int line = fgetc(file);
    int lineNumber = 1, last = '\n', last2 = '0';
    while (line != EOF) {
      if (flags.s && last == '\n' && line == '\n' && last2 == '\n') {
        last2 = last;
        last = line;
        line = fgetc(file);
        continue;
      }
      if (flags.n && !flags.b && last == '\n') {
        line_number(&lineNumber);
      }
      if (flags.b && last == '\n' && line != '\n') {
        line_number(&lineNumber);
      }
      if (flags.e && line == '\n') {
        putchar('$');
      }
      if (flags.t && line == '\t') {
        print_tab(line);
        line = 'I';
      }
      if (flags.v)
        print_non_printable(line);
      else {
        putchar(line);
      }
      last2 = last;
      last = line;
      line = fgetc(file);
    }
    fclose(file);
  } else {
    fprintf(stderr, "cat: %s: No such file or directory\n", f);
  }
}

void line_number(int *lineNumber) {
  printf("%6d\t", *lineNumber);
  (*lineNumber)++;
}
void print_tab(int line) {
  if (line == '\t') {
    putchar('^');
  }
}
void print_non_printable(int line) {
  if ((line < 32 && line != '\n' && line != '\t') || line == 127) {
    printf("^");
  }
  if ((line < 32 || line > 126) && line != '\n' && line != '\t') {
    if (line > 126) {
      line = line - 128 + 64;
    } else {
      line = line + 64;
    }
  }
  putchar(line);
}
