import smbus2
from typing import Union, Optional

import lib16inpind.lib16inpind_data as data

I2C_MEM = data.I2C_MEM
DEVICE_ADDRESS = data.DEVICE_ADDRESS
WDT_RESET_SIGNATURE = data.WDT_RESET_SIGNATURE

# Pin masks for 16-bit operations  
pinMask = [0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400, 0x0200, 0x0100, 
           0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01]


def readCh(stack, channel):
    """Read the value of a specific input channel on the 16-input industrial Raspberry Pi expansion card.

    This function reads the state of a single input channel from the specified stack level
    of the expansion card. The inputs are optically isolated and pulled-up to 24V.
    A return value of 1 indicates the input is active (connected to ground),
    while 0 indicates inactive (floating or connected to 24V).

    Args:
        stack (int): Stack level of the card (0-7). Each card in the stack must have
            a unique address set via the jumpers.
        channel (int): Input channel number to read (1-16).

    Returns:
        int: The state of the input channel:
            - 1: Input is active (connected to ground)
            - 0: Input is inactive (floating or connected to 24V)

    Raises:
        ValueError: If stack level is not 0-7 or channel is not 1-16
        Exception: If communication with the I2C bus fails

    Example:
        >>> # Read input channel 5 on card at stack level 0
        >>> import lib16inpind
        >>> value = lib16inpind.readCh(0, 5)
        >>> print(f"Channel 5 state: {'Active' if value else 'Inactive'}")
    """
    if stack < 0 or stack > 7:
        raise ValueError('Invalid stack level')
    stack = 0x07 ^ stack
    if channel < 1 or channel > 16:
        raise ValueError('Invalid channel')
    bus = smbus2.SMBus(1)
    hw_add = DEVICE_ADDRESS + stack
    try:
        val = bus.read_word_data(hw_add, I2C_MEM.INPORT_REG)
    except Exception as e:
        bus.close()
        raise Exception(e)
    bus.close()
    if val & pinMask[channel - 1] == 0:
        return 1
    return 0


def readAll(stack):
    """
    Read all input pins on the device.

    Args:
        stack (int): Stack level (0-7) of the card to read from

    Returns:
        int: 16-bit value representing the state of all inputs (1 = active, 0 = inactive)

    Raises:
        ValueError: If stack level is invalid
        Exception: If communication with device fails

    Example:
        inputs = lib16inpind.readAll(0) # Read all inputs from card at stack level 0
    """

    if stack < 0 or stack > 7:
        raise ValueError('Invalid stack level')
    stack = 0x07 ^ stack
    bus = smbus2.SMBus(1)
    hw_add = DEVICE_ADDRESS + stack
    try:
        val = bus.read_word_data(hw_add, I2C_MEM.INPORT_REG)
    except Exception as e:
        bus.close()
        raise Exception(e)
    bus.close()
    ret = 0
    for i in range(16):
        if val & pinMask[i] == 0:
            ret += 1 << i
    return ret

def getLed(stack: int, channel: int) -> int:
    """Get the state of an LED channel
    
    Args:
        stack: Board stack level (0-7)
        channel: LED channel number (1-16)
    
    Returns:
        int: LED state (0=OFF, 1=ON)
        
    Raises:
        ValueError: If invalid stack or channel
    """
    if stack < 0 or stack > 7:
        raise ValueError('Invalid stack level')
    if channel < 1 or channel > 16:
        raise ValueError('Invalid channel')
        
    stack = 0x07 ^ stack
    bus = smbus2.SMBus(1)
    hw_add = DEVICE_ADDRESS + stack
    
    try:
        val = bus.read_word_data(hw_add, I2C_MEM.LED)
    except Exception as e:
        bus.close()
        raise Exception(e)
    bus.close()

    return 1 if val & pinMask[channel-1] else 0

