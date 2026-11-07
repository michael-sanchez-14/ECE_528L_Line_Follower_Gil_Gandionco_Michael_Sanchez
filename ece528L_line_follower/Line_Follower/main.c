/**
 * @file main.c
 *
 * @brief Main source code for the Line_Follower program.
 *
 * This file contains the main entry point for the Line_Follower program.
 * The main controller demonstrates a Line Follower robot without using an advanced algorithm.
 *
 * It interfaces the following peripherals using GPIO to demonstrate line following:
 *  - 8-Channel QTRX Sensor Array module
 *
 * Timers are used in this lab:
 *  - SysTick:  Used to generate periodic interrupts at a specified rate (1 kHz)
 *  - Timer A0: Used to generate PWM signals that will be used to drive the DC motors
 *  - Timer A1: Used to generate periodic interrupts at a specified rate (1 kHz)
 *
 * @note For more information regarding the 8-Channel QTRX Sensor Array module,
 * refer to the product page: https://www.pololu.com/product/3672
 *
 * @author
 *
 */

#include "msp.h"
#include "inc/Clock.h"
#include "inc/CortexM.h"
#include "inc/GPIO.h"
#include "inc/EUSCI_A0_UART.h"
#include "inc/Motor.h"
#include "inc/Timer_A1_Interrupt.h"
#include "inc/SysTick_Interrupt.h"
#include "inc/Reflectance_Sensor.h"

// Initialize constant PWM duty cycle values for the motors
#define PWM_NOMINAL 2500

// Initialize a global variable for Timer A1 to keep track of elapsed time in milliseconds
static uint32_t Timer_A1_ms_elapsed = 0;

typedef enum
{
    CENTER              = 0b00011000,
    SLIGHT_LEFT_CENTER  = 0b00011100,
    SLIGHT_RIGHT_CENTER = 0b00111000,
    FAR_LEFT            = 0b00000001,
    FAR_RIGHT           = 0b10000000,
    LEFT_TURN           = 0b11111000,
    RIGHT_TURN          = 0b00011111,
    T_INTERSECTION      = 0b11111111,
    DEAD_END            = 0b00000000
} Line_Position;

typedef enum
{
    MOVE_FORWARD,
    MOVE_TO_LEFT,
    MOVE_TO_RIGHT,
    TURN_LEFT,
    TURN_RIGHT,
    HANDLE_INTERSECTION,
    NO_LINE,
    STOP
}Robot_State;

#define MIN_TURN_MS 100
#define MAX_TURN_MS 1200
#define MIN_U_TURN_MS 600
#define MAX_U_TURN_MS 4500

static Robot_State current_state = MOVE_FORWARD;
static uint32_t turn_start_ms = 0;

void Update_Robot_FSM(uint8_t Reflectance_Sensor_Data, int32_t Position)
{

    uint32_t elapsed = Timer_A1_ms_elapsed - turn_start_ms;

    switch (current_state)
    {
        case MOVE_FORWARD:
        case MOVE_TO_LEFT:
        case MOVE_TO_RIGHT:

            if (Reflectance_Sensor_Data == LEFT_TURN)
            {
                current_state = TURN_LEFT;
                turn_start_ms = Timer_A1_ms_elapsed;
            }
            else if (Reflectance_Sensor_Data == RIGHT_TURN)
            {
                current_state = TURN_RIGHT;
                turn_start_ms = Timer_A1_ms_elapsed;
            }
            else if (Reflectance_Sensor_Data == T_INTERSECTION)
            {
                current_state = HANDLE_INTERSECTION;
                turn_start_ms = Timer_A1_ms_elapsed;
            }
            else if (Reflectance_Sensor_Data == DEAD_END)
            {
                current_state = NO_LINE;
            }
            else if (Position <= -47 && Position > -332)
            {
                current_state = MOVE_TO_LEFT;
            }
            else if (Position >= 47 && Position < 332)
            {
                current_state = MOVE_TO_RIGHT;
            }
            else
            {
                current_state = MOVE_FORWARD;
            }
            break;

        case TURN_LEFT:
        case TURN_RIGHT:

            if ((elapsed >= MIN_TURN_MS) && ((Reflectance_Sensor_Data & CENTER) != 0) && (Position > -47 && Position < 47))
            {
                current_state = MOVE_FORWARD;
            }
            else if (elapsed >= MAX_TURN_MS)
            {
                current_state = STOP;
            }
            break;

        case HANDLE_INTERSECTION:
            if ((elapsed >= MIN_U_TURN_MS) && (Reflectance_Sensor_Data == CENTER) && (Position > -47 && Position < 47))
            {
                current_state = MOVE_FORWARD;
            }
            else if (elapsed >= MAX_U_TURN_MS)
            {
                current_state = STOP;
            }
            break;

        case NO_LINE:
            if (Reflectance_Sensor_Data != DEAD_END)
            {
                current_state = MOVE_FORWARD;
            }
            break;

        default:
            current_state = STOP;
            break;
    }

    // Execute motor commands
    switch (current_state)
    {
        case MOVE_FORWARD:
            Motor_Forward(PWM_NOMINAL, PWM_NOMINAL);
            break;

        case MOVE_TO_LEFT:
            Motor_Forward(PWM_NOMINAL - 1000, PWM_NOMINAL);
            break;

        case MOVE_TO_RIGHT:
            Motor_Forward(PWM_NOMINAL, PWM_NOMINAL - 1000);
            break;

        case TURN_LEFT:
            Motor_Left(PWM_NOMINAL, PWM_NOMINAL);
            break;

        case TURN_RIGHT:
            Motor_Right(PWM_NOMINAL, PWM_NOMINAL);
            break;

        case HANDLE_INTERSECTION:
            Motor_Right(PWM_NOMINAL, PWM_NOMINAL);
            break;

        case NO_LINE:
        default:
            Motor_Stop();
            break;
    }
}

