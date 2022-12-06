import smbus
import struct
import time

__HW_ADD_BASE = 0x50
VOLT_TO_MILIVOLT = 1000.0
BUS_NO = 1  # change this in case of using different SBC, for example use 7 for ROCK 4 SE

def checkStack(stack):
    if stack < 0 or stack > 7:
        raise ValueError('Invalid stack level!')
    return __HW_ADD_BASE + stack


def checkChannel(ch, limit=4):
    if ch < 1 or ch > limit:
        raise ValueError('Invalid channel number!')

def checkOwbSns(ch):
    if ch < 1 or ch > 16:
        raise ValueError('One Wire Bus sensor number out of range [1..16]')

def getWord(bus, hwAdd, add):
    retry = 0
    try:
        buff = bus.read_i2c_block_data(hwAdd, add, 2)
        val = bytearray(buff)[0] + 256 * bytearray(buff)[1]
    except Exception as e:
        bus.close()
        raise Exception("Fail to read with exception " + str(e))
    return val


# Diagnose functions
I2C_MEM_DIAG_TEMPERATURE = 114
I2C_MEM_DIAG_24V = 115
I2C_MEM_DIAG_5V = 117
I2C_MEM_REVISION_MAJOR = 120
I2C_MEM_REVISION_MINOR = 121


def getFwVer(stack):
    hwAdd = checkStack(stack)
    bus = smbus.SMBus(BUS_NO)
    try:
        major = bus.read_byte_data(hwAdd, I2C_MEM_REVISION_MAJOR)
        minor = bus.read_byte_data(hwAdd, I2C_MEM_REVISION_MINOR)
    except Exception as e:
        bus.close()
        raise Exception("Fail to read with exception " + str(e))
    bus.close()
    return major + minor / 100.0


def getRaspVolt(stack):
    hwAdd = checkStack(stack)
    bus = smbus.SMBus(BUS_NO)
    try:
        val = bus.read_word_data(hwAdd, I2C_MEM_DIAG_5V)
    except Exception as e:
        bus.close()
        raise Exception("Fail to read with exception " + str(e))
    bus.close()
    return val / VOLT_TO_MILIVOLT


def getPowerVolt(stack):
    hwAdd = checkStack(stack)
    bus = smbus.SMBus(BUS_NO)
    val = getWord(bus, hwAdd, I2C_MEM_DIAG_24V)
    bus.close()
    return val / VOLT_TO_MILIVOLT


def getCpuTemp(stack):
    hwAdd = checkStack(stack)
    bus = smbus.SMBus(BUS_NO)
    try:
        val = bus.read_byte_data(hwAdd, I2C_MEM_DIAG_TEMPERATURE)
    except Exception as e:
        bus.close()
        raise Exception("Fail to read with exception " + str(e))
    bus.close()
    return val


# 0 to 10 volts input and output functions
U0_10_IN_VAL1_ADD = 28
U_PM_10_IN_VAL1_ADD = 36
U_0_10_OUT_VAL1_ADD = 4


def get0_10In(stack, channel):
    checkChannel(channel)
    hwAdd = checkStack(stack)
    bus = smbus.SMBus(BUS_NO)
    val = getWord(bus, hwAdd, U0_10_IN_VAL1_ADD + (2 * (channel - 1)))
    bus.close()
    return val / VOLT_TO_MILIVOLT


def getpm10In(stack, channel):
    checkChannel(channel)
    hwAdd = checkStack(stack)
    bus = smbus.SMBus(BUS_NO)
    val = getWord(bus, hwAdd, U_PM_10_IN_VAL1_ADD + (2 * (channel - 1)))
    bus.close()
    return val / VOLT_TO_MILIVOLT - 10


def get0_10Out(stack, channel):
    checkChannel(channel)
    hwAdd = checkStack(stack)
    bus = smbus.SMBus(BUS_NO)
    val = getWord(bus, hwAdd, U_0_10_OUT_VAL1_ADD + (2 * (channel - 1)))
    bus.close()
    return val / VOLT_TO_MILIVOLT


def set0_10Out(stack, channel, value):
    checkChannel(channel)
    hwAdd = checkStack(stack)
    bus = smbus.SMBus(BUS_NO)
    if value < 0 or value > 10:
        raise ValueError("Invalid value!")
    try:
        bus.write_word_data(hwAdd, U_0_10_OUT_VAL1_ADD + (2 * (channel - 1)), int(value * 1000))
    except Exception as e:
        bus.close()
        raise Exception("Fail to Write 0-10V output with exception " + str(e))
    bus.close()


