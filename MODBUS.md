[![16inpind-rpi](pictures/sequent.jpg)](https://sequentmicrosystems.com)

# Modbus

The [Sixteen LV Digital Inputs 8-Layer Stackable HAT for Raspberry Pi](https://sequentmicrosystems.com/collections/all-io-cards/products/16-universal-inputs-card-for-raspberry-pi)  can be accessed through Modbus RTU protocol over RS-485 port.
You can set up the RS-485 port with **16inpind** command.

Example:
```bash
~$ 16inpind 0 cfg485wr 1 9600 1 0 1
```
Set Modbus RTU, Baudrate: 9600bps, 1 Stop Bit,  parity: None, slave address offset: 1
```bash
~$ 16inpind -h cfg485wr
```
display the full set of options

## Slave Address
The slave address is added with the "stack level" jumpers. For example, the jumpers configuration for stack level 1  (one jumper in position ID0) slave address offset to 1 corresponds to slave address 2.

## Modbus object types
Some of the Modbus RTU object types with standard addresses are implemented: Discrete Inputs and input Registers.

### Discrete Inputs

Access level Read Only, Size 16 bits

| Device function | Register Address | Modbus Address |
| --- | --- | --- |
| DI_IN1 | 10001 | 0x00 | 
| DI_IN2 | 10002 | 0x01 | 
| DI_IN3 | 10003 | 0x02 | 
| DI_IN4 | 10004 | 0x03 | 
| DI_IN5 | 10005 | 0x04 |
| DI_IN6 | 10006 | 0x05 |
| DI_IN7 | 10007 | 0x06 |
| DI_IN8 | 10008 | 0x07 |
| DI_IN9 | 10009 | 0x08 |
| DI_IN10 | 10010 | 0x09 |
| DI_IN11 | 10011 | 0x0a |
| DI_IN12 | 10012 | 0x0b |
| DI_IN13 | 10013 | 0x0c |
| DI_IN14 | 10014 | 0x0d |
| DI_IN15 | 10015 | 0x0e |
| DI_IN16 | 10016 | 0x0f |

## Function codes implemented

* Read Coils (0x01)
* Read Discrete Inputs (0x02)
* Read Holding Registers (0x03)
* Read Input Registers (0x04)
* Write Single Coil (0x05)
* Write Single Register (0x06)
* Write Multiple Coils (0x0f)
* Write Multiple registers (0x10)
