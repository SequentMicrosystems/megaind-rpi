
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "megaind.h"
#include "comm.h"
#include "thread.h"


int optoChGet(int dev, u8 channel, OutStateEnumType* state)
{
	u8 buff[2];

	if (NULL == state)
	{
		return ERROR;
	}

	if ( (channel < CHANNEL_NR_MIN) || (channel > OPTO_CH_NR_MAX))
	{
		printf("Invalid OPTO channel nr!\n");
		return ERROR;
	}

	if (FAIL == i2cMem8Read(dev, I2C_MEM_OPTO_IN_VAL, buff, 1))
	{
		return ERROR;
	}

	if (buff[0] & (1 << (channel - 1)))
	{
		*state = ON;
	}
	else
	{
		*state = OFF;
	}
	return OK;
}

int optoGet(int dev, int* val)
{
	u8 buff[2];

	if (NULL == val)
	{
		return ERROR;
	}
	if (FAIL == i2cMem8Read(dev, I2C_MEM_OPTO_IN_VAL, buff, 1))
	{
		return ERROR;
	}
	*val = buff[0];
	return OK;
}

int optoCountGet(int dev, u8 ch, u16* val)
{
	u8 buff[2];

	if (NULL == val)
	{
		return ERROR;
	}
	if ( (ch < CHANNEL_NR_MIN) || (ch > OPTO_CH_NR_MAX))
	{
		printf("Invalid OPTO channel nr!\n");
		return ERROR;
	}
	if (FAIL == i2cMem8Read(dev, I2C_MEM_OPTO_COUNT1 + 2 * (ch - 1), buff, 2))
	{
		return ERROR;
	}
	memcpy(val, buff, 2);
	return OK;
}

int optoCountReset(int dev, u8 channel)
{
	u8 buff[4];

	if ( (channel < CHANNEL_NR_MIN) || (channel > OPTO_CH_NR_MAX))
	{
		printf("Invalid OPTO channel nr!\n");
		return ERROR;
	}
	buff[0] = channel;
	if (FAIL == i2cMem8Write(dev, I2C_MEM_OPTO_CH_CONT_RESET, buff, 1))
	{
		printf("Fail to reset counter! \n");
		return ERROR;
	}
	return OK;
}

int optoCountRisingSet(int dev, u8 channel, u8 state)
{
	u8 buff[4];

	if ( (channel < CHANNEL_NR_MIN) || (channel > OPTO_CH_NR_MAX))
	{
		printf("Invalid DRY OPTO nr!\n");
		return ERROR;
	}

	if (FAIL == i2cMem8Read(dev, I2C_MEM_OPTO_RISING_ENABLE, buff, 1))
	{
		return ERROR;
	}
	if (state != 0)
	{
		buff[0] |= 1 << (channel - 1);
	}
	else
	{
		buff[0] &= ~ (1 << (channel - 1));
	}
	if (FAIL == i2cMem8Write(dev, I2C_MEM_OPTO_RISING_ENABLE, buff, 1))
	{
		return ERROR;
	}
	return OK;
}

int optoCountFallingSet(int dev, u8 channel, u8 state)
{
	u8 buff[4];

	if ( (channel < CHANNEL_NR_MIN) || (channel > OPTO_CH_NR_MAX))
	{
		printf("Invalid OPTO channel nr!\n");
		return ERROR;
	}

	if (FAIL == i2cMem8Read(dev, I2C_MEM_OPTO_FALLING_ENABLE, buff, 1))
	{
		return ERROR;
	}
	if (state != 0)
	{
		buff[0] |= 1 << (channel - 1);
	}
	else
	{
		buff[0] &= ~ (1 << (channel - 1));
	}
	if (FAIL == i2cMem8Write(dev, I2C_MEM_OPTO_FALLING_ENABLE, buff, 1))
	{
		return ERROR;
	}
	return OK;
}

int optoCountRisingGet(int dev, u8 channel, u8* state)
{
	u8 buff[4];

	if (NULL == state)
	{
		return ERROR;
	}
	if ( (channel < CHANNEL_NR_MIN) || (channel > OPTO_CH_NR_MAX))
	{
		printf("Invalid OPTO channel nr!\n");
		return ERROR;
	}

	if (FAIL == i2cMem8Read(dev, I2C_MEM_OPTO_RISING_ENABLE, buff, 1))
	{
		return ERROR;
	}
	if ( (buff[0] & (1 << (channel - 1))) != 0)
	{
		*state = 1;
	}
	else
	{
		*state = 0;
	}
	return OK;
}

int optoCountFallingGet(int dev, u8 channel, u8* state)
{
	u8 buff[4];

	if (NULL == state)
	{
		return ERROR;
	}
	if ( (channel < CHANNEL_NR_MIN) || (channel > OPTO_CH_NR_MAX))
	{
		printf("Invalid OPTO channel nr!\n");
		return ERROR;
	}

	if (FAIL == i2cMem8Read(dev, I2C_MEM_OPTO_FALLING_ENABLE, buff, 1))
	{
		return COMM_ERR;
	}
	if ( (buff[0] & (1 << (channel - 1))) != 0)
	{
		*state = 1;
	}
	else
	{
		*state = 0;
	}
	return OK;
}

