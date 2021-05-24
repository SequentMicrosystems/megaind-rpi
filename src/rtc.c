#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "megaind.h"
#include "comm.h"

int doRTCGet(int argc, char *argv[]);
const CliCmdType CMD_RTC_GET =
	{
		"rtcrd",
		2,
		&doRTCGet,
		"\trtcrd:		Get the internal RTC  date and time(mm/dd/yy hh:mm:ss)\n",
		"\tUsage:		megaind <id> rtcrd \n",
		"",
		"\tExample:	megaind 0 rtcrd; Get the nternal RTC time and date on Board #0\n"};

int doRTCGet(int argc, char *argv[])
{
	int dev = 0;
	u8 buff[6];

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 3)
	{
		if (OK != i2cMem8Read(dev, I2C_RTC_YEAR_ADD, buff, 6))
		{
			printf("Fail to read RTC!\n");
			return ERROR;
		}

		printf("%02u/%02u/%02u %02u:%02u:%02u\n", buff[1], buff[2], buff[0],
			buff[3], buff[4], buff[5]);
	}
	else
	{
		return ARG_CNT_ERR;
	}
	return OK;
}

int doRTCSet(int argc, char *argv[]);
const CliCmdType CMD_RTC_SET =
	{
		"rtcwr",
		2,
		&doRTCSet,
		"\trtcwr:		Set the internal RTC  date and time(mm/dd/yy hh:mm:ss)\n",
		"\tUsage:		megaind <id> rtcwr <mm> <dd> <yy> <hh> <mm> <ss> \n",
		"",
		"\tExample:	megaind 0 rtcwr 9 15 20 21 43 15; Set the internal RTC time and date on Board #0 at Sept/15/2020  21:43:15\n"};

int doRTCSet(int argc, char *argv[])
{
	int dev = 0;
	u8 buff[7];
	int i = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 9)
	{
		i = atoi(argv[3]); //month
		if (i < 1 || i > 12)
		{
			printf("Invalid month!\n");
			return ERROR;
		}
		buff[1] = i;
		i = atoi(argv[4]);
		if (i < 1 || i > 31)
		{
			printf("Invalid date!\n");
			return ERROR;
		}
		buff[2] = i;
		i = atoi(argv[5]);
		if (i < 0 || i > 99)
		{
			printf("Invalid year!\n");
			return ERROR;
		}
		buff[0] = i;
		i = atoi(argv[6]);
		if (i < 0 || i > 23)
		{
			printf("Invalid hour!\n");
			return ERROR;
		}
		buff[3] = i;
		i = atoi(argv[7]);
		if (i < 0 || i > 59)
		{
			printf("Invalid minute!\n");
			return ERROR;
		}
		buff[4] = i;
		i = atoi(argv[8]);
		if (i < 0 || i > 59)
		{
			printf("Invalid second!\n");
			return ERROR;
		}
		buff[5] = i;
		buff[6] = CALIBRATION_KEY;
		if (OK != i2cMem8Write(dev, I2C_RTC_SET_YEAR_ADD, buff, 7))
		{
			printf("Fail to read RTC!\n");
			return ERROR;
		}
		printf("done\n");
	}
	else
	{
		return ARG_CNT_ERR;
	}
	return OK;
}