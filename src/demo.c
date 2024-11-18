#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json.h"
#include "hxjson-convert.h"

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

  char* name2 = hxjsonGetString("name2", json);
  char* name3 = hxjsonGetString("name3", json);
  float actualAge = hxjsonGetFloat("actualAge", json);
  int bounty = hxjsonGetInt("bounty", json);

  printf("%s %s, (%.2f), Bounty: %i\n", name2, name3, actualAge, bounty);

  free(name2);
  free(name3);

  hxjsonWrite("out.json", json);
  char* text2 = read_all("out.json");
  struct hxjson* json2 = hxjson(text2);

  name2 = hxjsonGetString("name2", json2);
  name3 = hxjsonGetString("name3", json2);
  bounty = hxjsonGetInt("bounty", json2);

  printf("%s %s, Bounty: %i\n", name2, name3, bounty);
  printf("stillworkd: %i\n", hxjsonGetBool("stillworks", json));

  free(name2);
  free(name3);

  hxjsonFree(json);
  hxjsonFree(json2);
  free(text);
  free(text2);
  
  return 0;
}
