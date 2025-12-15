#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern char libs[4097 * 4097];
extern char head[4097 * 4097];
extern char body[4097 * 4097];
extern char line[4097];
extern char func_names[4096][4097];
extern char str_names[4096][4097];
extern char int_names[4096][4097];
char txt_names[4096][4097];
extern int eqs;
extern int funcs;
extern int strs;
extern int ints;
int txts = 0;
extern int strfchs;
extern int recs[4096];
extern int recs_2[4096];
int recs_3[4096];
extern int recs_4[4096];
extern int rec;
extern int rec_2;
int rec_3 = 0;
extern int rec_4;
extern int wall;
extern int werr;
extern int io;
extern int _str;
extern int _int;

extern void error(char * why);
extern void parse_variable_name(char * ptr);
extern void parse_function_name(char * ptr);

void parse_deftxt(char *ptr);
void parse_txt(char *ptr);
void parse_getlnf(char *ptr);
void parse_wrt(char *ptr);
void parse_wrtctf(char *ptr);
void parse_close(char *ptr);
void parse_sys(char *ptr);
void parse_rand(char *ptr);
void parse_strint(char *ptr);
void parse_continue();
void parse_break();
void parse_rem(char *ptr);
void parse_asm(char *ptr);

extern void parse_defflt(char *ptr);
extern void parse_flt(char *ptr);
extern void parse_printf(char *ptr);
extern void parse_fltadd(char *ptr);
extern void parse_fltsub(char *ptr);
extern void parse_fltmul(char *ptr);
extern void parse_fltdiv(char *ptr);
extern void parse_cos(char *ptr);
extern void parse_tan(char *ptr);
extern void parse_sin(char *ptr);
extern void parse_abs(char *ptr);
extern void parse_sqrt(char *ptr);
extern void parse_ceil(char *ptr);
extern void parse_floor(char *ptr);
extern void parse_flteq(char *ptr);
extern void parse_fltneq(char *ptr);
extern void parse_fltlw(char *ptr);
extern void parse_fltmr(char *ptr);

void check(char *arg) {
  int at = -1;
  for (size_t i = 0; i < strlen(arg); i++) {
    if (arg[i] == '.') {
      at = i;
    }
  }
  if (at == -1) {
    return;
  }
  for (int i = 0; i < at; i++) {
    arg++;
  }
  if (strcmp(arg, "exe") == 0) {
    printf("[ERROR] CANNOT RUN .EXE FILE, USE MSYS OR WSL TO COMPILE AND RUN ZENC.OUT FILE\n");
  }
}

