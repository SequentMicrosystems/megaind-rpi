/*
 * rtd.c:
 *	Command-line interface to the Raspberry
 *	Pi's MEGAS-RTD board.
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
#include <math.h>

#include "megaind.h"
#include "comm.h"
#include "thread.h"

#define VERSION_BASE	(int)1
#define VERSION_MAJOR	(int)1
#define VERSION_MINOR	(int)0

#define UNUSED(X) (void)X      /* To avoid gcc/g++ warnings */

char *warranty =
	"	       Copyright (c) 2016-2020 Sequent Microsystems\n"
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
	printf("Where: <id> = Board level id = 0..7\n");
	printf("Type megaind -h <command> for more help\n");
}

int doBoardInit(int stack)
{
	int dev = 0;
	int add = 0;
	uint8_t buff[8];

	if ( (stack < 0) || (stack > 7))
	{
		printf("Invalid stack level [0..7]!");
		return ERROR;
	}
	add = stack + SLAVE_OWN_ADDRESS_BASE;
	dev = i2cSetup(add);
	if (dev == -1)
	{
		return ERROR;
	}
	if (ERROR == i2cMem8Read(dev, I2C_MEM_REVISION_MAJOR, buff, 1))
	{
		printf("MEGA-IND id %d not detected\n", stack);
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
	add = stack + SLAVE_OWN_ADDRESS_BASE;
	dev = i2cSetup(add);
	if (dev == -1)
	{
		return ERROR;
	}
	if (ERROR == i2cMem8Read(dev, I2C_MEM_REVISION_MAJOR, buff, 1))
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
		"\tUsage:		megaind -h    Display command options list\n",
		"\tUsage:		megaind -h <param>   Display help for <param> command option\n",
		"\tExample:		megaind -h rread    Display help for \"rread\" command option\n"};

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
	"\t-v		Display the megaind command version number\n",
	"\tUsage:		megaind -v\n",
	"",
	"\tExample:		megaind -v  Display the version number\n"};

static void doVersion(int argc, char *argv[])
{
	UNUSED(argc);
	UNUSED(argv);
	printf("megaind v%d.%d.%d Copyright (c) 2016 - 2020 Sequent Microsystems\n",
	VERSION_BASE, VERSION_MAJOR, VERSION_MINOR);
	printf("\nThis is free software with ABSOLUTELY NO WARRANTY.\n");
	printf("For details type: megaind -warranty\n");

}

static void doWarranty(int argc, char* argv[]);
const CliCmdType CMD_WAR =
{
	"-warranty",
	1,
	&doWarranty,
	"\t-warranty	Display the warranty\n",
	"\tUsage:		megaind -warranty\n",
	"",
	"\tExample:		megaind -warranty  Display the warranty text\n"};

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
		"\t-list:		List all megaind boards connected\n\t\t\treturn the # of boards and stack level for every board\n",
		"\tUsage:		megaind -list\n",
		"",
		"\tExample:		megaind -list display: 1,0 \n"};

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

static void doBoard(int argc, char *argv[]);
const CliCmdType CMD_BOARD =
{
	"board",
	2,
	&doBoard,
	"\tboard		Display the board status and firmware version number\n",
	"\tUsage:		megaind <stack> board\n",
	"",
	"\tExample:		megaind 0 board  Display vcc, temperature, firmware version \n"};

static void doBoard(int argc, char *argv[])
{
	int dev = -1;
	u8 buff[5];
	int resp = 0;
	int temperature = 25;
	float vIn = 24;
	float vRasp = 5;

	if (argc != 3)
	{
		printf("Invalid arguments number type \"megaind -h\" for details\n");
		exit(1);
	}
	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}
	resp = i2cMem8Read(dev, I2C_MEM_DIAG_TEMPERATURE, buff, 5);
	if (FAIL == resp)
	{
		printf("Fail to read board info!\n");
		exit(1);
	}
	temperature = buff[0];
	memcpy(&resp, &buff[1], 2);
	vIn = (float)resp / VOLT_TO_MILIVOLT; //read in milivolts

	memcpy(&resp, &buff[3], 2);
	vRasp = (float)resp / VOLT_TO_MILIVOLT; //read in milivolts	

	resp = i2cMem8Read(dev, I2C_MEM_REVISION_MAJOR, buff, 2);
	if (FAIL == resp)
	{
		printf("Fail to read board info!\n");
		exit(1);
	}
	printf(
		"Firmware ver %02d.%02d, CPU temperature %d C, Power source %0.2f V, Raspberry %0.2f V\n",
		(int)buff[0], (int)buff[1], temperature, vIn, vRasp);
}

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

static void doOptoRead(int argc, char *argv[]);
const CliCmdType CMD_OPTO_READ =
	{
		"optord",
		2,
		&doOptoRead,
		"\toptord:		Read dry opto status\n",
		"\tUsage:		megaind <id> optord <channel>\n",
		"\tUsage:		megaind <id> optord\n",
		"\tExample:		megaind 0 optord 2; Read Status of opto input pin #2 on Board #0\n"};

static void doOptoRead(int argc, char *argv[])
{
	int pin = 0;
	int val = 0;
	int dev = 0;
	OutStateEnumType state = STATE_COUNT;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 4)
	{
		pin = atoi(argv[3]);
		if ( (pin < CHANNEL_NR_MIN) || (pin > OPTO_CH_NR_MAX))
		{
			printf("Opto channel number value out of range!\n");
			exit(1);
		}

		if (OK != optoChGet(dev, pin, &state))
		{
			printf("Fail to read!\n");
			exit(1);
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
			printf("Fail to read!\n");
			exit(1);
		}
		printf("%d\n", val);
	}
	else
	{
		printf("%s", CMD_OPTO_READ.usage1);
		printf("%s", CMD_OPTO_READ.usage2);
		exit(1);
	}
}

static void doCountRead(int argc, char *argv[]);
const CliCmdType CMD_COUNTER_READ =
	{
		"countrd",
		2,
		&doCountRead,
		"\tcountrd:	Read dry opto transitions count\n",
		"\tUsage:		megaind <id> countrd <channel>\n",
		"",
		"\tExample:		megaind 0 countrd 2; Read transitions count of opto input pin #2 on Board #0\n"};

