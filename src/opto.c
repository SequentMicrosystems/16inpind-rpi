#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "comm.h"
#include "data.h"
#include "opto.h"

// TODO: Add ranges in all error messages

bool badOptoCh(uint8_t ch)
{
	if (! (MIN_CH_NO <= ch && ch <= OPTO_CH_NO))
	{
		printf("Optocoupled channel number value out of range![%d..%d]\n",
			MIN_CH_NO, OPTO_CH_NO);
		return true;
	}
	return false;
}
bool badOptoEncCh(uint8_t ch)
{
	if (! (MIN_CH_NO <= ch && ch <= OPTO_CH_NO / 2))
	{
		printf("Optocoupled encoded channel number value out of range![%d..%d]\n",
			MIN_CH_NO, OPTO_CH_NO / 2);
		return true;
	}
	return false;
}

int optoChGet(int dev, uint8_t ch, State *state)
{
	if (NULL == state)
	{
		return ERROR ;
	}
	if (badOptoCh(ch))
	{
		printf("Invalid opto ch nr!\n");
		return ERROR ;
	}
	uint8_t buf[2];
	if (OK != i2cMem8Read(dev, I2C_MEM_OPTO, buf, 2))
	{
		return ERROR ;
	}
	uint16_t val = 0;
	memcpy(&val, buf, 2);
	if (val & (1 << (ch - 1)))
	{
		*state = ON;
	}
	else
	{
		*state = OFF;
	}
	return OK ;
}

int optoGet(int dev, int *val)
{
	uint8_t buff[2];
	if (NULL == val)
	{
		return ERROR ;
	}
	if (OK != i2cMem8Read(dev, I2C_MEM_OPTO, buff, 2))
	{
		return ERROR ;
	}
	memcpy(val, buff, 2);
	return OK ;
}

int optoEdgeGet(int dev, uint8_t ch, uint8_t *val)
{
	if (NULL == val)
	{
		return ERROR ;
	}
	if (badOptoCh(ch))
	{
		printf("Invalid opto ch nr!\n");
		return ERROR ;
	}
	uint8_t buf[4];
	if (OK != i2cMem8Read(dev, I2C_MEM_OPTO_IT_RISING_ADD, buf, 4))
	{
		return ERROR ;
	}
	uint16_t rising = 0; //buf[0];
	uint16_t falling = 0; //buf[1];
	memcpy(&rising, buf, 2);
	memcpy(&falling, &buf[2], 2);
	*val = 0;
	if (rising & (1 << (ch - 1)))
	{
		*val |= 1 << 0;
	}
	if (falling & (1 << (ch - 1)))
	{
		*val |= 1 << 1;
	}
	return OK ;
}

int optoEdgeSet(int dev, uint8_t ch, uint8_t val)
{
	if (badOptoCh(ch))
	{
		return ERROR ;
	}
	uint8_t buf[4];
	if (OK != i2cMem8Read(dev, I2C_MEM_OPTO_IT_RISING_ADD, buf, 4))
	{
		return ERROR ;
	}
	uint16_t rising = 0; //buf[0];
	uint16_t falling = 0; //buf[1];
	memcpy(&rising, buf, 2);
	memcpy(&falling, &buf[2], 2);
	uint32_t mask = 1 << (ch - 1);
	if (val & 1 << 0)
	{ //check rising
		rising |= mask;
	}
	else
	{
		rising &= ~mask;
	}
	if (val & 1 << 1)
	{ //check falling
		falling |= mask;
	}
	else
	{
		falling &= ~mask;
	}
	//buf[0] = rising;
	//buf[1] = falling;
	memcpy(buf, &rising, 2);
	memcpy(&buf[2], &falling, 2);

	if (OK != i2cMem8Write(dev, I2C_MEM_OPTO_IT_RISING_ADD, buf, 4))
	{
		return ERROR ;
	}
	return OK ;
}

int optoCountGet(int dev, uint8_t ch, uint32_t *val)
{
	if (badOptoCh(ch))
	{
		return ERROR ;
	}
	if (NULL == val)
	{
		return ERROR ;
	}
	uint8_t buf[4];
	if (OK
		!= i2cMem8Read(dev, I2C_MEM_OPTO_EDGE_COUNT_ADD + COUNTER_SIZE * (ch - 1),
			buf, COUNTER_SIZE))
	{
		return ERROR ;
	}
	memcpy(val, buf, 4);
	return OK ;
}

