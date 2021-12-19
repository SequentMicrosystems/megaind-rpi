# node-red-contrib-sm-ind

This is the node-red node to control Sequent Microsystems [Industrial Automation](https://sequentmicrosystems.com/products/raspberry-pi-industrial-automation) card.

## Manual Install

Clone or update the repository, follow the instrutions fron the [first page.](https://github.com/SequentMicrosystems/megaind-rpi)

In your node-red user directory, tipicaly ~/.node-red,

```bash
~$ cd ~/.node-red
```

Run the fallowing command:

```bash
~/.node-red$ npm install ~/megaind-rpi/node-red-contrib-sm-ind
```

In order to see the node in the palette and use-it you need to restart node-red. If you run node-red as a service:
 ```bash
 ~$ node-red-stop
 ~$ node-red-start
 ```

## Usage

After install and restart the node-red you will see on the node palete, under Sequent Microsystems category 12 new nodes:

### IND 0 10V in

This node reads one 0-10V input channel (Make sure the corresponding jumper is removed in order for this read to be correct).
The card stack level and channel number can be set in the node dialog box or dynamically through ```msg.stack``` and ```msg.channel```.
The read is triggered by the message input and output can be found in the output message payload as a number representing the voltage readings in volts.

### IND 0 10V out

This node controls one 0-10V output channel.
The card stack level and channel number can be set in the node dialog box or dynamically through ```msg.stack``` and ```msg.channel```.
The value in volts is set dynamically as a number between 0..10 thru ```msg.payload```.

### IND 4 20mA in

This node reads one 4-20mA input channel. 
The card stack level and channel number can be set in the node dialog box or dynamically through ```msg.stack``` and ```msg.channel```.
The read is triggered by the message input and output can be found in the output message payload as a number representing the current readings in milliamps.

### IND 4 20mA out

This node controls one 4-20mA output channel.
The card stack level and channel number can be set in the node dialog box or dynamically through ```msg.stack``` and ```msg.channel```.
The value in milliamps is set dynamically as a number between 4..20 thru ```msg.payload```.

### IND +/-10V in

This node reads one +/-10V input channel (Make sure the corresponding jumper is placed in order for this read to be correct).
The card stack level and channel number can be set in the node dialog box or dynamically through ```msg.stack``` and ```msg.channel```.
The read is triggered by the message input and output can be found in the output message payload as a number representing the voltage readings in volts.

### IND OPT cnt

This node reads the optically coupled input counter for one particular channel and sets the counting edges for that channel.
Card stack level and optically coupled input counter channel number can be set in the node dialog box or dynamically through ```msg.stack``` and ```msg.channel```.
Rising edge counting and/or falling edge counting can be enabled/diabled with corresponding check box in the node dialog.
Edge settings are sent to the card every time you deploy this node or the flow starts or you select a different channel for reading through ```msg.channel```.
The read is triggered by the message input and output can be found in the output message payload.
The counter can be reseted by sending to the node a message with the attribute ```reset``` equal to 1, ```msg.reset = 1```.

### IND OPT in

This node reads the optically coupled input state for one particular channel.
Card stack level and optically coupled input channel number can be set in the node dialog box or dynamically through ```msg.stack``` and ```msg.channel```.
The read is triggered by the message input and output can be found in the output message payload as boolean.

### IND OD out

This node controls one Open-Drain output channel.
Card stack level and channel number can be set in the node dialog box or dynamically through ```msg.stack``` and ```msg.channel```.
The value in percentage is set dynamically as a number between 0..100 thru ```msg.payload```.

### IND CPU Temp
This node reads the temperature from the CPU on the Sequent board.
Card stack level can be set in the node dialog box or dynamically through ```msg.stack```.
The integer value in units of degrees Celcius is set dynamically as a number via ```msg.payload```.  *NB - not sure if the byte read from the Sequent board is signed or unsigned, so the range might be -128C..+127C, or it might be 0C..255C.*

### IND PS Voltage
This node reads the power supply voltage (nominally 24V) from the CPU on the Sequent board.
Card stack level can be set in the node dialog box or dynamically through ```msg.stack```.
The floating point value in volts is set dynamically as a number via ```msg.payload```.

### IND RasPi Voltage
This node reads the voltage supplied to the Raspberry Pi (nominally 5V) from the CPU on the Sequent board.
Card stack level can be set in the node dialog box or dynamically through ```msg.stack```.
The floating point value in volts is set dynamically as a number via ```msg.payload```.

### IND LED Out
This node controls one LED channel.
The card stack level and LED channel number can be set in the node dialog box or dynamically through ```msg.stack``` and ```msg.channel```.
The value in volts is set dynamically as a number. 0 is off; anything else is on via ```msg.payload```.

## Important note

This node is using the I2C-bus package from @fivdi, you can visit his work on github [here](https://github.com/fivdi/i2c-bus). 
The inspiration for this node came from @nielsnl68 work with [node-red-contrib-i2c](https://github.com/nielsnl68/node-red-contrib-i2c).
Thank both for the great job.
