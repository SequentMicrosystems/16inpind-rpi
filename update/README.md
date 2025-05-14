# update

This is the [Sequent Microsystems](https://www.sequentmicrosystems.com) [Sixteen LV Digital Inputs 8-Layer Stackable HAT for Raspberry Pi](https://sequentmicrosystems.com/collections/all-io-cards/products/16-universal-inputs-card-for-raspberry-pi)
firmware update tool. This tool is compatible with hardware version 3.0 and up.

## Usage

```bash 
~$ git clone https://github.com/SequentMicrosystems/16inpind-rpi.git
~$ cd 16inpind-rpi/update/ 
~/16inpind-rpi/update$ ./update 0 
``` 
Or if you are using a 64-bits OS

```bash
./update64 0
```

If you already cloned the repository, skip the first step.
The command will download the newest firmware version from our server and write it to the board. 
The stack level of the board must be provided as a parameter. Make sure no script or program accesses the I2C port during the update process.
