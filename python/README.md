# lib16inpind

This is the python library to control [16- INPUTS card for Raspberry Pi](https://sequentmicrosystems.com/collections/all-io-cards/products/16-universal-inputs-3-240vac-dc-br-8-layer-stackable-card-br-for-raspberry-pi)

## Install

```bash
~$ sudo apt-get update
~$ sudo apt-get install build-essential python-pip python-dev python-smbus git
~$ git clone https://github.com/SequentMicrosystems/16inpind-rpi.git
~$ cd 16inpind-rpi/python/16inpind/
~/16inpind-rpi/python/16inpind$ sudo python setup.py install
```

for Python 3.x usge replace last line with:
```bash
~/16inpind-rpi/python/16inpind$ sudo python3 setup.py install
```

## Update

```bash
~$ cd 16inpind-rpi/
~/16inpind-rpi$ git pull
~$ cd 16inpind-rpi/python/16inpind/
~/16inpind-rpi/python/16inpind$ sudo python setup.py install
```

## Usage 

Now you can import the lib16inpind library and use its functions. To test, read the channel 1:

```bash
~$ python
Python 2.7.9 (default, Sep 17 2016, 20:26:04)
[GCC 4.9.2] on linux2
Type "help", "copyright", "credits" or "license" for more information.
>>> import lib16inpind
>>> lib16inpind.readCh(0, 1)
0
>>>
```
## [Documentation](https://github.com/SequentMicrosystems/16inpind-rpi/blob/master/python/16inpind/README.md). 

This library works with both Python2 and Python3
