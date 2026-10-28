# ECE 528/L - Robotics and Embedded Systems with Lab
**CSU Northridge**

**Department of Electrical and Computer Engineering**

## Line Follower Lab
The Line Follower lab interfaces with the following:

* User LEDs of the TI MSP432 LaunchPad
* Pololu Gearmotor with Encoder - [Product Link](https://www.pololu.com/product/3675)
* 8-Channel QTRX Sensor Array - [Product Link](https://www.pololu.com/product/3672)
## Overview
For Lab 3 we combine what we learned from the previous labs. By combining the techniques from the Reflectance sensor lab and the motor control lab, we implement code to have the robot navigate through a maze by itself.

## Components Used:
* MSP432 LaunchPad
* USB-A to Micro-USB cable
* TI-RSLK MAX Chassis
* 8-Channel QTRX Sensor Array

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
To start this lab we first had to include many different header files, once this was done we created a constant PWM duty cycle and a global variable for the Timer A1. Moving on we created a typedef enum that contained the variables that were assigned different values relating to the position the robot is in. Using these variables we used the Detect_Line_Position function created in Lab 2 and replaced the switch case values with the appropriate values. Next we created the Timer_A1_Periodic_Task function that would increment our global variable by 1 every time a periodic interrupt was generated, this was every 1 ms. Next we used if statements to start sampling every 10 milliseconds, then every 11 milliseconds we stop sampling and called hte Detect_Line_Position function.

Once this was done we called several different functions within the main function, two of these functions were SysTick_Interrupt_Init and Timer_A1_Interrupt_Init which create periodic interrupts every 1ms. Next we tested what we had so far by placing the robot in the appropriate positions, once we did this we encountered an error, that repeatedly reset our global variable. Once we got rid of this line of code we were ready to move onto the task.


## Known Issues or Limitations
No known issues or limitations.

## Author Contribution
* Michael Sanchez
    * Worked on Pre-Lab question 1
    * Implemented the necessary lines of code from the procedure
* Gil Gandionco 
    * Word on Pre-Lab question 2
    * Implemented the necessary code for the task
## References

* MSP432P4xx SimpleLink™ Microcontrollers
Technical Reference Manual