static void doCountRead(int argc, char *argv[])
{
	u8 pin = 0;
	u16 val = 0;
	int dev = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 4)
	{
		pin = (u8)atoi(argv[3]);

		if (OK != optoCountGet(dev, pin, &val))
		{
			printf("Fail to read!\n");
			exit(1);
		}
		printf("%u\n", (unsigned int)val);
	}
	else
	{
		printf("%s", CMD_COUNTER_READ.usage1);
		exit(1);
	}
}

static void doCountReset(int argc, char *argv[]);
const CliCmdType CMD_COUNTER_RST =
	{
		"countrst",
		2,
		&doCountReset,
		"\tcountrst:	Reset opto transitions countors\n",
		"\tUsage:		megaind <id> countrst <channel>\n",
		"",
		"\tExample:		megaind 0 countrst 2; Reset transitions count of opto input pin #2 on Board #0\n"};

static void doCountReset(int argc, char *argv[])
{
	u8 pin = 0;
	int dev = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 4)
	{
		pin = (u8)atoi(argv[3]);

		if (OK != optoCountReset(dev, pin))
		{
			printf("Fail to read!\n");
			exit(1);
		}
		printf("done\n");
	}
	else
	{
		printf("%s", CMD_COUNTER_RST.usage1);
		exit(1);
	}
}

static void doEdgeRead(int argc, char *argv[]);
const CliCmdType CMD_EDGE_READ =
	{
		"edgerd",
		2,
		&doEdgeRead,
		"\tedgerd:		Read opto inputs transitions type, ret 0 - disable, 1 - rising, 2 - falling, 3 - both\n",
		"\tUsage:		megaind <id> edgerd <channel> \n",
		"",
		"\tExample:		megaind 0 edgerd 2; Read transitions type of opto input pin #2 on Board #0\n"};

static void doEdgeRead(int argc, char *argv[])
{
	u8 pin = 0;
	u8 rising = 0;
	u8 falling = 0;
	int dev = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 4)
	{
		pin = (u8)atoi(argv[3]);

		if (OK != optoCountRisingGet(dev, pin, &rising))
		{
			printf("Fail to read!\n");
			exit(1);
		}
		if (OK != optoCountFallingGet(dev, pin, &falling))
		{
			printf("Fail to read!\n");
			exit(1);
		}
		printf("%d\n", (int) (rising + falling * 2));
	}
	else
	{
		printf("%s", CMD_EDGE_READ.usage1);
		exit(1);
	}
}

static void doEdgeWrite(int argc, char *argv[]);
const CliCmdType CMD_EDGE_WRITE =
	{
		"edgewr",
		2,
		&doEdgeWrite,
		"\tedgewr:		Writ opto inputs transitions type: 0 - disable, 1 - rising, 2 - falling, 3 - both\n",
		"\tUsage:		megaind <id> edgewr <channel> <val>\n",
		"",
		"\tExample:		megaind 0 edgewr 2 1; Set transitions type of opto input #2 on Board #0 to rising\n"};

static void doEdgeWrite(int argc, char *argv[])
{
	u8 pin = 0;
	u8 rising = 0;
	u8 falling = 0;
	int dev = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
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
			exit(1);
		}
		if (OK != optoCountFallingSet(dev, pin, falling))
		{
			printf("Fail to write!\n");
			exit(1);
		}

	}
	else
	{
		printf("%s", CMD_EDGE_WRITE.usage1);
		exit(1);
	}
}

int val16Get(int dev, int baseAdd, int ch, float scale, float* val)
{
	u8 buff[2] =
	{
		0,
		0};
	u16 raw = 0;
	u8 add = 0;

	if (ch < CHANNEL_NR_MIN)
	{
		return ERROR;
	}

	if ( (baseAdd + 2 * (ch - 1)) >= I2C_MEM_SIZE)
	{
		return ERROR;
	}
	add = baseAdd + 2 * (ch - 1);
	if (OK != i2cMem8Read(dev, add, buff, 2))
	{
		printf("Fail to read!\n");
		return ERROR;
	}
	memcpy(&raw, buff, 2);
	*val = (float)raw / scale;
	return OK;
}

int val16Set(int dev, int baseAdd, int ch, float scale, float val)
{
	u8 buff[2] =
	{
		0,
		0};
	u16 raw = 0;
	u8 add = 0;

	if (ch < CHANNEL_NR_MIN)
	{
		return ERROR;
	}

	if ( (baseAdd + 2 * (ch - 1)) >= I2C_MEM_SIZE)
	{
		return ERROR;
	}
	add = baseAdd + 2 * (ch - 1);
	raw = (u16)ceil(val * scale); //transform to milivolts
	memcpy(buff, &raw, 2);
	if (OK != i2cMem8Write(dev, add, buff, 2))
	{
		printf("Fail to write!\n");
		return ERROR;
	}
	return OK;
}

static void doUOutRead(int argc, char *argv[]);
const CliCmdType CMD_UOUT_READ =
	{
		"uoutrd",
		2,
		&doUOutRead,
		"\tuoutrd:		Read 0-10V Output voltage value(V) \n",
		"\tUsage:		megaind <id> uoutrd <channel>\n",
		"",
		"\tExample:		megaind 0 uoutrd 2; Read the voltage on 0-10V out channel #2 on Board #0\n"};

static void doUOutRead(int argc, char *argv[])
{
	int ch = 0;
	float val = 0;
	int dev = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 4)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > U_OUT_CH_NR_MAX))
		{
			printf("0-10V Output channel out of range!\n");
			exit(1);
		}
		if (OK
			!= val16Get(dev, I2C_MEM_U0_10_OUT_VAL1, ch, VOLT_TO_MILIVOLT, &val))
		{
			exit(1);
		}
		printf("%0.3f\n", val);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_UOUT_READ.usage1);
		exit(1);
	}
}

