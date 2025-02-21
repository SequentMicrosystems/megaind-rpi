#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "megaind.h"
#include "comm.h"
#include "thread.h"

int val16Get(int dev, int baseAdd, int ch, float scale, float *val)
{
	u8 buff[2] = {0, 0};
	s16 raw = 0;
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
	u8 buff[2] = {0, 0};
	//u16 raw = 0;
	s16 raw = 0;
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
	raw = (s16)ceil(val * scale); //transform to milivolts
	memcpy(buff, &raw, 2);
	if (OK != i2cMem8Write(dev, add, buff, 2))
	{
		printf("Fail to write!\n");
		return ERROR;
	}
	return OK;
}

int setUinType(int dev, u8 ch, u8 val)
{
	u8 buf = 0;
	u8 needWrite = 0;

	if (OK != i2cMem8Read(dev, I2C_MEM_PM_IN_SW, &buf, 1))
	{
		printf("Fail to check input type\n");
		return ERROR;
	}
	if (val == 0)
	{
		if ((buf & (1 << (ch - 1))) != 0)
		{
			buf &= ~ (1 << (ch - 1));
			needWrite = 1;
		}
	}
	else
	{
		if ((buf & (1 << (ch - 1))) == 0)
		{
			buf |= 1 << (ch - 1);
			needWrite = 1;
		}
	}
	if (needWrite)
	{
		if (OK != i2cMem8Write(dev, I2C_MEM_PM_IN_SW, &buf, 1))
		{
			printf("Fail to set input type!\n");
			return ERROR;
		}
	}
	return OK;

}

int doUOutRead(int argc, char *argv[]);
const CliCmdType CMD_UOUT_READ =
	{"uoutrd", 2, &doUOutRead,
		"\tuoutrd:		Read 0-10V Output voltage value(V) \n",
		"\tUsage:		megaind <id> uoutrd <channel>\n", "",
		"\tExample:		megaind 0 uoutrd 2; Read the voltage on 0-10V out channel #2 on Board #0\n"};

int doUOutRead(int argc, char *argv[])
{
	int ch = 0;
	float val = 0;
	int dev = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 4)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > U_OUT_CH_NR_MAX))
		{
			printf("0-10V Output channel out of range!\n");
			return ERROR;
		}
		if (OK
			!= val16Get(dev, I2C_MEM_U0_10_OUT_VAL1, ch, VOLT_TO_MILIVOLT, &val))
		{
			return ERROR;
		}
		printf("%0.3f\n", val);
	}
	else
	{
		return ARG_CNT_ERR;
	}
	return OK;
}

int doIOutRead(int argc, char *argv[]);
const CliCmdType CMD_IOUT_READ =
	{"ioutrd", 2, &doIOutRead,
		"\tioutrd:		Read 4-20mA Output current value (mA) \n",
		"\tUsage:		megaind <id> ioutrd <channel>\n", "",
		"\tExample:		megaind 0 ioutrd 2; Read the current on 4-20mA out channel #2 on Board #0\n"};

int doIOutRead(int argc, char *argv[])
{
	int ch = 0;
	float val = 0;
	int dev = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 4)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > I_OUT_CH_NR_MAX))
		{
			printf("4-20mA Output channel out of range!\n");
			return ERROR;
		}
		if (OK
			!= val16Get(dev, I2C_MEM_I4_20_OUT_VAL1, ch, VOLT_TO_MILIVOLT, &val))
		{
			return ERROR;
		}
		printf("%0.3f\n", val);
	}
	else
	{
		return ARG_CNT_ERR;
	}
	return OK;
}

int doODRead(int argc, char *argv[]);
const CliCmdType CMD_OD_READ =
	{"odrd", 2, &doODRead,
		"\todrd:		Read open-drain Output PWM value(0..100%) \n",
		"\tUsage:		megaind <id> odrd <channel>\n", "",
		"\tExample:		megaind 0 odrd 2; Read the PWM value on open-drain output channel #2 on Board #0\n"};