int optoFreqGet(int dev, uint8_t ch, uint16_t *val)
{
	if (badOptoCh(ch))
	{
		return ERROR ;
	}
	if (NULL == val)
	{
		return ERROR ;
	}
	uint8_t buf[2];
	if (OK
		!= i2cMem8Read(dev,
			I2C_MEM_IN_FREQENCY + OPTO_FREQUENCY_DATA_SIZE * (ch - 1), buf,
			OPTO_FREQUENCY_DATA_SIZE))
	{
		return ERROR ;
	}
	memcpy(val, buf, OPTO_FREQUENCY_DATA_SIZE);
	return OK ;
}

int optoPWMFillGet(int dev, uint8_t ch, float *val)
{
	if (badOptoCh(ch))
	{
		return ERROR ;
	}
	if (NULL == val)
	{
		return ERROR ;
	}
	uint8_t buf[2];
	if (OK
		!= i2cMem8Read(dev,
			I2C_MEM_PWM_IN_FILL + OPTO_FREQUENCY_DATA_SIZE * (ch - 1), buf,
			OPTO_FREQUENCY_DATA_SIZE))
	{
		return ERROR ;
	}
	uint16_t raw = 0;
	memcpy(&raw, buf, OPTO_FREQUENCY_DATA_SIZE);
	*val = (float)raw / OPTO_FILL_FACTOR_SCALE;
	return OK ;
}

int optoCountReset(int dev, uint8_t ch)
{
	if (badOptoCh(ch))
	{
		return ERROR ;
	}
	if (OK != i2cMem8Write(dev, I2C_MEM_OPTO_CNT_RST_ADD, &ch, 1))
	{
		return ERROR ;
	}
	return OK ;
}

int optoEncStateRead(int dev, uint8_t ch, uint8_t *val)
{
	if (badOptoEncCh(ch))
	{
		return ERROR ;
	}
	if (NULL == val)
	{
		return ERROR ;
	}
	uint8_t buf[1];
	if (OK != i2cMem8Read(dev, I2C_MEM_OPTO_ENC_ENABLE_ADD, buf, 1))
	{
		return ERROR ;
	}
	if ( (1 << (ch - 1)) & *buf)
	{
		*val = 1;
	}
	else
	{
		*val = 0;
	}
	return OK ;
}

int optoEncStateWrite(int dev, uint8_t ch, uint8_t val)
{
	if (badOptoCh(ch))
	{
		return ERROR ;
	}
	uint8_t buf[1];
	if (OK != i2cMem8Read(dev, I2C_MEM_OPTO_ENC_ENABLE_ADD, buf, 1))
	{
		return ERROR ;
	}
	uint32_t mask = 1 << (ch - 1);
	if (val != 0)
	{
		*buf |= mask;
	}
	else
	{
		*buf &= ~mask;
	}
	if (OK != i2cMem8Write(dev, I2C_MEM_OPTO_ENC_ENABLE_ADD, buf, 1))
	{
		return ERROR ;
	}
	return OK ;
}

int optoEncGetCnt(int dev, uint8_t ch, int *val)
{
	if (badOptoEncCh(ch))
	{
		return ERROR ;
	}
	if (NULL == val)
	{
		return ERROR ;
	}
	uint8_t buf[4];
	if (OK
		!= i2cMem8Read(dev, I2C_MEM_OPTO_ENC_COUNT_ADD + COUNTER_SIZE * (ch - 1),
			buf, COUNTER_SIZE))
	{
		return ERROR ;
	}
	memcpy(val, buf, 4);
	return OK ;
}

int optoEncRstCnt(int dev, uint8_t ch)
{
	if (badOptoEncCh(ch))
	{
		return ERROR ;
	}
	if (OK != i2cMem8Write(dev, I2C_MEM_OPTO_ENC_CNT_RST_ADD, &ch, 1))
	{
		return ERROR ;
	}
	return OK ;
}

