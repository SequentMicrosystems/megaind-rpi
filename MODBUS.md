[![megaind-rpi](res/sequent.jpg)](https://sequentmicrosystems.com)

# Modbus

The [Industrial](https://sequentmicrosystems.com/product/raspberry-pi-industrial-automation/) Automation I/O Expansion cards for Raspberry Pi can be accessed thru Modbus RTU protocol over RS-485 port.
You can set-up the RS-485 port with **megaind** command.

Example:
```bash
~$ megaind 0 rs485wr 1 9600 1 0 1
```
Set Modbus RTU , Baudrate: 9600bps, 1 Stop Bit,  parity: None, slave address offset: 1
```bash
~$ megaind -h rs485wr
```
display the full set of options

## Slave Address
The slave address is add with the "stack level" jumpers. For example the jumpers configuration for stack level 1  (one jumper in position ID0) slave address offset to 1 corespond to slave address 2.

## Modbus object types
All modbus RTU object type with standard addresses are implemented : Coils, Discrete Inputs, Input registers, Holding registers.

### Coils

Acces level Read/Write, Size 1 bit

| Device function | Register Address | Modbus Address |
| --- | --- | --- |
| OPEN_DRAIN_1 | 0001 | 0x00 |
| OPEN_DRAIN_2 | 0002 | 0x01 |
| OPEN_DRAIN_3 | 0003 | 0x02 |
| OPEN_DRAIN_4 | 0004 | 0x03 |
| LED_1 | 0005 | 0x04 |
| LED_2 | 0006 | 0x05 |
| LED_3 | 0007 | 0x06 |
| LED_4 | 0008 | 0x07 |
| OPTO_RISING_COUNT_1 | 0009 | 0x08 |
| OPTO_RISING_COUNT_2 | 0010 | 0x09 |
| OPTO_RISING_COUNT_3 | 0011 | 0x0a |
| OPTO_RISING_COUNT_4 | 0012 | 0x0b |
| OPTO_FALLING_COUNT_1 | 0013 | 0x0c |
| OPTO_FALLING_COUNT_2 | 0014 | 0x0d |
| OPTO_FALLING_COUNT_3 | 0015 | 0x0e |
| OPTO_FALLING_COUNT_4 | 0016 | 0x0f |

OPTO_RISING_COUNT coil enable/disable counting on rising edge of coresponding opto channel 
OPTO_FALLING_COUNT coil enable/disable counting on falling edge of coresponding opto channel 

### Discrete Inputs

Access level Read Only, Size 1 bit

| Device function | Register Address | Modbus Address |
| --- | --- | --- |
| OPTO_1 | 10001 | 0x00 |
| OPTO_2 | 10002 | 0x01 |
| OPTO_3 | 10003 | 0x02 |
| OPTO_4 | 10004 | 0x03 |
| OWB_PRESENT | 10005 | 0x04 |


### Input registers

Access level Read Only, Size 16 bits

| Device function | Register Address | Description | Measurement Unit |
| --- | --- | --- | --- |
| 0-10V_IN_1 | 30001 | 0-10V Input 1 | mV |
| 0-10V_IN_2 | 30002 | 0-10V Input 2 | mV |
| 0-10V_IN_3 | 30003 |0-10V Input 3 | mV |
| 0-10V_IN_4 | 30004 | 0-10V Input 4 | mV |
| +/-10V_IN_1 | 30005 | +/-10V Input 1 | mV |
| +/-10V_IN_2 | 30006 | +/-10V Input 2 | mV |
| +/-10V_IN_3 | 30007 | +/-10V Input 3 | mV |
| +/-10V_IN_4 | 30008 | +/-10V Input 4 | mV |
| 4-20mA_IN_1 | 30009 | 4-20mA Input 1 | uA |
| 4-20mA_IN_2 | 30010 | 4-20mA Input 2 | uA |
| 4-20mA_IN_3 | 30011 | 4-20mA Input 3 | uA |
| 4-20mA_IN_4 | 30012 | 4-20mA Input 4 | uA |
| OPTO_COUNT_1 | 30013 | Opto Count 1 |  |
| OPTO_COUNT_2 | 30014 | Opto Count 2 |  |
| OPTO_COUNT_3 | 30015 | Opto Count 3 |  |
| OPTO_COUNT_4 | 30016 | Opto Count 4 |  |
| OWB_TEMP1| 30017 |  Temperature 1 | 0.01C|
| OWB_TEMP2| 30018 |  Temperature 2 | 0.01C|
| OWB_TEMP3| 30019 |  Temperature 3 | 0.01C|
| OWB_TEMP4| 30020 |  Temperature 4 | 0.01C|
| OWB_TEMP5| 30021 |  Temperature 5 | 0.01C|
| OWB_TEMP6| 30022 |  Temperature 6 | 0.01C|
| OWB_TEMP7| 30023 |  Temperature 7 | 0.01C|
| OWB_TEMP8| 30024 |  Temperature 8 | 0.01C|
| OWB_TEMP9| 30025 |  Temperature 9 | 0.01C|
| OWB_TEMP10| 30026 |  Temperature 10 | 0.01C|
| OWB_TEMP11| 30027 |  Temperature 11 | 0.01C|
| OWB_TEMP12| 30028 |  Temperature 12 | 0.01C|
| OWB_TEMP13| 30028 |  Temperature 13 | 0.01C|
| OWB_TEMP14| 30030 |  Temperature 14 | 0.01C|
| OWB_TEMP15| 30031 |  Temperature 15 | 0.01C|
| OWB_TEMP16| 30032 |  Temperature 16 | 0.01C|
| OWB_ID_A_TEMP1| 30033 |  ID A Temperature 1 | 64 bit ID |
| OWB_ID_B_TEMP1| 30034 |  ID B Temperature 1 | |
| OWB_ID_C_TEMP1| 30035 |  ID C Temperature 1 | |
| OWB_ID_D_TEMP1| 30036 |  ID D Temperature 1 | |
| OWB_ID_A_TEMP2| 30037 |  ID A Temperature 2 | 64 bit ID |
| OWB_ID_B_TEMP2| 30038 |  ID B Temperature 2 |  |
| OWB_ID_C_TEMP2| 30039 |  ID C Temperature 2 |  |
| OWB_ID_D_TEMP2| 30040 |  ID D Temperature 2 |  |
| OWB_ID_A_TEMP3| 30041 |  ID A Temperature 3 | 64 bit ID|
| OWB_ID_B_TEMP3| 30042 |  ID B Temperature 3 |  |
| OWB_ID_C_TEMP3| 30043 |  ID C Temperature 3 |  |
| OWB_ID_D_TEMP3| 30044 |  ID D Temperature 3 |  |
| OWB_ID_A_TEMP4| 30045 |  ID A Temperature 4 | 64 bit ID|
| OWB_ID_B_TEMP4| 30046 |  ID B Temperature 4 |  |
| OWB_ID_C_TEMP4| 30047 |  ID C Temperature 4 |  |
| OWB_ID_D_TEMP4| 30048 |  ID D Temperature 4 |  |
| OWB_ID_A_TEMP5| 30049 |  ID A Temperature 5 | 64 bit ID|
| OWB_ID_B_TEMP5| 30050 |  ID B Temperature 5 |  |
| OWB_ID_C_TEMP5| 30051 |  ID C Temperature 5 |  |
| OWB_ID_D_TEMP5| 30052 |  ID D Temperature 5 |  |
| OWB_ID_A_TEMP6| 30053 |  ID A Temperature 6 | 64 bit ID|
| OWB_ID_B_TEMP6| 30054 |  ID B Temperature 6 |  |
| OWB_ID_C_TEMP6| 30055 |  ID C Temperature 6 |  |
| OWB_ID_D_TEMP6| 30056 |  ID D Temperature 6 |  |
| OWB_ID_A_TEMP7| 30057 |  ID A Temperature 7 | 64 bit ID|
| OWB_ID_B_TEMP7| 30058 |  ID B Temperature 7 |  |
| OWB_ID_C_TEMP7| 30059 |  ID C Temperature 7 |  |
| OWB_ID_D_TEMP7| 30060 |  ID D Temperature 7 |  |
| OWB_ID_A_TEMP8| 30061 |  ID A Temperature 8 | 64 bit ID|
| OWB_ID_B_TEMP8| 30062 |  ID B Temperature 8 |  |
| OWB_ID_C_TEMP8| 30063 |  ID C Temperature 8 |  |
| OWB_ID_D_TEMP8| 30064 |  ID D Temperature 8 |  |
| OWB_ID_A_TEMP9| 30065 |  ID A Temperature 9 | 64 bit ID|
| OWB_ID_B_TEMP9| 30066 |  ID B Temperature 9 |  |
| OWB_ID_C_TEMP9| 30067 |  ID C Temperature 9 |  |
| OWB_ID_D_TEMP9| 30068 |  ID D Temperature 9 |  |
| OWB_ID_A_TEMP10| 30069 |  ID A Temperature 10 | 64 bit ID|
| OWB_ID_B_TEMP10| 30070 |  ID B Temperature 10 |  |
| OWB_ID_C_TEMP10| 30071 |  ID C Temperature 10 |  |
| OWB_ID_D_TEMP10| 30072 |  ID D Temperature 10 |  |
| OWB_ID_A_TEMP11| 30073 |  ID A Temperature 11 | 64 bit ID|
| OWB_ID_B_TEMP11| 30074 |  ID B Temperature 11 |  |
| OWB_ID_C_TEMP11| 30075 |  ID C Temperature 11 |  |
| OWB_ID_D_TEMP11| 30076 |  ID D Temperature 11 |  |
| OWB_ID_A_TEMP12| 30077 |  ID A Temperature 12 | 64 bit ID|
| OWB_ID_B_TEMP12| 30078 |  ID B Temperature 12 |  |
| OWB_ID_C_TEMP12| 30079 |  ID C Temperature 12 |  |
| OWB_ID_D_TEMP12| 30080 |  ID D Temperature 12 |  |
| OWB_ID_A_TEMP13| 30081 |  ID A Temperature 13 | 64 bit ID|
| OWB_ID_B_TEMP13| 30082 |  ID B Temperature 13 |  |
| OWB_ID_C_TEMP13| 30083 |  ID C Temperature 13 |  |
| OWB_ID_D_TEMP13| 30084 |  ID D Temperature 13 |  |
| OWB_ID_A_TEMP14| 30085 |  ID A Temperature 14 | 64 bit ID|
| OWB_ID_B_TEMP14| 30086 |  ID B Temperature 14 |  |
| OWB_ID_C_TEMP14| 30087 |  ID C Temperature 14 |  |
| OWB_ID_D_TEMP14| 30088 |  ID D Temperature 14 |  |
| OWB_ID_A_TEMP15| 30089 |  ID A Temperature 15 | 64 bit ID|
| OWB_ID_B_TEMP15| 30090 |  ID B Temperature 15 |  |
| OWB_ID_C_TEMP15| 30091 |  ID C Temperature 15 |  |
| OWB_ID_D_TEMP15| 30092 |  ID D Temperature 15 |  |
| OWB_ID_A_TEMP16| 30093 |  ID A Temperature 16 | 64 bit ID|
| OWB_ID_B_TEMP16| 30094 |  ID B Temperature 16 |  |
| OWB_ID_C_TEMP16| 30095 |  ID C Temperature 16 |  |
| OWB_ID_D_TEMP16| 30096 |  ID D Temperature 16 |  |


### Holding registers

Access level Read/Write, Size 16 bits

| Device function | Register Address | Modbus Address | Measurement Unit | Range |
| --- | --- | --- | --- | --- |
| 0-10V_OUT_1 | 40001 | 0x00 | mV | 0..10000 |
| 0-10V_OUT_2 | 40002 | 0x01 | mV | 0..10000 |
| 0-10V_OUT_3 | 40003 | 0x02 | mV | 0..10000 |
| 0-10V_OUT_4 | 40004 | 0x03 | mV | 0..10000 |
| 4-20mA_OUT_1 | 40005 | 0x04 | uA | 4000..20000 |
| 4-20mA_OUT_2 | 40006 | 0x05 | uA | 4000..20000 |
| 4-20mA_OUT_3 | 40007 | 0x06 | uA | 4000..20000 |
| 4-20mA_OUT_4 | 40008 | 0x07 | uA | 4000..20000 |
| OPEN_DRAIN_PWM_1 | 40009 | 0x08 |  | 0..10000 |
| OPEN_DRAIN_PWM_2 | 40010 | 0x09 |  | 0..10000 |
| OPEN_DRAIN_PWM_3 | 40011 | 0x0a |  | 0..10000 |
| OPEN_DRAIN_PWM_4 | 40012 | 0x0b |  | 0..10000 |


## Function codes implemented

* Read Coils (0x01)
* Read Discrete Inputs (0x02)
* Read Holding Registers (0x03)
* Read Input Registers (0x04)
* Write Single Coil (0x05)
* Write Single Register (0x06)
* Write Multiple Coils (0x0f)
* Write Multiple registers (0x10)
