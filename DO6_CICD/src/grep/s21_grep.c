#include "s21_grep.h"

int main(int argc, char *argv[]) {
  Flags flag = {0};
  read_of_flags_grep(argc, argv, &flag);
  // printf("%d,%d",argc,optind);
  while (optind < argc) {
    processing_file(argv, &flag);
    optind++;
  }

  return 0;
}
void read_of_flags_grep(int argc, char *argv[], Flags *flag) {
  int currentFlag;
  struct option longOptions[] = {{"regexp", 0, NULL, 'e'},
                                 {"file", 0, NULL, 'f'},
                                 {"ignore-case", 0, NULL, 'i'},
                                 {"no-messages", 0, NULL, 's'},
                                 {"invert-match", 0, NULL, 'v'},
                                 {"line-number", 0, NULL, 'n'},
                                 {"no-filename", 0, NULL, 'h'},
                                 {"only-matching", 0, NULL, 'o'},
                                 {"files-with-matches", 0, NULL, 'l'},
                                 {"count", 0, NULL, 'c'},
                                 {NULL, 0, NULL, 0}};
  while ((currentFlag = getopt_long(argc, argv, "e:ivclnhsf:o", longOptions,
                                    NULL)) != -1) {
    switch (currentFlag) {
    case 'e':
      flag->e = 1;
      strcat(flag->argument_str, optarg);
      strcat(flag->argument_str, "|");
      break;
    case 'f':
      flag->f = 1;
      strcpy(flag->filename, optarg);
      f_file(argv, flag);
      break;
    case 'i':
      flag->i = 1;
      break;
    case 's':
      flag->s = 1;
      break;
    case 'v':
      flag->v = 1;
      break;
    case 'n':
      flag->n = 1;
      break;
    case 'h':
      flag->h = 1;
      break;
    case 'o':
      flag->o = 1;
      break;
    case 'l':
      flag->l = 1;
      break;
    case 'c':
      flag->c = 1;
      break;
    default:
      break;
    }
    if (flag->v && flag->o) {
      flag->o = 0;
    }
  }
  if (!flag->e && !flag->f) { //не используются флаги
    if (argc > optind) {
      strcat(flag->argument_str, argv[optind]);
    }
    optind++;
  }
  if (flag->e || flag->f) {
    flag->argument_str[strlen(flag->argument_str) - 1] =
        '\0'; //удалить послед пайп
  }
  if (argc - optind > 1)
    flag->many_flags = 1;
}

void processing_file(char *argv[], Flags *flag) {
  FILE *file;
  regex_t reg;
  regmatch_t structt;
  int count_identical_lines = 0, number_line = 0, match = 0,
      fl_i = REG_EXTENDED;
  if (flag->i)
    ignores_case(&fl_i);
  regcomp(&reg, flag->argument_str, fl_i);
  file = fopen(argv[optind], "r");
  if (file != NULL) {
    while (fgets(flag->str, SIZE, file) != NULL) {
      match = regexec(&reg, flag->str, 1, &structt, 0); //сравнение
      number_line++;
      if ((!match || flag->v) && flag->many_flags && !flag->l && !flag->c &&
          !flag->h)
        printf("%s:", argv[optind]);
      if (!match)
        count_identical_lines++; // для флага c
      if (flag->v)
        inverts_meaning_of_searches(&match);
      if (!match && !flag->l && !flag->c && !flag->n && !flag->o) {
        print_grep(flag);
      }
      if (flag->n && !match && !flag->c && !flag->l) {
        produces_text_line_number(&number_line, flag);
      }
      if (flag->o) {
        strcpy(flag->str_o, flag->str);
        if (!match && !flag->l && !flag->c) {
          only_matching(flag, &structt, &reg);
        }
      }
    }
    regfree(&reg);
    if (flag->l && count_identical_lines < 1 && flag->v) {
      printf("%s\n", argv[optind]);
    }
    if (flag->l && !flag->c && count_identical_lines > 0)
      printf("%s\n", argv[optind]);
    if (flag->c) {
      flags_c(flag, &number_line, count_identical_lines, argv);
    }
    fclose(file);
  } else {
    regfree(&reg);
    if (!flag->s)
      fprintf(stderr, "grep: %s: No such file or directory\n", argv[optind]);
  }
}

void produces_text_line_number(int *number_line, Flags *flag) {
  if (flag->o) {
    printf("%d:", *number_line);
  } else
    printf("%d:%s", *number_line, flag->str);
  if (flag->str[strlen(flag->str) - 1] != '\n') {
    printf("\n");
  }
}
void ignores_case(int *fl_i) { *fl_i = REG_EXTENDED | REG_ICASE; }
void inverts_meaning_of_searches(int *match) { *match = !*match; }

void print_grep(Flags *flag) {
  printf("%s", flag->str);
  if (flag->str[strlen(flag->str) - 1] != '\n') {
    printf("\n");
  }
}

void flags_c(Flags *flag, int *number_line, int count_identical_lines,
             char *argv[]) {
  if (flag->many_flags && !flag->h) {
    printf("%s:", argv[optind]);
  }
  if (!flag->l && !flag->v) {
    printf("%d\n", count_identical_lines); // c без флагов
  }
  if (!flag->l && flag->v) {
    printf("%d\n", *number_line - count_identical_lines);
  }
  if (flag->l) {
    if (count_identical_lines > 0) {
      count_identical_lines = 1;
      printf("%d\n%s\n", count_identical_lines, argv[optind]);
    } else if (count_identical_lines <= 0) {
      printf("%d\n", count_identical_lines);
    }
  }
}

void only_matching(Flags *flag, regmatch_t *structt, regex_t *reg) {
  char *line = flag->str_o;
  while ((regexec(reg, line, 1, structt, 0) == 0)) {
    printf("%.*s\n", (int)(structt->rm_eo - structt->rm_so),
           line + structt->rm_so);
    line += structt->rm_eo;
  }
}

void f_file(char *argv[], Flags *flag) {
  FILE *f;
  f = fopen(flag->filename, "r");
  if (f != NULL) {
    while (!feof(f)) {
      if (fgets(flag->str, 1000, f) != NULL) {
        if (flag->str[strlen(flag->str) - 1] == '\n' &&
            strlen(flag->str) - 1 != 0)
          flag->str[strlen(flag->str) - 1] = '\0';
        strcat(flag->argument_str, flag->str);
        strcat(flag->argument_str, "|");
      }
    }
    fclose(f);
  } else
    fprintf(stderr, "grep: %s: No such file or directory\n", argv[optind]);
}