def setLed(stack: int, channel: int, state: int) -> None:
    """Set the state of an LED channel
    
    Args:
        stack: Board stack level (0-7)
        channel: LED channel number (1-16) 
        state: LED state (0=OFF, 1=ON)
    
    Raises:
        ValueError: If invalid stack, channel or state
    """
    if stack < 0 or stack > 7:
        raise ValueError('Invalid stack level')
    if channel < 1 or channel > 16:
        raise ValueError('Invalid channel')
    if state not in [0,1]:
        raise ValueError('Invalid state')
        
    stack = 0x07 ^ stack
    bus = smbus2.SMBus(1)
    hw_add = DEVICE_ADDRESS + stack
    
    try:
        val = bus.read_word_data(hw_add, I2C_MEM.LED)
        if state:
            val |= pinMask[channel-1]
        else:
            val &= ~pinMask[channel-1]
        bus.write_word_data(hw_add, I2C_MEM.LED, val)
    except Exception as e:
        bus.close()
        raise Exception(e)
    bus.close()

def getLedMode(stack: int, channel: int) -> int:
    """Get the mode of an LED channel
    
    Args:
        stack: Board stack level (0-7)
        channel: LED channel number (1-16)
    
    Returns:
        int: LED mode (0=OFF, 1=ON, 2=BLINK)
        
    Raises:
        ValueError: If invalid stack or channel
    """
    if stack < 0 or stack > 7:
        raise ValueError('Invalid stack level')
    if channel < 1 or channel > 16:
        raise ValueError('Invalid channel')
        
    stack = 0x07 ^ stack
    bus = smbus2.SMBus(1)
    hw_add = DEVICE_ADDRESS + stack
    
    try:
        val = bus.read_word_data(hw_add, I2C_MEM.LED_MODE)
        mode = (val >> ((channel-1)*2)) & 0x03
    except Exception as e:
        bus.close()
        raise Exception(e)
    bus.close()
    
    return mode

def setLedMode(stack: int, channel: int, mode: int) -> None:
    """Set the mode of an LED channel
    
    Args:
        stack: Board stack level (0-7)
        channel: LED channel number (1-16)
        mode: LED mode (0=OFF, 1=ON, 2=BLINK)
    
    Raises:
        ValueError: If invalid stack, channel or mode
    """
    if stack < 0 or stack > 7:
        raise ValueError('Invalid stack level')
    if channel < 1 or channel > 16:
        raise ValueError('Invalid channel')
    if mode not in [0,1,2]:
        raise ValueError('Invalid mode')
        
    stack = 0x07 ^ stack
    bus = smbus2.SMBus(1)
    hw_add = DEVICE_ADDRESS + stack
    
    try:
        val = bus.read_word_data(hw_add, I2C_MEM.LED_MODE)
        val &= ~(0x03 << ((channel-1)*2))
        val |= (mode << ((channel-1)*2))
        bus.write_word_data(hw_add, I2C_MEM.LED_MODE, val)
    except Exception as e:
        bus.close()
        raise Exception(e)
    bus.close()

def getPowerLedMode(stack: int) -> int:
    """Get the power LED mode
    
    Args:
        stack: Board stack level (0-7)
    
    Returns:
        int: Power LED mode (0=OFF, 1=ON, 2=BLINK)
        
    Raises:
        ValueError: If invalid stack
    """
    if stack < 0 or stack > 7:
        raise ValueError('Invalid stack level')
        
    stack = 0x07 ^ stack
    bus = smbus2.SMBus(1)
    hw_add = DEVICE_ADDRESS + stack
    
    try:
        val = bus.read_byte_data(hw_add, I2C_MEM.PWR_LED_MODE)
    except Exception as e:
        bus.close()
        raise Exception(e)
    bus.close()
    
    return val & 0x03

def setPowerLedMode(stack: int, mode: int) -> None:
    """Set the power LED mode
    
    Args:
        stack: Board stack level (0-7)
        mode: LED mode (0=OFF, 1=ON, 2=BLINK)
    
    Raises:
        ValueError: If invalid stack or mode
    """
    if stack < 0 or stack > 7:
        raise ValueError('Invalid stack level')
    if mode not in [0,1,2]:
        raise ValueError('Invalid mode')
        
    stack = 0x07 ^ stack
    bus = smbus2.SMBus(1)
    hw_add = DEVICE_ADDRESS + stack
    
    try:
        bus.write_byte_data(hw_add, I2C_MEM.PWR_LED_MODE, mode)
    except Exception as e:
        bus.close()
        raise Exception(e)
    bus.close()

