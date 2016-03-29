#ifndef FILES_H
#define FILES_H

#include <dirent.h>
#include <stdbool.h>

typedef struct dir_cnt {
  char path[255];
  int plen;
  DIR *dir;
  char **cnt;
  int clen;
} dir_cnt;


dir_cnt *dir_cnt_new(const char *path);

void dir_cnt_free(dir_cnt *d);

void dir_cnt_up(dir_cnt *d);

bool dir_cnt_to(dir_cnt *d, char *fp);

bool is_dir(char *path);



#endif