int doODRead(int argc, char *argv[])
{
	int ch = 0;
	float val = 0;
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
		if (OK != val16Get(dev, I2C_MEM_OD_PWM1, ch, 100, &val))
		{
			return ERROR;
		}
		printf("%0.3f\n", val);
	}
	else
	{
		return ARG_CNT_ERR;
	}
	return OK;
}

int doODFreqRead(int argc, char *argv[]);
const CliCmdType CMD_OD_FREQ_READ =
	{"odfrd", 2, &doODFreqRead,
		"\todfrd:		Read open-drain Output PWM frequency(10..6400Hz) \n",
		"\tUsage:		megaind <id> odfrd <channel>\n", "",
		"\tExample:		megaind 0 odfrd 2; Read the PWM frequency on open-drain output channel #2 on Board #0\n"};

int doODFreqRead(int argc, char *argv[])
{
	int ch = 0;
	float val = 0;
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
		if(ch == OD_CH_NR_MAX)
		{
			val = 6400;
		}
		else
		if (OK != val16Get(dev, I2C_MEM_OD1_FREQ, ch, 1, &val))
		{
			return ERROR;
		}
		printf("%d\n", (int)val);
	}
	else
	{
		return ARG_CNT_ERR;
	}
	return OK;
}


int doOptoFreqRead(int argc, char *argv[]);
const CliCmdType CMD_IF_READ =
	{"ifrd", 2, &doOptoFreqRead,
		"\tifrd:		Read frequency applied to opto inputs \n",
		"\tUsage:		megaind <id> ifrd <channel>\n", "",
		"\tExample:		megaind 0 ifrd 2; Read the signal frequency applied to opto input channel #2 on Board #0\n"};

int doOptoFreqRead(int argc, char *argv[])
{
	int ch = 0;
	float val = 0;
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
			printf("Opto input channel out of range!\n");
			return ERROR;
		}
		if (OK != val16Get(dev, I2C_MEM_OPTO_FREQ1, ch, 1, &val))
		{
			return ERROR;
		}
		printf("%d\n", (int)val);
	}
	else
	{
		return ARG_CNT_ERR;
	}
	return OK;
}

int doUOutWrite(int argc, char *argv[]);
const CliCmdType CMD_UOUT_WRITE =
	{"uoutwr", 2, &doUOutWrite,
		"\tuoutwr:		Write 0-10V output voltage value (V)\n",
		"\tUsage:		megaind <id> uoutwr <channel> <value(V)>\n", "",
		"\tExample:		megaind 0 uoutwr 2 2.5; Write 2.5V to 0-10V output channel #2 on Board #0\n"};

