# Welcome to SM16inpind’s documentation!

# Install

```bash
sudo pip install SM16inpind
```

or

```bash
sudo pip3 install SM16inpind
```

# Update

```bash
sudo pip install SM16inpind -U
```

or

```bash
sudo pip3 install SM16inpind -U
```

# Initiate class

```console
$ python
Python 3.11.8 (main, Feb 12 2024, 14:50:05) [GCC 13.2.1 20230801] on linux
Type "help", "copyright", "credits" or "license" for more information.
>>> import lib16inpind
>>> lib16inpind.getLed(0, 1)
>>>
```

# Documentation

<a id="module-lib16inpind"></a>

### readCh(stack, channel)

Read the value of a specific input channel on the 16-input industrial Raspberry Pi expansion card.

This function reads the state of a single input channel from the specified stack level
of the expansion card. The inputs are optically isolated and pulled-up to 24V.
A return value of 1 indicates the input is active (connected to ground),
while 0 indicates inactive (floating or connected to 24V).

* **Parameters:**
  * **stack** (*int*) – Stack level of the card (0-7). Each card in the stack must have
    a unique address set via the jumpers.
  * **channel** (*int*) – Input channel number to read (1-16).
* **Returns:**
  The state of the input channel:
  : - 1: Input is active (connected to ground)
    - 0: Input is inactive (floating or connected to 24V)
* **Return type:**
  int
* **Raises:**
  * **ValueError** – If stack level is not 0-7 or channel is not 1-16
  * **Exception** – If communication with the I2C bus fails

### Example

```pycon
>>> # Read input channel 5 on card at stack level 0
>>> import lib16inpind
>>> value = lib16inpind.readCh(0, 5)
>>> print(f"Channel 5 state: {'Active' if value else 'Inactive'}")
```

### readAll(stack)

Read all input pins on the device.

* **Parameters:**
  **stack** (*int*) – Stack level (0-7) of the card to read from
* **Returns:**
  16-bit value representing the state of all inputs (1 = active, 0 = inactive)
* **Return type:**
  int
* **Raises:**
  * **ValueError** – If stack level is invalid
  * **Exception** – If communication with device fails

### Example

inputs = lib16inpind.readAll(0) # Read all inputs from card at stack level 0

### getLed(stack: int, channel: int)

Get the state of an LED channel

* **Parameters:**
  * **stack** – Board stack level (0-7)
  * **channel** – LED channel number (1-16)
* **Returns:**
  LED state (0=OFF, 1=ON)
* **Return type:**
  int
* **Raises:**
  **ValueError** – If invalid stack or channel

### setLed(stack: int, channel: int, state: int)

Set the state of an LED channel

* **Parameters:**
  * **stack** – Board stack level (0-7)
  * **channel** – LED channel number (1-16)
  * **state** – LED state (0=OFF, 1=ON)
* **Raises:**
  **ValueError** – If invalid stack, channel or state

### getLedMode(stack: int, channel: int)

Get the mode of an LED channel

* **Parameters:**
  * **stack** – Board stack level (0-7)
  * **channel** – LED channel number (1-16)
* **Returns:**
  LED mode (0=OFF, 1=ON, 2=BLINK)
* **Return type:**
  int
* **Raises:**
  **ValueError** – If invalid stack or channel

### setLedMode(stack: int, channel: int, mode: int)

Set the mode of an LED channel

* **Parameters:**
  * **stack** – Board stack level (0-7)
  * **channel** – LED channel number (1-16)
  * **mode** – LED mode (0=OFF, 1=ON, 2=BLINK)
* **Raises:**
  **ValueError** – If invalid stack, channel or mode

### getPowerLedMode(stack: int)

Get the power LED mode

* **Parameters:**
  **stack** – Board stack level (0-7)
* **Returns:**
  Power LED mode (0=OFF, 1=ON, 2=BLINK)
* **Return type:**
  int
* **Raises:**
  **ValueError** – If invalid stack

### setPowerLedMode(stack: int, mode: int)

Set the power LED mode

* **Parameters:**
  * **stack** – Board stack level (0-7)
  * **mode** – LED mode (0=OFF, 1=ON, 2=BLINK)
* **Raises:**
  **ValueError** – If invalid stack or mode

### wdtReload(stack: int)

Reload watchdog timer

* **Parameters:**
  **stack** – Board stack level (0-7)
* **Raises:**
  **ValueError** – If invalid stack

### getWdtPeriod(stack: int)

Get watchdog period in seconds

* **Parameters:**
  **stack** – Board stack level (0-7)
* **Returns:**
  Watchdog period in seconds
* **Return type:**
  int
* **Raises:**
  **ValueError** – If invalid stack

### setWdtPeriod(stack: int, period: int)

Set watchdog period in seconds

* **Parameters:**
  * **stack** – Board stack level (0-7)
  * **period** – Watchdog period in seconds (0-65535)
* **Raises:**
  **ValueError** – If invalid stack or period

### getWdtInitPeriod(stack: int)

Get watchdog initial period in seconds

* **Parameters:**
  **stack** – Board stack level (0-7)
* **Returns:**
  Watchdog initial period in seconds
* **Return type:**
  int
* **Raises:**
  **ValueError** – If invalid stack

### setWdtInitPeriod(stack: int, period: int)

Set watchdog initial period in seconds

* **Parameters:**
  * **stack** – Board stack level (0-7)
  * **period** – Initial period in seconds (0-65535)
* **Raises:**
  **ValueError** – If invalid stack or period

### getWdtOffPeriod(stack: int)

Get watchdog off period in seconds

* **Parameters:**
  **stack** – Board stack level (0-7)
