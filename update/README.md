# update

This is the [Sequent Microsystems](https://www.sequentmicrosystems.com) [Sixteen LV Digital Inputs 8-Layer Stackable HAT for Raspberry Pi](https://sequentmicrosystems.com/collections/all-io-cards/products/16-universal-inputs-card-for-raspberry-pi)
firmware update tool.

## Usage

```bash 
~$ git clone https://github.com/SequentMicrosystems/16inpind-rpi.git
~$ cd 16inpind-rpi/update/ 
~/16inpind-rpi/update$ ./update 0 
``` 

If you already cloned the repository, skip the first step.
The command will download the newest firmware version from our server and write it  to the board. 
The stack level of the board must be provided as a parameter.
