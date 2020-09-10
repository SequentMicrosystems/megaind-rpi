# update

This is the [Sequent Microsystems](https://www.sequentmicrosystems.com) Industrial Automation Systems board firmware update tool.

## Usage

```bash
~$ git clone https://github.com/SequentMicrosystems/megaind-rpi.git
~$ cd megaind-rpi/update/
~/megaind-rpi/update$ ./update 0
```

If you clone the repository already, skip the first step. 
The command will download the newest firmware version from our server and write it  to the board.
The stack level of the board must be provided as a parameter. 
During firmware update we strongly recommend to disconnect all outputs from the board since they can change state unpredictably.
