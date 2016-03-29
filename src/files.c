#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "files.h"


void dir_cnt_free(dir_cnt *d)
{
  if(d->cnt) {
    for(int n = 0; n < d->clen; n++)
      free(d->cnt[n]);
    free(d->cnt);
  }
  if(d->dir)
    closedir(d->dir);
  free(d);
}

void close_dir(dir_cnt *d)
{
  if(d->dir)
    closedir(d->dir);
  d->dir = 0;
}

void dir_cnt_free_cnt(dir_cnt *d)
{
  if(d->cnt) {
    for(int n = 0; n < d->clen; n++)
      free((d->cnt)[n]);
    free(d->cnt);
    d->cnt = 0;
  }
}

void dir_cnt_clean(dir_cnt *d)
{
  dir_cnt_free_cnt(d);
  close_dir(d);
}



void dir_cnt_all(dir_cnt *d)
{
  puts(d->path);
  if(!d->dir)
    return;
  DIR *dir = d->dir;
  rewinddir(dir);
  struct dirent *dp;
  int count = 0;
  while(dp = readdir(dir)) {
    if(!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, ".."))
      continue;
    count++;
  }
  rewinddir(dir);
  dir_cnt_free_cnt(d);
  d->cnt = malloc(count * sizeof(char*));
  int i = 0;
  while(dp = readdir(dir)) {
    if(!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..")) {
      continue;
    }
    int size = strlen(dp->d_name) + 1;
    (d->cnt)[i] = malloc(size);
    strncpy((d->cnt)[i], dp->d_name, size); 
    i++;
  }
  d->clen = count;
}

dir_cnt *dir_cnt_new(const char *path)
{
  if(path[0] != '/')
    return 0;
  int tmplen = strlen(path);
  if(tmplen > 254)
    return 0;
  DIR * d = opendir(path);
  if(!d)
    return 0;
  dir_cnt *c = malloc(sizeof(*c));
  c->plen = tmplen;
  strcpy(c->path, path);
  c->dir = d;
  c->cnt = 0;
  c->clen = 0;
  dir_cnt_all(c);
  return c;
}

void dir_cnt_reload(dir_cnt *d)
{
  dir_cnt_clean(d);
  d->dir = opendir(d->path);
  if(!d->dir)
    return;
  dir_cnt_all(d);
}


int slash_count(char *p, int len)
{
  int c, n;
  c = n = 0;
  while(n < len)
    if(p[n++] == '/')
      c++;
  return c;
}

void dir_cnt_up(dir_cnt *d)
{
  dir_cnt_clean(d);
  int sc = slash_count(d->path, d->plen);
  if(sc <= 2) {
    d->plen = 1;
    strcpy(d->path, "/");
    dir_cnt_reload(d);
    return;
  }
  if(d->path[d->plen - 1] == '/')
    sc--;
  int tmpsc = 0;
  for(int n = 0; n < d->plen; n++) {
    if(d->path[n] == '/')
      tmpsc++;
    if(tmpsc == sc) {
      d->path[n + 1 ] = '\0';
      d->plen = n + 1;
      break;
    }
  }
  dir_cnt_reload(d);
}


bool is_dir(char *path)
{
  struct stat s;
  int err = stat(path, &s);
  if(err < 0)
    return false;
  return S_ISDIR(s.st_mode);
}

bool dir_cnt_to(dir_cnt *d, char *fp)
{

  int fplen = strlen(fp);
  if(fp[fplen - 1] != '/') {
    fp[fplen] = '/';
    fplen++;
    fp[fplen] = '\0';
  }
  strcpy(d->path, fp);
  dir_cnt_reload(d);
  return true;
}

