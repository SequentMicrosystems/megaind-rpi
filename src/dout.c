#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "megaind.h"
#include "comm.h"

int doDODRead(int argc, char *argv[]);
const CliCmdType CMD_DOD_READ =
	{
		"dodrd",
		2,
		&doDODRead,
		"\tdodrd:		Read open-drain output digital value  \n",
		"\tUsage:		megaind <id> dodrd <channel>\n",
		"\tUsage:		megaind <id> dodrd\n",
		"\tExample:		megaind 0 dodrd 2; Read the digital value on open-drain output channel #2 on Board #0\n"};

int doDODRead(int argc, char *argv[])
{
	int ch = 0;
	u8 buff[2];
	int dev = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}
	if (argc == 4)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > OD_CH_NR_MAX))
		{
			printf("Open-drain Output channel out of range!\n");
			return ERROR;
		}
		if (OK != i2cMem8Read(dev, I2C_MEM_RELAY_VAL, buff, 1))
		{
			return COMM_ERR;
		}
		printf("%d\n", (int) (0x01 & (buff[0] >> (ch - 1))));
	}
	else if (argc == 3)
	{
		if (OK != i2cMem8Read(dev, I2C_MEM_RELAY_VAL, buff, 1))
		{
			return COMM_ERR;
		}
		printf("%d\n", (int) (0x0f & buff[0]));
	}
	else
	{
		return ARG_CNT_ERR;
	}
	return OK;
}

int doDODWrite(int argc, char *argv[]);
const CliCmdType CMD_DOD_WRITE =
	{
		"dodwr",
		2,
		&doDODWrite,
		"\tdodwr:		Write open-drain output digital value  \n",
		"\tUsage:		megaind <id> dodwr <channel> <val>\n",
		"",
		"\tExample:		megaind 0 dodwr 2 1; Set the digital value on open-drain output channel #2 on Board #0 to enable\n"};

int doDODWrite(int argc, char *argv[])
{
	int ch = 0;
	int val = 0;
	u8 buff[2];
	int dev = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 5)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > OD_CH_NR_MAX))
		{
			printf("Open-drain Output channel out of range!\n");
			return ERROR;
		}
		val = atoi(argv[4]);
		buff[0] = ch;
		if (val > 0)
		{
			if (OK != i2cMem8Write(dev, I2C_MEM_RELAY_SET, buff, 1))
			{
				return COMM_ERR;
			}
		}
		else
		{
			if (OK != i2cMem8Write(dev, I2C_MEM_RELAY_CLR, buff, 1))
			{
				return COMM_ERR;
			}
		}
	}
	else
	{
		return ARG_CNT_ERR;
	}
	return OK;
}


int doLEDRead(int argc, char *argv[]);
const CliCmdType CMD_LED_READ =
	{
		"ledrd",
		2,
		&doLEDRead,
		"\tledrd:		Read leds state  \n",
		"\tUsage:		megaind <id> ledrd <channel>\n",
		"\tUsage:		megaind <id> ledrd\n",
		"\tExample:		megaind 0 ledrd 2; Read the state of LED #2 on Board #0\n"};

int doLEDRead(int argc, char *argv[])
{
	int ch = 0;
	u8 buff[2];
	int dev = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}
	if (argc == 4)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > LED_CH_NR_MAX))
		{
			printf("LED number out of range!\n");
			return ERROR;
		}
		if (OK != i2cMem8Read(dev, I2C_MEM_RELAY_VAL, buff, 1))
		{
			return COMM_ERR;
		}
		printf("%d\n", (int) (0x01 & (buff[0] >> (ch - 1 + OD_CH_NR_MAX))));
	}
	else if (argc == 3)
	{
		if (OK != i2cMem8Read(dev, I2C_MEM_RELAY_VAL, buff, 1))
		{
			return COMM_ERR;
		}
		printf("%d\n", (int) (0x0f & (buff[0] >> OD_CH_NR_MAX)));
	}
	else
	{
		return ARG_CNT_ERR;
	}
	return OK;
}

int doLEDWrite(int argc, char *argv[]);
const CliCmdType CMD_LED_WRITE =
	{
		"ledwr",
		2,
		&doLEDWrite,
		"\tledwr:		Write LED state  \n",
		"\tUsage:		megaind <id> ledwr <channel> <state>\n",
		"",
		"\tExample:		megaind 0 ledwr 2 1; Turn on the led #2 on Board #0 \n"};

int doLEDWrite(int argc, char *argv[])
{
	int ch = 0;
	int val = 0;
	u8 buff[2];
	int dev = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 5)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > LED_CH_NR_MAX))
		{
			printf("Open-drain Output channel out of range!\n");
			return ERROR;
		}
		val = atoi(argv[4]);
		buff[0] = ch + OD_CH_NR_MAX;
		if (val > 0)
		{
			if (OK != i2cMem8Write(dev, I2C_MEM_RELAY_SET, buff, 1))
			{
				return COMM_ERR;
			}
		}
		else
		{
			if (OK != i2cMem8Write(dev, I2C_MEM_RELAY_CLR, buff, 1))
			{
				return COMM_ERR;
			}
		}
	}
	else
	{
		return ARG_CNT_ERR;
	}
	return OK;
}

