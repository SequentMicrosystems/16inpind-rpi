/*
 * 16in.c:
 *	Command-line interface to the Raspberry
 *	Pi's 16-inputs board.
 *	Copyright (c) 2016-2020 Sequent Microsystem
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
#define VERSION_MAJOR	(int)0
#define VERSION_MINOR	(int)0

#define UNUSED(X) (void)X      /* To avoid gcc/g++ warnings */
#define CMD_ARRAY_SIZE	7

const uint16_t pinMask[16] = { 0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400, 0x0200, 0x0100, 
			      0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
char *warranty =
	"	       Copyright (c) 2016-2021 Sequent Microsystems\n"
		"                                                             \n"
		"		This program is free software; you can redistribute it and/or modify\n"
		"		it under the terms of the GNU Leser General Public License as published\n"
		"		by the Free Software Foundation, either version 3 of the License, or\n"
		"		(at your option) any later version.\n"
		"                                    \n"
		"		This program is distributed in the hope that it will be useful,\n"
		"		but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
		"		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
		"		GNU Lesser General Public License for more details.\n"
		"			\n"
		"		You should have received a copy of the GNU Lesser General Public License\n"
		"		along with this program. If not, see <http://www.gnu.org/licenses/>.";

void usage(void)
{
	int i = 0;
	while (gCmdArray[i] != NULL)
	{
		if (gCmdArray[i]->name != NULL)
		{
			if (strlen(gCmdArray[i]->usage1) > 2)
			{
				printf("%s", gCmdArray[i]->usage1);
			}
			if (strlen(gCmdArray[i]->usage2) > 2)
			{
				printf("%s", gCmdArray[i]->usage2);
			}
		}
		i++;
	}
	printf("Where: <stack> = Board stack level id = 0..7\n");
	printf("Type 16inpind -h <command> for more help\n");
}

int doBoardInit(int stack)
{
	int dev = 0;
	int add = 0;

	if ( (stack < 0) || (stack > 7))
	{
		printf("Invalid stack level [0..7]!");
		return ERROR;
	}
	add = (stack + INPUT16_HW_I2C_BASE_ADD) ^ 0x07;
	dev = i2cSetup(add);
	if (dev == -1)
	{
		return ERROR;
	}
	return dev;
}

int boardCheck(int stack)
{
	int dev = 0;
	int add = 0;
	uint8_t buff[8];

	if ( (stack < 0) || (stack > 7))
	{
		printf("Invalid stack level [0..7]!");
		return ERROR;
	}
	add = (stack + INPUT16_HW_I2C_BASE_ADD) ^ 0x07;
	dev = i2cSetup(add);
	if (dev == -1)
	{
		return ERROR;
	}
	if (ERROR == i2cMem8Read(dev, INPUTS16_INPORT_REG_ADD, buff, 2))
	{
		return ERROR;
	}
	return OK;
}

static void doHelp(int argc, char *argv[]);
const CliCmdType CMD_HELP =
{
	"-h",
	1,
	&doHelp,
	"\t-h		Display the list of command options or one command option details\n",
	"\tUsage:		16inpind -h    Display command options list\n",
	"\tUsage:		16inpind -h <param>   Display help for <param> command option\n",
	"\tExample:	16inpind -h r    Display help for \"r\" command option\n"};

static void doHelp(int argc, char *argv[])
{
	int i = 0;
	if (argc == 3)
	{
		while (NULL != gCmdArray[i])
		{
			if (gCmdArray[i]->name != NULL)
			{
				if (strcasecmp(argv[2], gCmdArray[i]->name) == 0)
				{
					printf("%s%s%s%s", gCmdArray[i]->help, gCmdArray[i]->usage1,
						gCmdArray[i]->usage2, gCmdArray[i]->example);
					break;
				}
			}
			i++;
		}
		if (NULL == gCmdArray[i])
		{
			printf("Option \"%s\" not found\n", argv[2]);
			i = 0;
			while (NULL != gCmdArray[i])
			{
				if (gCmdArray[i]->name != NULL)
				{
					printf("%s", gCmdArray[i]->help);
					break;
				}
				i++;
			}
		}
	}
	else
	{
		i = 0;
		while (NULL != gCmdArray[i])
		{
			if (gCmdArray[i]->name != NULL)
			{
				printf("%s", gCmdArray[i]->help);
			}
			i++;
		}
	}
}

static void doVersion(int argc, char *argv[]);
const CliCmdType CMD_VERSION =
{
	"-v",
	1,
	&doVersion,
	"\t-v		Display the 16inpind command version number\n",
	"\tUsage:		16inpind -v\n",
	"",
	"\tExample:	16inpind -v  Display the version number\n"};

static void doVersion(int argc, char *argv[])
{
	UNUSED(argc);
	UNUSED(argv);
	printf(
		"16inpind command line interface v%d.%d.%d Copyright (c) 2016 - 2020 Sequent Microsystems\n",
		VERSION_BASE, VERSION_MAJOR, VERSION_MINOR);
	printf("\nThis is free software with ABSOLUTELY NO WARRANTY.\n");
	printf("For details type: 16inpind -warranty\n");

}

static void doWarranty(int argc, char* argv[]);
const CliCmdType CMD_WAR =
{
	"-warranty",
	1,
	&doWarranty,
	"\t-warranty	Display the warranty\n",
	"\tUsage:		16inpind -warranty\n",
	"",
	"\tExample:	16inpind -warranty  Display the warranty text\n"};

static void doWarranty(int argc UNU, char* argv[] UNU)
{
	printf("%s\n", warranty);
}

static void doList(int argc, char *argv[]);
const CliCmdType CMD_LIST =
	{
		"-list",
		1,
		&doList,
		"\t-list:		List all 16inpind boards connected\n\t\t\treturn the # of boards and stack level for every board\n",
		"\tUsage:		16inpind -list\n",
		"",
		"\tExample:	16inpind -list display: 1,0 \n"};

static void doList(int argc, char *argv[])
{
	int ids[8];
	int i;
	int cnt = 0;

	UNUSED(argc);
	UNUSED(argv);

	for (i = 0; i < 8; i++)
	{
		if (boardCheck(i) == OK)
		{
			ids[cnt] = i;
			cnt++;
		}
	}
	printf("%d board(s) detected\n", cnt);
	if (cnt > 0)
	{
		printf("Id:");
	}
	while (cnt > 0)
	{
		cnt--;
		printf(" %d", ids[cnt]);
	}
	printf("\n");
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

static void doRead(int argc, char *argv[]);
const CliCmdType CMD_READ =
	{
		"rd",
		2,
		&doRead,
		"\trd:		Read inputs state\n",
		"\tUsage:		16inpind <stack> rd <channel>\n",
		"\tUsage:		16inpind <stack> rd\n",
		"\tExample:	16inpind 0 rd 2; Read Status input channel #2 on Board #0\n"};

static void doRead(int argc, char *argv[])
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
		printf("%s", CMD_READ.usage1);
		printf("%s", CMD_READ.usage2);
		exit(1);
	}
}

const CliCmdType* gCmdArray[] =
{
	&CMD_VERSION,
	&CMD_HELP,
	&CMD_WAR,
	&CMD_LIST,
	&CMD_READ,
	NULL}; //null terminated array of cli structure pointers

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
