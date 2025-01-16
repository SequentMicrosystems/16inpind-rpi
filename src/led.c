#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "comm.h"
#include "led.h"
#include "data.h"

const CliCmdType CMD_LED_READ = {
	"ledrd",
        2,
        &doLedRead,
        "  ledrd            Display the state of general purpose LEDS on the card\n",
        "  Usage 1:         "PROGRAM_NAME" <id> ledrd <led[1.."STR(LED_CH_NO)"]>\n"
        "  Usage 2:         "PROGRAM_NAME" <id> ledrd\n",
        "  Example:         "PROGRAM_NAME" 0 ledrd 2  Get the state of #2 on board #0\n"
};
int doLedRead(int argc, char *argv[]) {
        if(!(argc == 3 || argc == 4)) {
                return ARG_CNT_ERR;
        }
        int dev = doBoardInit(atoi(argv[1]));
        if(dev < 0) {
                return ERROR;
        }
        if(argc == 3) {
		uint8_t buf[1];
                if(OK != i2cMem8Read(dev, I2C_MEM_LEDS, buf, 1)) {
                        printf("Fail to read!\n");
                        return ERROR;
                }
		for(int led = 1; led <= LED_CH_NO; ++led) {
			if(buf[0] & (1 << (led - 1))) {
				printf("1 ");
			} else {
				printf("0 ");
			}
		}
		printf("\n");
        }
        else if(argc == 4) {
		uint8_t buf[1];
		if(OK != i2cMem8Read(dev, I2C_MEM_LEDS, buf, 1)) {
			printf("Fail to write!\n");
			return ERROR;
		}
		int led = atoi(argv[3]);
                if(!(1 <= led && led <= LED_CH_NO)) {
			printf("Led number out of range");
                        return ARG_RANGE_ERROR;
                }
		if(buf[0] & (1 << (led - 1))) {
			printf("1\n"); /* LED ON */
		} else {
			printf("0\n");
		}
        }
        return OK;
} 

const CliCmdType CMD_LED_WRITE = {
	"ledwr",
        2,
        &doLedWrite,
        "  ledwr            Set the state of general purpose LEDS on the card\n",
        "  Usage 1:         "PROGRAM_NAME" <id> ledwr <led[1.."STR(LED_CH_NO)"]> <state(0/1)>\n"
        "  Usage 2:         "PROGRAM_NAME" <id> ledwr <mask[0.."STR(MASK(LED_CH_NO))"]>\n",
        "  Example:         "PROGRAM_NAME" 0 ledwr 2 1  Turn ON the LED #2 on board #0\n"
};
int doLedWrite(int argc, char *argv[]) {
        if(!(argc == 4 || argc == 5)) {
                return ARG_CNT_ERR;
        }
        int dev = doBoardInit(atoi(argv[1]));
        if(dev < 0) {
                return ERROR;
        }
        if(argc == 4) {
                int mask = atoi(argv[3]);
                if(!(0 <= mask && mask <= (1 << LED_CH_NO))) {
                        return ARG_RANGE_ERROR;
                }
                uint8_t buf[1];
                buf[0] = 0xff & mask;
                if(OK != i2cMem8Write(dev, I2C_MEM_LEDS, buf, 1)) {
                        printf("Fail to write!\n");
                        return ERROR;
                }
        }
        else if(argc == 5) {
                int led = atoi(argv[3]);
                if(!(1 <= led && led <= LED_CH_NO)) {
			printf("Led number out of range");
                        return ARG_RANGE_ERROR;
                }
                int state = atoi(argv[4]);
                uint8_t buf[1];
                buf[0] = 0xff & led;
                if(state > 0) {
                        if(OK != i2cMem8Write(dev, I2C_MEM_LED_SET, buf, 1)) {
                                printf("Fail to write!\n");
                                return ERROR;
                        }
                }
                else {
                        if(OK != i2cMem8Write(dev, I2C_MEM_LED_CLR, buf, 1)) {
                                printf("Fail to write!\n");
                                return ERROR;
                        }
                }
        }
        return OK;
} 

int doLedModeRead(int argc, char *argv[]);
const CliCmdType CMD_LED_MODE_READ =
	{
		"ledmrd",
		2,
		&doLedModeRead,
		"  ledmrd:          Read "PROGRAM_NAME" led's mode: auto<0>; manual<1>\n",
		"  Usage:           "PROGRAM_NAME" <id> ledmrd <led[1.."STR(LED_CH_NO)"]>\n",
		"  Example:         "PROGRAM_NAME" 0 ledmrd 2; Read the led mode on channel #2 on Board #0\n"};

int doLedModeWrite(int argc, char *argv[]);
const CliCmdType CMD_LED_MODE_WRITE =
	{
		"ledmwr",
		2,
		&doLedModeWrite,
		"  ledmwr:          Write "PROGRAM_NAME" led's mode: auto<0>; manual<1>\n",
		"  Usage:           "PROGRAM_NAME" <id> ledmwr <led[1.."STR(LED_CH_NO)"]> <value>\n",
		"  Example:         "PROGRAM_NAME" 0 ledmwr 2 1; Write the led mode on channel #2 on Board #0 to manual\n"};

