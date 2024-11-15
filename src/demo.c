#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

  char* name2 = hxjsonGet("name2", json);
  char* name3 = hxjsonGet("name3", json);
  char* bounty = hxjsonGet("bounty", json);

  printf("%s %s, Bounty: %li\n", name2, name3, strtol(bounty, NULL, 0));

  free(name2);
  free(name3);
  free(bounty);

  hxjsonWrite("out.json", json);
  char* text2 = read_all("out.json");
  struct hxjson* json2 = hxjson(text2);

  name2 = hxjsonGet("name2", json2);
  name3 = hxjsonGet("name3", json2);
  bounty = hxjsonGet("bounty", json2);

  printf("%s %s, Bounty: %li\n", name2, name3, strtol(bounty, NULL, 0));

  free(name2);
  free(name3);
  free(bounty);

  hxjsonFree(json);
  hxjsonFree(json2);
  free(text);
  free(text2);
  
  return 0;
}
