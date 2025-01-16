#include "cli.h"

extern const CliCmdType CMD_LED_READ;
extern const CliCmdType CMD_LED_WRITE;
extern const CliCmdType CMD_LED_MODE_READ;
extern const CliCmdType CMD_LED_MODE_WRITE;
extern const CliCmdType CMD_POWER_LED_MODE_READ;
extern const CliCmdType CMD_POWER_LED_MODE_WRITE;

int doLedRead(int argc, char *argv[]);
int doLedWrite(int argc, char *argv[]);
