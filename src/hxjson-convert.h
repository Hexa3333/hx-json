#ifndef HXJSON_CONVERT_H
#define HXJSON_CONVERT_H

#include "json.h"

// Type definitions for getters; feel free to modify
// (You may have to modify the libc logic too)
typedef int hxjsonInt;
typedef float hxjsonFloat;
typedef bool hxjsonBool;

// Gets the value of key, converts it to string (0 if not found)
char* hxjsonGetString(char* key, struct hxjson* json);
// Gets the value of key, converts it to hxjsonInt (0 if not found)
hxjsonInt hxjsonGetInt(char* key, struct hxjson* json);
// Gets the value of key, converts it to hxjsonFloat (0 if not found)
hxjsonFloat hxjsonGetFloat(char* key, struct hxjson* json);
// Gets the value of key, converts it to hxjsonBool (0 if not found)
hxjsonBool hxjsonGetBool(char* key, struct hxjson* json);


// Gets the value of key[index], converts it to string (0 if not found)
char* hxjsonGetArrString(char* key, int index, struct hxjson* json);
// Gets the value of key[index], converts it to hxjsonInt (0 if not found)
hxjsonInt hxjsonGetArrInt(char* key, int index, struct hxjson* json);
// Gets the value of key[index], converts it to hxjsonFloat (0 if not found)
hxjsonFloat hxjsonGetArrFloat(char* key, int index, struct hxjson* json);
// Gets the value of key[index], converts it to hxjsonBool (0 if not found)
hxjsonBool hxjsonGetArrBool(char* key, int index, struct hxjson* json);


#endif // HXJSON_CONVERT_H