static void doIOutRead(int argc, char *argv[]);
const CliCmdType CMD_IOUT_READ =
	{
		"ioutrd",
		2,
		&doIOutRead,
		"\tioutrd:		Read 4-20mA Output current value (mA) \n",
		"\tUsage:		megaind <id> ioutrd <channel>\n",
		"",
		"\tExample:		megaind 0 ioutrd 2; Read the current on 4-20mA out channel #2 on Board #0\n"};

static void doIOutRead(int argc, char *argv[])
{
	int ch = 0;
	float val = 0;
	int dev = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 4)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > I_OUT_CH_NR_MAX))
		{
			printf("4-20mA Output channel out of range!\n");
			exit(1);
		}
		if (OK
			!= val16Get(dev, I2C_MEM_I4_20_OUT_VAL1, ch, VOLT_TO_MILIVOLT, &val))
		{
			exit(1);
		}
		printf("%0.3f\n", val);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_IOUT_READ.usage1);
		exit(1);
	}
}

static void doODRead(int argc, char *argv[]);
const CliCmdType CMD_OD_READ =
	{
		"odrd",
		2,
		&doODRead,
		"\todrd:		Read open-drain Output PWM value(0..100%) \n",
		"\tUsage:		megaind <id> odrd <channel>\n",
		"",
		"\tExample:		megaind 0 odrd 2; Read the PWM value on open-drain output channel #2 on Board #0\n"};

static void doODRead(int argc, char *argv[])
{
	int ch = 0;
	float val = 0;
	int dev = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 4)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > OD_CH_NR_MAX))
		{
			printf("Open-drain Output channel out of range!\n");
			exit(1);
		}
		if (OK != val16Get(dev, I2C_MEM_OD_PWM1, ch, 100, &val))
		{
			exit(1);
		}
		printf("%0.3f\n", val);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_OD_READ.usage1);
		exit(1);
	}
}

static void doUOutWrite(int argc, char *argv[]);
const CliCmdType CMD_UOUT_WRITE =
	{
		"uoutwr",
		2,
		&doUOutWrite,
		"\tuoutwr:		Write 0-10V output voltage value (V)\n",
		"\tUsage:		megaind <id> uoutwr <channel> <value(V)>\n",
		"",
		"\tExample:		megaind 0 uoutwr 2 2.5; Write 2.5V to 0-10V output channel #2 on Board #0\n"};

static void doUOutWrite(int argc, char *argv[])
{
	int ch = 0;
	int dev = 0;
	float volt = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 5)
	{
		ch = atoi(argv[3]);

		volt = atof(argv[4]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > U_OUT_CH_NR_MAX))
		{
			printf("0-10V Output channel out of range!\n");
			exit(1);
		}
		if (volt < 0 || volt > 10)
		{
			printf("Invalid voltage value, must be 0..10 \n");
			exit(1);
		}

		if (OK
			!= val16Set(dev, I2C_MEM_U0_10_OUT_VAL1, ch, VOLT_TO_MILIVOLT, volt))
		{
			exit(1);
		}
		printf("done\n");
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_UOUT_WRITE.usage1);
		exit(1);
	}
}

static void doIOutWrite(int argc, char *argv[]);
const CliCmdType CMD_IOUT_WRITE =
	{
		"ioutwr",
		2,
		&doIOutWrite,
		"\tioutwr:		Write 4-20mA output value (mA)\n",
		"\tUsage:		megaind <id> ioutwr <channel> <value(mA)>\n",
		"",
		"\tExample:		megaind 0 ioutwr 2 10.5; Set 10.5mA to 4-20mA output channel #2 on Board #0\n"};

static void doIOutWrite(int argc, char *argv[])
{
	int ch = 0;
	int dev = 0;
	float volt = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 5)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > I_OUT_CH_NR_MAX))
		{
			printf("4-20mA Output channel out of range!\n");
			exit(1);
		}
		volt = atof(argv[4]);
		if (volt < 4 || volt > 20)
		{
			printf("Invalid current value, must be 4..20 \n");
			exit(1);
		}

		if (OK
			!= val16Set(dev, I2C_MEM_I4_20_OUT_VAL1, ch, VOLT_TO_MILIVOLT, volt))
		{
			exit(1);
		}
		printf("done\n");
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_IOUT_WRITE.usage1);
		exit(1);
	}
}

static void doODWrite(int argc, char *argv[]);
const CliCmdType CMD_OD_WRITE =
	{
		"odwr",
		2,
		&doODWrite,
		"\todwr:		Write open-drain output PWM value (0..100%)\n",
		"\tUsage:		megaind <id> odwr <channel> <value>\n",
		"",
		"\tExample:		megaind 0 odwr 2 10.5; Set PWM 10.5% to open-drain output channel #2 on Board #0\n"};

static void doODWrite(int argc, char *argv[])
{
	int ch = 0;
	int dev = 0;
	float volt = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 5)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > OD_CH_NR_MAX))
		{
			printf("Open-drain channel out of range!\n");
			exit(1);
		}
		volt = atof(argv[4]);
		if (volt < 0 || volt > 100)
		{
			printf("Invalid PWM value, must be 0..100 \n");
			exit(1);
		}

		if (OK != val16Set(dev, I2C_MEM_OD_PWM1, ch, 100, volt))
		{
			exit(1);
		}
		printf("done\n");
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_OD_WRITE.usage1);
		exit(1);
	}
}

static void doUInRead(int argc, char *argv[]);
const CliCmdType CMD_UIN_READ =
	{
		"uinrd",
		2,
		&doUInRead,
		"\tuinrd:		Read 0-10V input value (V) \n",
		"\tUsage:		megaind <id> uinrd <channel>\n",
		"",
		"\tExample:		megaind 0 uinrd 2; Read the voltage input on 0-10V in channel #2 on Board #0\n"};

static void doUInRead(int argc, char *argv[])
{
	int ch = 0;
	float val = 0;
	int dev = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 4)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > U_IN_CH_NR_MAX))
		{
			printf("0-10V input channel out of range!\n");
			exit(1);
		}

		if (OK
			!= val16Get(dev, I2C_MEM_U0_10_IN_VAL1, ch, VOLT_TO_MILIVOLT, &val))
		{
			exit(1);
		}

		printf("%0.3f\n", val);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_UIN_READ.usage1);
		exit(1);
	}
}

