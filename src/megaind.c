/*
 * rtd.c:
 *	Command-line interface to the Raspberry
 *	Pi's MEGAS-RTD board.
 *	Copyright (c) 2016-2022 Sequent Microsystem
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
#include "cli_def.h"

#define VERSION_BASE	(int)1
#define VERSION_MAJOR	(int)1
#define VERSION_MINOR	(int)4

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
int doHelp(int argc, char *argv[]);
const CliCmdType CMD_HELP =
	{
		"-h",
		1,
		&doHelp,
		"\t-h		Display the list of command options or one command option details\n",
		"\tUsage:		megaind -h    Display command options list\n",
		"\tUsage:		megaind -h <param>   Display help for <param> command option\n",
		"\tExample:		megaind -h rread    Display help for \"rread\" command option\n"};

int doHelp(int argc, char *argv[])
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
	return OK;
}

int doVersion(int argc, char *argv[]);
const CliCmdType CMD_VERSION =
{
	"-v",
	1,
	&doVersion,
	"\t-v		Display the megaind command version number\n",
	"\tUsage:		megaind -v\n",
	"",
	"\tExample:		megaind -v  Display the version number\n"};

int doVersion(int argc, char *argv[])
{
	UNUSED(argc);
	UNUSED(argv);
	printf("megaind v%d.%d.%d Copyright (c) 2016 - 2020 Sequent Microsystems\n",
	VERSION_BASE, VERSION_MAJOR, VERSION_MINOR);
	printf("\nThis is free software with ABSOLUTELY NO WARRANTY.\n");
	printf("For details type: megaind -warranty\n");
	return OK;
}

int doWarranty(int argc, char *argv[]);
const CliCmdType CMD_WAR =
{
	"-warranty",
	1,
	&doWarranty,
	"\t-warranty	Display the warranty\n",
	"\tUsage:		megaind -warranty\n",
	"",
	"\tExample:		megaind -warranty  Display the warranty text\n"};

int doWarranty(int argc UNU, char* argv[] UNU)
{
	printf("%s\n", warranty);
	return OK;
}

int doList(int argc, char *argv[]);
const CliCmdType CMD_LIST =
	{
		"-list",
		1,
		&doList,
		"\t-list:		List all megaind boards connected\n\t\t\treturn the # of boards and stack level for every board\n",
		"\tUsage:		megaind -list\n",
		"",
		"\tExample:		megaind -list display: 1,0 \n"};

int doList(int argc, char *argv[])
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
	return OK;
}

int doBoard(int argc, char *argv[]);
const CliCmdType CMD_BOARD =
{
	"board",
	2,
	&doBoard,
	"\tboard		Display the board status and firmware version number\n",
	"\tUsage:		megaind <stack> board\n",
	"",
	"\tExample:		megaind 0 board  Display vcc, temperature, firmware version \n"};

int doBoard(int argc, char *argv[])
{
	int dev = -1;
	u8 buff[5];
	int resp = 0;
	int temperature = 25;
	float vIn = 24;
	float vRasp = 5;

	if (argc != 3)
	{
		return ARG_CNT_ERR;
	}
	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}
	resp = i2cMem8Read(dev, I2C_MEM_DIAG_TEMPERATURE, buff, 5);
	if (FAIL == resp)
	{
		printf("Fail to read board info!\n");
		return ERROR;
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
		return ERROR;
	}
	printf(
		"Firmware ver %02d.%02d, CPU temperature %d C, Power source %0.2f V, Raspberry %0.2f V\n",
		(int)buff[0], (int)buff[1], temperature, vIn, vRasp);
	return OK;
}

int doVbRead(int argc, char *argv[]);
const CliCmdType CMD_VB_RD =
{
	"vbrd",
	2,
	&doVbRead,
	"\tvbrd		Display the board RTC Backup battery voltage\n",
	"\tUsage:		megaind <stack> vbrd\n",
	"",
	"\tExample:		megaind 0 vbrd  Display backup battery voltage \n"};

int doVbRead(int argc, char *argv[])
{
	int dev = -1;
	u8 buff[5];
	int resp = 0;
	float vBattery = 5;

	if (argc != 3)
	{
		return ARG_CNT_ERR;
	}
	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}
	resp = i2cMem8Read(dev, I2C_MEM_DIAG_3V, buff, 2);
	if (FAIL == resp)
	{
		printf("Fail to read board info!\n");
		return ERROR;
	}

	memcpy(&resp, &buff[0], 2);
	vBattery = (float)resp / VOLT_TO_MILIVOLT; //read in milivolts

	printf("%0.3f V\n", vBattery);
	return OK;
}

int doOwbGet(int argc, char *argv[]);
const CliCmdType CMD_OWB_RD =
	{
		"owbtrd",
		2,
		&doOwbGet,
		"\towbtrd		Display the temperature readed from a one wire bus connected sensor\n",
		"\tUsage:		megaind <stack> owbtrd <sensor (1..16)>\n",
		"",
		"\tExample:		megaind 0 owbtrd 1 Display the temperature of the sensor #1\n"};

int doOwbGet(int argc, char *argv[])
{
	int dev = -1;
	u8 buff[5];
	int resp = 0;
	int channel = 0;
	float temp = 0;

	if (argc != 4)
	{
		return ARG_CNT_ERR;
	}
	channel = atoi(argv[3]);
	if(channel < 1 || channel > 16)
	{
		return ERROR;
	}
	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}
	resp = i2cMem8Read(dev, I2C_MEM_1WB_DEV, buff, 1);
	if (FAIL == resp)
	{
		printf("Fail to read one wire bus info!\n");
		return ERROR;
	}
	if(channel > buff[0])
	{
		printf("Invalid channel number, only %d sensors connected!\n", buff[0]);
		return ERROR;
	}
	
	resp = i2cMem8Read(dev, I2C_MEM_1WB_T1 + (channel - 1) *OWB_TEMP_SIZE_B , buff, OWB_TEMP_SIZE_B);
	if (FAIL == resp)
	{
		printf("Fail to read one wire bus info!\n");
		return ERROR;
	}
//	if (buff[0] == 0)
//	{
//		printf("No sensor connected!\n");
//		return OK;
//	}
	memcpy(&resp, &buff[0], 2);
	temp = (float)resp / 100;

	printf("%0.2f C\n", temp);
	return OK;
}


int doOwbIdGet(int argc, char *argv[]);
const CliCmdType CMD_OWB_ID_RD =
	{
		"owbidrd",
		2,
		&doOwbIdGet,
		"\towbidrd		Display the 64bits ROM ID of the one wire bus connected sensor\n",
		"\tUsage:		megaind <stack> owbidrd <sensor (1..16)>\n",
		"",
		"\tExample:		megaind 0 owbidrd 1 Display the ROM ID of the sensor #1\n"};

int doOwbIdGet(int argc, char *argv[])
{
	int dev = -1;
	u8 buff[8];
	int resp = 0;
	int channel = 0;
	uint64_t romID = 0;

	if (argc != 4)
	{
		return ARG_CNT_ERR;
	}
	channel = atoi(argv[3]);
	if(channel < 1 || channel > 16)
	{
		return ERROR;
	}
	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}
	buff[0] = 0xff & (channel - 1);
	resp = i2cMem8Write(dev, I2C_MEM_1WB_ROM_CODE_IDX, buff, 1);//Select sensor ID to read
	if (FAIL == resp)
	{
		printf("Fail to read one wire bus info!\n");
		return ERROR;
	}
	resp = i2cMem8Read(dev, I2C_MEM_1WB_DEV, buff, 1);//check the number of connected sensors
	if (FAIL == resp)
	{
		printf("Fail to read one wire bus info!\n");
		return ERROR;
	}
	if(channel > buff[0])
	{
		printf("Invalid channel number, only %d sensors connected!\n", buff[0]);
		return ERROR;
	}
	
	resp = i2cMem8Read(dev, I2C_MEM_1WB_ROM_CODE , buff, 8);
	if (FAIL == resp)
	{
		printf("Fail to read one wire bus info!\n");
		return ERROR;
	}

	memcpy(&romID, &buff[0], 8);
	
	printf("0x%llx\n", romID);
	return OK;
}

int doOwbSensCountRead(int argc, char *argv[]);
const CliCmdType CMD_OWB_SNS_CNT_RD =
{
	"owbcntrd",
	2,
	&doOwbSensCountRead,
	"\towbcntrd		Display the number of One Wire Bus connected sensors\n",
	"\tUsage:		megaind <stack> owbcntrd\n",
	"",
	"\tExample:		megaind 0 owbcntrd  Display the number of sensors connected\n"};

int doOwbSensCountRead(int argc, char *argv[])
{
	int dev = -1;
	u8 buff[2];
	int resp = 0;
	
	if (argc != 3)
	{
		return ARG_CNT_ERR;
	}
	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}
	resp = i2cMem8Read(dev, I2C_MEM_1WB_DEV, buff, 1);
	if (FAIL == resp)
	{
		printf("Fail to read!\n");
		return ERROR;
	}

	printf("%d\n", buff[0]);
	return OK;
}


int doOwbScan(int argc, char *argv[]);
const CliCmdType CMD_OWB_SCAN =
{
	"owbscan",
	2,
	&doOwbScan,
	"\towbscan		Start One Wire Bus scaning procedure\n",
	"\tUsage:		megaind <stack> owbscan\n",
	"",
	"\tExample:		megaind 0 owbscan  Start One Wire Bus scaning procedure\n"};

int doOwbScan(int argc, char *argv[])
{
	int dev = -1;
	u8 buff[2];
	int resp = 0;
	
	if (argc != 3)
	{
		return ARG_CNT_ERR;
	}
	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}
	buff[0] = 0xaa;
	resp = i2cMem8Write(dev, I2C_MEM_1WB_START_SEARCH, buff, 1);
	if (FAIL == resp)
	{
		printf("Fail to write!\n");
		return ERROR;
	}

	printf("OK\n");
	return OK;
}

const CliCmdType *gCmdArray[] =
{
	&CMD_VERSION,
	&CMD_HELP,
	&CMD_WAR,
	&CMD_LIST,
	&CMD_BOARD,
	&CMD_VB_RD,
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
	&CMD_DOD_READ,
	&CMD_DOD_WRITE,
	&CMD_LED_READ,
	&CMD_LED_WRITE,
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
	&CMD_WDT_GET_RESETS_COUNT,
	&CMD_WDT_CLR_RESETS_COUNT,
	&CMD_RS485_READ,
	&CMD_RS485_WRITE,
	&CMD_RTC_GET,
	&CMD_RTC_SET,
	&CMD_OWB_RD,
	&CMD_OWB_ID_RD,
	&CMD_OWB_SNS_CNT_RD,
	&CMD_OWB_SCAN,
	NULL}; //null terminated array of cli structure pointers

int main(int argc, char *argv[])
{
	int i = 0;
	int ret = OK;

	if (argc == 1)
	{
		usage();
		return ERROR;
	}
	while (NULL != gCmdArray[i])
	{
		if ( (gCmdArray[i]->name != NULL) && (gCmdArray[i]->namePos < argc))
		{
			if (strcasecmp(argv[gCmdArray[i]->namePos], gCmdArray[i]->name) == 0)
			{
				ret = gCmdArray[i]->pFunc(argc, argv);
				if (ARG_CNT_ERR == ret)
				{
					printf("Invalid parameters number!\n");
					printf("%s", gCmdArray[i]->usage1);
					if (strlen(gCmdArray[i]->usage2) > 2)
					{
						printf("%s", gCmdArray[i]->usage2);
					}
				}
				else if (COMM_ERR == ret)
				{
					printf("Unable to communicate with the card!\n");
				}
				return ret;
			}
		}
		i++;
	}
	printf("Invalid command option\n");
	usage();

	return ERROR;
}
