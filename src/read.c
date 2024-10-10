#include <stdio.h>
#include <stdlib.h>

char* read_all(const char* fPath)
{
  FILE* fp = fopen(fPath, "r");
  if (fp == NULL)
  {
    perror(fPath);
    return "";
  }
  fseek(fp, 0, SEEK_END);
  size_t fileLength = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  /* Load file into memory */
  char* text = malloc(fileLength);
  fread(text, 1, fileLength, fp);
  text[fileLength] = 0;
  fclose(fp);

  return text;
}
