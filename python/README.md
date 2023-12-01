# lib16inpind

This is the python library to control [16- INPUTS card for Raspberry Pi](https://sequentmicrosystems.com/collections/all-io-cards/products/16-universal-inputs-3-240vac-dc-br-8-layer-stackable-card-br-for-raspberry-pi)

## Install

```bash
sudo pip install SM16inpind
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
# lib16inpind

## Functions

## readCh(stack, channel)

 - Card stack level [0..7]

 - channel Selected channel to be read [1..16]
 
 - Return one channel input state 0 / 1
 
## readAll(stack)

 - Card stack level [0..7]

 - Return all channels inputs states as 16 bits unsigned int [0..65535]
 
 
 
 
 
 