def wdtReload(stack: int) -> None:
    """Reload watchdog timer
    
    Args:
        stack: Board stack level (0-7)
    
    Raises:
        ValueError: If invalid stack
    """
    if stack < 0 or stack > 7:
        raise ValueError('Invalid stack level')
        
    stack = 0x07 ^ stack
    bus = smbus2.SMBus(1)
    hw_add = DEVICE_ADDRESS + stack
    
    try:
        bus.write_byte_data(hw_add, I2C_MEM.WDT_RESET, WDT_RESET_SIGNATURE)
    except Exception as e:
        bus.close()
        raise Exception(e)
    bus.close()

def getWdtPeriod(stack: int) -> int:
    """Get watchdog period in seconds
    
    Args:
        stack: Board stack level (0-7)
    
    Returns:
        int: Watchdog period in seconds
        
    Raises:
        ValueError: If invalid stack
    """
    if stack < 0 or stack > 7:
        raise ValueError('Invalid stack level')
        
    stack = 0x07 ^ stack
    bus = smbus2.SMBus(1)
    hw_add = DEVICE_ADDRESS + stack
    
    try:
        val = bus.read_word_data(hw_add, I2C_MEM.WDT_INTERVAL_GET)
    except Exception as e:
        bus.close()
        raise Exception(e)
    bus.close()
    
    return val

def setWdtPeriod(stack: int, period: int) -> None:
    """Set watchdog period in seconds
    
    Args:
        stack: Board stack level (0-7)
        period: Watchdog period in seconds (0-65535)
    
    Raises:
        ValueError: If invalid stack or period
    """
    if stack < 0 or stack > 7:
        raise ValueError('Invalid stack level')
    if period < 0 or period > 65535:
        raise ValueError('Invalid period')
        
    stack = 0x07 ^ stack
    bus = smbus2.SMBus(1)
    hw_add = DEVICE_ADDRESS + stack
    
    try:
        bus.write_word_data(hw_add, I2C_MEM.WDT_INTERVAL_SET, period)
    except Exception as e:
        bus.close()
        raise Exception(e)
    bus.close()

def getWdtInitPeriod(stack: int) -> int:
    """Get watchdog initial period in seconds
    
    Args:
        stack: Board stack level (0-7)
    
    Returns:
        int: Watchdog initial period in seconds
        
    Raises:
        ValueError: If invalid stack
    """
    if stack < 0 or stack > 7:
        raise ValueError('Invalid stack level')
        
    stack = 0x07 ^ stack
    bus = smbus2.SMBus(1)
    hw_add = DEVICE_ADDRESS + stack
    
    try:
        val = bus.read_word_data(hw_add, I2C_MEM.WDT_INIT_INTERVAL_GET)
    except Exception as e:
        bus.close()
        raise Exception(e)
    bus.close()
    
    return val

def setWdtInitPeriod(stack: int, period: int) -> None:
    """Set watchdog initial period in seconds
    
    Args:
        stack: Board stack level (0-7)  
        period: Initial period in seconds (0-65535)
    
    Raises:
        ValueError: If invalid stack or period
    """
    if stack < 0 or stack > 7:
        raise ValueError('Invalid stack level')
    if period < 0 or period > 65535:
        raise ValueError('Invalid period')
        
    stack = 0x07 ^ stack
    bus = smbus2.SMBus(1)
    hw_add = DEVICE_ADDRESS + stack
    
    try:
        bus.write_word_data(hw_add, I2C_MEM.WDT_INIT_INTERVAL_SET, period)
    except Exception as e:
        bus.close()
        raise Exception(e)
    bus.close()

def getWdtOffPeriod(stack: int) -> int:
    """Get watchdog off period in seconds
    
    Args:
        stack: Board stack level (0-7)
    
    Returns:
        int: Watchdog off period in seconds
        
    Raises:
        ValueError: If invalid stack
    """
    if stack < 0 or stack > 7:
        raise ValueError('Invalid stack level')
        
    stack = 0x07 ^ stack  
    bus = smbus2.SMBus(1)
    hw_add = DEVICE_ADDRESS + stack
    
    try:
        val = bus.read_word_data(hw_add, I2C_MEM.WDT_POWER_OFF_INTERVAL_GET)
    except Exception as e:
        bus.close()
        raise Exception(e)
    bus.close()
    
    return val