int optoIntSet(int dev, uint8_t ch, uint8_t val)
{
	if (badOptoCh(ch))
	{
		return ERROR ;
	}
	uint8_t buf[2];
	if (OK != i2cMem8Read(dev, I2C_MEM_EXTI_EN_ADD, buf, 2))
	{
		return ERROR ;
	}
	uint16_t interrupt = 0;
	memcpy(&interrupt, buf, 2);
	
	uint32_t mask = 1 << (ch - 1);
	if (val)
	{ 
		interrupt |= mask;
	}
	else
	{
		interrupt &= ~mask;
	}
	memcpy(buf, &interrupt, 2);
	if (OK != i2cMem8Write(dev, I2C_MEM_EXTI_EN_ADD, buf, 2))
	{
		return ERROR ;
	}
	return OK ;
}

int optoIntRead(int dev, uint8_t ch, uint8_t *val)
{
	if (badOptoCh(ch))
	{
		return ERROR ;
	}
	if (NULL == val)
	{
		return ERROR ;
	}
	uint8_t buf[2];
	if (OK != i2cMem8Read(dev, I2C_MEM_EXTI_EN_ADD, buf, 2))
	{
		return ERROR ;
	}
	uint16_t interrupt = 0;
	memcpy(&interrupt, buf, 2);
	if ( (1 << (ch - 1)) & interrupt)
	{
		*val = 1;
	}
	else
	{
		*val = 0;
	}
	return OK ;
}

const CliCmdType CMD_OPTO_READ =
{
	"optrd",
	2,
	&doOptoRead,
	"  optrd            Read optocoupled inputs status\n",
	"  Usage:           "PROGRAM_NAME" <id> optrd <channel>\n"
	"  Usage:           "PROGRAM_NAME" <id> optrd\n",
	"  Example:         "PROGRAM_NAME" 0 optrd 2; Read Status of Optocoupled input ch #2 on Board #0\n"
};
int doOptoRead(int argc, char *argv[])
{
	int dev = doBoardInit(atoi(argv[1]));
	if (dev < 0)
	{
		return ERROR ;
	}
	if (argc == 4)
	{
		int channel = atoi(argv[3]);
		if (badOptoCh(channel))
		{
			return ARG_RANGE_ERROR;
		}
		State state = STATE_COUNT;
		if (OK != optoChGet(dev, channel, &state))
		{
			printf("Fail to read!\n");
			return ERROR ;
		}
		if (state != OFF)
		{
			printf("1\n");
		}
		else
		{
			printf("0\n");
		}
	}
	else if (argc == 3)
	{
		int val;
		if (OK != optoGet(dev, &val))
		{
			printf("Fail to read!\n");
			return ERROR ;
		}
		printf("%d\n", val);
	}
	else
	{
		return ARG_CNT_ERR;
	}
	return OK ;
}

const CliCmdType CMD_OPTO_EDGE_READ =
{
	"optedgerd",
	2,
	&doOptoEdgeRead,
	"  optedgerd        Read optocoupled counting edges 0 - none; 1 - rising; 2 - falling; 3 - both\n",
	"  Usage:           "PROGRAM_NAME" <id> optedgerd <channel>\n",
	"  Example:         "PROGRAM_NAME" 0 optedgerd 2; Read counting edges of optocoupled channel #2 on Board #0\n"
};
int doOptoEdgeRead(int argc, char *argv[])
{
	if (argc != 4)
	{
		return ARG_CNT_ERR;
	}
	int dev = doBoardInit(atoi(argv[1]));
	if (dev < 0)
	{
		return ERROR ;
	}
	uint8_t channel = (uint8_t)atoi(argv[3]);
	if (badOptoCh(channel))
	{
		return ARG_RANGE_ERROR;
	}
	uint8_t val;
	if (OK != optoEdgeGet(dev, channel, &val))
	{
		printf("Fail to read!\n");
		return ERROR ;
	}
	printf("%d\n", val);
	return OK ;
}