void parse_2(char *ptr) {
  if (strncmp(ptr, "deftxt ", 7) == 0) {
    parse_deftxt(ptr);
  } else if (strncmp(ptr, "txt ", 4) == 0) {
    parse_txt(ptr);
  } else if (strncmp(ptr, "getlnf ", 7) == 0) {
    parse_getlnf(ptr);
  } else if (strncmp(ptr, "wrt ", 4) == 0) {
    parse_wrt(ptr);
  } else if (strncmp(ptr, "wrtctf ", 7) == 0) {
    parse_wrtctf(ptr);
  } else if (strncmp(ptr, "close ", 6) == 0) {
    parse_close(ptr);
  } else if (strncmp(ptr, "sys ", 4) == 0) {
    parse_sys(ptr);
  } else if (strncmp(ptr, "rand ", 5) == 0) {
    parse_rand(ptr);
  } else if (strncmp(ptr, "strint ", 7) == 0) {
    parse_strint(ptr);
  } else if (strcmp(ptr, "continue") == 0) {
    parse_continue();
  } else if (strcmp(ptr, "break") == 0) {
    parse_break();
  } else if (strncmp(ptr, "rem ", 4) == 0) {
    parse_rem(ptr);
  } else if (strncmp(ptr, "defflt ", 7) == 0) {
    parse_defflt(ptr);
  } else if (strncmp(ptr, "printf ", 7) == 0) {
    parse_printf(ptr);
  } else if (strncmp(ptr, "flt ", 4) == 0) {
    parse_flt(ptr);
  } else if (strncmp(ptr, "fltadd ", 7) == 0) {
    parse_fltadd(ptr);
  } else if (strncmp(ptr, "fltsub ", 7) == 0) {
    parse_fltsub(ptr);
  } else if (strncmp(ptr, "fltmul ", 7) == 0) {
    parse_fltmul(ptr);
  } else if (strncmp(ptr, "fltdiv ", 7) == 0) {
    parse_fltdiv(ptr);
  } else if (strncmp(ptr, "cos ", 4) == 0) {
    parse_cos(ptr);
  } else if (strncmp(ptr, "tan ", 4) == 0) {
    parse_tan(ptr);
  } else if (strncmp(ptr, "sin ", 4) == 0) {
    parse_sin(ptr);
  } else if (strncmp(ptr, "abs ", 4) == 0) {
    parse_abs(ptr);
  } else if (strncmp(ptr, "sqrt ", 5) == 0) {
    parse_sqrt(ptr);
  } else if (strncmp(ptr, "ceil ", 5) == 0) {
    parse_ceil(ptr);
  } else if (strncmp(ptr, "floor ", 6) == 0) {
    parse_floor(ptr);
  } else if (strncmp(ptr, "flteq ", 6) == 0) {
    parse_flteq(ptr);
  } else if (strncmp(ptr, "fltneq ", 6) == 0) {
    parse_fltneq(ptr);
  } else if (strncmp(ptr, "fltlw ", 6) == 0) {
    parse_fltlw(ptr);
  } else if (strncmp(ptr, "fltmr ", 6) == 0) {
    parse_fltmr(ptr);
  } else if (strncmp(ptr, "asm", 3) == 0) {
    parse_asm(ptr);
  } else {
    error("INVALID COMMAND");
  }
}

void parse_asm(char *ptr) {
  ptr += 4;
  while (ptr[0] == ' ') {
    ptr++;
  }
  if (ptr[0] == '\0') {
    error("INVALID USE OF ASM");
  }
  if (ptr[0] != '\"' || ptr[strlen(ptr) - 1] != '\"') {
    error("INVALID STRING");
  }
  ptr++;
  ptr[strlen(ptr) - 1] = '\0';
  if (ptr[0] == '^') {
    error("ASM DOES NOT SUPPORT VARIABLE STRING");
  }
  char content[4097] = "asm volatile(\"";
  strcat(content, ptr);
  strcat(content, "\");");
  if (rec - 1 != -1 && recs[rec - 1] == 1) {
    strcat(head, content);
    return;
  }
  strcat(body, content);
}

