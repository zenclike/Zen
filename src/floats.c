#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char flt_names[4096][4097];
int flts = 0;
int flt = 0;

extern char body[];
extern char head[];
extern int recs[];
extern int rec;
extern void error(char *why);
extern void parse_variable_name(char *ptr);
extern int io;

void parse_cos(char *ptr) {
  if (!flt) {
    error("INVALID COMMAND");
  }
  ptr += 4;
  while (ptr[0] == ' ') {
    ptr++;
  }
  while (ptr[strlen(ptr) - 1] == ' ') {
    ptr[strlen(ptr) - 1] = '\0';
  }
  if (ptr[0] == '\0') {
    error("INVALID USE OF COS");
  }
  if (ptr[0] != '\"' || ptr[strlen(ptr) - 1] != '\"') {
    error("INVALID STRING");
  }
  ptr++;
  ptr[strlen(ptr) - 1] = '\0';
  int at = -1;
  for (int i = 0; i < flts; i++) {
    char *flt_name = flt_names[i];
    if (strcmp(ptr, flt_name) == 0) {
      at = i;
    }
  }
  if (at == -1) {
    error("INVALID FLOAT NAME");
  }
  char content[4097] = "_____";
  strcat(content, ptr);
  strcat(content, "=cos(_____");
  strcat(content, ptr);
  strcat(content, ");");
  if (rec - 1 != -1 && recs[rec - 1] == 1) {
    strcat(head, content);
    return;
  }
  strcat(body, content);
}