int ledGetMode(int dev, int ch, int* val)
{
	uint8_t buff[2];
	uint16_t readVal = 0;

	if (NULL == val)
	{
		return ERROR;
	}

	if (FAIL == i2cMem8Read(dev, I2C_MEM_LED_MODE, buff, 2))
	{
		return ERROR;
	}
	memcpy(&readVal, buff, 2);
	*val = 0x03 & (readVal >> (2 * (ch - 1)));
	return OK;
}

int ledSetMode(int dev, int ch, int val)
{
	uint8_t buff[2];
	uint16_t readVal = 0;

	if ( (val > 2) || (val < 0))
	{
		return ERROR;
	}
	if (FAIL == i2cMem8Read(dev, I2C_MEM_LED_MODE, buff, 2))
	{
		return ERROR;
	}
	memcpy(&readVal, buff, 2);

	readVal &= ~ ((uint16_t)0x03 << (2 * (ch - 1)));
	readVal += val << (2 * (ch - 1));
	memcpy(buff, &readVal, 2);
	if (FAIL == i2cMem8Write(dev, I2C_MEM_LED_MODE, buff, 2))
	{
		return ERROR;
	}
	return OK;
}
	

int doLedModeRead(int argc, char *argv[])
{
	int ch = 0;
	int val = 0;
	int dev = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 4)
	{
		ch = atoi(argv[3]);
		if ( (ch < MIN_CH_NO) || (ch > LED_CH_NO))
		{
			printf("RTD channel number value out of range!\n");
			exit(1);
		}

		if (OK != ledGetMode(dev, ch, &val))
		{
			printf("Fail to read!\n");
			exit(1);
		}
		printf("%d\n", val);
	}
	else
	{
		printf("Invalid arguments number for %s cmd\n", argv[0]);
		exit(1);
	}
	return OK;
}

int doLedModeWrite(int argc, char *argv[])
{
	int ch = 0;
	int val = 0;
	int dev = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 5)
	{
		ch = atoi(argv[3]);
		if ( (ch < MIN_CH_NO) || (ch > LED_CH_NO))
		{
			printf("RTD channel number value out of range!\n");
			exit(1);
		}

		val = atoi(argv[4]);

		if (OK != ledSetMode(dev, ch, val))
		{
			printf("Fail to write!\n");
			exit(1);
		}
	}
	else
	{
		printf("Invalid arguments number for %s cmd\n", argv[0]);
		exit(1);
	}
	return OK;
}

int doPowerLedModeRead(int argc, char *argv[]);
const CliCmdType CMD_POWER_LED_MODE_READ =
	{
		"ledplrd",
		2,
		&doPowerLedModeRead,
		"  ledpmrd:          Read "PROGRAM_NAME" power led's mode: blink<0>; solid<1>; off<2>\n",
		"  Usage:           "PROGRAM_NAME" <id> ledplrd\n",
		"  Example:         "PROGRAM_NAME" 0 ledplrd; Read the power led mode on Board #0\n"};

int doPowerLedModeWrite(int argc, char *argv[]);
const CliCmdType CMD_POWER_LED_MODE_WRITE =
	{
		"ledplwr",
		2,
		&doPowerLedModeWrite,
		"  ledplwr:          Write "PROGRAM_NAME" power led's mode: blink<0>; solid<1>; off<2>\n",
		"  Usage:           "PROGRAM_NAME" <id> ledplwr <value>\n",
		"  Example:         "PROGRAM_NAME" 0 ledplwr 1; Write the power led mode on Board #0 to solid<1>\n"};

int powerLedGetMode(int dev, int* val)
{
	uint8_t buff[1];
	uint8_t readVal = 0;

	if (NULL == val)
	{
		return ERROR;
	}

	if (FAIL == i2cMem8Read(dev, I2C_MEM_PWR_LED_MODE, buff, 1))
	{
		return ERROR;
	}
	memcpy(&readVal, buff, 1);
	*val = readVal;
	return OK;
}

int powerLedSetMode(int dev, int val)
{
	uint8_t buff[1];
	uint8_t readVal = 0;

	if ( (val > 3) || (val < 0))
	{
		return ERROR;
	}
	memcpy(buff, &readVal, 1);

	if (FAIL == i2cMem8Write(dev, I2C_MEM_PWR_LED_MODE, buff, 1))
	{
		return ERROR;
	}
	return OK;
}
	

int doPowerLedModeRead(int argc, char *argv[])
{
	int val = 0;
	int dev = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 3)
	{
		if (OK != powerLedGetMode(dev, &val))
		{
			printf("Fail to read!\n");
			exit(1);
		}
		printf("%d\n", val);
	}
	else
	{
		printf("Invalid arguments number for %s cmd\n", argv[0]);
		exit(1);
	}
	return OK;
}

int doPowerLedModeWrite(int argc, char *argv[])
{
	int val = 0;
	int dev = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 4)
	{
		val = atoi(argv[3]);

		if (OK != powerLedSetMode(dev, val))
		{
			printf("Fail to write!\n");
			exit(1);
		}
	}
	else
	{
		printf("Invalid arguments number for %s cmd\n", argv[0]);
		exit(1);
	}
	return OK;
}