def setWdtOffPeriod(stack: int, period: int) -> None:
    """Set watchdog off period in seconds
    
    Args:
        stack: Board stack level (0-7)
        period: Off period in seconds (0-65535)
    
    Raises:
        ValueError: If invalid stack or period
    """
    if stack < 0 or stack > 7:
        raise ValueError('Invalid stack level')
    if period < 0 or period > 65535:
        raise ValueError('Invalid period')
        
    stack = 0x07 ^ stack
    bus = smbus2.SMBus(1)
    hw_add = DEVICE_ADDRESS + stack
    
    try:
        bus.write_word_data(hw_add, I2C_MEM.WDT_POWER_OFF_INTERVAL_SET, period)
    except Exception as e:
        bus.close()
        raise Exception(e)
    bus.close()

def getWdtResetCount(stack: int) -> int:
    """Get watchdog reset counter value
    
    Args:
        stack: Board stack level (0-7)
    
    Returns:
        int: Number of watchdog resets
        
    Raises:
        ValueError: If invalid stack
    """
    if stack < 0 or stack > 7:
        raise ValueError('Invalid stack level')
        
    stack = 0x07 ^ stack
    bus = smbus2.SMBus(1)
    hw_add = DEVICE_ADDRESS + stack
    
    try:
        val = bus.read_word_data(hw_add, I2C_MEM.WDT_RESET_COUNT)
    except Exception as e:
        bus.close()
        raise Exception(e)
    bus.close()
    
    return val

def getOpto(stack: int, channel: int) -> int:
    """Get the state of an opto-isolated input
    
    Args:
        stack: Board stack level (0-7)
        channel: Input channel number (1-16)
    
    Returns:
        int: Input state (0=OFF, 1=ON)
        
    Raises:
        ValueError: If invalid stack or channel
    """
    if stack < 0 or stack > 7:
        raise ValueError('Invalid stack level')
    if channel < 1 or channel > 16:
        raise ValueError('Invalid channel')
        
    stack = 0x07 ^ stack
    bus = smbus2.SMBus(1)
    hw_add = DEVICE_ADDRESS + stack
    
    try:
        val = bus.read_word_data(hw_add, I2C_MEM.OPTO_IN)
    except Exception as e:
        bus.close()
        raise Exception(e)
    bus.close()
    
    return 1 if val & pinMask[channel-1] else 0

def getOptoAll(stack: int) -> int:
    """Get all opto-isolated input states
    
    Args:
        stack: Board stack level (0-7)
    
    Returns:
        int: 16-bit value with input states
        
    Raises:
        ValueError: If invalid stack
    """
    if stack < 0 or stack > 7:
        raise ValueError('Invalid stack level')
        
    stack = 0x07 ^ stack
    bus = smbus2.SMBus(1)
    hw_add = DEVICE_ADDRESS + stack
    
    try:
        val = bus.read_word_data(hw_add, I2C_MEM.OPTO_IN)
    except Exception as e:
        bus.close()
        raise Exception(e)
    bus.close()
    
    return val

def getOptoEdge(stack: int, channel: int, edge: int) -> int:
    """Get edge detection state for an opto-isolated input
    
    Args:
        stack: Board stack level (0-7)
        channel: Input channel number (1-16)
        edge: Edge type (0=falling, 1=rising)
    
    Returns:
        int: Edge detection state (0=disabled, 1=enabled)
        
    Raises:
        ValueError: If invalid stack, channel or edge
    """
    if stack < 0 or stack > 7:
        raise ValueError('Invalid stack level')
    if channel < 1 or channel > 16:
        raise ValueError('Invalid channel')
    if edge not in [0,1]:
        raise ValueError('Invalid edge type')
        
    stack = 0x07 ^ stack
    bus = smbus2.SMBus(1)
    hw_add = DEVICE_ADDRESS + stack
    
    try:
        addr = I2C_MEM.OPTO_IT_FALLING if edge == 0 else I2C_MEM.OPTO_IT_RISING
        val = bus.read_word_data(hw_add, addr)
    except Exception as e:
        bus.close()
        raise Exception(e)
    bus.close()
    
    return 1 if val & pinMask[channel-1] else 0

