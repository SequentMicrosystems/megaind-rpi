#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "megaind.h"
#include "comm.h"


int doWdtReload(int argc, char *argv[]);
const CliCmdType CMD_WDT_RELOAD =
	{
		"wdtr",
		2,
		&doWdtReload,
		"\twdtr:		Reload the watchdog timer and enable the watchdog if is disabled\n",
		"\tUsage:		megaind <id> wdtr\n",
		"",
		"\tExample:		megaind 0 wdtr; Reload the watchdog timer on Board #0 with the period \n"};

int doWdtReload(int argc, char *argv[])
{
	int dev = 0;
	u8 buff[2] =
	{
		0,
		0};

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 3)
	{
		buff[0] = WDT_RESET_SIGNATURE;
		if (OK != i2cMem8Write(dev, I2C_MEM_WDT_RESET_ADD, buff, 1))
		{
			printf("Fail to write watchdog reset key!\n");
			return ERROR;
		}
	}
	else
	{
		return ARG_CNT_ERR;
	}
	return OK;
}

int doWdtSetPeriod(int argc, char *argv[]);
const CliCmdType CMD_WDT_SET_PERIOD =
	{
		"wdtpwr",
		2,
		&doWdtSetPeriod,
		"\twdtpwr:		Set the watchdog period in seconds, \n\t\t\treload command must be issue in this interval to prevent Raspberry Pi power off\n",
		"\tUsage:		megaind <id> wdtpwr <val> \n",
		"",
		"\tExample:		megaind 0 wdtpwr 10; Set the watchdog timer period on Board #0 at 10 seconds \n"};

int doWdtSetPeriod(int argc, char *argv[])
{
	int dev = 0;
	u16 period;
	u8 buff[2] =
	{
		0,
		0};

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 4)
	{
		period = (u16)atoi(argv[3]);
		if (0 == period)
		{
			printf("Invalid period!\n");
			return ERROR;
		}
		memcpy(buff, &period, 2);
		if (OK != i2cMem8Write(dev, I2C_MEM_WDT_INTERVAL_SET_ADD, buff, 2))
		{
			printf("Fail to write watchdog period!\n");
			return ERROR;
		}
	}
	else
	{
		return ARG_CNT_ERR;
	}
	return OK;
}

int doWdtGetPeriod(int argc, char *argv[]);
const CliCmdType CMD_WDT_GET_PERIOD =
	{
		"wdtprd",
		2,
		&doWdtGetPeriod,
		"\twdtprd:		Get the watchdog period in seconds, \n\t\t\treload command must be issue in this interval to prevent Raspberry Pi power off\n",
		"\tUsage:		megaind <id> wdtprd \n",
		"",
		"\tExample:		megaind 0 wdtprd; Get the watchdog timer period on Board #0\n"};

int doWdtGetPeriod(int argc, char *argv[])
{
	int dev = 0;
	u16 period;
	u8 buff[2] =
	{
		0,
		0};

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 3)
	{
		if (OK != i2cMem8Read(dev, I2C_MEM_WDT_INTERVAL_GET_ADD, buff, 2))
		{
			printf("Fail to read watchdog period!\n");
			return ERROR;
		}
		memcpy(&period, buff, 2);
		printf("%d\n", (int)period);
	}
	else
	{
		return ARG_CNT_ERR;
	}
	return OK;
}

int doWdtSetInitPeriod(int argc, char *argv[]);
const CliCmdType CMD_WDT_SET_INIT_PERIOD =
	{
		"wdtipwr",
		2,
		&doWdtSetInitPeriod,
		"\twdtipwr:	Set the watchdog initial period in seconds, \n\t\t\tThis period is loaded after power cycle, giving Raspberry time to boot\n",
		"\tUsage:		megaind <id> wdtipwr <val> \n",
		"",
		"\tExample:		megaind 0 wdtipwr 10; Set the watchdog timer initial period on Board #0 at 10 seconds \n"};

