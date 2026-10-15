# ECE 528/L - Robotics and Embedded Systems with Lab
**CSU Northridge**

**Department of Electrical and Computer Engineering**

## Line Follower Lab
The Line Follower lab interfaces with the following:

* User LEDs of the TI MSP432 LaunchPad
* Pololu Gearmotor with Encoder - [Product Link](https://www.pololu.com/product/3675)
* 8-Channel QTRX Sensor Array - [Product Link](https://www.pololu.com/product/3672)
## Overview

## Components Used:

## Pre Lab
1. Write a void function named Chassis_Board_LEDs_Init that takes no arguments and initializes the follwing pins as output GPIO pins. Initailize the value of the pins to zero.
    - P8.0 (Front Left Yellow LED)
    - P8.5 (Front Right Yellow LED)
    - P8.6 (Back Left Red LED)
    - P8.7 (Back Right Red LED)
```
void Chassis_Board_LEDs_Init()
{
    P8->SEL0 &= ~0xE1;
    P8->SEL1 &= ~0xE1;
    P8->DIR |= 0xE1;
    P8->OUT &= ~0xE1;
}
```
2. Write a void function named Print_Tenth that takes no arguments. Your function
must iterate from 10 to 51 using a for-loop. Print a message for every 10th iteration
(10, 20, 30, 40, 50) that states “Index %d: Every 10th iteration.\n” and then print a
different message for the iterations that follow after the 10th iteration (11, 21, 31,
41, 51) that states “Index %d: Following the 10th iteration.\n.”
Use the following starter code for your main program. Save the main program as
print_tenth.c in your assignment repository.
Make sure that you include the code documentation in your print_tenth.c source file
and add your name in the author section to get full credit. If you’ve used references
(e.g. GeeksforGeeks, StackOverflow, etc.), cite them below

## Analysis and Results:

## Known Issues or Limitations
No known issues or limitations.

## Author Contribution

## References

* MSP432P4xx SimpleLink™ Microcontrollers
Technical Reference Manual
