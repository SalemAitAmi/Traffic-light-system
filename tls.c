/*
    FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wwrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

/*
FreeRTOS is a market leading RTOS from Real Time Engineers Ltd. that supports
31 architectures and receives 77500 downloads a year. It is professionally
developed, strictly quality controlled, robust, supported, and free to use in
commercial products without any requirement to expose your proprietary source
code.

This simple FreeRTOS demo does not make use of any IO ports, so will execute on
any Cortex-M3 of Cortex-M4 hardware.  Look for TODO markers in the code for
locations that may require tailoring to, for example, include a manufacturer
specific header file.

This is a starter project, so only a subset of the RTOS features are
demonstrated.  Ample source comments are provided, along with web links to
relevant pages on the http://www.FreeRTOS.org site.

Here is a description of the project's functionality:

The main() Function:
main() creates the tasks and software timers described in this section, before
starting the scheduler.

The Queue Send Task:
The queue send task is implemented by the prvQueueSendTask() function.
The task uses the FreeRTOS vTaskDelayUntil() and xQueueSend() API functions to
periodically send the number 100 on a queue.  The period is set to 200ms.  See
the comments in the function for more details.
http://www.freertos.org/vtaskdelayuntil.html
http://www.freertos.org/a00117.html

The Queue Receive Task:
The queue receive task is implemented by the prvQueueReceiveTask() function.
The task uses the FreeRTOS xQueueReceive() API function to receive values from
a queue.  The values received are those sent by the queue send task.  The queue
receive task increments the ulCountOfItemsReceivedOnQueue variable each time it
receives the value 100.  Therefore, as values are sent to the queue every 200ms,
the value of ulCountOfItemsReceivedOnQueue will increase by 5 every second.
http://www.freertos.org/a00118.html

An example software timer:
A software timer is created with an auto reloading period of 1000ms.  The
timer's callback function increments the ulCountOfTimerCallbackExecutions
variable each time it is called.  Therefore the value of
ulCountOfTimerCallbackExecutions will count seconds.
http://www.freertos.org/RTOS-software-timer.html

The FreeRTOS RTOS tick hook (or callback) function:
The tick hook function executes in the context of the FreeRTOS tick interrupt.
The function 'gives' a semaphore every 500th time it executes.  The semaphore
is used to synchronise with the event semaphore task, which is described next.

The event semaphore task:
The event semaphore task uses the FreeRTOS xSemaphoreTake() API function to
wait for the semaphore that is given by the RTOS tick hook function.  The task
increments the ulCountOfReceivedSemaphores variable each time the semaphore is
received.  As the semaphore is given every 500ms (assuming a tick frequency of
1KHz), the value of ulCountOfReceivedSemaphores will increase by 2 each second.

The idle hook (or callback) function:
The idle hook function queries the amount of free FreeRTOS heap space available.
See vApplicationIdleHook().

The malloc failed and stack overflow hook (or callback) functions:
These two hook functions are provided as examples, but do not contain any
functionality.
*/

/* Standard includes. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "stm32f4_discovery.h"
/* Kernel includes. */
#include "stm32f4xx.h"
#include "../FreeRTOS_Source/include/FreeRTOS.h"
#include "../FreeRTOS_Source/include/queue.h"
#include "../FreeRTOS_Source/include/semphr.h"
#include "../FreeRTOS_Source/include/task.h"
#include "../FreeRTOS_Source/include/timers.h"



/*-----------------------------------------------------------*/
#define mainQUEUE_LENGTH 100

static void prvSetupHardware( void );
static void trafficFlowAdjustmentTask( void *pvParameters );
static void trafficGeneratorTask( void *pvParameters );
static void trafficLightStateTask( void *pvParameters );
static void systemDisplayTask( void *pvParameters );

/*
 * The queue send and receive tasks as described in the comments at the top of
 * this file.
 */

#define red		0
#define amber	1
#define green	2


void GPIOC_Init(void);
void setTrafficFlow(int*);


xQueueHandle carQueue; 			// Contains locations of cars
xQueueHandle trafficLightQueue; // Contains color of traffic light
xQueueHandle adcQueue; 			// Contains return value from ADC


/*-----------------------------------------------------------*/