# 4 - 20 mA in/out functions
I4_20_IN_VAL1_ADD = 44
I4_20_OUT_VAL1_ADD = 12
MILLIAMP_TO_MICROAMP = 1000.0


def get4_20In(stack, channel):
    checkChannel(channel)
    hwAdd = checkStack(stack)
    bus = smbus.SMBus(BUS_NO)
    val = getWord(bus, hwAdd, I4_20_IN_VAL1_ADD + (2 * (channel - 1)))
    bus.close()
    return val / 1000.0


def get4_20Out(stack, channel):
    checkChannel(channel)
    hwAdd = checkStack(stack)
    bus = smbus.SMBus(BUS_NO)
    val = getWord(bus, hwAdd, I4_20_OUT_VAL1_ADD + (2 * (channel - 1)))
    bus.close()
    return val / 1000.0


def set4_20Out(stack, channel, value):
    checkChannel(channel)
    hwAdd = checkStack(stack)
    bus = smbus.SMBus(BUS_NO)
    if value < 4 or value > 20:
        raise ValueError("Invalid value!")
    try:
        bus.write_word_data(hwAdd, I4_20_OUT_VAL1_ADD + (2 * (channel - 1)), int(value * 1000))
    except Exception as e:
        bus.close()
        raise Exception("Fail to Write 4-20mA output with exception " + str(e))
    bus.close()


# digital in/out functions
I2C_MEM_RELAY_VAL = 0
I2C_MEM_RELAY_SET = 1
I2C_MEM_RELAY_CLR = 2
I2C_MEM_OPTO_IN_VAL = 3
I2C_MEM_OD_PWM1 = 20
I2C_MEM_OPTO_RISING_ENABLE = 103
I2C_MEM_OPTO_FALLING_ENABLE = 104
I2C_MEM_OPTO_CH_CONT_RESET = 105
I2C_MEM_OPTO_COUNT1 = 106


def getOptoCh(stack, channel):
    checkChannel(channel)
    hwAdd = checkStack(stack)
    bus = smbus.SMBus(BUS_NO)
    try:
        val = bus.read_byte_data(hwAdd, I2C_MEM_OPTO_IN_VAL)
    except Exception as e:
        bus.close()
        raise Exception("Fail to read with exception " + str(e))
    bus.close()
    if (1 << (channel - 1)) & val:
        return 1
    return 0


def getOpto(stack):
    hwAdd = checkStack(stack)
    bus = smbus.SMBus(BUS_NO)
    try:
        val = bus.read_byte_data(hwAdd, I2C_MEM_OPTO_IN_VAL)
    except Exception as e:
        bus.close()
        raise Exception("Fail to read with exception " + str(e))
    bus.close()
    return val


def getOptoCount(stack, channel):
    checkChannel(channel)
    hwAdd = checkStack(stack)
    bus = smbus.SMBus(BUS_NO)
    try:
        val = bus.read_word_data(hwAdd, I2C_MEM_OPTO_COUNT1 + (2 * (channel - 1)))
    except Exception as e:
        bus.close()
        raise Exception("Fail to read with exception " + str(e))
    bus.close()
    return val


def rstOptoCount(stack, channel):
    checkChannel(channel)
    hwAdd = checkStack(stack)
    bus = smbus.SMBus(BUS_NO)
    try:
        bus.write_byte_data(hwAdd, I2C_MEM_OPTO_CH_CONT_RESET, int(channel))
    except Exception as e:
        bus.close()
        raise Exception("Fail to write with exception " + str(e))
    bus.close()


def getOptoRisingCountEnable(stack, channel):
    checkChannel(channel)
    hwAdd = checkStack(stack)
    bus = smbus.SMBus(BUS_NO)
    try:
        val = bus.read_byte_data(hwAdd, I2C_MEM_OPTO_RISING_ENABLE)
    except Exception as e:
        bus.close()
        raise Exception("Fail to read with exception " + str(e))
    bus.close()
    if (1 << (channel - 1)) & val != 0:
        return 1
    return 0


def setOptoRisingCountEnable(stack, channel, state):
    checkChannel(channel)
    hwAdd = checkStack(stack)
    bus = smbus.SMBus(BUS_NO)
    try:
        val = bus.read_byte_data(hwAdd, I2C_MEM_OPTO_RISING_ENABLE)
    except Exception as e:
        bus.close()
        raise Exception("Fail to read with exception " + str(e))
    if state == 0:
        val &= ~(1 << (channel - 1))
    else:
        val |= 1 << (channel - 1)
    try:
        bus.write_byte_data(hwAdd, I2C_MEM_OPTO_RISING_ENABLE, val)
    except Exception as e:
        bus.close()
        raise Exception("Fail to write with exception " + str(e))
    bus.close()