int doUOutWrite(int argc, char *argv[])
{
	int ch = 0;
	int dev = 0;
	float volt = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 5)
	{
		ch = atoi(argv[3]);

		volt = atof(argv[4]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > U_OUT_CH_NR_MAX))
		{
			printf("0-10V Output channel out of range!\n");
			return ERROR;
		}
		if (volt < -10 || volt > 10)
		{
			printf("Invalid voltage value, must be -10..10 \n");
			return ERROR;
		}

		if (OK
			!= val16Set(dev, I2C_MEM_U0_10_OUT_VAL1, ch, VOLT_TO_MILIVOLT, volt))
		{
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

int doIOutWrite(int argc, char *argv[]);
const CliCmdType CMD_IOUT_WRITE =
	{"ioutwr", 2, &doIOutWrite, "\tioutwr:		Write 4-20mA output value (mA)\n",
		"\tUsage:		megaind <id> ioutwr <channel> <value(mA)>\n", "",
		"\tExample:		megaind 0 ioutwr 2 10.5; Set 10.5mA to 4-20mA output channel #2 on Board #0\n"};

int doIOutWrite(int argc, char *argv[])
{
	int ch = 0;
	int dev = 0;
	float volt = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 5)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > I_OUT_CH_NR_MAX))
		{
			printf("4-20mA Output channel out of range!\n");
			return ERROR;
		}
		volt = atof(argv[4]);
		if (volt < 0 || volt > 20)
		{
			printf("Invalid current value, must be 0..20 \n");
			return ERROR;
		}

		if (OK
			!= val16Set(dev, I2C_MEM_I4_20_OUT_VAL1, ch, VOLT_TO_MILIVOLT, volt))
		{
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

int doODWrite(int argc, char *argv[]);
const CliCmdType CMD_OD_WRITE =
	{"odwr", 2, &doODWrite,
		"\todwr:		Write open-drain output PWM value (0..100%)\n",
		"\tUsage:		megaind <id> odwr <channel> <value>\n", "",
		"\tExample:		megaind 0 odwr 2 10.5; Set PWM 10.5% to open-drain output channel #2 on Board #0\n"};

int doODWrite(int argc, char *argv[])
{
	int ch = 0;
	int dev = 0;
	float volt = 0;

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
			printf("Open-drain channel out of range!\n");
			return ERROR;
		}
		volt = atof(argv[4]);
		if (volt < 0 || volt > 100)
		{
			printf("Invalid PWM value, must be 0..100 \n");
			return ERROR;
		}

		if (OK != val16Set(dev, I2C_MEM_OD_PWM1, ch, 100, volt))
		{
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
int doODFreqWrite(int argc, char *argv[]);
const CliCmdType CMD_OD_FREQ_WRITE =
	{"odfwr", 2, &doODFreqWrite,
		"\todfwr:		Write open-drain output PWM frequency value (10..6400Hz)\n",
		"\tUsage:		megaind <id> odfwr <channel[1..3]> <value[10..6400]>\n", "",
		"\tExample:		megaind 0 odfwr 2 250; Set PWM frequency of 250Hz for open-drain output channel #2 on Board #0\n"};

int doODFreqWrite(int argc, char *argv[])
{
	int ch = 0;
	int dev = 0;
	int frequency = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 5)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > 3))
		{
			printf("Invalid Open-drain channel for frequency change, must be 1..3!\n");
			return ERROR;
		}
		frequency = atoi(argv[4]);
		if (frequency < 10 || frequency > 6400)
		{
			printf("Invalid PWM frequency value, must be 10..6400 \n");
			return ERROR;
		}

		if (OK != val16Set(dev, I2C_MEM_OD1_FREQ, ch, 1, frequency))
		{
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
int doUInRead(int argc, char *argv[]);
const CliCmdType CMD_UIN_READ =
	{"uinrd", 2, &doUInRead, "\tuinrd:		Read 0-10V input value (V) \n",
		"\tUsage:		megaind <id> uinrd <channel>\n", "",
		"\tExample:		megaind 0 uinrd 2; Read the voltage input on 0-10V in channel #2 on Board #0\n"};

int doUInRead(int argc, char *argv[])
{
	int ch = 0;
	float val = 0;
	int dev = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 4)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > U_IN_CH_NR_MAX))
		{
			printf("0-10V input channel out of range!\n");
			return ERROR;
		}
		setUinType(dev, ch, 0);// skip the error
		if (OK
			!= val16Get(dev, I2C_MEM_U0_10_IN_VAL1, ch, VOLT_TO_MILIVOLT, &val))
		{
			return ERROR;
		}

		printf("%0.3f\n", val);
	}
	else
	{
		return ARG_CNT_ERR;
	}
	return OK;
}

int doPmUInRead(int argc, char *argv[]);
const CliCmdType CMD_PMUIN_READ =
	{"pmuinrd", 2, &doPmUInRead,
		"\tpmuinrd:		Read +/-10V input value (V). Warning: This value is valid only if the corespondung jumper is connected\n",
		"\tUsage:		megaind <id> pmuinrd <channel>\n", "",
		"\tExample:		megaind 0 pmuinrd 2; Read the voltage input on +/-10V in channel #2 on Board #0\n"};

int doPmUInRead(int argc, char *argv[])
{
	int ch = 0;
	float val = 0;
	int dev = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 4)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > U_IN_CH_NR_MAX))
		{
			printf("+/-10V input channel out of range!\n");
			return ERROR;
		}
		setUinType(dev, ch, 1);// skip the error

		if (OK
			!= val16Get(dev, I2C_MEM_U_PM_10_IN_VAL1, ch, VOLT_TO_MILIVOLT, &val))
		{
			return ERROR;
		}
		val -= 10;
		printf("%0.3f\n", val);
	}
	else
	{
		return ARG_CNT_ERR;
	}
	return OK;
}

