[![megaind-rpi](res/sequent.jpg)](https://www.sequentmicrosystems.com)

# megaind-rpi

This is the command to control [Industrial Automation Stackable Card for Raspberry Pi](https://sequentmicrosystems.com/product/industrial-automation-stackable-card-for-raspberry-pi/).

![MEGA-IND](res/IND.jpg)

## Setup

Enable I2C communication first:
```bash
~$ sudo raspi-config
```

## Usage

```bash
~$ git clone https://github.com/SequentMicrosystems/megaind-rpi.git
~$ cd megaind-rpi/
~/megaind-rpi$ sudo make install
```

Now you can access all the functions of the [Industrial Automation Stackable Card for Raspberry Pi](https://sequentmicrosystems.com/product/industrial-automation-stackable-card-for-raspberry-pi/) through the command "megaind". Use -h option for help:
```bash
~$ megaind -h
```

If you clone the repository any update can be made with the following commands:

```bash
~$ cd megaind-rpi/  
~/megaind-rpi$ git pull
~/megaind-rpi$ sudo make install
```  
Checkout [Python Library](https://github.com/SequentMicrosystems/megaind-rpi/tree/master/python)

Checkout [firmware update instructions](https://github.com/SequentMicrosystems/megaind-rpi/tree/master/update).

The board can act as Modbus RTU slave device, checkout [modbus instructions](https://github.com/SequentMicrosystems/megaind-rpi/blob/master/MODBUS.md).
