#include <stdio.h>
#include <stdlib.h>
#include "json.h"

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
  char* text = malloc(fileLength+1);
  fread(text, 1, fileLength, fp);
  text[fileLength] = 0;
  fclose(fp);

  return text;
}

int main(void)
{
  char* text = read_all("test.json");

  struct hxjson* json = hxjson(text);
  char* name = hxjsonGet("hmm", json);
  printf("%s\n", name);
  free(name);
  
  hxjsonFree(json);
  free(text);
  
  return 0;
}