void parse_rem(char *ptr) {
  if (!_int) {
    error("INVALID COMMAND");
  }
  ptr += 4;
  while (ptr[0] == ' ') {
    ptr++;
  }
  int and = -1;
  int in_str = 0;
  for (size_t i = 0; i < strlen(ptr); i++) {
    if (ptr[i] == '\"') {
      if (!in_str) {
        in_str = 1;
      } else {
        in_str = 0;
      }
    }
    if (ptr[i] == ',' && !in_str) {
      if (and != -1) {
        if (!in_str) {
          error("TOO MUCH ARGUMENTS");
        }
      }
      and = i;
    }
  }
  if (in_str) {
    error("INVALID STRING");
  }
  if (and == -1) {
    error("INVALID USE OF REM");
  }
  if (ptr[0] == '\0') {
    error("INVALID USE OF REM");
  }
  char *str = ptr;
  str[and] = '\0';
  while (str[strlen(str) - 1] == ' ') {
    str[strlen(str) - 1] = '\0';
  }
  if (str[0] != '\"' || str[strlen(str) - 1] != '\"') {
    error("INVALID STRING");
  }
  str++;
  str[strlen(str) - 1] = '\0';
  int at = -1;
  for (int i = 0; i < ints; i++) {
    char *int_name = int_names[i];
    if (strcmp(str, int_name) == 0) {
      at = i;
      break;
    }
  }
  if (at == -1) {
    error("INVALID INTEGER NAME");
  }
  for (int i = 0; i < and; i++) {
    ptr++;
  }
  ptr++;
  while (ptr[0] == ' ') {
    ptr++;
  }
  while(ptr[strlen(ptr) - 1] == ' ') {
    ptr[strlen(ptr) - 1] = '\0';
  }
  if (ptr[0] == '\"' && ptr[strlen(ptr) - 1] == '\"') {
    ptr++;
    ptr[strlen(ptr) - 1] = '\0';
    at = -1;
    for (int i = 0; i < ints; i++) {
      char *int_name = int_names[i];
      if (strcmp(ptr, int_name) == 0) {
        at = i;
        break;
      }
    }
    if (at == -1) {
    error("INVALID INTEGER NAME");
    }
    char content[4097];
    strcpy(content, "___");
    strcat(content, str);
    strcat(content, "%=___");
    strcat(content, ptr);
    strcat(content, ";");
    if (rec - 1 != -1 && recs[rec - 1] == 1) {
      strcat(head, content);
      return;
    }
    strcat(body, content);
    return;
  }
  for (size_t i = 0; i < strlen(ptr); i++) {
    if (ptr[i] < '0' || ptr[i] > '9') {
      error("INVALID INTEGER");
    }
  }
  char content[4097];
  strcpy(content, "___");
  strcat(content, str);
  strcat(content, "%=");
  strcat(content, ptr);
  strcat(content, ";");
  if (rec - 1 != -1 && recs[rec - 1] == 1) {
    strcat(head, content);
    return;
  }
  strcat(body, content);
}

void parse_break() {
  if (rec_2 - 1 != -1 && recs_2[rec_2 - 1] == 1) {
    if (rec - 1 != -1 && recs[rec - 1] == 1) {
      strcat(head, "break;");
      return;
    }
    strcat(body, "break;");
  } else {
    error("INVALID USE OF CONTINUE");
  }
}

void parse_continue() {
  if (rec_2 - 1 != -1 && recs_2[rec_2 - 1] == 1) {
    if (rec - 1 != -1 && recs[rec - 1] == 1) {
      strcat(head, "continue;");
      return;
    }
    strcat(body, "continue;");
  } else if (rec_3 - 1 != -1 && recs_3[rec_3 - 1] == 1) {
    if (rec - 1 != -1 && recs[rec - 1] == 1) {
      strcat(head, "continue;");
      return;
    }
    strcat(body, "continue;");
  } else {
    error("INVALID USE OF CONTINUE");
  }
}

void parse_strint(char *ptr) {
  if (!_str || !_int) {
    error("INVALID COMMAND");
  }
  ptr += 7;
  while (ptr[0] == ' ') {
    ptr++;
  }
  int and = -1;
  int in_str = 0;
  for (size_t i = 0; i < strlen(ptr); i++) {
    if (ptr[i] == '\"') {
      if (!in_str) {
        in_str = 1;
      } else {
        in_str = 0;
      }
    }
    if (ptr[i] == ',' && !in_str) {
      if (and != -1) {
        if (!in_str) {
          error("TOO MUCH ARGUMENTS");
        }
      }
      and = i;
    }
  }
  if (in_str) {
    error("INVALID STRING");
  }
  if (and == -1) {
    error("INVALID USE OF STRINT");
  }
  if (ptr[0] == '\0') {
    error("INVALID USE OF STRINT");
  }
  char *str = ptr;
  str[and] = '\0';
  while (str[strlen(str) - 1] == ' ') {
    str[strlen(str) - 1] = '\0';
  }
  if (str[0] != '\"' || str[strlen(str) - 1] != '\"') {
    error("INVALID STRING");
  }
  str++;
  str[strlen(str) - 1] = '\0';
  int at = -1;
  for (int i = 0; i < strs; i++) {
    char *str_name = str_names[i];
    if (strcmp(str, str_name) == 0) {
      at = i;
      break;
    }
  }
  if (at == -1) {
    error("INVALID STRING NAME");
  }
  for (int i = 0; i < and; i++) {
    ptr++;
  }
  ptr++;
  while (ptr[0] == ' ') {
    ptr++;
  }
  while(ptr[strlen(ptr) - 1] == ' ') {
    ptr[strlen(ptr) - 1] = '\0';
  }
  if (ptr[0] != '\"' || ptr[strlen(ptr) - 1] != '\"') {
    error("INVALID STRING");
  }
  ptr++;
  ptr[strlen(ptr) - 1] = '\0';
  at = -1;
  for (int i = 0; i < ints; i++) {
    char *int_name = int_names[i];
    if (strcmp(ptr, int_name) == 0) {
      at = i;
      break;
    }
  }
  if (at == -1) {
    error("INVALID INTEGER NAME");
  }
  char content[4097];
  strcpy(content, "___");
  strcat(content, ptr);
  strcat(content, "=strtoul(__");
  strcat(content, str);
  strcat(content, ",NULL");
  strcat(content, ",0);");
  if (rec - 1 != -1 && recs[rec - 1] == 1) {
    strcat(head, content);
    return;
  }
  strcat(body, content);
}