int main(void) {
	// Configure the system
	prvSetupHardware();
	printf("GPIO Initialized!\n");


	// Initialize Queues
	carQueue = xQueueCreate(1, sizeof(uint16_t));
	trafficLightQueue = xQueueCreate(1, sizeof(uint16_t));
	adcQueue = xQueueCreate(1, sizeof(uint16_t));

	// Add to the registry, for the benefit of kernel aware debugging
	vQueueAddToRegistry(carQueue, "Car Array Queue");
	vQueueAddToRegistry(trafficLightQueue, "Traffic Light Queue");
	vQueueAddToRegistry(adcQueue, "ADC Value Queue");

	// Create tasks 
	xTaskCreate(trafficFlowAdjustmentTask, "Traffic Flow Adjustment", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
	xTaskCreate(trafficGeneratorTask, "Traffic Generator", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
	xTaskCreate(trafficLightStateTask, "Traffic Light State", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
	xTaskCreate(systemDisplayTask, "System Display", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

	// Start tasks
	vTaskStartScheduler();

	printf("Program End\n");

	return 0;
}

void trafficFlowAdjustmentTask( void *pvParameters ) {
	int potVal;  // Placeholder for potentiometer value (loaded onto queue for other tasks)


	while(1) {
		ADC_SoftwareStartConv(ADC1);  					// Signal the ADC to start convertions from the potentiometer
		while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));  // Wait for the end of convertion flag 
		potVal = ADC_GetConversionValue(ADC1); 			// Fetch potentiometer value from ADC Output Data Register
		potVal /= 666;  								// ADC values range from 0 to ~4000 - We bring it to the range of 0 to 5 to represent traffic intensity 
		xQueueOverwrite(adcQueue, &potVal);  			// Load scaled potentiometer value onto queue (Overwrite is used because all queues are of size 1)
		vTaskDelay(500);  								// Block task for 500ms
	}
}

void trafficGeneratorTask( void *pvParameters ) {
	int trafficIntensity = 0;  // Value derived from scaled potentiometer value
	int secsSinceLastCar = 0;  // An integer timer for keeping track of the time between cars 
	int isCar = 0;			   // 1 if a car should enter the queue, 0 otherwise


	while(1) {
		vTaskDelay(1000);  											// Cars move at a rate of 1 LED/second 
		xQueuePeek(adcQueue, &trafficIntensity, pdMS_TO_TICKS(0));  // Peek the ADC queue to determine if a new car should be added
		if((5 - secsSinceLastCar) <= trafficIntensity) {			// Car rate should range between 1 every 6 seconds to 1 every second based on traffic intensity.
			isCar = 1;
		}
		secsSinceLastCar++;  										// Increment integer timer
		if(isCar) {													// If a car is to be added, reset the value of the "timer"
			secsSinceLastCar = 0;
		}
		xQueueOverwrite(carQueue, &isCar);  						// Load the car queue with the value of isCar 
	}
}

void trafficLightStateTask( void *pvParameters ) {
	int trafficIntensity = 0;	  // Value derived from scaled potentiometer value
	int secsSinceLastChange = 0;  // Integer timer for keeping track of time between traffic light state changes
	int state = green;			  // Current state of the traffic light 


	/* At minimum potentiometer value: green = 5 sec, yellow = 3 sec, red = 10 sec.
	   At maximum potentiometer value: green = 10 sec, yellow = 3 sec, red = 5 sec.*/
	while(1) {
		xQueuePeek(adcQueue, &trafficIntensity, pdMS_TO_TICKS(0));  		  // Peek ADC queue to determine traffic intensity
		if(state == green && secsSinceLastChange >= (trafficIntensity + 5)) { // The time between light transitions scales with traffic intensity 
			state = amber;													  // (i.e. low traffic flow = short green lights && high traffic flow = long green lights)
			secsSinceLastChange = 0;
		}
		if(state == amber && secsSinceLastChange >= 3) {					  // The amber light has a constant transition time 
			state = red;
			secsSinceLastChange = 0;
		}
		if(state == red && secsSinceLastChange >= (10 - trafficIntensity)) {  // Longer red lights with less intense traffic 
			state = green;
			secsSinceLastChange = 0;
		}
		secsSinceLastChange++;												  // Increment integer timer
		xQueueOverwrite(trafficLightQueue, &state);							  // Load traffic light state onto Traffic Light Queue
		vTaskDelay(1000);													  // Block task for 1000ms
	}
}

void systemDisplayTask( void *pvParameters ) {
	int carArr[] = {0, 0, 0, 0, 0,  // Array to represent the current state of cars in the Traffic Light System
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0};  
	int newCar = 0;                 // 1 if new car needs to be added, 0 otherwise
	int trafficLightState = green;  // Current state of traffic light 
	int backedUp = 0;               // 1 while cars are backed up, 0 otherwise

	while(1) {
		xQueuePeek(carQueue, &newCar, pdMS_TO_TICKS(0));					  // Determine if a new car should be added to the traffic flow		
		xQueuePeek(trafficLightQueue, &trafficLightState, pdMS_TO_TICKS(0));  // Retrieve the current state of the traffic light 

		if(trafficLightState == green) {									  // If the light is green, then shift all cars to the right by one.
			for(int i = 18; i > 0; i--) {
				carArr[i] = carArr[i-1];
			}
		}
		else {																  // Otherwise, 
			for(int i = 18; i > 8; i--) {									  // shift all cars to the right, except those before the white line (i.e. clear intersection)
				carArr[i] = carArr[i-1];
			}
			carArr[8] = 0;
			backedUp = 1;				
			for(int i = 7; i > 0; i--) {									  // Shift cars towards the "white line" until the flow is backed up
				if(carArr[i] == 0) {
					backedUp = 0;
					carArr[i] = carArr[i-1];
				} 
				else if(backedUp == 0) {									  // Else condition for when current car is backed up, so spot is occupied
					carArr[i] = carArr[i-1];
				}	
			}
			
			if(carArr[0] == 0){												  // Treats edge case where traffic overflows on a red light
				backedUp = 0;
			}
		}
		if(backedUp == 0){ 													  // Add new car to flow based on value retrived from car queue (make sure first spot is empty)
			carArr[0] = newCar;
		}

		for(int i = 18; i >= 0; i--){  										  // Update physical LED array to match code representation (19 green LEDs)
			if(*(carArr+i) == 1){
				GPIO_SetBits(GPIOC, GPIO_Pin_6);
				GPIO_SetBits(GPIOC, GPIO_Pin_7);
				GPIO_ResetBits(GPIOC, GPIO_Pin_7);

			}
			else{
				GPIO_ResetBits(GPIOC, GPIO_Pin_6);
				GPIO_SetBits(GPIOC, GPIO_Pin_7);
				GPIO_ResetBits(GPIOC, GPIO_Pin_7);
			}
		}
		
		if(trafficLightState == green) {	    // State == green
			GPIO_ResetBits(GPIOC, GPIO_Pin_0);  // Reset Red
			GPIO_SetBits(GPIOC, GPIO_Pin_2);	// Set Green
		}
		else if(trafficLightState == amber) {	// State == amber 
			GPIO_ResetBits(GPIOC, GPIO_Pin_2);  // Reset Green
			GPIO_SetBits(GPIOC, GPIO_Pin_1);	// Set Amber
		} 
		else {									// State == red
			GPIO_ResetBits(GPIOC, GPIO_Pin_1);  // Reset Amber
			GPIO_SetBits(GPIOC, GPIO_Pin_0);	// Set Red
		}

		vTaskDelay(1000);  						// Block task for 1000ms
	}
}

static void prvSetupHardware( void ) {
	NVIC_SetPriorityGrouping( 0 );  // Ensure all priority bits are assigned as preemption priority bits

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);  // Enable clock AHB1 Peripheral Clock for GPIOC, 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);   // and APB2 for ADC

	// GPIO + ADC initialization structs
	GPIO_InitTypeDef Pot_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	// Load with default values
	GPIO_StructInit(&Pot_InitStructure);
	GPIO_StructInit(&GPIO_InitStructure);
	ADC_StructInit(&ADC_InitStructure);

	Pot_InitStructure.GPIO_Mode = GPIO_Mode_AN; 	 // Analog mode
	Pot_InitStructure.GPIO_Pin = (GPIO_Pin_3); 		 // Pin 3 (Potentiometer)
	Pot_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  // No pull
	GPIO_Init(GPIOC, &Pot_InitStructure); 	         // Initialize

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 	 // General purpose output
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8); // LED and Shift Register pins
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;  
	GPIO_Init(GPIOC, &GPIO_InitStructure); 			   // Initialize

	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_Init(ADC1, &ADC_InitStructure);  // Initialize

	ADC_Cmd(ADC1, ENABLE); // Start ADC

	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 1, ADC_SampleTime_84Cycles);
	ADC_SoftwareStartConv(ADC1); // Start conversion
}