int doIInRead(int argc, char *argv[]);
const CliCmdType CMD_IIN_READ =
	{"iinrd", 2, &doIInRead, "\tiinrd:		Read 4-20mA input value (mA) \n",
		"\tUsage:		megaind <id> iinrd <channel>\n", "",
		"\tExample:		megaind 0 iinrd 2; Read the voltage input on 4-20mA in channel #2 on Board #0\n"};

int doIInRead(int argc, char *argv[])
{
	int ch = 0;
	float val = 0;
	int dev = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 4)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > I_IN_CH_NR_MAX))
		{
			printf("4-20mA input channel out of range!\n");
			return ERROR;
		}

		if (OK
			!= val16Get(dev, I2C_MEM_I4_20_IN_VAL1, ch, VOLT_TO_MILIVOLT, &val))
		{
			return ERROR;
		}

		printf("%0.3f\n", val);
	}
	else
	{
		return ARG_CNT_ERR;
	}
	return OK;
}

void getCalStat(int dev)
{
	u8 buff[2];

	busyWait(100);
	if (OK != i2cMem8Read(dev, I2C_MEM_CALIB_STATUS, buff, 1))
	{
		printf("Fail to read calibration status!\n");
		return;
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

int doUInCal(int argc, char *argv[]);
const CliCmdType CMD_UIN_CAL =
	{"uincal", 2, &doUInCal,
		"\tuincal:		Calibrate one 0-10V input channel, the calibration must be done in 2 points at min 5V apart\n",
		"\tUsage:		megaind <id> uincal <channel> <value(V)>\n", "",
		"\tExample:		megaind 0 uincal 2 0.5; Calibrate the 0-10V input  channel #2 on Board #0 at 0.5V\n"};

int doUInCal(int argc, char *argv[])
{
	int ch = 0;
	float val = 0;
	int dev = 0;
	u8 buff[4] = {0, 0};
	u16 raw = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 5)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > U_IN_CH_NR_MAX))
		{
			printf("0-10V input channel out of range!\n");
			return ERROR;
		}

		val = atof(argv[4]);
		if ( (val < 0) || (val > 10))
		{
			printf("0-10V input calibration value out of range!\n");
			return ERROR;
		}
		raw = (u16)ceil(val * VOLT_TO_MILIVOLT);
		memcpy(buff, &raw, 2);
		buff[2] = ch + CAL_0_10V_IN_START_ID - 1;
		buff[3] = CALIBRATION_KEY;

		if (OK != i2cMem8Write(dev, I2C_MEM_CALIB_VALUE, buff, 4))
		{
			printf("Fail to write calibration data!\n");
			return ERROR;
		}

		getCalStat(dev);
	}
	else
	{
		return ARG_CNT_ERR;
	}
	return OK;
}

int doIInCal(int argc, char *argv[]);
const CliCmdType CMD_IIN_CAL =
	{"iincal", 2, &doIInCal,
		"\tiincal:		Calibrate one 4-20mA input channel, the calibration must be done in 2 points at min 10mA apart\n",
		"\tUsage:		megaind <id> iincal <channel> <value(mA)>\n", "",
		"\tExample:		megaind 0 iincal 2 5.5; Calibrate the 4-20mA input  channel #2 on Board #0 at 5.5mA\n"};

int doIInCal(int argc, char *argv[])
{
	int ch = 0;
	float val = 0;
	int dev = 0;
	u8 buff[4] = {0, 0};
	u16 raw = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 5)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > I_IN_CH_NR_MAX))
		{
			printf("4-20mA input channel out of range!\n");
			return ERROR;
		}

		val = atof(argv[4]);
		if ( (val < 4) || (val > 20))
		{
			printf("4-20mA input calibration value out of range!\n");
			return ERROR;
		}
		raw = (u16)ceil(val * VOLT_TO_MILIVOLT);
		memcpy(buff, &raw, 2);
		buff[2] = ch + CAL_4_20mA_IN_START_ID - 1;
		buff[3] = CALIBRATION_KEY;

		if (OK != i2cMem8Write(dev, I2C_MEM_CALIB_VALUE, buff, 4))
		{
			printf("Fail to write calibration data!\n");
			return ERROR;
		}

		getCalStat(dev);
	}
	else
	{
		return ARG_CNT_ERR;
	}
	return OK;
}

