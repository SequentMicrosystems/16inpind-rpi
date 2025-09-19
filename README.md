[![16inpind-rpi](pictures/sequent.jpg)](https://www.sequentmicrosystems.com )

# 16inpind-rpi

This is the command line and Python library to control the [Sequent Microsystems 16-Inputs Card for Raspberry PI](https://sequentmicrosystems.com/products/sixteen-lv-digital-inputs-card-for-raspberry-pi). This 8-layer stackable card contains 16 optically isolated digital inputs with reverse polarity protection.

To use this card, enable Raspberry Pi I2C communication
```bash
~$ sudo raspi-config
```

## Usage

```bash
cd
git clone https://github.com/SequentMicrosystems/16inpind-rpi.git
cd 16inpind-rpi/
sudo make install
```

Now you can access all the functions of the 16-Inputs board through the command "16inpind". Use -h option for help:
```bash
16inpind -h
```

If you clone the repository, any updates can be made with the following commands:

```bash
cd
cd 16inpind-rpi/  
git pull
sudo make install
```  

## [Python](https://github.com/SequentMicrosystems/16inpind-rpi/tree/main/python)

## [Firmware Update](https://github.com/SequentMicrosystems/16inpind-rpi/blob/main/update/README.md)