def setOptoEdge(stack: int, channel: int, edge: int, state: int) -> None:
    """Set edge detection for an opto-isolated input
    
    Args:
        stack: Board stack level (0-7)
        channel: Input channel number (1-16)
        edge: Edge type (0=falling, 1=rising)
        state: Detection state (0=disabled, 1=enabled)
    
    Raises:
        ValueError: If invalid stack, channel, edge or state
    """
    if stack < 0 or stack > 7:
        raise ValueError('Invalid stack level')
    if channel < 1 or channel > 16:
        raise ValueError('Invalid channel')
    if edge not in [0,1]:
        raise ValueError('Invalid edge type')
    if state not in [0,1]:
        raise ValueError('Invalid state')
        
    stack = 0x07 ^ stack
    bus = smbus2.SMBus(1)
    hw_add = DEVICE_ADDRESS + stack
    
    try:
        addr = I2C_MEM.OPTO_IT_FALLING if edge == 0 else I2C_MEM.OPTO_IT_RISING
        val = bus.read_word_data(hw_add, addr)
        if state:
            val |= pinMask[channel-1]
        else:
            val &= ~pinMask[channel-1]
        bus.write_word_data(hw_add, addr, val)
    except Exception as e:
        bus.close()
        raise Exception(e)
    bus.close()

def getOptoCount(stack: int, channel: int) -> int:
    """Get the counter value for an opto-isolated input
    
    Args:
        stack: Board stack level (0-7)
        channel: Input channel number (1-16)
    
    Returns:
        int: Counter value (0-65535)
        
    Raises:
        ValueError: If invalid stack or channel
    """
    if stack < 0 or stack > 7:
        raise ValueError('Invalid stack level')
    if channel < 1 or channel > 16:
        raise ValueError('Invalid channel')
        
    stack = 0x07 ^ stack
    bus = smbus2.SMBus(1)
    hw_add = DEVICE_ADDRESS + stack
    
    try:
        val = bus.read_word_data(hw_add, I2C_MEM.OPTO_EDGE_COUNT_ADD + (channel-1)*2)
    except Exception as e:
        bus.close()
        raise Exception(e)
    bus.close()
    
    return val

def resetOptoCount(stack: int, channel: int) -> None:
    """Reset the counter for an opto-isolated input
    
    Args:
        stack: Board stack level (0-7)
        channel: Input channel number (1-16)
    
    Raises:
        ValueError: If invalid stack or channel
    """
    if stack < 0 or stack > 7:
        raise ValueError('Invalid stack level')
    if channel < 1 or channel > 16:
        raise ValueError('Invalid channel')
        
    stack = 0x07 ^ stack
    bus = smbus2.SMBus(1)
    hw_add = DEVICE_ADDRESS + stack
    
    try:
        bus.write_word_data(hw_add, I2C_MEM.OPTO_CNT_RST, 0)
    except Exception as e:
        bus.close()
        raise Exception(e)
    bus.close()

def getOptoEncCount(stack: int, channel: int) -> int:
    """Get the counter value for an opto-isolated encoder input
    
    Args:
        stack: Board stack level (0-7)
        channel: Input channel number (1-16)
    
    Returns:
        int: Counter value (0-65535)
        
    Raises:
        ValueError: If invalid stack or channel
    """
    if stack < 0 or stack > 7:
        raise ValueError('Invalid stack level')
    if channel < 1 or channel > 16:
        raise ValueError('Invalid channel')
        
    stack = 0x07 ^ stack
    bus = smbus2.SMBus(1)
    hw_add = DEVICE_ADDRESS + stack
    
    try:
        val = bus.read_word_data(hw_add, I2C_MEM.OPTO_ENC_COUNT_ADD + (channel-1)*2)
    except Exception as e:
        bus.close()
        raise Exception(e)
    bus.close()
    
    return val