void parse_rand(char *ptr) {
  if (!_int) {
    error("INVALID COMMAND");
  }
  ptr += 5;
  while (ptr[0] == ' ') {
    ptr++;
  }
  int and = -1;
  int in_str = 0;
  for (size_t i = 0; i < strlen(ptr); i++) {
    if (ptr[i] == '\"') {
      if (!in_str) {
        in_str = 1;
      } else {
        in_str = 0;
      }
    }
    if (ptr[i] == ',' && !in_str) {
      if (and != -1) {
        if (!in_str) {
          error("TOO MUCH ARGUMENTS");
        }
      }
      and = i;
    }
  }
  if (in_str) {
    error("INVALID STRING");
  }
  if (and == -1) {
    error("INVALID USE OF RAND");
  }
  if (ptr[0] == '\0') {
    error("INVALID USE OF RAND");
  }
  char *str = ptr;
  str[and] = '\0';
  while (str[strlen(str) - 1] == ' ') {
    str[strlen(str) - 1] = '\0';
  }
  if (str[0] != '\"' || str[strlen(str) - 1] != '\"') {
    error("INVALID STRING");
  }
  str++;
  str[strlen(str) - 1] = '\0';
  int at = -1;
  for (int i = 0; i < ints; i++) {
    char *int_name = int_names[i];
    if (strcmp(str, int_name) == 0) {
      at = i;
      break;
    }
  }
  if (at == -1) {
    error("INVALID INTEGER NAME");
  }
  for (int i = 0; i < and; i++) {
    ptr++;
  }
  ptr++;
  while (ptr[0] == ' ') {
    ptr++;
  }
  while(ptr[strlen(ptr) - 1] == ' ') {
    ptr[strlen(ptr) - 1] = '\0';
  }
  if (ptr[0] == '\"' && ptr[strlen(ptr) - 1] == '\"') {
    ptr++;
    ptr[strlen(ptr) - 1] = '\0';
    at = -1;
    for (int i = 0; i < ints; i++) {
      char *int_name = int_names[i];
      if (strcmp(ptr, int_name) == 0) {
        at = i;
        break;
      }
    }
    if (at == -1) {
    error("INVALID INTEGER NAME");
    }
    char content[4097];
    strcpy(content, "srand(time(NULL));___");
    strcat(content, str);
    strcat(content, "=rand()%___");
    strcat(content, ptr);
    strcat(content, "+1;");
    if (rec - 1 != -1 && recs[rec - 1] == 1) {
      strcat(head, content);
      return;
    }
    strcat(body, content);
    return;
  }
  for (size_t i = 0; i < strlen(ptr); i++) {
    if (ptr[i] < '0' || ptr[i] > '9') {
      error("INVALID INTEGER");
    }
  }
  char content[4097];
  strcpy(content, "srand(time(NULL));___");
  strcat(content, str);
  strcat(content, "=rand()%");
  strcat(content, ptr);
  strcat(content, "+1;");
  if (rec - 1 != -1 && recs[rec - 1] == 1) {
    strcat(head, content);
    return;
  }
  strcat(body, content);
}