int doWdtSetInitPeriod(int argc, char *argv[])
{
	int dev = 0;
	u16 period;
	u8 buff[2] =
	{
		0,
		0};

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 4)
	{
		period = (u16)atoi(argv[3]);
		if (0 == period)
		{
			printf("Invalid period!\n");
			return ERROR;
		}
		memcpy(buff, &period, 2);
		if (OK != i2cMem8Write(dev, I2C_MEM_WDT_INIT_INTERVAL_SET_ADD, buff, 2))
		{
			printf("Fail to write watchdog period!\n");
			return ERROR;
		}
	}
	else
	{
		return ARG_CNT_ERR;
	}
	return OK;
}

int doWdtGetInitPeriod(int argc, char *argv[]);
const CliCmdType CMD_WDT_GET_INIT_PERIOD =
	{
		"wdtiprd",
		2,
		&doWdtGetInitPeriod,
		"\twdtiprd:	Get the watchdog initial period in seconds. \n\t\t\tThis period is loaded after power cycle, giving Raspberry time to boot\n",
		"\tUsage:		megaind <id> wdtiprd \n",
		"",
		"\tExample:		megaind 0 wdtiprd; Get the watchdog timer initial period on Board #0\n"};

int doWdtGetInitPeriod(int argc, char *argv[])
{
	int dev = 0;
	u16 period;
	u8 buff[2] =
	{
		0,
		0};

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 3)
	{
		if (OK != i2cMem8Read(dev, I2C_MEM_WDT_INIT_INTERVAL_GET_ADD, buff, 2))
		{
			printf("Fail to read watchdog period!\n");
			return ERROR;
		}
		memcpy(&period, buff, 2);
		printf("%d\n", (int)period);
	}
	else
	{
		return ARG_CNT_ERR;
	}
	return OK;
}

int doWdtSetOffPeriod(int argc, char *argv[]);
const CliCmdType CMD_WDT_SET_OFF_PERIOD =
	{
		"wdtopwr",
		2,
		&doWdtSetOffPeriod,
		"\twdtopwr:	Set the watchdog off period in seconds (max 48 days). \n\t\t\tThis is the time that watchdog mantain Raspberry turned off \n",
		"\tUsage:		megaind <id> wdtopwr <val> \n",
		"",
		"\tExample:		megaind 0 wdtopwr 10; Set the watchdog off interval on Board #0 at 10 seconds \n"};

int doWdtSetOffPeriod(int argc, char *argv[])
{
	int dev = 0;
	u32 period;
	u8 buff[4] =
	{
		0,
		0,
		0,
		0};

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 4)
	{
		period = (u32)atoi(argv[3]);
		if ( (0 == period) || (period > WDT_MAX_OFF_INTERVAL_S))
		{
			printf("Invalid period!\n");
			return ERROR;
		}
		memcpy(buff, &period, 4);
		if (OK
			!= i2cMem8Write(dev, I2C_MEM_WDT_POWER_OFF_INTERVAL_SET_ADD, buff, 4))
		{
			printf("Fail to write watchdog period!\n");
			return ERROR;
		}
	}
	else
	{
		return ARG_CNT_ERR;
	}
	return OK;
}

int doWdtGetOffPeriod(int argc, char *argv[]);
const CliCmdType CMD_WDT_GET_OFF_PERIOD =
	{
		"wdtoprd",
		2,
		&doWdtGetOffPeriod,
		"\twdtoprd:	Get the watchdog off period in seconds (max 48 days) \n\t\t\tThis is the time that watchdog mantain Raspberry turned off \n",
		"\tUsage:		megaind <id> wdtoprd \n",
		"",
		"\tExample:		megaind 0 wdtoprd; Get the watchdog off period on Board #0\n"};

int doWdtGetOffPeriod(int argc, char *argv[])
{
	int dev = 0;
	u32 period;
	u8 buff[4] =
	{
		0,
		0,
		0,
		0};

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 3)
	{
		if (OK
			!= i2cMem8Read(dev, I2C_MEM_WDT_POWER_OFF_INTERVAL_GET_ADD, buff, 4))
		{
			printf("Fail to read watchdog period!\n");
			return ERROR;
		}
		memcpy(&period, buff, 4);
		printf("%d\n", (int)period);
	}
	else
	{
		return ARG_CNT_ERR;
	}
	return OK;
}