def resetOptoEncCount(stack: int, channel: int) -> None:
    """Reset the counter for an opto-isolated encoder input
    
    Args:
        stack: Board stack level (0-7)
        channel: Input channel number (1-16)
    
    Raises:
        ValueError: If invalid stack or channel
    """
    if stack < 0 or stack > 7:
        raise ValueError('Invalid stack level')
    if channel < 1 or channel > 16:
        raise ValueError('Invalid channel')
        
    stack = 0x07 ^ stack
    bus = smbus2.SMBus(1)
    hw_add = DEVICE_ADDRESS + stack
    
    try:
        bus.write_word_data(hw_add, I2C_MEM.OPTO_ENC_CNT_RST, 0)
    except Exception as e:
        bus.close()
        raise Exception(e)
    bus.close()


def getOptoEncoder(stack: int, channel: int) -> int:
    """Get encoder mode for an opto-isolated input pair
    
    Args:
        stack: Board stack level (0-7)
        channel: Input channel number (1-8, must be odd)
    
    Returns:
        int: Encoder mode (0=disabled, 1=enabled)
        
    Raises:
        ValueError: If invalid stack or channel
    """
    if stack < 0 or stack > 7:
        raise ValueError('Invalid stack level')
    if channel < 1 or channel > 8 or channel % 2 == 0:
        raise ValueError('Invalid channel')
        
    stack = 0x07 ^ stack
    bus = smbus2.SMBus(1)
    hw_add = DEVICE_ADDRESS + stack
    
    try:
        val = bus.read_word_data(hw_add, I2C_MEM.OPTO_ENC_ENABLE)
    except Exception as e:
        bus.close()
        raise Exception(e)
    bus.close()
    
    return 1 if val & (1 << ((channel-1)//2)) else 0

def setOptoEncoder(stack: int, channel: int, state: int) -> None:
    """Set encoder mode for an opto-isolated input pair
    
    Args:
        stack: Board stack level (0-7)
        channel: Input channel number (1-8, must be odd)
        state: Encoder state (0=disabled, 1=enabled)
    
    Raises:
        ValueError: If invalid stack, channel or state
    """
    if stack < 0 or stack > 7:
        raise ValueError('Invalid stack level')
    if channel < 1 or channel > 8 or channel % 2 == 0:
        raise ValueError('Invalid channel')
    if state not in [0,1]:
        raise ValueError('Invalid state')
        
    stack = 0x07 ^ stack
    bus = smbus2.SMBus(1)
    hw_add = DEVICE_ADDRESS + stack
    
    try:
        val = bus.read_word_data(hw_add, I2C_MEM.OPTO_ENC_ENABLE)
        if state:
            val |= (1 << ((channel-1)//2))
        else:
            val &= ~(1 << ((channel-1)//2))
        bus.write_word_data(hw_add, I2C_MEM.OPTO_ENC_ENABLE, val)
    except Exception as e:
        bus.close()
        raise Exception(e)
    bus.close()

def getOptoFrequency(stack: int, channel: int) -> int:
    """Get frequency measurement for an opto-isolated input
    
    Args:
        stack: Board stack level (0-7)
        channel: Input channel number (1-16)
    
    Returns:
        int: Frequency in Hz
        
    Raises:
        ValueError: If invalid stack or channel
    """
    if stack < 0 or stack > 7:
        raise ValueError('Invalid stack level')
    if channel < 1 or channel > 16:
        raise ValueError('Invalid channel')
        
    stack = 0x07 ^ stack
    bus = smbus2.SMBus(1)
    hw_add = DEVICE_ADDRESS + stack
    
    try:
        val = bus.read_word_data(hw_add, I2C_MEM.IN_FREQENCY + (channel-1)*2)
    except Exception as e:
        bus.close()
        raise Exception(e)
    bus.close()
    
    return val

def getOptoPWM(stack: int, channel: int) -> float:
    """Get PWM duty cycle measurement for an opto-isolated input
    
    Args:
        stack: Board stack level (0-7)
        channel: Input channel number (1-16)
    
    Returns:
        float: PWM duty cycle (0-100%)
        
    Raises:
        ValueError: If invalid stack or channel
    """
    if stack < 0 or stack > 7:
        raise ValueError('Invalid stack level')
    if channel < 1 or channel > 16:
        raise ValueError('Invalid channel')
        
    stack = 0x07 ^ stack
    bus = smbus2.SMBus(1)
    hw_add = DEVICE_ADDRESS + stack
    
    try:
        val = bus.read_word_data(hw_add, I2C_MEM.PWM_IN_FILL + (channel-1)*2)
    except Exception as e:
        bus.close()
        raise Exception(e)
    bus.close()
    
    return val / 65535 * 100

def setOptoInterrupt(stack: int, channel: int, enabled: bool) -> None:
    """Enable/disable interrupts for an opto-isolated input
    
    Args:
        stack: Board stack level (0-7)
        channel: Input channel number (1-16)
        enabled: True to enable interrupts, False to disable
        
    Raises:
        ValueError: If invalid parameters
    """
    if stack < 0 or stack > 7:
        raise ValueError('Invalid stack level')
    if channel < 1 or channel > 16:
        raise ValueError('Invalid channel')
        
    stack = 0x07 ^ stack
    bus = smbus2.SMBus(1)
    hw_add = DEVICE_ADDRESS + stack
    
    try:
        # Read current interrupt mask
        val = bus.read_word_data(hw_add, I2C_MEM.EXTI_EN)
        
        # Modify bit for specified channel
        if enabled:
            val |= pinMask[channel-1] 
        else:
            val &= ~pinMask[channel-1]
            
        # Write back updated mask    
        bus.write_word_data(hw_add, I2C_MEM.EXTI_EN, val)
        
    except Exception as e:
        bus.close()
        raise Exception(e)
    bus.close()

def getOptoInterrupt(stack: int, channel: int) -> bool:
    """Get interrupt enable state for an opto-isolated input
    
    Args:
        stack: Board stack level (0-7)
        channel: Input channel number (1-16)
        
    Returns:
        bool: True if interrupts enabled, False if disabled
        
    Raises:
        ValueError: If invalid parameters
    """
    if stack < 0 or stack > 7:
        raise ValueError('Invalid stack level')
    if channel < 1 or channel > 16:
        raise ValueError('Invalid channel')
        
    stack = 0x07 ^ stack
    bus = smbus2.SMBus(1)
    hw_add = DEVICE_ADDRESS + stack
    
    try:
        # Read interrupt mask
        val = bus.read_word_data(hw_add, I2C_MEM.EXTI_EN)
    except Exception as e:
        bus.close()
        raise Exception(e)
    bus.close()
    
    # Return true if bit is set for channel
    return bool(val & pinMask[channel-1])

def setOptoInterruptMask(stack: int, mask: int) -> None:
    """Set interrupt enable mask for all opto-isolated inputs
    
    Args:
        stack: Board stack level (0-7)
        mask: 16-bit mask (1=enabled, 0=disabled)
        
    Raises:
        ValueError: If invalid parameters
    """
    if stack < 0 or stack > 7:
        raise ValueError('Invalid stack level')
    if mask < 0 or mask > 0xFFFF:
        raise ValueError('Invalid mask')
        
    stack = 0x07 ^ stack
    bus = smbus2.SMBus(1)
    hw_add = DEVICE_ADDRESS + stack
    
    try:
        bus.write_word_data(hw_add, I2C_MEM.EXTI_EN, mask)
    except Exception as e:
        bus.close()
        raise Exception(e)
    bus.close()

def getOptoInterruptMask(stack: int) -> int:
    """Get interrupt enable mask for all opto-isolated inputs
    
    Args:
        stack: Board stack level (0-7)
        
    Returns:
        int: 16-bit mask with interrupt enable states
        
    Raises:
        ValueError: If invalid stack
    """
    if stack < 0 or stack > 7:
        raise ValueError('Invalid stack level')
        
    stack = 0x07 ^ stack
    bus = smbus2.SMBus(1)
    hw_add = DEVICE_ADDRESS + stack
    
    try:
        val = bus.read_word_data(hw_add, I2C_MEM.EXTI_EN)
    except Exception as e:
        bus.close()
        raise Exception(e)
    bus.close()
    
    return val