void Detect_Line_Position(uint8_t Reflectance_Sensor_Data)
{
    switch(Reflectance_Sensor_Data)
    {
        case CENTER:
        {
            LED1_Output(RED_LED_OFF);
            LED2_Output(RGB_LED_GREEN);
        }
        break;

        case SLIGHT_LEFT_CENTER:
        {
            LED1_Output(RED_LED_OFF);
            LED2_Output(RGB_LED_YELLOW);
        }
        break;

        case SLIGHT_RIGHT_CENTER:
        {
            LED1_Output(RED_LED_OFF);
            LED2_Output(RGB_LED_PINK);

        }
        break;

        case FAR_LEFT:
        {
            LED1_Output(RED_LED_OFF);
            LED2_Output(RGB_LED_WHITE);
        }
        break;

        case FAR_RIGHT:
        {
            LED1_Output(RED_LED_OFF);
            LED2_Output(RGB_LED_SKY_BLUE);
        }
        break;

        case LEFT_TURN:
        {
            LED1_Output(RED_LED_ON);
            LED2_Output(RGB_LED_WHITE);
        }
        break;

        case RIGHT_TURN:
        {
            LED1_Output(RED_LED_ON);
            LED2_Output(RGB_LED_SKY_BLUE);
        }
        break;

        case T_INTERSECTION:
        {
            LED1_Output(RED_LED_OFF);
            LED2_Output(RGB_LED_BLUE);
        }
        break;

        case DEAD_END:
        {
            LED1_Output(RED_LED_OFF);
            LED2_Output(RGB_LED_RED);
        }
        break;

        default:
        {
            LED1_Output(RED_LED_OFF);
            LED2_Output(RGB_LED_OFF);
        }
    }
}

void Timer_A1_Periodic_Task(void)
{
    //Increment Timer_A1_ms_elapsed b 1 every time the Timer A1 periodic interrupt occurs
    Timer_A1_ms_elapsed++;

    //Start the process of reading the reflectance sensor array every 10 ms
    if ((Timer_A1_ms_elapsed % 10) == 0)
    {
        Reflectance_Sensor_Start();
    }

    //Finish reading the reflectance sensor array after 1 ms
    if((Timer_A1_ms_elapsed % 10) == 1)
    {
        uint8_t Reflectance_Sensor_Data = Reflectance_Sensor_End();
        int32_t Position = Reflectance_Sensor_Position(Reflectance_Sensor_Data);
        Detect_Line_Position(Reflectance_Sensor_Data);
        Update_Robot_FSM(Reflectance_Sensor_Data, Position);
    }
}

int main(void)
{
    //Ensure that interrupts are disabled during initialization
    DisableInterrupts();

    //Initialize the 48MHz clock
    Clock_Init48MHz();

    //Initialize the built in red LED and the RGB LED on the MSP432 microcontroller
    LED1_Init();
    LED2_Init();

    //Initialize the EUSCI_A0_UART module
    EUSCI_A0_UART_Init_Printf();

    //Initialize the 8 Channel QTRX Reflectance sensor array module
    Reflectance_Sensor_Init();

    //Initialize the SysTick timer to generate periodic interrupts every 1ms
    SysTick_Interrupt_Init(SYSTICK_INT_NUM_CLK_CYCLES, SYSTICK_INT_PRIORITY);

    //Initialize Timer A1 periodic interrupts every 1ms
    Timer_A1_Interrupt_Init(&Timer_A1_Periodic_Task, TIMER_A1_INT_CCR0_VALUE);
    Motor_Init();

    //Enable the interrupts used by the modules
    EnableInterrupts();

    while(1)
    {

    }
}