int doUInCalRst(int argc, char *argv[]);
const CliCmdType CMD_UIN_CAL_RST =
	{"uincalrst", 2, &doUInCalRst,
		"\tuincalrst:	Reset the calibration for one 0-10V input channel\n",
		"\tUsage:		megaind <id> uincalrst <channel>\n", "",
		"\tExample:		megaind 0 uincalrst 2 ; Reset the calibration on 0-10V input channel #2 on Board #0 at factory default\n"};

int doUInCalRst(int argc, char *argv[])
{
	int ch = 0;

	int dev = 0;
	u8 buff[4] = {0, 0, 0, 0};

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 4)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > U_IN_CH_NR_MAX))
		{
			printf("0-10V input channel out of range!\n");
			return ERROR;
		}

		buff[2] = ch + CAL_0_10V_IN_START_ID - 1;

		buff[3] = RESET_CALIBRATION_KEY;

		if (OK != i2cMem8Write(dev, I2C_MEM_CALIB_VALUE, buff, 4))
		{
			printf("Fail to write calibration data!\n");
			return ERROR;
		}

		getCalStat(dev);
	}
	else
	{
		return ARG_CNT_ERR;
	}
	return OK;
}

int doIInCalRst(int argc, char *argv[]);
const CliCmdType CMD_IIN_CAL_RST =
	{"iincalrst", 2, &doIInCalRst,
		"\tiincalrst:	Reset the calibration for one 4-20mA input channel\n",
		"\tUsage:		megaind <id> iincalrst <channel>\n", "",
		"\tExample:		megaind 0 iincalrst 2 ; Reset the calibration on 4-20mA input channel #2 on Board #0 at factory default\n"};

int doIInCalRst(int argc, char *argv[])
{
	int ch = 0;

	int dev = 0;
	u8 buff[4] = {0, 0, 0, 0};

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 4)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > I_IN_CH_NR_MAX))
		{
			printf("4-20mA input channel out of range!\n");
			return ERROR;
		}

		buff[2] = ch + CAL_4_20mA_IN_START_ID - 1;

		buff[3] = RESET_CALIBRATION_KEY;

		if (OK != i2cMem8Write(dev, I2C_MEM_CALIB_VALUE, buff, 4))
		{
			printf("Fail to write calibration data!\n");
			return ERROR;
		}

		getCalStat(dev);
	}
	else
	{
		return ARG_CNT_ERR;
	}
	return OK;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
int doUOutCal(int argc, char *argv[]);
const CliCmdType CMD_UOUT_CAL =
	{"uoutcal", 2, &doUOutCal,
		"\tuoutcal:		Calibrate one 0-10V output channel, the calibration must be done in 2 points at min 5V apart\n",
		"\tUsage:		megaind <id> uoutcal <channel> <value(V)>\n", "",
		"\tExample:		megaind 0 uoutcal 2 0.5; Calibrate the 0-10V output  channel #2 on Board #0 at 0.5V\n"};

int doUOutCal(int argc, char *argv[])
{
	int ch = 0;
	float val = 0;
	int dev = 0;
	u8 buff[4] = {0, 0};
	u16 raw = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 5)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > U_IN_CH_NR_MAX))
		{
			printf("0-10V output channel out of range!\n");
			return ERROR;
		}

		val = atof(argv[4]);
		if ( (val < 0) || (val > 10))
		{
			printf("0-10V input calibration value out of range!\n");
			return ERROR;
		}
		raw = (u16)ceil(val * VOLT_TO_MILIVOLT);
		memcpy(buff, &raw, 2);
		buff[2] = ch + CAL_0_10V_OUT_START_ID - 1;
		buff[3] = CALIBRATION_KEY;

		if (OK != i2cMem8Write(dev, I2C_MEM_CALIB_VALUE, buff, 4))
		{
			printf("Fail to write calibration data!\n");
			return ERROR;
		}

		getCalStat(dev);
	}
	else
	{
		return ARG_CNT_ERR;
	}
	return OK;
}