const CliCmdType CMD_OPTO_EDGE_WRITE =
{
	"optedgewr",
	2,
	&doOptoEdgeWrite,
	"  optedgewr        Set optocoupled channel counting edges  0- count disable;\n"
	"                   1-count rising edges; 2 - count falling edges; 3 - count both edges\n",
	"  Usage:           "PROGRAM_NAME" <id> optedgewr <channel> <edges> \n",
	"  Example:         "PROGRAM_NAME" 0 optedgewr 2 1; Set Optocoupled channel #2 on Board #0 to count rising edges\n"
};
int doOptoEdgeWrite(int argc, char *argv[])
{
	if ( (argc != 5))
	{
		return ARG_CNT_ERR;
	}
	int dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR ;
	}
	int channel = atoi(argv[3]);
	if (badOptoCh(channel))
	{
		return ARG_RANGE_ERROR;
	}
	uint8_t state = 0;
	if (strcasecmp(argv[4], "none") == 0)
	{
		state = 0;
	}
	else if (strcasecmp(argv[4], "up") == 0
		|| strcasecmp(argv[4], "rising") == 0)
	{
		state = 1;
	}
	else if (strcasecmp(argv[4], "down") == 0
		|| strcasecmp(argv[4], "falling") == 0)
	{
		state = 2;
	}
	else if (strcasecmp(argv[4], "both") == 0)
	{
		state = 3;
	}
	else
	{
		state = (uint8_t)atoi(argv[4]);
		// TODO: FIXME: bug: atoi returns 0 even if argv[4] is not a number
		if (! (state <= 3))
		{
			printf("Invalid edge counting type [0..3]!\n");
			return ARG_RANGE_ERROR;
		}
	}
	if (OK != optoEdgeSet(dev, channel, state))
	{
		printf("Fail to write optocoupled ch edge counting \n");
		return ERROR ;
	}
	return OK ;
}

const CliCmdType CMD_OPTO_CNT_READ =
{
	"optcntrd",
	2,
	&doOptoCntRead,
	"  optcntrd         Read optocoupled inputs edges count for one channel\n",
	"  Usage:           "PROGRAM_NAME" <id> optcntrd <channel>\n",
	"  Example:         "PROGRAM_NAME" 0 optcntrd 2; Read contor of opto input #2 on Board #0\n"
};
int doOptoCntRead(int argc, char *argv[])
{
	if (argc != 4)
	{
		return ARG_CNT_ERR;
	}
	int dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR ;
	}
	uint8_t channel = 0;
	channel = atoi(argv[3]);
	if (badOptoCh(channel))
	{
		printf("Optocoupled ch number value out of range!\n");
		return ARG_RANGE_ERROR;
	}
	uint32_t val = 0;
	if (OK != optoCountGet(dev, channel, &val))
	{
		printf("Fail to read!\n");
		return ERROR ;
	}
	printf("%u\n", val);
	return OK ;
}

const CliCmdType CMD_OPTO_CNT_RESET =
{
	"optcntrst",
	2,
	&doOptoCntReset,
	"  optcntrst        Reset optocoupled inputs edges count for one channel\n",
	"  Usage:           "PROGRAM_NAME" <id> optcntrst <channel>\n",
	"  Example:         "PROGRAM_NAME" 0 optcntrst 2; Reset contor of opto input #2 on Board #0\n"
};
int doOptoCntReset(int argc, char *argv[])
{
	if (argc != 4)
	{
		return ARG_CNT_ERR;
	}
	int dev = doBoardInit(atoi(argv[1]));
	if (dev < 0)
	{
		return ERROR ;
	}
	uint8_t channel = 0;
	channel = atoi(argv[3]);
	if (badOptoCh(channel))
	{
		printf("Optocoupled ch number value out of range!\n");
		return ARG_RANGE_ERROR;
	}
	if (OK != optoCountReset(dev, channel))
	{
		printf("Fail to reset!\n");
		return ERROR ;
	}
	return OK ;
}

const CliCmdType CMD_OPTO_ENC_READ =
{
	"optencrd",
	2,
	&doOptoEncoderRead,
	"  optencrd         Read optocoupled quadrature encoder state 0- disabled 1 - enabled\n",
	"  Usage:           "PROGRAM_NAME" <id> optencrd <channel>\n",
	"  Example:         "PROGRAM_NAME" 0 optencrd 2; Read state of optocoupled encoder channel #2 on Board #0\n"
};
int doOptoEncoderRead(int argc, char *argv[])
{
	if (argc != 4)
	{
		return ARG_CNT_ERR;
	}
	int dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR ;
	}
	uint8_t channel = atoi(argv[3]);
	if (badOptoEncCh(channel))
	{
		printf("Optocoupled encoder number value out of range!\n");
		return ARG_RANGE_ERROR;
	}
	uint8_t val = 0;
	if (OK != optoEncStateRead(dev, channel, &val))
	{
		printf("Fail to read!\n");
		return ERROR ;
	}
	printf("%d\n", val);
	return OK ;
}

