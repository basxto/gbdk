#ifndef ASM_GBZ80_TYPES_INCLUDE
#define ASM_GBZ80_TYPES_INCLUDE

#if SDCC_PORT!=gbz80
#error gbz80 only.
#endif

typedef char          	INT8;
typedef unsigned char 	UINT8;
typedef int           	INT16;
typedef unsigned int  	UINT16;
typedef long          	INT32;
typedef unsigned long 	UINT32;

typedef int	      	size_t;

#endif