void parse_sys(char *ptr) {
  if (!io) {
    error("INVALID COMMAND");
  }
  ptr += 4;
  while (ptr[0] == ' ') {
    ptr++;
  }
  if (ptr[0] == '\0') {
    error("INVALID USE OF SYS");
  }
  if (ptr[0] != '\"' || ptr[strlen(ptr) - 1] != '\"') {
    error("INVALID STRING");
  }
  ptr++;
  ptr[strlen(ptr) - 1] = '\0';
  if (ptr[0] == '^') {
    if (!_str) {
      error("INVALID USE OF STRING NAME WITHOUT LIBRARY STR");
    }
    ptr++;
    int at = -1;
    for (int i = 0; i < strs; i++) {
      char *str_name = str_names[i];
      if (strcmp(ptr, str_name) == 0) {
        at = i;
        break;
      }
    }
    if (at == -1) {
      error("INVALID STRING NAME");
    }
    char content[4097] = "system(__";
    strcat(content, ptr);
    strcat(content, ");");
    if (rec - 1 != -1 && recs[rec - 1] == 1) {
      strcat(head, content);
      return;
    }
    strcat(body, content);
    return;
  }
  char content[4097] = "system(\"";
  strcat(content, ptr);
  strcat(content, "\");");
  if (rec - 1 != -1 && recs[rec - 1] == 1) {
    strcat(head, content);
    return;
  }
  strcat(body, content);
}

void parse_close(char *ptr) {
  if (!io) {
    error("INVALID COMMAND");
  }
  ptr += 6;
  while (ptr[0] == ' ') {
    ptr++;
  }
  while (ptr[strlen(ptr) - 1] == ' ') {
    ptr[strlen(ptr) - 1] = '\0';
  }
  if (ptr[0] == '\0') {
    error("INVALID USE OF CLOSE");
  }
  if (ptr[0] != '\"' || ptr[strlen(ptr) - 1] != '\"') {
    error("INVALID STRING");
  }
  ptr++;
  ptr[strlen(ptr) - 1] = '\0';
  parse_variable_name(ptr);
  strcpy(txt_names[txts], ptr);
  char content[4097] = "fclose(____";
  strcat(content, ptr);
  strcat(content, ");\n");
  if (rec_4 - 1 != -1 && recs_4[rec_4 - 1] != 0) {
    error("CANNOT CLOSE A FILE IF THE FILE DOES NOT EXISTS");
  }
  if (rec - 1 != -1 && recs[rec - 1] == 1) {
    strcat(head, content);
    return;
  }
  strcat(body, content);
}