const CliCmdType CMD_OPTO_ENC_WRITE =
{
	"optencwr",
	2,
	&doOptoEncoderWrite,
	"  optencwr         Enable / Disable optocoupled quadrature encoder, encoder 1 \n"
	"                   connected to opto ch1 and 2, encoder 2 on ch3 and 4 ... \n",
	"  Usage:           "PROGRAM_NAME" <id> optencwr <channel> <0/1> \n",
	"  Example:         "PROGRAM_NAME" 0 optencwr 2 1; Enable encoder on opto channel 3/4  on Board stack level 0\n"
};
int doOptoEncoderWrite(int argc, char *argv[])
{
	if ( (argc != 5))
	{
		return ARG_CNT_ERR;
	}
	int dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR ;
	}
	int channel = 0;
	channel = atoi(argv[3]);
	if (badOptoEncCh(channel))
	{
		printf("Optocoupled encoder number value out of range [1..4]\n");
		return ARG_RANGE_ERROR;
	}
	uint8_t state = atoi(argv[4]);
	if (OK != optoEncStateWrite(dev, channel, state))
	{
		printf("Fail to write encoder State\n");
		return ERROR ;
	}
	return OK ;
}

const CliCmdType CMD_OPTO_ENC_CNT_READ =
{
	"optcntencrd",
	2,
	&doOptoEncoderCntRead,
	"  optcntencrd      Read optocoupled encoder count for one channel\n",
	"  Usage:           "PROGRAM_NAME" <id> optcntencrd <channel>\n",
	"  Example:         "PROGRAM_NAME" 0 optcntencrd 2; Read contor of opto encoder #2 on Board #0\n"
};
int doOptoEncoderCntRead(int argc, char *argv[])
{
	if (argc != 4)
	{
		return ARG_CNT_ERR;
	}
	int dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR ;
	}
	uint8_t channel = atoi(argv[3]);
	if (badOptoEncCh(channel))
	{
		return ARG_RANGE_ERROR;
	}
	int val = 0;
	if (OK != optoEncGetCnt(dev, channel, &val))
	{
		printf("Fail to read!\n");
		return ERROR ;
	}
	printf("%d\n", val);
	return OK ;
}

const CliCmdType CMD_OPTO_ENC_CNT_RESET =
{
	"optcntencrst",
	2,
	&doOptoEncoderCntReset,
	"  optcntencrst     Reset optocoupled encoder count \n",
	"  Usage:           "PROGRAM_NAME" <id> optcntencrst <channel>\n",
	"  Example:         "PROGRAM_NAME" 0 optcntencrst 2; Reset contor of encoder #2 on Board #0\n"
};
int doOptoEncoderCntReset(int argc, char *argv[])
{
	if (argc != 4)
	{
		return ARG_CNT_ERR;
	}
	int dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR ;
	}
	uint8_t channel = 0;
	channel = atoi(argv[3]);
	if (badOptoEncCh(channel))
	{
		return ARG_RANGE_ERROR;
	}
	if (OK != optoEncRstCnt(dev, channel))
	{
		printf("Fail to reset!\n");
		return ERROR ;
	}
	return OK ;
}

const CliCmdType CMD_OPTO_FREQUENCY_READ =
{
	"optfrd",
	2,
	&doOptoFreqRead,
	"  optfrd         Read optocoupled inputs signal frequency in Hz for one channel\n",
	"  Usage:           "PROGRAM_NAME" <id> optfrd <channel>\n",
	"  Example:         "PROGRAM_NAME" 0 optfrd 2; Read the frequency of the sygnal applied to opto input #2 on Board #0\n"
};
int doOptoFreqRead(int argc, char *argv[])
{
	if (argc != 4)
	{
		return ARG_CNT_ERR;
	}
	int dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR ;
	}
	uint8_t channel = 0;
	channel = atoi(argv[3]);
	if (badOptoCh(channel))
	{
		printf("Optocoupled ch number value out of range!\n");
		return ARG_RANGE_ERROR;
	}
	uint16_t val = 0;
	if (OK != optoFreqGet(dev, channel, &val))
	{
		printf("Fail to read!\n");
		return ERROR ;
	}
	printf("%d\n", (int)val);
	return OK ;
}