int doIOutCal(int argc, char *argv[]);
const CliCmdType CMD_IOUT_CAL =
	{"ioutcal", 2, &doIOutCal,
		"\tioutcal:		Calibrate one 4-20mA output channel, the calibration must be done in 2 points at min 10mA apart\n",
		"\tUsage:		megaind <id> ioutcal <channel> <value(mA)>\n", "",
		"\tExample:		megaind 0 ioutcal 2 5.5; Calibrate the 4-20mA output channel #2 on Board #0 at 5.5mA\n"};

int doIOutCal(int argc, char *argv[])
{
	int ch = 0;
	float val = 0;
	int dev = 0;
	u8 buff[4] = {0, 0};
	u16 raw = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 5)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > I_OUT_CH_NR_MAX))
		{
			printf("4-20mA output channel out of range!\n");
			return ERROR;
		}

		val = atof(argv[4]);
		if ( (val < 4) || (val > 20))
		{
			printf("4-20mA output calibration value out of range!\n");
			return ERROR;
		}
		raw = (u16)ceil(val * VOLT_TO_MILIVOLT);
		memcpy(buff, &raw, 2);
		buff[2] = ch + CAL_4_20mA_OUT_START_ID - 1;
		buff[3] = CALIBRATION_KEY;

		if (OK != i2cMem8Write(dev, I2C_MEM_CALIB_VALUE, buff, 4))
		{
			printf("Fail to write calibration data!\n");
			return ERROR;
		}

		getCalStat(dev);
	}
	else
	{
		return ARG_CNT_ERR;
	}
	return OK;
}
int doUOutCalRst(int argc, char *argv[]);
const CliCmdType CMD_UOUT_CAL_RST =
	{"uoutcalrst", 2, &doUOutCalRst,
		"\tuoutcalrst:	Reset the calibration for one 0-10V output channel\n",
		"\tUsage:		megaind <id> uoutcalrst <channel>\n", "",
		"\tExample:		megaind 0 uoutcalrst 2 ; Reset the calibration on 0-10V output channel #2 on Board #0 at factory default\n"};

int doUOutCalRst(int argc, char *argv[])
{
	int ch = 0;

	int dev = 0;
	u8 buff[4] = {0, 0, 0, 0};

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 4)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > U_OUT_CH_NR_MAX))
		{
			printf("0-10V output channel out of range!\n");
			return ERROR;
		}

		buff[2] = ch + CAL_0_10V_OUT_START_ID - 1;

		buff[3] = RESET_CALIBRATION_KEY;

		if (OK != i2cMem8Write(dev, I2C_MEM_CALIB_VALUE, buff, 4))
		{
			printf("Fail to write calibration data!\n");
			return ERROR;
		}

		getCalStat(dev);
	}
	else
	{
		return ARG_CNT_ERR;
	}
	return OK;
}

int doIOutCalRst(int argc, char *argv[]);
const CliCmdType CMD_IOUT_CAL_RST =
	{"ioutcalrst", 2, &doIOutCalRst,
		"\tioutcalrst:	Reset the calibration for one 4-20mA output channel\n",
		"\tUsage:		megaind <id> ioutcalrst <channel>\n", "",
		"\tExample:		megaind 0 ioutcalrst 2 ; Reset the calibration on 4-20mA output channel #2 on Board #0 at factory default\n"};

int doIOutCalRst(int argc, char *argv[])
{
	int ch = 0;

	int dev = 0;
	u8 buff[4] = {0, 0, 0, 0};

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 4)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > I_OUT_CH_NR_MAX))
		{
			printf("4-20mA output channel out of range!\n");
			return ERROR;
		}

		buff[2] = ch + CAL_4_20mA_OUT_START_ID - 1;

		buff[3] = RESET_CALIBRATION_KEY;

		if (OK != i2cMem8Write(dev, I2C_MEM_CALIB_VALUE, buff, 4))
		{
			printf("Fail to write calibration data!\n");
			return ERROR;
		}

		getCalStat(dev);
	}
	else
	{
		return ARG_CNT_ERR;
	}
	return OK;
}