* **Returns:**
  Watchdog off period in seconds
* **Return type:**
  int
* **Raises:**
  **ValueError** – If invalid stack

### setWdtOffPeriod(stack: int, period: int)

Set watchdog off period in seconds

* **Parameters:**
  * **stack** – Board stack level (0-7)
  * **period** – Off period in seconds (0-65535)
* **Raises:**
  **ValueError** – If invalid stack or period

### getWdtResetCount(stack: int)

Get watchdog reset counter value

* **Parameters:**
  **stack** – Board stack level (0-7)
* **Returns:**
  Number of watchdog resets
* **Return type:**
  int
* **Raises:**
  **ValueError** – If invalid stack

### getOpto(stack: int, channel: int)

Get the state of an opto-isolated input

* **Parameters:**
  * **stack** – Board stack level (0-7)
  * **channel** – Input channel number (1-16)
* **Returns:**
  Input state (0=OFF, 1=ON)
* **Return type:**
  int
* **Raises:**
  **ValueError** – If invalid stack or channel

### getOptoAll(stack: int)

Get all opto-isolated input states

* **Parameters:**
  **stack** – Board stack level (0-7)
* **Returns:**
  16-bit value with input states
* **Return type:**
  int
* **Raises:**
  **ValueError** – If invalid stack

### getOptoEdge(stack: int, channel: int, edge: int)

Get edge detection state for an opto-isolated input

* **Parameters:**
  * **stack** – Board stack level (0-7)
  * **channel** – Input channel number (1-16)
  * **edge** – Edge type (0=falling, 1=rising)
* **Returns:**
  Edge detection state (0=disabled, 1=enabled)
* **Return type:**
  int
* **Raises:**
  **ValueError** – If invalid stack, channel or edge

### setOptoEdge(stack: int, channel: int, edge: int, state: int)

Set edge detection for an opto-isolated input

* **Parameters:**
  * **stack** – Board stack level (0-7)
  * **channel** – Input channel number (1-16)
  * **edge** – Edge type (0=falling, 1=rising)
  * **state** – Detection state (0=disabled, 1=enabled)
* **Raises:**
  **ValueError** – If invalid stack, channel, edge or state

### getOptoCount(stack: int, channel: int)

Get the counter value for an opto-isolated input

* **Parameters:**
  * **stack** – Board stack level (0-7)
  * **channel** – Input channel number (1-16)
* **Returns:**
  Counter value (0-65535)
* **Return type:**
  int
* **Raises:**
  **ValueError** – If invalid stack or channel

### resetOptoCount(stack: int, channel: int)

Reset the counter for an opto-isolated input

* **Parameters:**
  * **stack** – Board stack level (0-7)
  * **channel** – Input channel number (1-16)
* **Raises:**
  **ValueError** – If invalid stack or channel

### getOptoEncoder(stack: int, channel: int)

Get encoder mode for an opto-isolated input pair

* **Parameters:**
  * **stack** – Board stack level (0-7)
  * **channel** – Input channel number (1-8, must be odd)
* **Returns:**
  Encoder mode (0=disabled, 1=enabled)
* **Return type:**
  int
* **Raises:**
  **ValueError** – If invalid stack or channel

### setOptoEncoder(stack: int, channel: int, state: int)

Set encoder mode for an opto-isolated input pair

* **Parameters:**
  * **stack** – Board stack level (0-7)
  * **channel** – Input channel number (1-8, must be odd)
  * **state** – Encoder state (0=disabled, 1=enabled)
* **Raises:**
  **ValueError** – If invalid stack, channel or state

### getOptoFrequency(stack: int, channel: int)

Get frequency measurement for an opto-isolated input

* **Parameters:**
  * **stack** – Board stack level (0-7)
  * **channel** – Input channel number (1-16)
* **Returns:**
  Frequency in Hz
* **Return type:**
  int
* **Raises:**
  **ValueError** – If invalid stack or channel

### getOptoPWM(stack: int, channel: int)

Get PWM duty cycle measurement for an opto-isolated input

* **Parameters:**
  * **stack** – Board stack level (0-7)
  * **channel** – Input channel number (1-16)
* **Returns:**
  PWM duty cycle (0-100%)
* **Return type:**
  float
* **Raises:**
  **ValueError** – If invalid stack or channel

### setOptoInterrupt(stack: int, channel: int, enabled: bool)

Enable/disable interrupts for an opto-isolated input

* **Parameters:**
  * **stack** – Board stack level (0-7)
  * **channel** – Input channel number (1-16)
  * **enabled** – True to enable interrupts, False to disable
* **Raises:**
  **ValueError** – If invalid parameters

### getOptoInterrupt(stack: int, channel: int)

Get interrupt enable state for an opto-isolated input

* **Parameters:**
  * **stack** – Board stack level (0-7)
  * **channel** – Input channel number (1-16)
* **Returns:**
  True if interrupts enabled, False if disabled
* **Return type:**
  bool
* **Raises:**
  **ValueError** – If invalid parameters

### setOptoInterruptMask(stack: int, mask: int)

Set interrupt enable mask for all opto-isolated inputs

* **Parameters:**
  * **stack** – Board stack level (0-7)
  * **mask** – 16-bit mask (1=enabled, 0=disabled)
* **Raises:**
  **ValueError** – If invalid parameters

### getOptoInterruptMask(stack: int)

Get interrupt enable mask for all opto-isolated inputs

* **Parameters:**
  **stack** – Board stack level (0-7)
* **Returns:**
  16-bit mask with interrupt enable states
* **Return type:**
  int
* **Raises:**
  **ValueError** – If invalid stack

<!-- vi:se ts=4 sw=4 et: -->