static void doPmUInRead(int argc, char *argv[]);
const CliCmdType CMD_PMUIN_READ =
	{
		"pmuinrd",
		2,
		&doPmUInRead,
		"\tpmuinrd:		Read +/-10V input value (V). Warning: This value is valid only if the corespondung jumper is connected\n",
		"\tUsage:		megaind <id> pmuinrd <channel>\n",
		"",
		"\tExample:		megaind 0 pmuinrd 2; Read the voltage input on +/-10V in channel #2 on Board #0\n"};

static void doPmUInRead(int argc, char *argv[])
{
	int ch = 0;
	float val = 0;
	int dev = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 4)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > U_IN_CH_NR_MAX))
		{
			printf("+/-10V input channel out of range!\n");
			exit(1);
		}

		if (OK
			!= val16Get(dev, I2C_MEM_U_PM_10_IN_VAL1, ch, VOLT_TO_MILIVOLT, &val))
		{
			exit(1);
		}
		val -= 10;
		printf("%0.3f\n", val);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_PMUIN_READ.usage1);
		exit(1);
	}
}

static void doIInRead(int argc, char *argv[]);
const CliCmdType CMD_IIN_READ =
	{
		"iinrd",
		2,
		&doIInRead,
		"\tiinrd:		Read 4-20mA input value (mA) \n",
		"\tUsage:		megaind <id> iinrd <channel>\n",
		"",
		"\tExample:		megaind 0 iinrd 2; Read the voltage input on 4-20mA in channel #2 on Board #0\n"};

static void doIInRead(int argc, char *argv[])
{
	int ch = 0;
	float val = 0;
	int dev = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 4)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > I_IN_CH_NR_MAX))
		{
			printf("4-20mA input channel out of range!\n");
			exit(1);
		}

		if (OK
			!= val16Get(dev, I2C_MEM_I4_20_IN_VAL1, ch, VOLT_TO_MILIVOLT, &val))
		{
			exit(1);
		}

		printf("%0.3f\n", val);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_IIN_READ.usage1);
		exit(1);
	}
}

void getCalStat(int dev)
{
	u8 buff[2];

	busyWait(100);
	if (OK != i2cMem8Read(dev, I2C_MEM_CALIB_STATUS, buff, 1))
	{
		printf("Fail to read calibration status!\n");
		exit(1);
	}
	switch (buff[0])
	{
	case 0:
		printf("Calibration in progress\n");
		break;
	case 1:
		printf("Calibration done\n");
		break;
	case 2:
		printf("Calibration error!\n");
		break;
	default:
		printf("Unknown calibration status\n");
		break;
	}
}

static void doUInCal(int argc, char *argv[]);
const CliCmdType CMD_UIN_CAL =
	{
		"uincal",
		2,
		&doUInCal,
		"\tuincal:		Calibrate one 0-10V input channel, the calibration must be done in 2 points at min 5V apart\n",
		"\tUsage:		megaind <id> uincal <channel> <value(V)>\n",
		"",
		"\tExample:		megaind 0 uincal 2 0.5; Calibrate the 0-10V input  channel #2 on Board #0 at 0.5V\n"};

static void doUInCal(int argc, char *argv[])
{
	int ch = 0;
	float val = 0;
	int dev = 0;
	u8 buff[4] =
	{
		0,
		0};
	u16 raw = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 5)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > U_IN_CH_NR_MAX))
		{
			printf("0-10V input channel out of range!\n");
			exit(1);
		}

		val = atof(argv[4]);
		if ( (val < 0) || (val > 10))
		{
			printf("0-10V input calibration value out of range!\n");
			exit(1);
		}
		raw = (u16)ceil(val * VOLT_TO_MILIVOLT);
		memcpy(buff, &raw, 2);
		buff[2] = ch + CAL_0_10V_IN_START_ID - 1;
		buff[3] = CALIBRATION_KEY;

		if (OK != i2cMem8Write(dev, I2C_MEM_CALIB_VALUE, buff, 4))
		{
			printf("Fail to write calibration data!\n");
			exit(1);
		}

		getCalStat(dev);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_UIN_CAL.usage1);
		exit(1);
	}
}

static void doIInCal(int argc, char *argv[]);
const CliCmdType CMD_IIN_CAL =
	{
		"iincal",
		2,
		&doIInCal,
		"\tiincal:		Calibrate one 4-20mA input channel, the calibration must be done in 2 points at min 10mA apart\n",
		"\tUsage:		megaind <id> iincal <channel> <value(mA)>\n",
		"",
		"\tExample:		megaind 0 iincal 2 5.5; Calibrate the 4-20mA input  channel #2 on Board #0 at 5.5mA\n"};

static void doIInCal(int argc, char *argv[])
{
	int ch = 0;
	float val = 0;
	int dev = 0;
	u8 buff[4] =
	{
		0,
		0};
	u16 raw = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 5)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > I_IN_CH_NR_MAX))
		{
			printf("4-20mA input channel out of range!\n");
			exit(1);
		}

		val = atof(argv[4]);
		if ( (val < 4) || (val > 20))
		{
			printf("4-20mA input calibration value out of range!\n");
			exit(1);
		}
		raw = (u16)ceil(val * VOLT_TO_MILIVOLT);
		memcpy(buff, &raw, 2);
		buff[2] = ch + CAL_4_20mA_IN_START_ID - 1;
		buff[3] = CALIBRATION_KEY;

		if (OK != i2cMem8Write(dev, I2C_MEM_CALIB_VALUE, buff, 4))
		{
			printf("Fail to write calibration data!\n");
			exit(1);
		}

		getCalStat(dev);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_IIN_CAL.usage1);
		exit(1);
	}
}
static void doUInCalRst(int argc, char *argv[]);
const CliCmdType CMD_UIN_CAL_RST =
	{
		"uincalrst",
		2,
		&doUInCalRst,
		"\tuincalrst:	Reset the calibration for one 0-10V input channel\n",
		"\tUsage:		megaind <id> uincalrst <channel>\n",
		"",
		"\tExample:		megaind 0 uincalrst 2 ; Reset the calibration on 0-10V input channel #2 on Board #0 at factory default\n"};

