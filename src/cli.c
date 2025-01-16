#include "16in.h"
#include "board.h"
#include "cli.h"
#include "led.h"
#include "opto.h"
#include "rs485.h"
#include "wdt.h"

const CliCmdType* gCmdArray[] =
{
	&CMD_VERSION,
	&CMD_HELP,
	&CMD_WAR,
	&CMD_LIST,
	&CMD_READ,
	&CMD_LED_READ,
	&CMD_LED_WRITE,
	&CMD_LED_MODE_READ,
	&CMD_LED_MODE_WRITE,
	&CMD_POWER_LED_MODE_READ,
	&CMD_POWER_LED_MODE_WRITE,
	&CMD_OPTO_READ,
	&CMD_OPTO_EDGE_READ,
	&CMD_OPTO_EDGE_WRITE,
	&CMD_OPTO_CNT_READ,
	&CMD_OPTO_CNT_RESET,
	&CMD_OPTO_ENC_READ,
	&CMD_OPTO_ENC_WRITE,
	&CMD_OPTO_ENC_CNT_READ,
	&CMD_OPTO_ENC_CNT_RESET,
	&CMD_OPTO_FREQUENCY_READ,
	&CMD_OPTO_PWM_READ,
	&CMD_CFG485_READ,
	&CMD_CFG485_WRITE,
	&CMD_WDT_RELOAD,
	&CMD_WDT_GET_PERIOD,
	&CMD_WDT_SET_PERIOD,
	&CMD_WDT_GET_INIT_PERIOD,
	&CMD_WDT_SET_INIT_PERIOD,
	&CMD_WDT_GET_OFF_PERIOD,
	&CMD_WDT_SET_OFF_PERIOD,
	&CMD_WDT_GET_RESET_COUNT,
	&CMD_WDT_CLR_RESET_COUNT,
	0
}; //null terminated array of cli structure pointers