void parse_wrtctf(char *ptr) {
  if (!io || !_str) {
    error("INVALID COMMAND");
  }
  ptr += 7;
  while (ptr[0] == ' ') {
    ptr++;
  }
  int and = -1;
  int in_str = 0;
  for (size_t i = 0; i < strlen(ptr); i++) {
    if (ptr[i] == '\"') {
      if (!in_str) {
        in_str = 1;
      } else {
        in_str = 0;
      }
    }
    if (ptr[i] == ',' && !in_str) {
      if (and != -1) {
        if (!in_str) {
          error("TOO MUCH ARGUMENTS");
        }
      }
      and = i;
    }
  }
  if (in_str) {
    error("INVALID STRING");
  }
  if (and == -1) {
    error("INVALID USE OF WRTCTF");
  }
  if (ptr[0] == '\0') {
    error("INVALID USE OF WRTCTF");
  }
  char *str = ptr;
  str[and] = '\0';
  while (str[strlen(str) - 1] == ' ') {
    str[strlen(str) - 1] = '\0';
  }
  if (str[0] != '\"' || str[strlen(str) - 1] != '\"') {
    error("INVALID STRING");
  }
  str++;
  str[strlen(str) - 1] = '\0';
  int at = -1;
  for (int i = 0; i < txts; i++) {
    char *txt_name = txt_names[i];
    if (strcmp(str, txt_name) == 0) {
      at = i;
      break;
    }
  }
  if (at == -1) {
    error("INVALID FILE NAME");
  }
  for (int i = 0; i < and; i++) {
    ptr++;
  }
  ptr++;
  while (ptr[0] == ' ') {
    ptr++;
  }
  while(ptr[strlen(ptr) - 1] == ' ') {
    ptr[strlen(ptr) - 1] = '\0';
  }
  if (ptr[0] != '\"' || ptr[strlen(ptr) - 1] != '\"') {
    error("INVALID STRING");
  }
  ptr++;
  ptr[strlen(ptr) - 1] = '\0';
  at = -1;
  for (int i = 0; i < strs; i++) {
    char *str_name = str_names[i];
    if (strcmp(ptr, str_name) == 0) {
      at = i;
      break;
    }
  }
  if (at == -1) {
    error("INVALID STRING NAME");
  }
  char content[4097];
  strcpy(content, "fprintf(____");
  strcat(content, str);
  strcat(content, ",\"%s\",__");
  strcat(content, ptr);
  strcat(content, ");");
  if (rec - 1 != -1 && recs[rec - 1] == 1) {
    strcat(head, content);
    return;
  } 
  strcat(body, content);
}

void parse_wrt(char *ptr) {
  if (!io) {
    error("INVALID COMMAND");
  }
  ptr += 4;
  while (ptr[0] == ' ') {
    ptr++;
  }
  int and = -1;
  int in_str = 0;
  for (size_t i = 0; i < strlen(ptr); i++) {
    if (ptr[i] == '\"') {
      if (!in_str) {
        in_str = 1;
      } else {
        in_str = 0;
      }
    }
    if (ptr[i] == ',' && !in_str) {
      if (and != -1) {
        if (!in_str) {
          error("TOO MUCH ARGUMENTS");
        }
      }
      and = i;
    }
  }
  if (in_str) {
    error("INVALID STRING");
  }
  if (and == -1) {
    error("INVALID USE OF WRT");
  }
  if (ptr[0] == '\0') {
    error("INVALID USE OF WRT");
  }
  char *str = ptr;
  str[and] = '\0';
  while (str[strlen(str) - 1] == ' ') {
    str[strlen(str) - 1] = '\0';
  }
  if (str[0] != '\"' || str[strlen(str) - 1] != '\"') {
    error("INVALID STRING");
  }
  str++;
  str[strlen(str) - 1] = '\0';
  int at = -1;
  for (int i = 0; i < txts; i++) {
    char *txt_name = txt_names[i];
    if (strcmp(str, txt_name) == 0) {
      at = i;
      break;
    }
  }
  if (at == -1) {
    error("INVALID FILE NAME");
  }
  for (int i = 0; i < and; i++) {
    ptr++;
  }
  ptr++;
  while (ptr[0] == ' ') {
    ptr++;
  }
  while(ptr[strlen(ptr) - 1] == ' ') {
    ptr[strlen(ptr) - 1] = '\0';
  }
  if (ptr[0] != '\"' || ptr[strlen(ptr) - 1] != '\"') {
    error("INVALID STRING");
  }
  ptr++;
  ptr[strlen(ptr) - 1] = '\0';
  if (ptr[0] == '^') {
    ptr++;
    parse_variable_name(ptr);
    at = -1;
    for (int i = 0; i < strs; i++) {
      char *str_name = str_names[i];
      if (strcmp(ptr, str_name) == 0) {
        at = i;
        break;
      }
    }
    if (at == -1) {
      error("INVALID STRING NAME");
    }
    char content[4097];
    strcpy(content, "____");
    strcat(content, str);
    strcat(content, "=fopen(__");
    strcat(content, ptr);
    strcat(content, ",\"w\"");
    strcat(content, ");");
    if (rec - 1 != -1 && recs[rec - 1] == 1) {
      strcat(head, content);
      return;
    }
    strcat(body, content);
    return;
  }
  char content[4097];
  strcpy(content, "____");
  strcat(content, str);
  strcat(content, "=fopen(\"");
  strcat(content, ptr);
  strcat(content, "\",\"w\"");
  strcat(content, ");");
  if (rec - 1 != -1 && recs[rec - 1] == 1) {
    strcat(head, content);
    return;
  }
  strcat(body, content);
}