static void doUInCalRst(int argc, char *argv[])
{
	int ch = 0;

	int dev = 0;
	u8 buff[4] =
	{
		0,
		0,
		0,
		0};

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 4)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > U_IN_CH_NR_MAX))
		{
			printf("0-10V input channel out of range!\n");
			exit(1);
		}

		buff[2] = ch + CAL_0_10V_IN_START_ID - 1;

		buff[3] = RESET_CALIBRATION_KEY;

		if (OK != i2cMem8Write(dev, I2C_MEM_CALIB_VALUE, buff, 4))
		{
			printf("Fail to write calibration data!\n");
			exit(1);
		}

		getCalStat(dev);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_UIN_CAL_RST.usage1);
		exit(1);
	}
}

static void doIInCalRst(int argc, char *argv[]);
const CliCmdType CMD_IIN_CAL_RST =
	{
		"iincalrst",
		2,
		&doIInCalRst,
		"\tiincalrst:	Reset the calibration for one 4-20mA input channel\n",
		"\tUsage:		megaind <id> iincalrst <channel>\n",
		"",
		"\tExample:		megaind 0 iincalrst 2 ; Reset the calibration on 4-20mA input channel #2 on Board #0 at factory default\n"};

static void doIInCalRst(int argc, char *argv[])
{
	int ch = 0;

	int dev = 0;
	u8 buff[4] =
	{
		0,
		0,
		0,
		0};

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 4)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > I_IN_CH_NR_MAX))
		{
			printf("4-20mA input channel out of range!\n");
			exit(1);
		}

		buff[2] = ch + CAL_4_20mA_IN_START_ID - 1;

		buff[3] = RESET_CALIBRATION_KEY;

		if (OK != i2cMem8Write(dev, I2C_MEM_CALIB_VALUE, buff, 4))
		{
			printf("Fail to write calibration data!\n");
			exit(1);
		}

		getCalStat(dev);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_IIN_CAL_RST.usage1);
		exit(1);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
static void doUOutCal(int argc, char *argv[]);
const CliCmdType CMD_UOUT_CAL =
	{
		"uoutcal",
		2,
		&doUOutCal,
		"\tuoutcal:		Calibrate one 0-10V output channel, the calibration must be done in 2 points at min 5V apart\n",
		"\tUsage:		megaind <id> uoutcal <channel> <value(V)>\n",
		"",
		"\tExample:		megaind 0 uoutcal 2 0.5; Calibrate the 0-10V output  channel #2 on Board #0 at 0.5V\n"};

static void doUOutCal(int argc, char *argv[])
{
	int ch = 0;
	float val = 0;
	int dev = 0;
	u8 buff[4] =
	{
		0,
		0};
	u16 raw = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 5)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > U_IN_CH_NR_MAX))
		{
			printf("0-10V output channel out of range!\n");
			exit(1);
		}

		val = atof(argv[4]);
		if ( (val < 0) || (val > 10))
		{
			printf("0-10V input calibration value out of range!\n");
			exit(1);
		}
		raw = (u16)ceil(val * VOLT_TO_MILIVOLT);
		memcpy(buff, &raw, 2);
		buff[2] = ch + CAL_0_10V_OUT_START_ID - 1;
		buff[3] = CALIBRATION_KEY;

		if (OK != i2cMem8Write(dev, I2C_MEM_CALIB_VALUE, buff, 4))
		{
			printf("Fail to write calibration data!\n");
			exit(1);
		}

		getCalStat(dev);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_UOUT_CAL.usage1);
		exit(1);
	}
}

static void doIOutCal(int argc, char *argv[]);
const CliCmdType CMD_IOUT_CAL =
	{
		"ioutcal",
		2,
		&doIOutCal,
		"\tioutcal:		Calibrate one 4-20mA output channel, the calibration must be done in 2 points at min 10mA apart\n",
		"\tUsage:		megaind <id> ioutcal <channel> <value(mA)>\n",
		"",
		"\tExample:		megaind 0 ioutcal 2 5.5; Calibrate the 4-20mA output channel #2 on Board #0 at 5.5mA\n"};

static void doIOutCal(int argc, char *argv[])
{
	int ch = 0;
	float val = 0;
	int dev = 0;
	u8 buff[4] =
	{
		0,
		0};
	u16 raw = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 5)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > I_OUT_CH_NR_MAX))
		{
			printf("4-20mA output channel out of range!\n");
			exit(1);
		}

		val = atof(argv[4]);
		if ( (val < 4) || (val > 20))
		{
			printf("4-20mA output calibration value out of range!\n");
			exit(1);
		}
		raw = (u16)ceil(val * VOLT_TO_MILIVOLT);
		memcpy(buff, &raw, 2);
		buff[2] = ch + CAL_4_20mA_OUT_START_ID - 1;
		buff[3] = CALIBRATION_KEY;

		if (OK != i2cMem8Write(dev, I2C_MEM_CALIB_VALUE, buff, 4))
		{
			printf("Fail to write calibration data!\n");
			exit(1);
		}

		getCalStat(dev);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_IOUT_CAL.usage1);
		exit(1);
	}
}
static void doUOutCalRst(int argc, char *argv[]);
const CliCmdType CMD_UOUT_CAL_RST =
	{
		"uoutcalrst",
		2,
		&doUOutCalRst,
		"\tuoutcalrst:	Reset the calibration for one 0-10V output channel\n",
		"\tUsage:		megaind <id> uoutcalrst <channel>\n",
		"",
		"\tExample:		megaind 0 uoutcalrst 2 ; Reset the calibration on 0-10V output channel #2 on Board #0 at factory default\n"};

