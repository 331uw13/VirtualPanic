#ifndef VIRTUAL_PANIC__TYPES_H
#define VIRTUAL_PANIC__TYPES_H

typedef unsigned char        uint8;
typedef unsigned short       uint16;
typedef unsigned int         uint32;
typedef unsigned long int    uint64;

typedef char                 int8;
typedef short                int16;
typedef int                  int32;
typedef long int             int64;


typedef struct {
	float x;
	float y;
	float z;
} Vector3;

typedef struct {
	uint8 r;
	uint8 g;
	uint8 b;
	uint8 a;
} RGBA;

#endif
