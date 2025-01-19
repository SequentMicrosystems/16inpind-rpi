/*
 * 16in.c:
 *	Command-line interface to the Raspberry
 *	Pi's 16-inputs board.
 *	Copyright (c) 2016-2025 Sequent Microsystem
 *	<http://www.sequentmicrosystem.com>
 ***********************************************************************
 *	Author: Alexandru Burcea
 ***********************************************************************
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "16in.h"
#include "comm.h"

#define VERSION_BASE	(int)1
#define VERSION_MAJOR	(int)1
#define VERSION_MINOR	(int)2

#define UNUSED(X) (void)X      /* To avoid gcc/g++ warnings */
#define CMD_ARRAY_SIZE	7

const uint16_t pinMask[16] = { 0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400, 0x0200, 0x0100, 
			      0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
void usage(void)
{
	int i = 0;
	while (gCmdArray[i] != NULL)
	{
		if (gCmdArray[i]->name != NULL)
		{
			if (strlen(gCmdArray[i]->usage) > 2)
			{
				printf("%s", gCmdArray[i]->usage);
			}
		}
		i++;
	}
	printf("Where: <id> = Board id(stack level) = 0..7\n");
	printf("Type 16inpind -h <command> for more help\n");
}


// call with channel = 0 for reading all inputs
int chGet(int dev, u8 channel, int* state)
{
	u8 buff[2];
	int val = 0;
	int i = 0;

	if (NULL == state)
	{
		return ERROR;
	}

	if ( ( (channel < CHANNEL_NR_MIN) || (channel > CHANNEL_NR_MAX))
		&& (channel != 0))
	{
		printf("Invalid channel nr!\n");
		return ERROR;
	}

	if (FAIL == i2cMem8Read(dev, INPUTS16_INPORT_REG_ADD, buff, 2))
	{
		return ERROR;
	}
	val = (0xff & (~buff[0])) + ((0xff & (~buff[1])) << 8);
	if (0 == channel)
	{	
		*state = 0;
		for(i = 0; i< 16; i++)
		{
			if(val & pinMask[i])
			{
				*state += 1 << i;
			}
		}
	}
	else
	{
		if (val & pinMask[channel - 1])
		{
			*state = ON;
		}
		else
		{
			*state = OFF;
		}
	}
	return OK;
}

static int doRead(int argc, char *argv[]);
const CliCmdType CMD_READ =
	{
		"rd",
		2,
		&doRead,
		"  rd:	            Read inputs state\n",
		"  Usage:           16inpind <id> rd <channel>\n"
		"  Usage:           16inpind <id> rd\n",
		"  Example:         16inpind 0 rd 2; Read Status input channel #2 on Board #0\n"};

static int doRead(int argc, char *argv[])
{
	int pin = 0;
	int dev = 0;
	int state = -1;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 4)
	{
		pin = atoi(argv[3]);
		if ( (pin < CHANNEL_NR_MIN) || (pin > CHANNEL_NR_MAX))
		{
			printf("Opto channel number value out of range!\n");
			exit(1);
		}

		if (OK != chGet(dev, pin, &state))
		{
			printf("Fail to read!\n");
			exit(1);
		}
		printf("%d\n", state);
	}
	else if (argc == 3)
	{
		if (OK != chGet(dev, 0, &state))
		{
			printf("Fail to read!\n");
			exit(1);
		}
		printf("%d\n", state);
	}
	else
	{
		printf("%s", CMD_READ.usage);
		exit(1);
	}
	return 0;
}

int main(int argc, char *argv[])
{
	int i = 0;

	if (argc == 1)
	{
		usage();
		return -1;
	}
	while (NULL != gCmdArray[i])
	{
		if ( (gCmdArray[i]->name != NULL) && (gCmdArray[i]->namePos < argc))
		{
			if (strcasecmp(argv[gCmdArray[i]->namePos], gCmdArray[i]->name) == 0)
			{
				gCmdArray[i]->pFunc(argc, argv);
				return 0;
			}
		}
		i++;
	}
	printf("Invalid command option\n");
	usage();

	return -1;
}