static void doUOutCalRst(int argc, char *argv[])
{
	int ch = 0;

	int dev = 0;
	u8 buff[4] =
	{
		0,
		0,
		0,
		0};

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 4)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > U_OUT_CH_NR_MAX))
		{
			printf("0-10V output channel out of range!\n");
			exit(1);
		}

		buff[2] = ch + CAL_0_10V_OUT_START_ID - 1;

		buff[3] = RESET_CALIBRATION_KEY;

		if (OK != i2cMem8Write(dev, I2C_MEM_CALIB_VALUE, buff, 4))
		{
			printf("Fail to write calibration data!\n");
			exit(1);
		}

		getCalStat(dev);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_UOUT_CAL_RST.usage1);
		exit(1);
	}
}

static void doIOutCalRst(int argc, char *argv[]);
const CliCmdType CMD_IOUT_CAL_RST =
	{
		"ioutcalrst",
		2,
		&doIOutCalRst,
		"\tioutcalrst:	Reset the calibration for one 4-20mA output channel\n",
		"\tUsage:		megaind <id> ioutcalrst <channel>\n",
		"",
		"\tExample:		megaind 0 ioutcalrst 2 ; Reset the calibration on 4-20mA output channel #2 on Board #0 at factory default\n"};

static void doIOutCalRst(int argc, char *argv[])
{
	int ch = 0;

	int dev = 0;
	u8 buff[4] =
	{
		0,
		0,
		0,
		0};

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 4)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > I_OUT_CH_NR_MAX))
		{
			printf("4-20mA output channel out of range!\n");
			exit(1);
		}

		buff[2] = ch + CAL_4_20mA_OUT_START_ID - 1;

		buff[3] = RESET_CALIBRATION_KEY;

		if (OK != i2cMem8Write(dev, I2C_MEM_CALIB_VALUE, buff, 4))
		{
			printf("Fail to write calibration data!\n");
			exit(1);
		}

		getCalStat(dev);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_IOUT_CAL_RST.usage1);
		exit(1);
	}
}

static void doWdtReload(int argc, char *argv[]);
const CliCmdType CMD_WDT_RELOAD =
	{
		"wdtr",
		2,
		&doWdtReload,
		"\twdtr:		Reload the watchdog timer and enable the watchdog if is disabled\n",
		"\tUsage:		megaind <id> wdtr\n",
		"",
		"\tExample:		megaind 0 wdtr; Reload the watchdog timer on Board #0 with the period \n"};

static void doWdtReload(int argc, char *argv[])
{
	int dev = 0;
	u8 buff[2] =
	{
		0,
		0};

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 3)
	{
		buff[0] = WDT_RESET_SIGNATURE;
		if (OK != i2cMem8Write(dev, I2C_MEM_WDT_RESET_ADD, buff, 1))
		{
			printf("Fail to write watchdog reset key!\n");
			exit(1);
		}
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_WDT_RELOAD.usage1);
		exit(1);
	}
}

static void doWdtSetPeriod(int argc, char *argv[]);
const CliCmdType CMD_WDT_SET_PERIOD =
	{
		"wdtpwr",
		2,
		&doWdtSetPeriod,
		"\twdtpwr:		Set the watchdog period in seconds, \n\t\t\treload command must be issue in this interval to prevent Raspberry Pi power off\n",
		"\tUsage:		megaind <id> wdtpwr <val> \n",
		"",
		"\tExample:		megaind 0 wdtpwr 10; Set the watchdog timer period on Board #0 at 10 seconds \n"};

static void doWdtSetPeriod(int argc, char *argv[])
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
		exit(1);
	}

	if (argc == 4)
	{
		period = (u16)atoi(argv[3]);
		if (0 == period)
		{
			printf("Invalid period!\n");
			exit(1);
		}
		memcpy(buff, &period, 2);
		if (OK != i2cMem8Write(dev, I2C_MEM_WDT_INTERVAL_SET_ADD, buff, 2))
		{
			printf("Fail to write watchdog period!\n");
			exit(1);
		}
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_WDT_SET_PERIOD.usage1);
		exit(1);
	}
}

static void doWdtGetPeriod(int argc, char *argv[]);
const CliCmdType CMD_WDT_GET_PERIOD =
	{
		"wdtprd",
		2,
		&doWdtGetPeriod,
		"\twdtprd:		Get the watchdog period in seconds, \n\t\t\treload command must be issue in this interval to prevent Raspberry Pi power off\n",
		"\tUsage:		megaind <id> wdtprd \n",
		"",
		"\tExample:		megaind 0 wdtprd; Get the watchdog timer period on Board #0\n"};

static void doWdtGetPeriod(int argc, char *argv[])
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
		exit(1);
	}

	if (argc == 3)
	{
		if (OK != i2cMem8Read(dev, I2C_MEM_WDT_INTERVAL_GET_ADD, buff, 2))
		{
			printf("Fail to read watchdog period!\n");
			exit(1);
		}
		memcpy(&period, buff, 2);
		printf("%d\n", (int)period);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_WDT_GET_PERIOD.usage1);
		exit(1);
	}
}

static void doWdtSetInitPeriod(int argc, char *argv[]);
const CliCmdType CMD_WDT_SET_INIT_PERIOD =
	{
		"wdtipwr",
		2,
		&doWdtSetInitPeriod,
		"\twdtipwr:	Set the watchdog initial period in seconds, \n\t\t\tThis period is loaded after power cycle, giving Raspberry time to boot\n",
		"\tUsage:		megaind <id> wdtipwr <val> \n",
		"",
		"\tExample:		megaind 0 wdtipwr 10; Set the watchdog timer initial period on Board #0 at 10 seconds \n"};

static void doWdtSetInitPeriod(int argc, char *argv[])
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
		exit(1);
	}

	if (argc == 4)
	{
		period = (u16)atoi(argv[3]);
		if (0 == period)
		{
			printf("Invalid period!\n");
			exit(1);
		}
		memcpy(buff, &period, 2);
		if (OK != i2cMem8Write(dev, I2C_MEM_WDT_INIT_INTERVAL_SET_ADD, buff, 2))
		{
			printf("Fail to write watchdog period!\n");
			exit(1);
		}
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_WDT_SET_INIT_PERIOD.usage1);
		exit(1);
	}
}