void parse_getlnf(char *ptr) {
  if (!io || !_str) {
    error("INVALID COMMAND");
  }
  ptr += 7;
  while (ptr[0] == ' ') {
    ptr++;
  }
  int and = -1;
  int in_str = 0;
  for (size_t i = 0; i < strlen(ptr); i++) {
    if (ptr[i] == '\"') {
      if (!in_str) {
        in_str = 1;
      } else {
        in_str = 0;
      }
    }
    if (ptr[i] == ',' && !in_str) {
      if (and != -1) {
        if (!in_str) {
          error("TOO MUCH ARGUMENTS");
        }
      }
      and = i;
    }
  }
  if (in_str) {
    error("INVALID STRING");
  }
  if (and == -1) {
    error("INVALID USE OF GETLNF");
  }
  if (ptr[0] == '\0') {
    error("INVALID USE OF GETLNF");
  }
  char *str = ptr;
  str[and] = '\0';
  while (str[strlen(str) - 1] == ' ') {
    str[strlen(str) - 1] = '\0';
  }
  if (str[0] != '\"' || str[strlen(str) - 1] != '\"') {
    error("INVALID STRING");
  }
  str++;
  str[strlen(str) - 1] = '\0';
  int at = -1;
  for (int i = 0; i < txts; i++) {
    char *txt_name = txt_names[i];
    if (strcmp(str, txt_name) == 0) {
      at = i;
      break;
    }
  }
  if (at == -1) {
    error("INVALID FILE NAME");
  }
  for (int i = 0; i < and; i++) {
    ptr++;
  }
  ptr++;
  while (ptr[0] == ' ') {
    ptr++;
  }
  while(ptr[strlen(ptr) - 1] == ' ') {
    ptr[strlen(ptr) - 1] = '\0';
  }
  if (ptr[0] != '\"' || ptr[strlen(ptr) - 1] != '\"') {
    error("INVALID STRING");
  }
  ptr++;
  ptr[strlen(ptr) - 1] = '\0';
  at = -1;
  for (int i = 0; i < strs; i++) {
    char *str_name = str_names[i];
    if (strcmp(ptr, str_name) == 0) {
      at = i;
      break;
    }
  }
  if (at == -1) {
    error("INVALID STRING NAME");
  }
  char content[4097];
  strcpy(content, "while(fgets(__");
  strcat(content, ptr);
  strcat(content, ",sizeof(__");
  strcat(content, ptr);
  strcat(content, "),____");
  strcat(content, str);
  strcat(content, ")){if(__");
  strcat(content, ptr);
  strcat(content, "[strlen(__");
  strcat(content, ptr);
  strcat(content, ")-1]!=\'\\n\'){__");
  strcat(content, ptr);
  strcat(content, "[strlen(__");
  strcat(content, ptr);
  strcat(content, ")]=\'\\0\';}else{__");
  strcat(content, ptr);
  strcat(content, "[strlen(__");
  strcat(content, ptr);
  strcat(content, ")-1]=\'\\n\';}");
  recs_3[rec_3] = 1;
  rec_3++;
  if (rec - 1 != -1 && recs[rec - 1] == 1) {
    strcat(head, content);
    return;
  }
  strcat(body, content);
}