def getOptoFallingCountEnable(stack, channel):
    checkChannel(channel)
    hwAdd = checkStack(stack)
    bus = smbus.SMBus(BUS_NO)
    try:
        val = bus.read_byte_data(hwAdd, I2C_MEM_OPTO_FALLING_ENABLE)
    except Exception as e:
        bus.close()
        raise Exception("Fail to read with exception " + str(e))
    bus.close()
    if (1 << (channel - 1)) & val != 0:
        return 1
    return 0


def setOptoFallingCountEnable(stack, channel, state):
    checkChannel(channel)
    hwAdd = checkStack(stack)
    bus = smbus.SMBus(BUS_NO)
    try:
        val = bus.read_byte_data(hwAdd, I2C_MEM_OPTO_FALLING_ENABLE)
    except Exception as e:
        bus.close()
        raise Exception("Fail to read with exception " + str(e))
    if state == 0:
        val &= ~(1 << (channel - 1))
    else:
        val |= 1 << (channel - 1)
    try:
        bus.write_byte_data(hwAdd, I2C_MEM_OPTO_FALLING_ENABLE, val)
    except Exception as e:
        bus.close()
        raise Exception("Fail to write with exception " + str(e))
    bus.close()


def setOdPWM(stack, channel, value):
    checkChannel(channel)
    hwAdd = checkStack(stack)
    bus = smbus.SMBus(BUS_NO)
    if value < 0 or value > 100:  # prcent
        raise ValueError("Invalid value!")
    try:
        bus.write_word_data(hwAdd, I2C_MEM_OD_PWM1 + (2 * (channel - 1)), int(value * 100))
    except Exception as e:
        bus.close()
        raise Exception("Fail to Write Open-Drain output PWM with exception " + str(e))
    bus.close()


def getOdPWM(stack, channel):
    checkChannel(channel)
    hwAdd = checkStack(stack)
    bus = smbus.SMBus(BUS_NO)
    val = getWord(bus, hwAdd, I2C_MEM_OD_PWM1 + (2 * (channel - 1)))
    bus.close()
    return val / 100.0


def setLed(stack, channel, val):
    checkChannel(channel)
    hwAdd = checkStack(stack)
    bus = smbus.SMBus(BUS_NO)
    out = channel + 4
    try:
        if val != 0:
            bus.write_byte_data(hwAdd, I2C_MEM_RELAY_SET, out)
        else:
            bus.write_byte_data(hwAdd, I2C_MEM_RELAY_CLR, out)
    except Exception as e:
        bus.close()
        raise Exception("Fail to Write LED's with exception " + str(e))
    bus.close()


def setLedAll(stack, val):
    if val < 0 or val > 15:
        raise ValueError("Invalid value!")
    val = val << 4
    hwAdd = checkStack(stack)
    bus = smbus.SMBus(BUS_NO)
    try:
        bus.write_byte_data(hwAdd, I2C_MEM_RELAY_VAL, val)
    except Exception as e:
        bus.close()
        raise Exception("Fail to Write LED's with exception " + str(e))
    bus.close()


def getLed(stack, channel):
    checkChannel(channel)
    hwAdd = checkStack(stack)
    bus = smbus.SMBus(BUS_NO)
    mask = 1 << (channel + 3)
    try:
        val = bus.read_byte_data(hwAdd, I2C_MEM_RELAY_VAL)
    except Exception as e:
        bus.close()
        raise Exception("Fail to Write LED's with exception " + str(e))
    bus.close()
    if val & mask:
        return 1
    return 0


