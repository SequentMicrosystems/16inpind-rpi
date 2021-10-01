#ifndef IN_16_H_
#define IN_16_H_

#include <stdint.h>

#define	UNU	__attribute__((unused))

#define RETRY_TIMES	10
#define INPUTS16_INPORT_REG_ADD	0x00
#define INPUTS16_OUTPORT_REG_ADD	0x02
#define INPUTS16_POLINV_REG_ADD	0x04
#define INPUTS16_CFG_REG_ADD		0x06

#define CHANNEL_NR_MIN		1
#define CHANNEL_NR_MAX		16

#define ERROR	-1
#define OK		0
#define FAIL	-1

#define INPUT16_HW_I2C_BASE_ADD	0x20
typedef uint8_t u8;
typedef uint16_t u16;

typedef enum
{
	OFF = 0,
	ON,
	STATE_COUNT
} OutStateEnumType;

typedef struct
{
 const char* name;
 const int namePos;
 void(*pFunc)(int, char**);
 const char* help;
 const char* usage1;
 const char* usage2;
 const char* example;
}CliCmdType;

const CliCmdType* gCmdArray[];

#endif //IN_16_H_