static void doWdtGetInitPeriod(int argc, char *argv[]);
const CliCmdType CMD_WDT_GET_INIT_PERIOD =
	{
		"wdtiprd",
		2,
		&doWdtGetInitPeriod,
		"\twdtiprd:	Get the watchdog initial period in seconds. \n\t\t\tThis period is loaded after power cycle, giving Raspberry time to boot\n",
		"\tUsage:		megaind <id> wdtiprd \n",
		"",
		"\tExample:		megaind 0 wdtiprd; Get the watchdog timer initial period on Board #0\n"};

static void doWdtGetInitPeriod(int argc, char *argv[])
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
		exit(1);
	}

	if (argc == 3)
	{
		if (OK != i2cMem8Read(dev, I2C_MEM_WDT_INIT_INTERVAL_GET_ADD, buff, 2))
		{
			printf("Fail to read watchdog period!\n");
			exit(1);
		}
		memcpy(&period, buff, 2);
		printf("%d\n", (int)period);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_WDT_GET_INIT_PERIOD.usage1);
		exit(1);
	}
}

static void doWdtSetOffPeriod(int argc, char *argv[]);
const CliCmdType CMD_WDT_SET_OFF_PERIOD =
	{
		"wdtopwr",
		2,
		&doWdtSetOffPeriod,
		"\twdtopwr:	Set the watchdog off period in seconds (max 48 days). \n\t\t\tThis is the time that watchdog mantain Raspberry turned off \n",
		"\tUsage:		megaind <id> wdtopwr <val> \n",
		"",
		"\tExample:		megaind 0 wdtopwr 10; Set the watchdog off interval on Board #0 at 10 seconds \n"};

static void doWdtSetOffPeriod(int argc, char *argv[])
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
		exit(1);
	}

	if (argc == 4)
	{
		period = (u32)atoi(argv[3]);
		if ( (0 == period) || (period > WDT_MAX_OFF_INTERVAL_S))
		{
			printf("Invalid period!\n");
			exit(1);
		}
		memcpy(buff, &period, 4);
		if (OK
			!= i2cMem8Write(dev, I2C_MEM_WDT_POWER_OFF_INTERVAL_SET_ADD, buff, 4))
		{
			printf("Fail to write watchdog period!\n");
			exit(1);
		}
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_WDT_SET_OFF_PERIOD.usage1);
		exit(1);
	}
}

static void doWdtGetOffPeriod(int argc, char *argv[]);
const CliCmdType CMD_WDT_GET_OFF_PERIOD =
	{
		"wdtoprd",
		2,
		&doWdtGetOffPeriod,
		"\twdtoprd:	Get the watchdog off period in seconds (max 48 days) \n\t\t\tThis is the time that watchdog mantain Raspberry turned off \n",
		"\tUsage:		megaind <id> wdtoprd \n",
		"",
		"\tExample:		megaind 0 wdtoprd; Get the watchdog off period on Board #0\n"};

static void doWdtGetOffPeriod(int argc, char *argv[])
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
		exit(1);
	}

	if (argc == 3)
	{
		if (OK
			!= i2cMem8Read(dev, I2C_MEM_WDT_POWER_OFF_INTERVAL_GET_ADD, buff, 4))
		{
			printf("Fail to read watchdog period!\n");
			exit(1);
		}
		memcpy(&period, buff, 4);
		printf("%d\n", (int)period);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_WDT_GET_OFF_PERIOD.usage1);
		exit(1);
	}
}

int rs485Set(int dev, u8 mode, u32 baud, u8 stopB, u8 parity, u8 add)
{
	ModbusSetingsType settings;
	u8 buff[5];

	if (baud > 920600 || baud < 1200)
	{
		printf("Invalid RS485 Baudrate [1200, 920600]!\n");
		return ERROR;
	}
	if (mode > 1)
	{
		printf("Invalid RS485 mode : 0 = disable, 1= Modbus RTU (Slave)!\n");
		return ERROR;
	}
	if (stopB < 1 || stopB > 2)
	{
		printf("Invalid RS485 stop bits [1, 2]!\n");
		return ERROR;
	}
	if (parity > 2)
	{
		printf("Invalid RS485 parity 0 = none; 1 = even; 2 = odd! \n");
		return ERROR;
	}
	if (add < 1)
	{
		printf("Invalid MODBUS device address: [1, 255]!\n");
	}
	settings.mbBaud = baud;
	settings.mbType = mode;
	settings.mbParity = parity;
	settings.mbStopB = stopB;
	settings.add = add;

	memcpy(buff, &settings, sizeof(ModbusSetingsType));
	if (OK != i2cMem8Write(dev, I2C_MODBUS_SETINGS_ADD, buff, 5))
	{
		printf("Fail to write RS485 settings!\n");
		return ERROR;
	}
	return OK;
}

int rs485Get(int dev)
{
	ModbusSetingsType settings;
	u8 buff[5];

	if (OK != i2cMem8Read(dev, I2C_MODBUS_SETINGS_ADD, buff, 5))
	{
		printf("Fail to read RS485 settings!\n");
		return ERROR;
	}
	memcpy(&settings, buff, sizeof(ModbusSetingsType));
	printf("<mode> <baudrate> <stopbits> <parity> <add> %d %d %d %d %d\n",
		(int)settings.mbType, (int)settings.mbBaud, (int)settings.mbStopB,
		(int)settings.mbParity, (int)settings.add);
	return OK;
}

static void doRs485Read(int argc, char *argv[]);
const CliCmdType CMD_RS485_READ =
{
	"rs485rd",
	2,
	&doRs485Read,
	"\trs485rd:	Read the RS485 communication settings\n",
	"\tUsage:		megaind <id> rs485rd\n",
	"",
	"\tExample:		megaind 0 rs485rd; Read the RS485 settings on Board #0\n"};