void parse_fltdiv(char *ptr) {
  if (!flt) {
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
    error("INVALID USE OF FLTDIV");
  }
  if (ptr[0] == '\0') {
    error("INVALID USE OF FLTDIV");
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
  for (int i = 0; i < flts; i++) {
    char *flt_name = flt_names[i];
    if (strcmp(str, flt_name) == 0) {
      at = i;
      break;
    }
  }
  if (at == -1) {
    error("INVALID FLOAT NAME");
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
    parse_variable_name(ptr);
    for (int i = 0; i < flts; i++) {
      char *flt_name = flt_names[i];
      if (strcmp(ptr, flt_name) == 0) {
        at = i;
        break;
      }
    }
    if (at == -1) {
      error("INVALID FLOAT NAME");
    }
    char content[4097] = "";
    strcat(content, "_____");
    strcat(content, str);
    strcat(content, "/=_____");
    strcat(content, ptr);
    strcat(content, ";");
    if (rec - 1 != -1 && recs[rec - 1] == 1) {
      strcat(head, content);
      return;
    }
    strcat(body, content);
    return;
  }
  int dots = 0;
  for (size_t i = 0; i < strlen(ptr); i++) {
    if ((ptr[i] < '0' || ptr[i] > '9') && ptr[i] != '.') {
      error("INVALID FLOAT");
    }
    if (ptr[i] == '.') {
      if (dots > 0) {
        error("CANNOT USE FLOATING POINT MORE THAN ONE TIME");
      }
      dots++;
    }
  }
  char buf[4097];
  for (size_t i = 0; i < strlen(buf); i++) {
    buf[i] = ptr[i];
  }
  buf[strlen(buf)] = '\0';
  float val = strtof(buf, NULL);
  char content[4097] = "";
  if (val == 0) {
    strcat(content, "_____");
    strcat(content, str);
    strcat(content, "=0;");
  } else {
    strcat(content, "_____");
    strcat(content, str);
    strcat(content, "/=");
    strcat(content, ptr);
    strcat(content, ";");
  }
  if (rec - 1 != -1 && recs[rec - 1] == 1) {
    strcat(head, content);
    return;
  }
  strcat(body, content);
}

void parse_fltmul(char *ptr) {
  if (!flt) {
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
    error("INVALID USE OF FLTMUL");
  }
  if (ptr[0] == '\0') {
    error("INVALID USE OF FLTMUL");
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
  for (int i = 0; i < flts; i++) {
    char *flt_name = flt_names[i];
    if (strcmp(str, flt_name) == 0) {
      at = i;
      break;
    }
  }
  if (at == -1) {
    error("INVALID FLOAT NAME");
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
    parse_variable_name(ptr);
    for (int i = 0; i < flts; i++) {
      char *flt_name = flt_names[i];
      if (strcmp(ptr, flt_name) == 0) {
        at = i;
        break;
      }
    }
    if (at == -1) {
      error("INVALID FLOAT NAME");
    }
    char content[4097] = "";
    strcat(content, "_____");
    strcat(content, str);
    strcat(content, "*=_____");
    strcat(content, ptr);
    strcat(content, ";");
    if (rec - 1 != -1 && recs[rec - 1] == 1) {
      strcat(head, content);
      return;
    }
    strcat(body, content);
    return;
  }
  int dots = 0;
  for (size_t i = 0; i < strlen(ptr); i++) {
    if ((ptr[i] < '0' || ptr[i] > '9') && ptr[i] != '.') {
      error("INVALID FLOAT");
    }
    if (ptr[i] == '.') {
      if (dots > 0) {
        error("CANNOT USE FLOATING POINT MORE THAN ONE TIME");
      }
      dots++;
    }
  }
  char content[4097] = "";
  strcat(content, "_____");
  strcat(content, str);
  strcat(content, "*=");
  strcat(content, ptr);
  strcat(content, ";");
  if (rec - 1 != -1 && recs[rec - 1] == 1) {
    strcat(head, content);
    return;
  }
  strcat(body, content);
}

void parse_fltsub(char *ptr) {
  if (!flt) {
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
    error("INVALID USE OF FLTSUB");
  }
  if (ptr[0] == '\0') {
    error("INVALID USE OF FLTSUB");
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
  for (int i = 0; i < flts; i++) {
    char *flt_name = flt_names[i];
    if (strcmp(str, flt_name) == 0) {
      at = i;
      break;
    }
  }
  if (at == -1) {
    error("INVALID FLOAT NAME");
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
    parse_variable_name(ptr);
    for (int i = 0; i < flts; i++) {
      char *flt_name = flt_names[i];
      if (strcmp(ptr, flt_name) == 0) {
        at = i;
        break;
      }
    }
    if (at == -1) {
      error("INVALID FLOAT NAME");
    }
    char content[4097] = "";
    strcat(content, "_____");
    strcat(content, str);
    strcat(content, "+=_____");
    strcat(content, ptr);
    strcat(content, ";");
    if (rec - 1 != -1 && recs[rec - 1] == 1) {
      strcat(head, content);
      return;
    }
    strcat(body, content);
    return;
  }
  int dots = 0;
  for (size_t i = 0; i < strlen(ptr); i++) {
    if ((ptr[i] < '0' || ptr[i] > '9') && ptr[i] != '.') {
      error("INVALID FLOAT");
    }
    if (ptr[i] == '.') {
      if (dots > 0) {
        error("CANNOT USE FLOATING POINT MORE THAN ONE TIME");
      }
      dots++;
    }
  }
  char content[4097] = "";
  if (strcmp(ptr, "1") == 0) {
    strcat(content, "--_____");
    strcat(content, str);
    strcat(content, ";");
  } else {
    strcat(content, "_____");
    strcat(content, str);
    strcat(content, "-=");
    strcat(content, ptr);
    strcat(content, ";");
  }
  if (rec - 1 != -1 && recs[rec - 1] == 1) {
    strcat(head, content);
    return;
  }
  strcat(body, content);
}

void parse_fltadd(char *ptr) {
  if (!flt) {
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
    error("INVALID USE OF FLTADD");
  }
  if (ptr[0] == '\0') {
    error("INVALID USE OF FLTADD");
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
  for (int i = 0; i < flts; i++) {
    char *flt_name = flt_names[i];
    if (strcmp(str, flt_name) == 0) {
      at = i;
      break;
    }
  }
  if (at == -1) {
    error("INVALID FLOAT NAME");
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
    parse_variable_name(ptr);
    for (int i = 0; i < flts; i++) {
      char *flt_name = flt_names[i];
      if (strcmp(ptr, flt_name) == 0) {
        at = i;
        break;
      }
    }
    if (at == -1) {
      error("INVALID FLOAT NAME");
    }
    char content[4097] = "";
    strcat(content, "_____");
    strcat(content, str);
    strcat(content, "+=_____");
    strcat(content, ptr);
    strcat(content, ";");
    if (rec - 1 != -1 && recs[rec - 1] == 1) {
      strcat(head, content);
      return;
    }
    strcat(body, content);
    return;
  }
  int dots = 0;
  for (size_t i = 0; i < strlen(ptr); i++) {
    if ((ptr[i] < '0' || ptr[i] > '9') && ptr[i] != '.') {
      error("INVALID FLOAT");
    }
    if (ptr[i] == '.') {
      if (dots > 0) {
        error("CANNOT USE FLOATING POINT MORE THAN ONE TIME");
      }
      dots++;
    }
  }
  char content[4097] = "";
  if (strcmp(ptr, "1") == 0) {
    strcat(content, "++_____");
    strcat(content, str);
    strcat(content, ";");
  } else {
    strcat(content, "_____");
    strcat(content, str);
    strcat(content, "+=");
    strcat(content, ptr);
    strcat(content, ";");
  }
  if (rec - 1 != -1 && recs[rec - 1] == 1) {
    strcat(head, content);
    return;
  }
  strcat(body, content);
}

void parse_flt(char *ptr) {
  if (!flt) {
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
    error("INVALID USE OF FLT");
  }
  if (ptr[0] == '\0') {
    error("INVALID USE OF FLT");
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
  for (int i = 0; i < flts; i++) {
    char *flt_name = flt_names[i];
    if (strcmp(str, flt_name) == 0) {
      at = i;
      break;
    }
  }
  if (at == -1) {
    error("INVALID FLOAT NAME");
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
    parse_variable_name(ptr);
    at = -1;
    for (int i = 0; i < flts; i++) {
      char *flt_name = flt_names[i];
      if (strcmp(ptr, flt_name) == 0) {
        at = i;
        break;
      }
    }
    if (at == -1) {
      error("INVALID FLOAT NAME");
    }
    char content[4097];
    strcpy(content, "_____");
    strcat(content, str);
    strcat(content, "=_____");
    strcat(content, ptr);
    strcat(content, ";");
    if (rec - 1 != -1 && recs[rec - 1] == 1) {
      strcat(head, content);
      return;
    }
    strcat(body, content);
    return;
  }
  int dots = 0;
  for (size_t i = 0; i < strlen(ptr); i++) {
    if ((ptr[i] < '0' || ptr[i] > '9') && ptr[i] != '.') {
      error("INVALID FLOAT");
    }
    if (ptr[i] == '.') {
      if (dots > 0) {
        error("CANNOT USE FLOATING POINT MORE THAN ONE TIME");
      }
      dots++;
    }
  }
  char content[4097];
  strcpy(content, "_____");
  strcat(content, str);
  strcat(content, "=");
  strcat(content, ptr);
  strcat(content, ";");
  if (rec - 1 != -1 && recs[rec - 1] == 1) {
    strcat(head, content);
    return;
  }
  strcat(body, content);
}

void parse_printf(char *ptr) {
  if (!io || !flt) {
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
    error("INVALID USE OF PRINTF");
  }
  if (ptr[0] != '\"' || ptr[strlen(ptr) - 1] != '\"') {
    error("INVALID STRING");
  }
  ptr++;
  ptr[strlen(ptr) - 1] = '\0';
  int at = -1;
  for (int i = 0; i < flts; i++) {
    char *flt_name = flt_names[i];
    if (strcmp(ptr, flt_name) == 0) {
      at = i;
    }
  }
  if (at == -1) {
    error("INVALID FLOAT NAME");
  }
  char content[4097] = "printf(\"%g\",_____";
  strcat(content, ptr);
  strcat(content, ");");
  if (rec - 1 != -1 && recs[rec - 1] == 1) {
    strcat(head, content);
    return;
  }
  strcat(body, content);
}

void parse_defflt(char *ptr) {
  if (!flt) {
    error("INVALID COMMAND");
  }
  ptr += 7;
  if (ptr[0] != '\"' && ptr[strlen(ptr) - 1] == '\"') {
    error("INVALID STRING");
  }
  if (rec - 1 != -1 && recs[rec - 1] == 1) {
    error("CANNOT USE DEFFLT INSIDE A FUNCTION");
  }
  ptr++;
  ptr[strlen(ptr) - 1] = '\0';
  parse_variable_name(ptr);
  char content[4097] = "";
  strcat(content, "float _____");
  strcat(content, ptr);
  strcat(content, ";");
  strcat(body, content);
  strcpy(flt_names[flts], ptr);
  flts++;
}