void vApplicationMallocFailedHook( void )
{
	/* The malloc failed hook is enabled by setting
	configUSE_MALLOC_FAILED_HOOK to 1 in FreeRTOSConfig.h.

	Called if a call to pvPortMalloc() fails because there is insufficient
	free memory available in the FreeRTOS heap.  pvPortMalloc() is called
	internally by FreeRTOS API functions that create tasks, queues, software 
	timers, and semaphores.  The size of the FreeRTOS heap is set by the
	configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( xTaskHandle pxTask, signed char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configconfigCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected.  pxCurrentTCB can be
	inspected in the debugger if the task name passed into this function is
	corrupt. */
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
volatile size_t xFreeStackSpace;

	/* The idle task hook is enabled by setting configUSE_IDLE_HOOK to 1 in
	FreeRTOSConfig.h.

	This function is called on each cycle of the idle task.  In this case it
	does nothing useful, other than report the amount of FreeRTOS heap that
	remains unallocated. */
	xFreeStackSpace = xPortGetFreeHeapSize();

	if( xFreeStackSpace > 100 )
	{
		/* By now, the kernel has allocated everything it is going to, so
		if there is a lot of heap remaining unallocated then
		the value of configTOTAL_HEAP_SIZE in FreeRTOSConfig.h can be
		reduced accordingly. */
	}
}
/*-----------------------------------------------------------*/