static void doRs485Read(int argc, char *argv[])
{
	int dev = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 3)
	{
		if (OK != rs485Get(dev))
		{
			exit(1);
		}
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_RS485_READ.usage1);
		exit(1);
	}
}

static void doRs485Write(int argc, char *argv[]);
const CliCmdType CMD_RS485_WRITE =
	{
		"rs485wr",
		2,
		&doRs485Write,
		"\trs485wr:	Write the RS485 communication settings\n",
		"\tUsage:		megaind <id> rs485wr <mode> <baudrate> <stopBits> <parity> <slaveAddr>\n",
		"",
		"\tExample:		megaind 0 rs485wr 1 9600 1 0 1; Write the RS485 settings on Board #0 \n\t\t\t(mode = Modbus RTU; baudrate = 9600 bps; stop bits one; parity none; modbus slave address = 1)\n"};

static void doRs485Write(int argc, char *argv[])
{
	int dev = 0;
	u8 mode = 0;
	u32 baud = 1200;
	u8 stopB = 1;
	u8 parity = 0;
	u8 add = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 8)
	{
		mode = 0xff & atoi(argv[3]);
		baud = atoi(argv[4]);
		stopB = 0xff & atoi(argv[5]);
		parity = 0xff & atoi(argv[6]);
		add = 0xff & atoi(argv[7]);
		if (OK != rs485Set(dev, mode, baud, stopB, parity, add))
		{
			exit(1);
		}
		printf("done\n");
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_RS485_WRITE.usage1);
		exit(1);
	}
}

static void doRTCGet(int argc, char *argv[]);
const CliCmdType CMD_RTC_GET =
	{
		"rtcrd",
		2,
		&doRTCGet,
		"\trtcrd:		Get the internal RTC  date and time(mm/dd/yy hh:mm:ss)\n",
		"\tUsage:		megaind <id> rtcrd \n",
		"",
		"\tExample:	megaind 0 rtcrd; Get the nternal RTC time and date on Board #0\n"};

static void doRTCGet(int argc, char *argv[])
{
	int dev = 0;
	u8 buff[6];

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 3)
	{
		if (OK != i2cMem8Read(dev, I2C_RTC_YEAR_ADD, buff, 6))
		{
			printf("Fail to read RTC!\n");
			exit(1);
		}

		printf("%02u/%02u/%02u %02u:%02u:%02u\n", buff[1], buff[2], buff[0],
			buff[3], buff[4], buff[5]);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_RTC_GET.usage1);
		exit(1);
	}
}

static void doRTCSet(int argc, char *argv[]);
const CliCmdType CMD_RTC_SET =
	{
		"rtcwr",
		2,
		&doRTCSet,
		"\trtcwr:		Set the internal RTC  date and time(mm/dd/yy hh:mm:ss)\n",
		"\tUsage:		megaind <id> rtcwr <mm> <dd> <yy> <hh> <mm> <ss> \n",
		"",
		"\tExample:	megaind 0 rtcwr 9 15 20 21 43 15; Set the internal RTC time and date on Board #0 at Sept/15/2020  21:43:15\n"};

static void doRTCSet(int argc, char *argv[])
{
	int dev = 0;
	u8 buff[7];
	int i = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 9)
	{
		i = atoi(argv[3]);//month
		if(i < 1 || i > 12)
		{
			printf("Invalid month!\n");
			exit(1);
		}
		buff[1] = i;
		i = atoi(argv[4]);
		if(i < 1 || i > 31)
		{
			printf("Invalid date!\n");
			exit(1);
		}
		buff[2] = i;
		i = atoi(argv[5]);
		if(i < 0 || i > 99)
		{
			printf("Invalid year!\n");
			exit(1);
		}
		buff[0] = i;
		i = atoi(argv[6]);
		if(i < 0 || i > 23)
		{
			printf("Invalid hour!\n");
			exit(1);
		}
		buff[3] = i;
		i = atoi(argv[7]);
		if(i < 0 || i > 59)
		{
			printf("Invalid minute!\n");
			exit(1);
		}
		buff[4] = i;
		i = atoi(argv[8]);
		if(i < 0 || i > 59)
		{
			printf("Invalid second!\n");
			exit(1);
		}
		buff[5] = i;
		buff[6] = CALIBRATION_KEY;
		if (OK != i2cMem8Write(dev, I2C_RTC_SET_YEAR_ADD, buff, 7))
		{
			printf("Fail to read RTC!\n");
			exit(1);
		}
		printf("done\n");
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_RTC_SET.usage1);
		exit(1);
	}
}

const CliCmdType* gCmdArray[] =
{
	&CMD_VERSION,
	&CMD_HELP,
	&CMD_WAR,
	&CMD_LIST,
	&CMD_BOARD,
	&CMD_OPTO_READ,
	&CMD_COUNTER_READ,
	&CMD_COUNTER_RST,
	&CMD_EDGE_READ,
	&CMD_EDGE_WRITE,
	&CMD_UOUT_READ,
	&CMD_UOUT_WRITE,
	&CMD_IOUT_READ,
	&CMD_IOUT_WRITE,
	&CMD_OD_READ,
	&CMD_OD_WRITE,
	&CMD_UIN_READ,
	&CMD_PMUIN_READ,
	&CMD_IIN_READ,
	&CMD_UIN_CAL,
	&CMD_IIN_CAL,
	&CMD_UIN_CAL_RST,
	&CMD_IIN_CAL_RST,
	&CMD_UOUT_CAL,
	&CMD_IOUT_CAL,
	&CMD_UOUT_CAL_RST,
	&CMD_IOUT_CAL_RST,
	&CMD_WDT_RELOAD,
	&CMD_WDT_SET_PERIOD,
	&CMD_WDT_GET_PERIOD,
	&CMD_WDT_SET_INIT_PERIOD,
	&CMD_WDT_GET_INIT_PERIOD,
	&CMD_WDT_SET_OFF_PERIOD,
	&CMD_WDT_GET_OFF_PERIOD,
	&CMD_RS485_READ,
	&CMD_RS485_WRITE,
	&CMD_RTC_GET,
	&CMD_RTC_SET,
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
