#include <stdlib.h>
#include <stdio.h>

#include "files.h"


/*
 * Sets output to point to a string containing the contents of file filename
 * Returns the length of the file
 */

int getfileasstring(char* filename, char** output) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    return -1;
  }

  fseek(fp, 0, SEEK_END);
  unsigned int length = ftell(fp);
  rewind(fp);


  char* contents = malloc(length);
  int status = fread(contents, sizeof(char), length/sizeof(char), fp);
  if (status <= 0) {
    return -1;
  }
  (*output) = contents;
  return length;
}