int doOptoRead(int argc, char *argv[]);
const CliCmdType CMD_OPTO_READ =
	{
		"optord",
		2,
		&doOptoRead,
		"\toptord:		Read dry opto status\n",
		"\tUsage:		megaind <id> optord <channel>\n",
		"\tUsage:		megaind <id> optord\n",
		"\tExample:		megaind 0 optord 2; Read Status of opto input pin #2 on Board #0\n"};

int doOptoRead(int argc, char *argv[])
{
	int pin = 0;
	int val = 0;
	int dev = 0;
	OutStateEnumType state = STATE_COUNT;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 4)
	{
		pin = atoi(argv[3]);
		if ( (pin < CHANNEL_NR_MIN) || (pin > OPTO_CH_NR_MAX))
		{
			printf("Opto channel number value out of range!\n");
			return ERROR;
		}

		if (OK != optoChGet(dev, pin, &state))
		{
			return COMM_ERR;
		}
		if (state != 0)
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
		if (OK != optoGet(dev, &val))
		{
			return COMM_ERR;
		}
		printf("%d\n", val);
	}
	else
	{
		return ARG_CNT_ERR;
	}
	return OK;
}

int doCountRead(int argc, char *argv[]);
const CliCmdType CMD_COUNTER_READ =
	{
		"countrd",
		2,
		&doCountRead,
		"\tcountrd:	Read dry opto transitions count\n",
		"\tUsage:		megaind <id> countrd <channel>\n",
		"",
		"\tExample:		megaind 0 countrd 2; Read transitions count of opto input pin #2 on Board #0\n"};

int doCountRead(int argc, char *argv[])
{
	u8 pin = 0;
	u16 val = 0;
	int dev = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 4)
	{
		pin = (u8)atoi(argv[3]);

		if (OK != optoCountGet(dev, pin, &val))
		{
			return COMM_ERR;
		}
		printf("%u\n", (unsigned int)val);
	}
	else
	{
		return ARG_CNT_ERR;
	}
	return OK;
}

int doCountReset(int argc, char *argv[]);
const CliCmdType CMD_COUNTER_RST =
	{
		"countrst",
		2,
		&doCountReset,
		"\tcountrst:	Reset opto transitions countors\n",
		"\tUsage:		megaind <id> countrst <channel>\n",
		"",
		"\tExample:		megaind 0 countrst 2; Reset transitions count of opto input pin #2 on Board #0\n"};

int doCountReset(int argc, char *argv[])
{
	u8 pin = 0;
	int dev = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 4)
	{
		pin = (u8)atoi(argv[3]);

		if (OK != optoCountReset(dev, pin))
		{
			printf("Fail to read!\n");
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

int doEdgeRead(int argc, char *argv[]);
const CliCmdType CMD_EDGE_READ =
	{
		"edgerd",
		2,
		&doEdgeRead,
		"\tedgerd:		Read opto inputs transitions type, ret 0 - disable, 1 - rising, 2 - falling, 3 - both\n",
		"\tUsage:		megaind <id> edgerd <channel> \n",
		"",
		"\tExample:		megaind 0 edgerd 2; Read transitions type of opto input pin #2 on Board #0\n"};

int doEdgeRead(int argc, char *argv[])
{
	u8 pin = 0;
	u8 rising = 0;
	u8 falling = 0;
	int dev = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 4)
	{
		pin = (u8)atoi(argv[3]);

		if (OK != optoCountRisingGet(dev, pin, &rising))
		{
			printf("Fail to read!\n");
			return ERROR;
		}
		if (OK != optoCountFallingGet(dev, pin, &falling))
		{
			printf("Fail to read!\n");
			return ERROR;
		}
		printf("%d\n", (int) (rising + falling * 2));
	}
	else
	{
		return ARG_CNT_ERR;
	}
	return OK;
}

int doEdgeWrite(int argc, char *argv[]);
const CliCmdType CMD_EDGE_WRITE =
	{
		"edgewr",
		2,
		&doEdgeWrite,
		"\tedgewr:		Writ opto inputs transitions type: 0 - disable, 1 - rising, 2 - falling, 3 - both\n",
		"\tUsage:		megaind <id> edgewr <channel> <val>\n",
		"",
		"\tExample:		megaind 0 edgewr 2 1; Set transitions type of opto input #2 on Board #0 to rising\n"};

int doEdgeWrite(int argc, char *argv[])
{
	u8 pin = 0;
	u8 rising = 0;
	u8 falling = 0;
	int dev = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 5)
	{
		pin = (u8)atoi(argv[3]);

		if ( (1 & atoi(argv[4])) != 0)
		{
			rising = 1;
		}
		if ( (2 & atoi(argv[4])) != 0)
		{
			falling = 1;
		}
		if (OK != optoCountRisingSet(dev, pin, rising))
		{
			printf("Fail to write!\n");
			return ERROR;
		}
		if (OK != optoCountFallingSet(dev, pin, falling))
		{
			printf("Fail to write!\n");
			return ERROR;
		}
	}
	else
	{
		return ARG_CNT_ERR;
	}
	return OK;
}