void parse_txt(char *ptr) {
  if (!io) {
    error("INVALID COMMAND");
  }
  ptr += 4;
  while (ptr[0] == ' ') {
    ptr++;
  }
  int and = -1;
  int in_str = 0;
  for (size_t i = 0; i < strlen(ptr); i++) {
    if (ptr[i] == '\"') {
      if (!in_str) {
        in_str = 1;
      } else {
        in_str = 0;
      }
    }
    if (ptr[i] == ',' && !in_str) {
      if (and != -1) {
        if (!in_str) {
          error("TOO MUCH ARGUMENTS");
        }
      }
      and = i;
    }
  }
  if (in_str) {
    error("INVALID STRING");
  }
  if (and == -1) {
    error("INVALID USE OF TXT");
  }
  if (ptr[0] == '\0') {
    error("INVALID USE OF TXT");
  }
  char *str = ptr;
  str[and] = '\0';
  while (str[strlen(str) - 1] == ' ') {
    str[strlen(str) - 1] = '\0';
  }
  if (str[0] != '\"' || str[strlen(str) - 1] != '\"') {
    error("INVALID STRING");
  }
  str++;
  str[strlen(str) - 1] = '\0';
  int at = -1;
  for (int i = 0; i < txts; i++) {
    char *txt_name = txt_names[i];
    if (strcmp(str, txt_name) == 0) {
      at = i;
      break;
    }
  }
  if (at == -1) {
    error("INVALID FILE NAME");
  }
  for (int i = 0; i < and; i++) {
    ptr++;
  }
  ptr++;
  while (ptr[0] == ' ') {
    ptr++;
  }
  while(ptr[strlen(ptr) - 1] == ' ') {
    ptr[strlen(ptr) - 1] = '\0';
  }
  if (ptr[0] != '\"' || ptr[strlen(ptr) - 1] != '\"') {
    error("INVALID STRING");
  }
  ptr++;
  ptr[strlen(ptr) - 1] = '\0';
  recs_4[rec_4] = 1;
  rec_4++;
  if (ptr[0] == '^') {
    ptr++;
    parse_variable_name(ptr);
    at = -1;
    for (int i = 0; i < strs; i++) {
      char *str_name = str_names[i];
      if (strcmp(ptr, str_name) == 0) {
        at = i;
        break;
      }
    }
    if (at == -1) {
      error("INVALID STRING NAME");
    }
    char content[4097];
    strcpy(content, "____");
    strcat(content, str);
    strcat(content, "=fopen(__");
    strcat(content, ptr);
    strcat(content, ",\"r\"");
    strcat(content, ");if(____");
    strcat(content, str);
    strcat(content, "==NULL){");
    if (rec - 1 != -1 && recs[rec - 1] == 1) {
      strcat(head, content);
      return;
    }
    strcat(body, content);
    return;
  }
  char content[4097];
  strcpy(content, "____");
  strcat(content, str);
  strcat(content, "=fopen(\"");
  strcat(content, ptr);
  strcat(content, "\",\"r\"");
  strcat(content, ");if(____");
  strcat(content, str);
  strcat(content, "==NULL){");
  recs_4[rec_4] = 1;
  rec_4++;
  if (rec - 1 != -1 && recs[rec - 1] == 1) {
    strcat(head, content);
    return;
  }
  strcat(body, content);
}

void parse_deftxt(char *ptr) {
  if (rec - 1 != -1 && recs[rec - 1] == 1) {
    error("CANNOT USE DEFTXT INSIDE A FUNCTION");
  }
  if (!io) {
    error("INVALID COMMAND");
  }
  ptr += 7;
  while (ptr[0] == ' ') {
    ptr++;
  }
  while (ptr[strlen(ptr) - 1] == ' ') {
    ptr[strlen(ptr) - 1] = '\0';
  }
  if (ptr[0] == '\0') {
    error("INVALID USE OF DEFTXT");
  }
  if (ptr[0] != '\"' || ptr[strlen(ptr) - 1] != '\"') {
    error("INVALID STRING");
  }
  ptr++;
  ptr[strlen(ptr) - 1] = '\0';
  parse_variable_name(ptr);
  strcpy(txt_names[txts], ptr);
  char content[4097] = "FILE *____";
  strcat(content, ptr);
  strcat(content, ";\n");
  strcat(head, content);
  txts++;
}
