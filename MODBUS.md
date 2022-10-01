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


### Input registers

Access level Read Only, Size 16 bits

| Device function | Register Address | Modbus Address | Measurement Unit |
| --- | --- | --- | --- |
| 0-10V_IN_1 | 30001 | 0x00 | mV |
| 0-10V_IN_2 | 30002 | 0x01 | mV |
| 0-10V_IN_3 | 30003 | 0x02 | mV |
| 0-10V_IN_4 | 30004 | 0x03 | mV |
| +/-10V_IN_1 | 30005 | 0x04 | mV |
| +/-10V_IN_2 | 30006 | 0x05 | mV |
| +/-10V_IN_3 | 30007 | 0x06 | mV |
| +/-10V_IN_4 | 30008 | 0x07 | mV |
| 4-20mA_IN_1 | 30009 | 0x08 | uA |
| 4-20mA_IN_2 | 30010 | 0x09 | uA |
| 4-20mA_IN_3 | 30011 | 0x0a | uA |
| 4-20mA_IN_4 | 30012 | 0x0b | uA |
| OPTO_COUNT_1 | 30013 | 0x0c |  |
| OPTO_COUNT_2 | 30014 | 0x0d |  |
| OPTO_COUNT_3 | 30015 | 0x0e |  |
| OPTO_COUNT_4 | 30016 | 0x0f |  |


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
