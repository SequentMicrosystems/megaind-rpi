
# megaind

This is the python library to control the [Industrial Automation Stackable Card for Raspberry Pi](https://sequentmicrosystems.com/collections/all-io-cards/products/industrial-raspberry-pi). 

## Install

```bash
sudo pip install SMmegaind
```

## Update

```bash
sudo pip install --upgrade SMmegaind
```

## Usage 

Now you can import the megaind library and use its functions. To test, read 0 - 10V input channel 1 from the MEGA-IND board with stack level 0:

```bash
~$ python
Python 2.7.9 (default, Sep 17 2016, 20:26:04)
[GCC 4.9.2] on linux2
Type "help", "copyright", "credits" or "license" for more information.
>>> import megaind
>>> megaind.get0_10In(0, 1)
0.003
>>>
```
Checkout the [/tests](https://github.com/SequentMicrosystems/megaind-rpi/tree/master/python/tests) subfolder for more usage examples. 
All tests assume that one card with no address jumpers (stack level = 0) is present.


## Diagnose Functions

### getFwVer(stack)
Return firmware version

stack - stack level of the megaind  card (selectable from address jumpers [0..7])


### getRaspVolt(stack)
Read raspberry voltage

stack - stack level of the megaind  card (selectable from address jumpers [0..7])

return Raspberry PI voltage in volts


## Analog input/output Functions

###  getPowerVolt(stack)
Read power source voltage

stack - stack level of the megaind  card (selectable from address jumpers [0..7])

return voltage in volts


### getCpuTemp(stack)
Get the cpu temperature 

stack - stack level of the megaind  card (selectable from address jumpers [0..7])

return - temperature in deg Celsius


### get0_10In(stack, ch)
Return the selected input 0-10V channel value in volts. 
Make sure the selection jumper is not connectd in order for this measurement to be correct.

stack - stack level of the megaind  card (selectable from address jumpers [0..7])

ch - selected channel number [1..4]

return - value in volts 


### getpm10In(stack, ch)
Return the selected input +/-10V channel value in volts.
Make sure the selection jumper is connectd in order for this measurement to be correct.

stack - stack level of the megaind  card (selectable from address jumpers [0..7])

ch - selected channel number [1..4]

return - value in volts 


### get0_10Out(stack, ch)
Get the selected output 0-10V channel value in volts  

stack - stack level of the megaind  card (selectable from address jumpers [0..7])

ch - selected channel number [1..4]

return - value in volts [0..10]


### set0_10Out(stack, ch, value)
Set the selected output 0-10V channel value in volts

stack - stack level of the megaind  card (selectable from address jumpers [0..7])

ch - selected channel number [1..4]

value - voltage output value in V [0..10]

return - none


### get4_20In(stack, ch)
Return the selected input 4 - 20mA channel value. 

stack - stack level of the megaind  card (selectable from address jumpers [0..7])

ch - selected channel number [1..4]

return - value in milliamps 


### get4_20Out(stack, ch)
Get the selected output 4 - 20mA channel value.  

stack - stack level of the megaind  card (selectable from address jumpers [0..7])

ch - selected channel number [1..4]

return - value in milliamps [4..20]


### set4_20Out(stack, ch, value)
Set the selected output 4 - 20mA channel value.

stack - stack level of the megaind  card (selectable from address jumpers [0..7])

ch - selected channel number [1..4]

value - current output value in milliamps [4..20]

return - none


## Digital input/output Functions

### getOptoCh(stack, ch)
Get the state of the optically isolated digital input channel

stack - stack level of the megaind  card (selectable from address jumpers [0..7])

ch - selected channel number [1..4]

return - value of the inputs [0/1]


### getOpto(stack)
Get the state of the all optically isolated digital inputs

stack - stack level of the megaind  card (selectable from address jumpers [0..7])

return - value of the inputs [0..15]


### getOptoCount(stack, ch)
Return the counter value for corresponding optically isolated input.

stack - stack level of the megaind  card (selectable from address jumpers [0..7])

ch - selected channel number [1..4]

return - counter value [0..65535]


### rstOptoCount(stack, ch)
Reset the counter value for corresponding optically isolated input.

stack - stack level of the megaind  card (selectable from address jumpers [0..7])

ch - selected channel number [1..4]

return none


### getOptoRisingCountEnable(stack, ch)
Get the rising edge enable counting for corresponding optically isolated input.

stack - stack level of the megaind  card (selectable from address jumpers [0..7])

ch - selected channel number [1..4]

return 1 - rising edges will be counted; 0 - not


## setOptoRisingCountEnable(stack, ch, state)
Set the rising edge enable counting for corresponding optically isolated input.

stack - stack level of the megaind  card (selectable from address jumpers [0..7])

ch - selected channel number [1..4]

state - 1 = enable; 0 - disable

return none


### getOptoFallingCountEnable(stack, ch)
Get the falling edge enable counting for corresponding optically isolated input.

stack - stack level of the megaind  card (selectable from address jumpers [0..7])

ch - selected channel number [1..4]

return 1 - falling edges will be counted; 0 - not


### setOptoFallingCountEnable(stack, ch, state)
Set the falling edge enable counting for corresponding optically isolated input.

stack - stack level of the megaind  card (selectable from address jumpers [0..7])

ch - selected channel number [1..4]

state - 1 = enable; 0 - disable

return none


### setOdPWM(stack, ch, value)
Set the filling factor for Open-Drain outputs

stack - stack level of the megaind  card (selectable from address jumpers [0..7])

ch - selected channel number [1..4]

value - filling factor in precent [0..100]

return none


### getOdPWM(stack, ch)
Read the filling factor for Open-Drain outputs

stack - stack level of the megaind  card (selectable from address jumpers [0..7])

ch - selected channel number [1..4]

return - filling factor in precent [0..100]


### setLed(stack, channel, val)
Set one of the 4 LED's on the card.

stack - stack level of the megaind  card (selectable from address jumpers [0..7])

channel - selected LED number [1..4]

val - 0 = OFF, 1 = ON


### setLedAll(stack, val)
Set all 4 LED's on the card.

stack - stack level of the megaind  card (selectable from address jumpers [0..7])

val - bitmap of the 4 LED's [0..15]


### getLed(stack, channel)
Get the state of one of the 4 LED's om the card.

stack - stack level of the megaind  card (selectable from address jumpers [0..7])

channel - selected LED number [1..4]

return 0 if the LED is OFF, 1 if is ON


## Watcdog Timer Functions

### wdtGetPeriod(stack)
Return the current period of the watchdog timer in seconds

stack - stack level of the megaind  card (selectable from address jumpers [0..7])


### wdtSetPeriod(stack, val)
Set the period of the watchdog in seconds, val = 65000 disable the watchdog

stack - stack level of the megaind  card (selectable from address jumpers [0..7])

val - [10..65000]


### wdtReload(stack)
Reload the watchdog timer with the current period.
The next reload command must occur in no more the "period" time in order to prevent watchdog to re-power the Raspberry.
This command also enables the watchdog if is disabled (power-up disabled).

stack - stack level of the megaind  card (selectable from address jumpers [0..7])


### wdtSetDefaultPeriod(stack, val)
This function updates the period that will be loaded after Raspberry power is turned off and back on. You must set this period long enough to let Raspberry boot-up and your "watchdog maintaining" script to start.

stack - stack level of the megaind  card (selectable from address jumpers [0..7])

value - [10...64999] seconds


### wdtGetDefaultPeriod(stack)
Return the default period 

stack - stack level of the megaind  card (selectable from address jumpers [0..7])

value - [10...64999] seconds


### wdtSetOffInterval(stack, val)
Set the time interval in seconds for keeping Raspberry power off in case of watchdog timer expire. 

stack - stack level of the megaind  card (selectable from address jumpers [0..7])

val - [10...4147200] seconds 


### wdtGetOffInterval(stack)
Return the Off time interval in seconds

stack - stack level of the megaind  card (selectable from address jumpers [0..7])

return - [10...4147200] seconds 


### wdtGetResetCount(stack)
Return the numbers of Raspberry re-powers performed by the watchdog 

stack - stack level of the megaind  card (selectable from address jumpers [0..7])

return - [0..65535]

## RTC Functions

### rtcGet(stack)
Return the RTC date and time as a list

stack - stack level of the megaind  card (selectable from address jumpers [0..7])

return (year, month, day, hour, minute, seconds)

### rtcSet(stack, y, mo, d, h, m, s)
Set the RTC date and time

stack - stack level of the megaind  card (selectable from address jumpers [0..7])

y - year between 2000..2255 or between 0..255

mo - month 1..12

d - day

h - hour

m - minutes

s - seconds


## Owire Bus Functions

### owbScan(stack)
Start scanning for connected sensors

 stack - stack level of the megaind  card (selectable from address jumpers [0..7])

### owbGetSensorNo(stack)
Get the numbers of 18B20 sensors connected on the bus

 stack - stack level of the megaind  card (selectable from address jumpers [0..7])

 return number of connected sensors

### owbGetTemp(stack, sensor)
Read the temperature aquired by one sensor

 stack - stack level of the megaind  card (selectable from address jumpers [0..7])
 
 sensor - sensor number [1..16]

 return temperature in degree Celsius
 
 ### owbGetRomCode(stack, sensor)
 Read the unic ROM code of one sensor
 
  stack - stack level of the megaind  card (selectable from address jumpers [0..7])
 
  sensor - sensor number [1..16]

  return ROM code as 8 bytes array