const CliCmdType CMD_OPTO_PWM_READ =
{
	"optpwmrd",
	2,
	&doOptoPWMRead,
	"  optpwmrd         Read optocoupled inputs signal pwm fill factor (\%), \n",
	"  Usage:           "PROGRAM_NAME" <id> optpwmrd <channel>\n",
	"  Example:         "PROGRAM_NAME" 0 optpwmrd 2; Read the pwm fill factor of the sygnal applied to opto input #2 on Board #0\n"
};
int doOptoPWMRead(int argc, char *argv[])
{
	if (argc != 4)
	{
		return ARG_CNT_ERR;
	}
	int dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR ;
	}
	uint8_t channel = 0;
	channel = atoi(argv[3]);
	if (badOptoCh(channel))
	{
		printf("Optocoupled ch number value out of range!\n");
		return ARG_RANGE_ERROR;
	}
	float val = 0;
	if (OK != optoPWMFillGet(dev, channel, &val))
	{
		printf("Fail to read!\n");
		return ERROR ;
	}
	printf("%.02f\n", val);
	return OK ;
}

const CliCmdType CMD_OPTO_INT_WR =
{
	"optintwr",
	2,
	&doOptoIntEn,
	"  optintwr         Enable/Disable interrupt generation for opto inputs change \n",
	"  Usage:           "PROGRAM_NAME" <id> optintwr <channel> <0/1>\n"
	"  Usage:           "PROGRAM_NAME" <id> optintwr <bitmap>\n",
	"  Example:         "PROGRAM_NAME" 0 optintwr 2 1; Enable interrupt generation on input channel 2\n"
};
int doOptoIntEn(int argc, char *argv[])
{
	if (argc != 5 && argc != 4)
	{
		return ARG_CNT_ERR;
	}
	int dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR ;
	}
	if(argc == 5)
	{
		uint8_t channel = 0;
		channel = atoi(argv[3]);
		if (badOptoCh(channel))
		{
			return ARG_RANGE_ERROR;
		}
		uint8_t enable = 0;
		enable = atoi(argv[4]);
		if (OK != optoIntSet(dev, channel, enable))
		{
			printf("Fail to change interrupt settings!\n");
			return ERROR ;
		}
	}
	else //argc == 4
	{
		uint16_t val = 0;
		val = 0xffff & atoi(argv[3]);
		uint8_t buff[2];
		memcpy(buff, &val, 2);
		if (OK != i2cMem8Write(dev, I2C_MEM_EXTI_EN_ADD, buff, 2))
		{
			printf("Fail to change interrupt settings!\n");
			return ERROR ;
		}
	}
	return OK ;
}

const CliCmdType CMD_OPTO_INT_RD =
{
	"optintrd",
	2,
	&doOptoIntRd,
	"  optintrd         Display interrupt generation settings for opto inputs\n",
	"  Usage:           "PROGRAM_NAME" <id> optintrd <channel>\n"
	"  Usage:           "PROGRAM_NAME" <id> optintrd\n",
	"  Example:         "PROGRAM_NAME" 0 optintrd 2; Return interrupt generation settings on input channel 2\n"
};
int doOptoIntRd(int argc, char *argv[])
{
	if (argc != 3 && argc != 4)
	{
		return ARG_CNT_ERR;
	}
	int dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR ;
	}
	if(argc == 4)
	{
		uint8_t channel = 0;
		channel = atoi(argv[3]);
		if (badOptoCh(channel))
		{
			return ARG_RANGE_ERROR;
		}
		uint8_t enable = 0;
		
		if (OK != optoIntRead(dev, channel, &enable))
		{
			printf("Fail to read interrupt settings!\n");
			return ERROR ;
		}
		printf("%d\n",(int)enable);
	}
	else //argc == 3
	{
		uint16_t val = 0;
		
		uint8_t buff[2];
		
		if (OK != i2cMem8Read(dev, I2C_MEM_EXTI_EN_ADD, buff, 2))
		{
			printf("Fail to change interrupt settings!\n");
			return ERROR ;
		}
		memcpy(&val, buff, 2);
		printf("%d\n", (int)val);
	}
	return OK ;
}