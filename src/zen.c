#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *file;
FILE *new_file;
char libs[4097 * 4097];
char head[4097 * 4097];
char body[4097 * 4097];
char line[4097];
char func_names[4096][4097];
char str_names[4096][4097];
char int_names[4096][4097];
int eqs = 0;
int funcs = 0;
int strs = 0;
int ints = 0;
int strfchs = 0;
int recs[4096];
int recs_2[4096];
int recs_4[4096];
int rec = 0;
int rec_2 = 0;
int rec_4 = 0;
int wall = 0;
int werr = 0;
int o = 0;
int io = 0;
int _str = 0;
int _int = 0;
char *version = "006";
int is_stable = 0;

void error(char * why);
void parse_variable_name(char * ptr);
void parse_function_name(char * ptr);

extern int recs_3[4096];
extern int rec_3;
extern void parse_2(char *ptr);
extern void check(char *arg);
extern int zgl;
extern int flt;
extern char flt_names[];
extern int flts;

void parse(char command[]) {
  char *ptr = command;
  while (ptr[0] == ' ') {
    ptr++;
  }
  while (ptr[strlen(ptr) - 1] == ' ') {
    ptr[strlen(ptr) - 1] = '\0';
  }
  if (ptr[0] == '\0' || ptr[0] == ';') {
    return;
  }
  int semicolon = -1;
  for (size_t i = 0; i < strlen(ptr); i++) {
    if (ptr[i] == ';') {
      semicolon = i;
    }
  }
  if (semicolon == -1) {
    error("INVALID USE OF COMMAND WITHOUT SEMICOLON AT THE END");
  }
  ptr[semicolon] = '\0';
  if (strncmp(ptr, "deflib ", 7) == 0) {
    if (rec - 1 != -1 && recs[rec - 1] == 1) {
      error("CANNOT USE DEFLIB INSIDE A FUNCTION");
    }
    ptr += 7;
    while (ptr[0] == ' ') {
      ptr++;
    }
    while (ptr[strlen(ptr) - 1] == ' ') {
      ptr[strlen(ptr) - 1] = '\0';
    }
    if (ptr[0] == '\0') {
      error("INVALID USE OF DEFLIB");
    }
    if (ptr[0] != '\"' || ptr[strlen(ptr) - 1] != '\"') {
      error("INVALID STRING");
    }
    ptr++;
    ptr[strlen(ptr) - 1] = '\0';
    if (strcmp(ptr, "io") == 0) {
      io = 1;
      strcat(libs, "#include <stdio.h>\n#include <unistd.h>\n");
    } else if (strcmp(ptr, "str") == 0) {
      _str = 1;
      strcat(libs, "#include <string.h>\n");
    } else if (strcmp(ptr, "int") == 0) {
      _int = 1;
    } else if (strcmp(ptr, "zgl") == 0) {
      zgl = 1;
      strcat(libs, "#include <SDL/sdl.h>\n");
      strcat(head, "SDL_Window *window;\nSDL_Renderer *renderer;\n");
      strcat(body, "window = SDL_CreateWindow(\"ZGL Window\", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 512, 512, 0);");
      strcat(body, "renderer = SDL_CreateRenderer(window);");
    } else if (strcmp(ptr, "flt") == 0) {
      flt = 1;
    } else {
      error("INVALID LIBRARY");
    }
  } else if (strncmp(ptr, "defstr ", 7) == 0) {
    if (rec - 1 != -1 && recs[rec - 1] == 1) {
      error("CANNOT USE DEFSTR INSIDE A FUNCTION");
    }
    if (!_str) {
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
      error("INVALID USE OF DEFSTR");
    }
    if (ptr[0] != '\"' || ptr[strlen(ptr) - 1] != '\"') {
      error("INVALID STRING");
    }
    ptr++;
    ptr[strlen(ptr) - 1] = '\0';
    parse_variable_name(ptr);
    strcpy(str_names[strs], ptr);
    char content[4097] = "char __";
    strcat(content, ptr);
    strcat(content, "[2049]=\"\";\n");
    strcat(head, content);
    strs++;
  } else if (strncmp(ptr, "func ", 5) == 0) {
    ptr += 5;
    while (ptr[0] == ' ') {
      ptr++;
    }
    while (ptr[strlen(ptr) - 1] == ' ') {
      ptr[strlen(ptr) - 1] = '\0';
    }
    if (ptr[0] == '\0') {
      error("INVALID USE OF FUNC");
    }
    if (ptr[0] != '\"' || ptr[strlen(ptr) - 1] != '\"') {
      error("INVALID STRING");
    }
    ptr++;
    ptr[strlen(ptr) - 1] = '\0';
    for (int i = 0; i < funcs; i++) {
      char *func_name = func_names[i];
      if (strcmp(ptr, func_name) == 0) {
        error("ALREADY USE THIS FUNCTION NAME");
      }
    }
    if (rec - 1 > 0) {
      error("CANNOT STACK FUNC INSIDE ANOTHER FUNC");
    }
    parse_function_name(ptr);
    recs[rec] = 1;
    rec++;
    char content[4097] = "void _";
    strcat(content, ptr);
    strcat(content, "(){");
    strcat(head, content);
    strcpy(func_names[funcs], ptr);
    funcs++;
  } else if (strcmp(ptr, "end") == 0) {
    if (eqs - 1 >= 0) {
      eqs--;
      if (rec - 1 != -1 && recs[rec - 1] == 1) {
        strcat(head, "}\n");
      } else {
        strcat(body, "}");
      }
    } else if (rec_4 - 1 != -1 && recs_4[rec_4 - 1] == 1) {
      recs_4[rec_4 - 1] = 0;
      rec_4--;
      if (rec - 1 != -1 && recs[rec - 1] == 1) {
        strcat(head, "exit(-1);}\n");
      } else {
        strcat(body, "exit(-1);}");
      }
    } else if (rec_3 - 1 != -1 && recs_3[rec_3 - 1] == 1) {
      recs_3[rec_3 - 1] = 0;
      rec_3--;
      if (rec - 1 != -1 && recs[rec - 1] == 1) {
        strcat(head, "}\n");
      } else {
        strcat(body, "}");
      }
    } else if (rec_2 - 1 != -1 && recs_2[rec_2 - 1] == 1) {
      recs_2[rec_2 - 1] = 0;
      rec_2--;
      if (rec - 1 != -1 && recs[rec - 1] == 1) {
        strcat(head, "}\n");
      } else {
        strcat(body, "}");
      }
    } else if (rec - 1 != -1 && recs[rec - 1] == 1) {
      recs[rec - 1] = 0;
      rec--;
      strcat(head, "}\n");
    } else {
      error("INVALID USE OF END");
    }
  } else if (strncmp(ptr, "run ", 4) == 0) {
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
      error("INVALID USE OF RUN");
    }
    if (ptr[0] == '\0') {
      error("INVALID USE OF RUN");
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
    for (int i = 0; i < funcs; i++) {
      char *func_name = func_names[i];
      if (strcmp(str, func_name) == 0) {
        at = i;
      }
    }
    if (at == -1) {
      error("INVALID FUNCTION NAME");
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
    if (ptr[0] == '-') {
      error("CANNOT USE A NEGATIVE INTEGER");
    }
    for (size_t i = 0; i < strlen(ptr); i++) {
      if (ptr[i] < '0' || ptr[i] > '9') {
        error("INVALID INTEGER");
      }
    }
    char content[4097] = "for (itr=0;itr<";
    strcat(content, ptr);
    strcat(content, ";itr++){_");
    strcat(content, str);
    strcat(content, "();}");
    if (rec - 1 != -1 && recs[rec - 1] == 1) {
      if (funcs > 0 && strcmp(str, func_names[funcs - 1]) == 0) {
        error("CANNOT USE RUN TO REPEAT THE FUNCTION ITSELF");
      }
      strcat(head, content);
      return;
    }
    strcat(body, content);
  } else if (strncmp(ptr, "printw ", 7) == 0) {
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
      error("INVALID USE OF PRINTW");
    }
    if (ptr[0] != '\"' || ptr[strlen(ptr) - 1] != '\"') {
      error("INVALID STRING");
    }
    ptr++;
    ptr[strlen(ptr) - 1] = '\0';
    char content[4097] = "printf(\"";
    strcat(content, ptr);
    strcat(content, "\");");
    if (rec - 1 != -1 && recs[rec - 1] == 1) {
      strcat(head, content);
      return;
    }
    strcat(body, content);
  } else if (strncmp(ptr, "printr ", 7) == 0) {
    if (!io || !_str) {
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
      error("INVALID USE OF PRINTR");
    }
    if (ptr[0] != '\"' || ptr[strlen(ptr) - 1] != '\"') {
      error("INVALID STRING");
    }
    ptr++;
    ptr[strlen(ptr) - 1] = '\0';
    int at = -1;
    for (int i = 0; i < strs; i++) {
      char *str_name = str_names[i];
      if (strcmp(ptr, str_name) == 0) {
        at = i;
      }
    }
    if (at == -1) {
      error("INVALID STRING NAME");
    }
    char content[4097] = "printf(\"%s\",__";
    strcat(content, ptr);
    strcat(content, ");");
    if (rec - 1 != -1 && recs[rec - 1] == 1) {
      strcat(head, content);
      return;
    }
    strcat(body, content);
  } else if (strncmp(ptr, "str ", 4) == 0) {
    if (!_str) {
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
      error("INVALID USE OF STR");
    }
    if (ptr[0] == '\0') {
      error("INVALID USE OF STR");
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
      strcpy(content, "strcpy(__");
      strcat(content, str);
      strcat(content, ",__");
      strcat(content, ptr);
      strcat(content, ");");
      if (rec - 1 != -1 && recs[rec - 1] == 1) {
        strcat(head, content);
        return;
      }
      strcat(body, content);
      return;
    }
    char content[4097];
    strcpy(content, "strcpy(__");
    strcat(content, str);
    strcat(content, ",\"");
    strcat(content, ptr);
    strcat(content, "\");");
    if (rec - 1 != -1 && recs[rec - 1] == 1) {
      strcat(head, content);
      return;
    }
    strcat(body, content);
  } else if (strncmp(ptr, "streq ", 6) == 0) {
    if (!_str) {
      error("INVALID COMMAND");
    }
    ptr += 6;
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
      error("INVALID USE OF STREQ");
    }
    if (ptr[0] == '\0') {
      error("INVALID USE OF STREQ");
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
      error("INVALID FIRST STRING NAME");
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
      error("INVALID SECOND STRING NAME");
    }
    char content[4097];
    strcpy(content, "if(strcmp(__");
    strcat(content, str);
    strcat(content, ",__");
    strcat(content, ptr);
    strcat(content, ")==0){");
    eqs++;
    if (rec - 1 != -1 && recs[rec - 1] == 1) {
      strcat(head, content);
      return;
    }
    strcat(body, content);
  } else if (strncmp(ptr, "strneq ", 7) == 0) {
    if (!_str) {
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
      error("INVALID USE OF STRNEQ");
    }
    if (ptr[0] == '\0') {
      error("INVALID USE OF STRNEQ");
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
      error("INVALID FIRST STRING NAME");
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
      error("INVALID SECOND STRING NAME");
    }
    char content[4097];
    strcpy(content, "if(strcmp(__");
    strcat(content, str);
    strcat(content, ",__");
    strcat(content, ptr);
    strcat(content, ")!=0){");
    eqs++;
    if (rec - 1 != -1 && recs[rec - 1] == 1) {
      strcat(head, content);
      return;
    }
    strcat(body, content);
  } else if (strncmp(ptr, "exit ", 5) == 0) {
    ptr += 5;
    while (ptr[0] == ' ') {
      ptr++;
    }
    while (ptr[strlen(ptr) - 1] == ' ') {
      ptr[strlen(ptr) - 1] = '\0';
    }
    if (ptr[0] == '\0') {
      error("INVALID USE OF EXIT");
    }
    int is_neg = 0;
    if (ptr[0] == '-') {
      is_neg = 1;
      ptr++;
    }
    for (size_t i = 0; i < strlen(ptr); i++) {
      if (ptr[i] < '0' || ptr[i] > '9') {
        error("INVALID INTEGER");
      }
    }
    char content[4097] = "exit(";
    if (is_neg) {
      strcat(content, "-");
    }
    strcat(content, ptr);
    strcat(content, ");");
    if (rec - 1 != -1 && recs[rec - 1] == 1) {
      strcat(head, content);
      return;
    }
    strcat(body, content);
  } else if (strncmp(ptr, "sleep ", 6) == 0) {
    ptr += 6;
    while (ptr[0] == ' ') {
      ptr++;
    }
    while (ptr[strlen(ptr) - 1] == ' ') {
      ptr[strlen(ptr) - 1] = '\0';
    }
    if (ptr[0] == '\0') {
      error("INVALID USE OF SLEEP");
    }
    if (ptr[0] == '-') {
      error("CANNOT USE A NEGATIVE INTEGER");
    }
    for (size_t i = 0; i < strlen(ptr); i++) {
      if (ptr[i] < '0' || ptr[i] > '9') {
        error("INVALID INTEGER");
      }
    }
    char content[4097] = "usleep(";
    strcat(content, ptr);
    strcat(content, ");");
    if (rec - 1 != -1 && recs[rec - 1] == 1) {
      strcat(head, content);
      return;
    }
    strcat(body, content);
  } else if (strncmp(ptr, "get ", 4) == 0) {
    if (!io || !_str) {
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
      error("INVALID USE OF GET");
    }
    if (ptr[0] != '\"' || ptr[strlen(ptr) - 1] != '\"') {
      error("INVALID STRING");
    }
    ptr++;
    ptr[strlen(ptr) - 1] = '\0';
    int at = -1;
    for (int i = 0; i < strs; i++) {
      char *str_name = str_names[i];
      if (strcmp(ptr, str_name) == 0) {
        at = i;
      }
    }
    if (at == -1) {
      error("INVALID STRING NAME");
    }
    char content[4097] = "while(fgets(__";
    strcat(content, ptr);
    strcat(content, ",sizeof(__");
    strcat(content, ptr);
    strcat(content, "),stdin)){break;}__");
    strcat(content, ptr);
    strcat(content, "[strlen(__");
    strcat(content, ptr);
    strcat(content, ")-1]=\'\\0\';");
    if (rec - 1 != -1 && recs[rec - 1] == 1) {
      strcat(head, content);
      return;
    }
    strcat(body, content);
  } else if (strcmp(ptr, "clear") == 0) {
    if (!io) {
      error("INVALID COMMAND");
    }
    char content[] = "printf(\"\\e[2J\e[H\");";
    if (rec - 1 != -1 && recs[rec - 1] == 1) {
      strcat(head, content);
      return;
    }
    strcat(body, content);
  } else if (strncmp(ptr, "strup ", 6) == 0) {
    if (!io || !_str) {
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
      error("INVALID USE OF STRUP");
    }
    if (ptr[0] != '\"' || ptr[strlen(ptr) - 1] != '\"') {
      error("INVALID STRING");
    }
    ptr++;
    ptr[strlen(ptr) - 1] = '\0';
    int at = -1;
    for (int i = 0; i < strs; i++) {
      char *str_name = str_names[i];
      if (strcmp(ptr, str_name) == 0) {
        at = i;
      }
    }
    if (at == -1) {
      error("INVALID STRING NAME");
    }
    char content[4097 + sizeof(ptr)] = "for(itr=0;itr<strlen(__";
    strcat(content, ptr);
    strcat(content, ");itr++){if(__");
    strcat(content, ptr);
    strcat(content, "[itr]>=\'a\'||__");
    strcat(content, ptr);
    strcat(content, "[itr]<=\'z\'){__");
    strcat(content, ptr);
    strcat(content, "[itr]=__");
    strcat(content, ptr);
    strcat(content, "[itr]-32;}}");
    if (rec - 1 != -1 && recs[rec - 1] == 1) {
      strcat(head, content);
      return;
    }
    strcat(body, content);
  } else if (strncmp(ptr, "strlw ", 6) == 0) {
    if (!io || !_str) {
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
      error("INVALID USE OF STRLW");
    }
    if (ptr[0] != '\"' || ptr[strlen(ptr) - 1] != '\"') {
      error("INVALID STRING");
    }
    ptr++;
    ptr[strlen(ptr) - 1] = '\0';
    int at = -1;
    for (int i = 0; i < strs; i++) {
      char *str_name = str_names[i];
      if (strcmp(ptr, str_name) == 0) {
        at = i;
      }
    }
    if (at == -1) {
      error("INVALID STRING NAME");
    }
    char content[4097 + sizeof(ptr)] = "for(itr=0;itr<strlen(__";
    strcat(content, ptr);
    strcat(content, ");itr++){if(__");
    strcat(content, ptr);
    strcat(content, "[itr]>=\'A\'||__");
    strcat(content, ptr);
    strcat(content, "[itr]<=\'Z\'){__");
    strcat(content, ptr);
    strcat(content, "[itr]=__");
    strcat(content, ptr);
    strcat(content, "[itr]+32;}}");
    if (rec - 1 != -1 && recs[rec - 1] == 1) {
      strcat(head, content);
      return;
    }
    strcat(body, content);
  } else if (strncmp(ptr, "defpac ", 7) == 0) {
    ptr += 7;
    while (ptr[0] == ' ') {
      ptr++;
    }
    while (ptr[strlen(ptr) - 1] == ' ') {
      ptr[strlen(ptr) - 1] = '\0';
    }
    if (ptr[0] == '\0') {
      error("INVALID USE OF DEFPAC");
    }
    if (ptr[0] != '\"' || ptr[strlen(ptr) - 1] != '\"') {
      error("INVALID STRING");
    }
    ptr++;
    ptr[strlen(ptr) - 1] = '\0';
    int dot = 0;
    for (size_t i = 0; i < strlen(ptr); i++) {
      if (ptr[i] == '.') {
        dot = 1;
      }
    }
    if (!dot) {
      char content[4097 + sizeof(ptr)];
      strcpy(content, ptr);
      strcat(content, ".zf");
      new_file = fopen(content, "r");
      if (new_file == NULL) {
        error("INVALID FILE");
      }
    } else {
      if (strcmp(strrchr(ptr, '.') + 1, "zf") != 0) {
        error("INVALID FILE EXTENSION");
      }
      new_file = fopen(ptr, "r");
      if (new_file == NULL) {
        error("INVALID FILE");
      }
    }
    char new_line[4097];
    while (fgets(new_line, sizeof(new_line), new_file) != NULL) {
      FILE *local_file = new_file;
      if (funcs > 2048) {
        error("TOO MANY FUNCTIONS");
      } else if (strs > 2048) {
        error("TOO MANY STRING VARIABLES");
      } else if (ints > 2048) {
        error("TOO MANY INTEGER VARIABLES");
      }
      if (new_line[strlen(new_line) - 1] != '\n') {
        new_line[strlen(new_line)] = '\0';
      } else {
        new_line[strlen(new_line) - 1] = '\0';
      }
      printf("[\e[38;5;218m\e[1mPARSING\e[0m] %s\n", new_line);
      parse(new_line);
      new_file = local_file;
    }
    if (rec != 0) {
      error("USE OF FUNC WITHOUT END TO CLOSE THE FUNCTION");
    } else if (rec_2 != 0) {
      error("USE OF LOOP WITHOUT END TO CLOSE THE LOOP");
    } else if (rec_3 != 0) {
      error("USE OF GETLNF WITHOUT END TO CLOSE THE ITERATION");
    } else if (rec_4 != 0) {
      error("USE OF TXT WITHOUT END TO CLOSE THE CHECKER");
    } else if (eqs != 0) {
      error("USE OF COMPARISON WITHOUT END TO CLOSE THE COMPARISON");
    }
    fclose(new_file);
  } else if (strncmp(ptr, "defint ", 7) == 0) {
    if (rec - 1 != -1 && recs[rec - 1] == 1) {
      error("CANNOT USE DEFINT INSIDE A FUNCTION");
    }
    if (!_int) {
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
      error("INVALID USE OF DEFINT");
    }
    if (ptr[0] != '\"' || ptr[strlen(ptr) - 1] != '\"') {
      error("INVALID STRING");
    }
    ptr++;
    ptr[strlen(ptr) - 1] = '\0';
    parse_variable_name(ptr);
    strcpy(int_names[ints], ptr);
    char content[4097] = "unsigned long int ___";
    strcat(content, ptr);
    strcat(content, "=0;\n");
    strcat(head, content);
    ints++;
  } else if (strncmp(ptr, "int ", 4) == 0) {
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
      error("INVALID USE OF INT");
    }
    if (ptr[0] == '\0') {
      error("INVALID USE OF INT");
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
      strcat(content, "=___");
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
    strcat(content, "=");
    strcat(content, ptr);
    strcat(content, ";");
    if (rec - 1 != -1 && recs[rec - 1] == 1) {
      strcat(head, content);
      return;
    }
    strcat(body, content);
  } else if (strncmp(ptr, "add ", 4) == 0) {
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
      error("INVALID USE OF ADD");
    }
    if (ptr[0] == '\0') {
      error("INVALID USE OF ADD");
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
      parse_variable_name(ptr);
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
      char content[4097] = "";
      strcat(content, "___");
      strcat(content, str);
      strcat(content, "+=___");
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
    char content[4097] = "";
    if (strcmp(ptr, "1") == 0) {
      strcat(content, "++___");
      strcat(content, str);
      strcat(content, ";");
    } else {
      strcat(content, "___");
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
  } else if (strncmp(ptr, "sub ", 4) == 0) {
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
      error("INVALID USE OF SUB");
    }
    if (ptr[0] == '\0') {
      error("INVALID USE OF SUB");
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
      parse_variable_name(ptr);
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
      char content[4097] = "";
      strcat(content, "___");
      strcat(content, str);
      strcat(content, "-=___");
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
    if (strcmp(ptr, "1") == 0) {
      strcat(content, "--___");
      strcat(content, str);
      strcat(content, ";");
    } else {
      strcat(content, "___");
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
  } else if (strncmp(ptr, "mul ", 4) == 0) {
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
      error("INVALID USE OF MUL");
    }
    if (ptr[0] == '\0') {
      error("INVALID USE OF MUL");
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
      parse_variable_name(ptr);
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
      char content[4097] = "";
      strcat(content, "___");
      strcat(content, str);
      strcat(content, "*=___");
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
    strcat(content, "___");
    strcat(content, str);
    strcat(content, "*=");
    strcat(content, ptr);
    strcat(content, ";");
    if (rec - 1 != -1 && recs[rec - 1] == 1) {
      strcat(head, content);
      return;
    }
    strcat(body, content);
  } else if (strncmp(ptr, "div ", 4) == 0) {
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
      error("INVALID USE OF DIV");
    }
    if (ptr[0] == '\0') {
      error("INVALID USE OF DIV");
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
      parse_variable_name(ptr);
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
      char content[4097] = "";
      strcat(content, "___");
      strcat(content, str);
      strcat(content, "/=___");
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
    strcat(content, "___");
    strcat(content, str);
    strcat(content, "/=");
    strcat(content, ptr);
    strcat(content, ";");
    if (rec - 1 != -1 && recs[rec - 1] == 1) {
      strcat(head, content);
      return;
    }
    strcat(body, content);
  } else if (strncmp(ptr, "printd ", 7) == 0) {
    if (!io || !_int) {
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
      error("INVALID USE OF PRINTD");
    }
    if (ptr[0] != '\"' || ptr[strlen(ptr) - 1] != '\"') {
      error("INVALID STRING");
    }
    ptr++;
    ptr[strlen(ptr) - 1] = '\0';
    int at = -1;
    for (int i = 0; i < ints; i++) {
      char *int_name = int_names[i];
      if (strcmp(ptr, int_name) == 0) {
        at = i;
      }
    }
    if (at == -1) {
      error("INVALID INTEGER NAME");
    }
    char content[4097] = "printf(\"%ld\",___";
    strcat(content, ptr);
    strcat(content, ");");
    if (rec - 1 != -1 && recs[rec - 1] == 1) {
      strcat(head, content);
      return;
    }
    strcat(body, content);
  } else if (strncmp(ptr, "strlen ", 7) == 0) {
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
      error("INVALID USE OF STRLEN");
    }
    if (ptr[0] == '\0') {
      error("INVALID USE OF STRLEN");
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
    parse_variable_name(str);
    int at = -1;
    for (int i = 0; i < strs; i++) {
      char *str_name = str_names[i];
      if (strcmp(str, str_name) == 0) {
        at = i;
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
    parse_variable_name(ptr);
    at = -1;
    for (int i = 0; i < ints; i++) {
      char *int_name = int_names[i];
      if (strcmp(ptr, int_name) == 0) {
        at = i;
      }
    }
    if (at == -1) {
      error("INVALID INTEGER NAME");
    }
    char content[4097] = "___";
    strcat(content, ptr);
    strcat(content, "=strlen(__");
    strcat(content, str);
    strcat(content, ");");
    if (rec - 1 != -1 && recs[rec - 1] == 1) {
      strcat(head, content);
      return;
    }
    strcat(body, content);
  } else if (strncmp(ptr, "stradd ", 7) == 0) {
    if (!_str) {
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
      error("INVALID USE OF STR");
    }
    if (ptr[0] == '\0') {
      error("INVALID USE OF STR");
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
      strcpy(content, "strcat(__");
      strcat(content, str);
      strcat(content, ",__");
      strcat(content, ptr);
      strcat(content, ");");
      if (rec - 1 != -1 && recs[rec - 1] == 1) {
        strcat(head, content);
        return;
      }
      strcat(body, content);
      return;
    }
    char content[4097];
    strcpy(content, "strcat(__");
    strcat(content, str);
    strcat(content, ",\"");
    strcat(content, ptr);
    strcat(content, "\");");
    if (rec - 1 != -1 && recs[rec - 1] == 1) {
      strcat(head, content);
      return;
    }
    strcat(body, content);
  } else if (strncmp(ptr, "strfch ", 7) == 0) {
    if (!_str) {
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
      error("INVALID USE OF STRFCH");
    }
    if (ptr[0] != '\"' || ptr[strlen(ptr) - 1] != '\"') {
      error("INVALID STRING");
    }
    ptr++;
    ptr[strlen(ptr) - 1] = '\0';
    int at = -1;
    for (int i = 0; i < strs; i++) {
      char *str_name = str_names[i];
      if (strcmp(ptr, str_name) == 0) {
        at = i;
      }
    }
    if (at == -1) {
      error("INVALID STRING NAME");
    }
    char _strfchs[4097] = "";
    sprintf(_strfchs, "%d", strfchs);
    char content[4097] = "char *ptr_";
    strcat(content, _strfchs);
    strcat(content, "_");
    strcat(content, ptr);
    strcat(content, "=__");
    strcat(content, ptr);
    strcat(content, ";ptr_");
    strcat(content, _strfchs);
    strcat(content, "_");
    strcat(content, ptr);
    strcat(content, "++;strcpy(__");
    strcat(content, ptr);
    strcat(content, ",ptr_");
    strcat(content, _strfchs);
    strcat(content, "_");
    strcat(content, ptr);
    strcat(content, ");");
    strfchs++;
    if (rec - 1 != -1 && recs[rec - 1] == 1) {
      strcat(head, content);
      return;
    }
    strcat(body, content);
  } else if (strncmp(ptr, "strlch ", 7) == 0) {
    if (!_str) {
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
      error("INVALID USE OF STRLCH");
    }
    if (ptr[0] != '\"' || ptr[strlen(ptr) - 1] != '\"') {
      error("INVALID STRING");
    }
    ptr++;
    ptr[strlen(ptr) - 1] = '\0';
    int at = -1;
    for (int i = 0; i < strs; i++) {
      char *str_name = str_names[i];
      if (strcmp(ptr, str_name) == 0) {
        at = i;
      }
    }
    if (at == -1) {
      error("INVALID STRING NAME");
    }
    char content[4097] = "__";
    strcat(content, ptr);
    strcat(content, "[strlen(__");
    strcat(content, ptr);
    strcat(content, ") - 1] = \'\\0\';");
    if (rec - 1 != -1 && recs[rec - 1] == 1) {
      strcat(head, content);
      return;
    }
    strcat(body, content);
  } else if (strncmp(ptr, "strchr ", 7) == 0) {
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
      error("INVALID USE OF STRCHR");
    }
    if (ptr[0] == '\0') {
      error("INVALID USE OF STRCHR");
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
    for (int i = 0; i < funcs; i++) {
      char *func_name = func_names[i];
      if (strcmp(str, func_name) == 0) {
        at = i;
      }
    }
    if (at == -1) {
      error("INVALID FUNCTION NAME");
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
    if (ptr[0] == '-') {
      error("CANNOT USE A NEGATIVE INTEGER");
    }
    for (size_t i = 0; i < strlen(ptr); i++) {
      if (ptr[i] < '0' || ptr[i] > '9') {
        error("INVALID INTEGER");
      }
    }
    char content[4097] = "if(strlen(";
    strcat(content, str);
    strcat(content, ")>");
    strcat(content, ptr);
    strcat(content, "){char_temp[0]=__");
    strcat(content, str);
    strcat(content, "[");
    strcat(content, ptr);
    strcat(content, "];strcpy(__");
    strcat(content, str);
    strcat(content, ",char_temp);}");
    if (rec - 1 != -1 && recs[rec - 1] == 1) {
      strcat(head, content);
      return;
    }
    strcat(body, content);
  } else if (strcmp(ptr, "loop") == 0) {
    char content[4097] = "while(1){";
    recs_2[rec_2] = 1;
    rec_2++;
    if (rec - 1 != -1 && recs[rec - 1] == 1) {
      strcat(head, content);
      return;
    }
    strcat(body, content);
  } else if (strncmp(ptr, "inteq ", 6) == 0) {
    if (!_int) {
      error("INVALID COMMAND");
    }
    ptr += 6;
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
      error("INVALID USE OF INTEQ");
    }
    if (ptr[0] == '\0') {
      error("INVALID USE OF INTEQ");
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
      error("INVALID FIRST INTEGER NAME");
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
      error("INVALID SECOND INTEGER NAME");
    }
    char content[4097];
    strcpy(content, "if(___");
    strcat(content, str);
    strcat(content, "==___");
    strcat(content, ptr);
    strcat(content, "){");
    eqs++;
    if (rec - 1 != -1 && recs[rec - 1] == 1) {
      strcat(head, content);
      return;
    }
    strcat(body, content);
  } else if (strncmp(ptr, "intneq ", 7) == 0) {
    if (!_int) {
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
      error("INVALID USE OF INTNEQ");
    }
    if (ptr[0] == '\0') {
      error("INVALID USE OF INTNEQ");
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
      error("INVALID FIRST INTEGER NAME");
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
      error("INVALID SECOND INTEGER NAME");
    }
    char content[4097];
    strcpy(content, "if(___");
    strcat(content, str);
    strcat(content, "!=___");
    strcat(content, ptr);
    strcat(content, "){");
    eqs++;
    if (rec - 1 != -1 && recs[rec - 1] == 1) {
      strcat(head, content);
      return;
    }
    strcat(body, content);
  } else if (strncmp(ptr, "intlw ", 6) == 0) {
    if (!_int) {
      error("INVALID COMMAND");
    }
    ptr += 6;
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
      error("INVALID USE OF INTLW");
    }
    if (ptr[0] == '\0') {
      error("INVALID USE OF INTLW");
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
      error("INVALID FIRST INTEGER NAME");
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
      error("INVALID SECOND INTEGER NAME");
    }
    char content[4097];
    strcpy(content, "if(___");
    strcat(content, str);
    strcat(content, "<___");
    strcat(content, ptr);
    strcat(content, "){");
    eqs++;
    if (rec - 1 != -1 && recs[rec - 1] == 1) {
      strcat(head, content);
      return;
    }
    strcat(body, content);
  } else if (strncmp(ptr, "intmr ", 6) == 0) {
    if (!_int) {
      error("INVALID COMMAND");
    }
    ptr += 6;
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
      error("INVALID USE OF INTMR");
    }
    if (ptr[0] == '\0') {
      error("INVALID USE OF INTMR");
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
      error("INVALID FIRST INTEGER NAME");
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
      error("INVALID SECOND INTEGER NAME");
    }
    char content[4097];
    strcpy(content, "if(___");
    strcat(content, str);
    strcat(content, ">___");
    strcat(content, ptr);
    strcat(content, "){");
    eqs++;
    if (rec - 1 != -1 && recs[rec - 1] == 1) {
      strcat(head, content);
      return;
    }
    strcat(body, content);
  } else {
    parse_2(ptr);
  }
}

void parse_function_name(char * ptr) {
  for (size_t i = 0; i < strlen(ptr); i++) {
    if (ptr[i] != '_' && (ptr[i] < 'a' || ptr[i] > 'z') && (ptr[i] < 'A' || ptr[i] > 'Z')) {
      error("INVALID FUNCTION NAME");
    }
  }
}

void parse_variable_name(char * ptr) {
  for (size_t i = 0; i < strlen(ptr); i++) {
    if (ptr[i] != '_' && (ptr[i] < 'a' || ptr[i] > 'z') && (ptr[i] < 'A' || ptr[i] > 'z')) {
      error("INVALID VARIABLE NAME");
    }
  }
}

void error(char * why) {
  printf("[\e[38;5;223m\e[1mERROR\e[0m] %s\n", why);
  if (file != NULL) {
    fclose(file);
  }
  if (new_file != NULL) {
    fclose(new_file);
  }
  exit(-1);
}

int main(int argc, char *argv[]) {
  check(argv[0]);
  char stable[257] = "";
  if (is_stable) {
    strcpy(stable, "\e[38;5;71mSTABLE");
  } else {
    strcpy(stable, "\e[38;5;131mNON-STABLE");
  }
  printf("[\e[38;5;110m\e[1mZEN COMPILER VERSION \e[38;5;139m%s %s\e[0m]\n", version, stable);
  #ifndef linux
    printf("[\e[38;5;37m\e[1m?\e[0m] DO YOU ALREADY HAVE GCC [Y/n]");
    char answer[4097];
    fgets(answer, sizeof(answer), stdin);
    answer[strlen(answer) - 1] = '\0';
    if (strcmp(answer, "") == 0 || strcmp(answer, "y") == 0 || strcmp(answer, "Y") == 0) {
    } else if (strcmp(answer, "n") == 0 || strcmp(answer, "N") == 0) {
      error("PLEASE INSTALL GCC");
    }
  #endif
  if (argc < 2) {
    error("NO ARGUMENT(S) OR INPUT FILE TO COMPILE, TRY -h FOR ALL COMMANDS, COMPILATION'S FLAGS AND LIBRARIES");
  }
  int input = -1;
  int output = -1;
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      char *arg = argv[i];
      arg++;
      if (strcmp(arg, "wall") == 0) {
        wall = 1;
      } else if (strcmp(arg, "werr") == 0) {
        werr = 1;
      } else if (strcmp(arg, "-help") == 0) {
        system("zen-readme");
        return 0;
      } else if (strcmp(arg, "h") == 0) {
        system("zen-readme");
        return 0;
      } else if (strcmp(arg, "o") == 0) {
        o = 1;
      } else if (strcmp(arg, "-obj") == 0) {
        o = 1;
      } else {
        error("INVALID CFLAG");
      }
    }
    if (input == -1) { 
      input = i;
      continue;
    } else {
      output = i;
      continue;
    }
    error("TOO MUCH FILES");
  }
  file = fopen(argv[input], "r");
  if (file == NULL) {
    error("INVALID INPUT FILE");
  }
  if (strcmp(strrchr(argv[input], '.') + 1, "zf") != 0) {
    error("INVALID FILE EXTENSION");
  }
  if (output != -1 && sizeof(argv[output]) > 2097) {
    error("OUTPUT FILE NAME IS TOO BIG");
  }
  while (fgets(line, sizeof(line), file) != NULL) {
    if (funcs > 2048) {
      error("TOO MANY FUNCTIONS");
    } else if (strs > 2048) {
      error("TOO MANY STRING VARIABLES");
    } else if (ints > 2048) {
      error("TOO MANY INTEGER VARIABLES");
    }
    if (line[strlen(line) - 1] != '\n') {
      line[strlen(line)] = '\0';
    } else {
      line[strlen(line) - 1] = '\0';
    }
    printf("[\e[1m\e[38;5;146mPARSING\e[0m] %s\n", line);
    parse(line);
  }
  if (rec != 0) {
    error("USE OF FUNC WITHOUT END TO CLOSE THE FUNCTION");
  } else if (rec_2 != 0) {
    error("USE OF LOOP WITHOUT END TO CLOSE THE LOOP");
  } else if (rec_3 != 0) {
    error("USE OF GETLNF WITHOUT END TO CLOSE THE ITERATION");
  } else if (rec_4 != 0) {
    error("USE OF TXT WITHOUT END TO CLOSE THE CHECKER");
  } else if (eqs != 0) {
    error("USE OF STREQ WITHOUT END TO CLOSE THE COMPARISON");
  }
  fclose(file);
  if (funcs > 0) { 
    strcat(libs, "int itr=0;\n");
  }
  if (_str) {
    strcat(libs, "char char_temp[2]=\"\";\n");
  }
  strcat(libs, "#include <stdlib.h>\n#include <time.h>\n");
  file = fopen(".out-zf.c", "w");
  fprintf(file, "%s", libs);
  fprintf(file, "%s", head);
  fprintf(file, "int main(int argc,char *argv[]){");
  fprintf(file, "%s", body);
  fprintf(file, "}");
  fclose(file);
  char filename[sizeof(argv[output]) + 4097] = "";
  if (output == -1) {
    strcpy(filename, "a");
    #ifndef unix
      strcat(filename, ".out");
    #endif
  } else {
    strcpy(filename, argv[output]);
    #ifndef unix
      strcat(filename, ".out");
    #endif
  }
  char content[4097] = "";
  strcpy(content, "gcc ");
  if (zgl) {
    printf("%d\n", system("sdl2-config > /dev/null"));
    return 0;
  }
  strcat(content, " -O3 -fsingle-precision-constant -march=native -o ");
  strcat(content, filename);
  if (!o) {
    strcat(content, " .out-zf.c");
  } else {
    strcat(content, " -c .out-zf.c");
  }
  system(content);
  system("rm .out-zf.c");
  if (o) {
    system("rm ./-o");
  }
  printf("[\e[38;5;225m\e[1mCOMPILATION SUCCESSFUL\e[0m]\n");
  return 0;
}
