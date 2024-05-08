# Prototype-Medication-Alarm-Clock
A prototype medication alarm was built using the STM32 microcontroller. 

DISCLAIMER: This is not intended for medical use in any form; it is meant for educational purposes only.

## Features: 
- Easy-to-use text-based UI
- Set the time
- Create multiple alarms
- When the alarm goes off, a buzzer plays, and the LCD screen blinks
- The buzzer has Low/high volume settings
- LCD blinking can be toggled on and off

## Requirments
- STM32F401RE
- I2C 1602 LCD Display Module 16X2
- I2C 1602 LCD Display Module 16X2 Library can be found [here](https://github.com/eziya/STM32_HAL_I2C_HD44780/tree/master/Src). 
Note: "liquidcrystal_i2c.c" goes in the "Src" folder, and "liquidcrystal_i2c.h" goes in the "Inc" folder
- Breadboard
- 6x6x4.3mm TACT Switch Push Button for Arduino (2 of these)
- basic Arduino resistors for the buttons
- Dupont Jump Wires
- STM32CubeIDE
- Universal Passive Buzzers (2 of these)

## Circuit Layout:
The left button is to scroll down. The Right right button is to scroll up. The blue built-in button on the STM32 is to select.

![circuit drawing](https://github.com/Apollo99-Games/Medication-Alarm-Clock/assets/163193765/f479b356-270e-4c47-af63-77055ab3fe09)

## Demonstration:

https://github.com/Apollo99-Games/Medication-Alarm-Clock/assets/163193765/c6dd59c9-375f-42b1-be5d-aee8d93f541b


https://github.com/Apollo99-Games/Medication-Alarm-Clock/assets/163193765/646e8575-f676-4235-a231-ae75a57e286d