# watchdog functions
I2C_MEM_WDT_RESET_ADD = 83
I2C_MEM_WDT_INTERVAL_SET_ADD = 84
I2C_MEM_WDT_INTERVAL_GET_ADD = I2C_MEM_WDT_INTERVAL_SET_ADD + 2
I2C_MEM_WDT_INIT_INTERVAL_SET_ADD = I2C_MEM_WDT_INTERVAL_GET_ADD + 2
I2C_MEM_WDT_INIT_INTERVAL_GET_ADD = I2C_MEM_WDT_INIT_INTERVAL_SET_ADD + 2
I2C_MEM_WDT_RESET_COUNT_ADD = I2C_MEM_WDT_INIT_INTERVAL_GET_ADD + 2
I2C_MEM_WDT_CLEAR_RESET_COUNT_ADD = I2C_MEM_WDT_RESET_COUNT_ADD + 2
I2C_MEM_WDT_POWER_OFF_INTERVAL_SET_ADD = I2C_MEM_WDT_CLEAR_RESET_COUNT_ADD + 1
I2C_MEM_WDT_POWER_OFF_INTERVAL_GET_ADD = I2C_MEM_WDT_POWER_OFF_INTERVAL_SET_ADD + 4
WDT_MAX_POWER_OFF_INTERVAL = 4147200
RELOAD_KEY = 202


def wdtGetPeriod(stack):
    hwAdd = checkStack(stack)
    bus = smbus.SMBus(BUS_NO)
    try:
        val = bus.read_word_data(hwAdd, I2C_MEM_WDT_INTERVAL_GET_ADD)
    except Exception as e:
        bus.close()
        raise ValueError(e)
    bus.close()
    return val


def wdtSetPeriod(stack, val):
    ret = 1
    hwAdd = checkStack(stack)
    if val < 10 or val > 65000:
        raise ValueError('Invalid interval value [10..65000]')
    bus = smbus.SMBus(BUS_NO)
    try:
        bus.write_word_data(hwAdd, I2C_MEM_WDT_INTERVAL_SET_ADD, val)
    except Exception as e:
        bus.close()
        raise ValueError(e)
    bus.close()
    return ret


def wdtReload(stack):
    ret = 1
    hwAdd = checkStack(stack)
    bus = smbus.SMBus(BUS_NO)
    try:
        bus.write_byte_data(hwAdd, I2C_MEM_WDT_RESET_ADD, RELOAD_KEY)
    except Exception as e:
        bus.close()
        raise ValueError(e)
    bus.close()
    return ret


def wdtSetDefaultPeriod(stack, val):
    ret = 1
    hwAdd = checkStack(stack)
    if val < 10 or val > 64999:
        raise ValueError('Invalid interval value [10..64999]')
    bus = smbus.SMBus(BUS_NO)
    try:
        bus.write_word_data(hwAdd, I2C_MEM_WDT_INIT_INTERVAL_SET_ADD, val)
    except:
        bus.close()
        raise ValueError(e)
    bus.close()
    return ret


def wdtGetDefaultPeriod(stack):
    hwAdd = checkStack(stack)
    bus = smbus.SMBus(BUS_NO)
    try:
        val = bus.read_word_data(hwAdd, I2C_MEM_WDT_INIT_INTERVAL_GET_ADD)
    except Exception as e:
        bus.close()
        raise ValueError(e)
    bus.close()
    return val


def wdtSetOffInterval(stack, val):
    ret = 1
    hwAdd = checkStack(stack)
    if 10 > val or val > WDT_MAX_POWER_OFF_INTERVAL:
        raise ValueError('Invalid interval value [2..4147200]')
    bus = smbus.SMBus(BUS_NO)
    buff = [0, 0, 0, 0]
    buff[0] = 0xff & val
    buff[1] = 0xff & (val >> 8)
    buff[2] = 0xff & (val >> 16)
    buff[3] = 0xff & (val >> 24)
    try:
        bus.write_i2c_block_data(hwAdd, I2C_MEM_WDT_POWER_OFF_INTERVAL_SET_ADD, buff)
    except Exception as e:
        bus.close()
        raise ValueError(e)
    bus.close()
    return ret


def wdtGetOffInterval(stack):
    hwAdd = checkStack(stack)
    bus = smbus.SMBus(BUS_NO)
    try:
        buff = bus.read_i2c_block_data(hwAdd, I2C_MEM_WDT_POWER_OFF_INTERVAL_GET_ADD, 4)
        val = buff[0] + (buff[1] << 8) + (buff[2] << 16) + (buff[3] << 24)
    except Exception as e:
        bus.close()
        raise ValueError(e)
    bus.close()
    return val


def wdtGetResetCount(stack):
    hwAdd = checkStack(stack)
    bus = smbus.SMBus(BUS_NO)
    try:
        val = bus.read_word_data(hwAdd, I2C_MEM_WDT_RESET_COUNT_ADD)
    except Exception as e:
        bus.close()
        raise ValueError(e)
    bus.close()
    return val


