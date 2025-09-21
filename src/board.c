#include "board.h"
#include "comm.h"
#include "data.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define VERSION_BASE	(int)1
#define VERSION_MAJOR	(int)1
#define VERSION_MINOR	(int)3


static int doHelp(int argc, char *argv[]);
const CliCmdType CMD_HELP =
{
	"-h",
	1,
	&doHelp,
	"  -h	            Display the list of command options or one command option details\n",
	"  Usage:           16inpind -h    Display command options list\n"
	"  Usage:           16inpind -h <param>   Display help for <param> command option\n",
	"  Example:         16inpind -h r    Display help for \"r\" command option\n"};

static int doHelp(int argc, char *argv[])
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
					printf("%s%s%s", gCmdArray[i]->help, gCmdArray[i]->usage,
						gCmdArray[i]->example);
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
	return 0;
}

static int doVersion(int argc, char *argv[]);
const CliCmdType CMD_VERSION =
{
	"-v",
	1,
	&doVersion,
	"  -v	            Display the 16inpind command version number\n",
	"  Usage:           16inpind -v\n",
	"  Example:         16inpind -v  Display the version number\n"};

static int doVersion(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	printf(
		"16inpind command line interface v%d.%d.%d Copyright (c) 2016 - 2025 Sequent Microsystems\n",
		VERSION_BASE, VERSION_MAJOR, VERSION_MINOR);
	printf("\nThis is free software with ABSOLUTELY NO WARRANTY.\n");
	printf("For details type: 16inpind -warranty\n");
	return 0;
}
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

static int doWarranty(int argc, char* argv[]);
const CliCmdType CMD_WAR =
{
	"-warranty",
	1,
	&doWarranty,
	"  -warranty        Display the warranty\n",
	"  Usage:           16inpind -warranty\n",
	"  Example:         16inpind -warranty  Display the warranty text\n"};

static int doWarranty(int argc, char* argv[])
{
	(void)argc;
	(void)argv;
	printf("%s\n", warranty);
	return 0;
}

static int doList(int argc, char *argv[]);
const CliCmdType CMD_LIST =
{
	"-list",
	1,
	&doList,
	"  -list:           List all 16inpind boards connected\n      return the # of boards and stack level for every board\n",
	"  Usage:           16inpind -list\n",
	"  Example:         16inpind -list display: 1,0 \n"};
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

static int doList(int argc, char *argv[])
{
	int ids[8];
	int i;
	int cnt = 0;

	(void)argc;
	(void)argv;

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
	return 0;
}

static int doBoard(int argc, char *argv[]);
const CliCmdType CMD_BOARD =
	{
		"board",
		2,
		&doBoard,
		"  board:	        Read card firmware version \n",
		"  Usage:           16inpind <id> board <channel>\n"
		"",
		"  Example:         16inpind 0 board; Read firmware version on Board #0\n"};

static int doBoard(int argc, char *argv[])
{
	int dev = 0;
	uint8_t buff[8];

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}
	if (argc == 3)
	{
		if (ERROR == i2cMem8Read(dev, I2C_MEM_REVISION_MAJOR_ADD, buff, 2))
		{
			return ERROR;
		}
		printf("Sixteen LV Digital Inputs firmware version %d.%02d\n", (int)buff[0], (int)buff[1]);
	}
	else
	{
		return ERROR;
	}
	return OK;
}