I2C_RTC_YEAR_ADD = 70
I2C_RTC_MONTH_ADD = 71
I2C_RTC_DAY_ADD = 72
I2C_RTC_HOUR_ADD = 73
I2C_RTC_MINUTE_ADD = 74
I2C_RTC_SECOND_ADD = 75
I2C_RTC_SET_YEAR_ADD = 76
I2C_RTC_SET_MONTH_ADD = 77
I2C_RTC_SET_DAY_ADD = 78
I2C_RTC_SET_HOUR_ADD = 79
I2C_RTC_SET_MINUTE_ADD = 80
I2C_RTC_SET_SECOND_ADD = 81
I2C_RTC_CMD_ADD = 82


def rtcGet(stack):
    hwAdd = checkStack(stack)
    bus = smbus.SMBus(BUS_NO)
    try:
        buff = bus.read_i2c_block_data(hwAdd, I2C_RTC_YEAR_ADD, 6)
    except Exception as e:
        bus.close()
        raise ValueError(e)
    bus.close()
    t = (2000 + buff[0], buff[1], buff[2], buff[3], buff[4], buff[5])
    return t


def rtcSet(stack, y, mo, d, h, m, s):
    if y > 2000:
        y -= 2000
    if y < 0 or y > 255:
        raise ValueError("Invalid year!")
    if mo > 12 or mo < 1:
        raise ValueError("Invalid month!")
    if d < 1 or d > 31:
        raise ValueError("Invalid day!")
    if h < 0 or h > 23:
        raise ValueError("Invalid hour!")
    if m < 0 or m > 59:
        raise ValueError("Invalid minute!")
    if s < 0 or s > 59:
        raise ValueError("Invalid seconds!")
    hwAdd = checkStack(stack)
    bus = smbus.SMBus(BUS_NO)
    buff = [int(y), int(mo), int(d), int(h), int(m), int(s), 0xaa]
    # buff[0] = int(y)
    # buff[1] = int(mo)
    # buff[2] = int(d)
    # buff[3] = int(h)
    # buff[4] = int(m)
    # buff[5] = int(s)
    # buff[6] = 0xaa
    try:
        bus.write_i2c_block_data(hwAdd, I2C_RTC_SET_YEAR_ADD, buff)
    except Exception as e:
        bus.close()
        raise ValueError(e)
    bus.close()


I2C_MEM_1WB_DEV = 147
I2C_MEM_1WB_TEMP_ALL = 148
I2C_MEM_1WB_START_SEARCH = 173
I2C_MEM_1WB_T1 = 174
I2C_MEM_1WB_ROM_CODE_IDX = 150
I2C_MEM_1WB_ROM_CODE = 151
OWB_TEMP_SIZE_B = 2
OWM_ROM_CODE_SIZE_B = 8

def owbScan(stack):
    hwAdd = checkStack(stack)
    bus = smbus.SMBus(BUS_NO)
    buf = 0xaa
    try:
        bus.write_byte_data(hwAdd, I2C_MEM_1WB_START_SEARCH, buf)
    except Exception as e:
        bus.close()
        raise Exception("Fail to write with exception " + str(e))
    bus.close()


def owbGetSensorNo(stack):
    hwAdd = checkStack(stack)
    bus = smbus.SMBus(BUS_NO)
    try:
        no = bus.read_byte_data(hwAdd, I2C_MEM_1WB_DEV)
    except Exception as e:
        bus.close()
        raise Exception("Fail to read with exception " + str(e))
    bus.close()
    return no


def owbGetTemp(stack, sensor):
    checkOwbSns(sensor)
    hwAdd = checkStack(stack)
    bus = smbus.SMBus(BUS_NO)
    try:
        temp = bus.read_word_data(hwAdd, I2C_MEM_1WB_T1 + OWB_TEMP_SIZE_B * (sensor - 1))
    except Exception as e:
        bus.close()
        raise Exception("Fail to read with exception " + str(e))
    bus.close()
    return temp / 100.0


def owbGetRomCode(stack, sensor):
    checkOwbSns(sensor)
    hwAdd = checkStack(stack)
    bus = smbus.SMBus(BUS_NO)
    try:
        bus.write_byte_data(hwAdd, I2C_MEM_1WB_ROM_CODE_IDX, sensor - 1)  # Select the sensor
        buff = bus.read_i2c_block_data(hwAdd, I2C_MEM_1WB_ROM_CODE, 8)
    except Exception as e:
        bus.close()
        raise Exception("Fail to read with exception " + str(e))
    bus.close()
    return buff
