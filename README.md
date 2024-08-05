<a name="readme-top"></a>

<!-- Title -->
<div align="center">
<h3 align="center">Traffic Light System</h3>

  <p align="center">
    A single lane traffic simulation using FreeRTOS & STM32F405 MCU
  </p>
</div>


<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#overview">Overview</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#design-solution">Design Solution</a>
      <ul>
        <li><a href="#prv-setup">PRV Setup</a></li>
        <li><a href="#task-1">Task 1</a></li>
        <li><a href="#task-2">Task 2</a></li>
        <li><a href="#task-3">Task 3</a></li>
        <li><a href="#task-4">Task 4</a></li>
      </ul>
    </li>
    <li><a href="#discussion">Discussion</a></li>
    <li><a href="#limitations-and-potential-improvements">Limitations and Potential Improvements</a></li>
    <li><a href="#contact">Contact</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## Overview

This project involves using an STM32F405 microcontroller in conjunction with FreeRTOS to simulate a single-lane traffic intersection. 
The system is physically simulated on a breadboard with 19 small green LEDs as the lane, 3 large LEDs (green, amber, red) as the traffic light, a potentiometer, 3 shift registers, and several wires and resistors.
The potentiometer's variable voltage is used to implement variable traffic flow and traffic light timings. FreeRTOS APIs are used to facilitate inter-task communication and simplify the hardware-softawre co-design.

The overall design consists of 4 tasks and 3 queues

<p align="right">(<a href="#readme-top">back to top</a>)</p>


### Built With

* C
* FreeRTOS
* STM32f405 MCU

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- DESIGN SOLUTION -->
## Design Solution

### Includes & Main

The program uses few standard libraries, including `stdint.h` for exact-width integers, `stdio.h` for printing to the screen, and `stdlib.h` for utility functions. 
The files, `stm32f4_discovery.h` and `stm32f4xx.h`, are included for configuration purposes for FreeRTOS to determine the STM32F4xx device used in the target application and whether to use periphal drivers or not.
Lastly, the files `FreeRTOS.h`, `queue.h`, `semphr.h`, `task.h`. and `timers.h` are included to enable access to the queue, semophore, task, and timer APIs respectively. 
Function prototypes are used to make the functions visible to main alongside defines for traffic light states and global queue handles. 
![includes]

The first function called in main `prvSetupHardware` configures hardware parameters and initializes the GPIO pins used by the system. Then 3 queues are initialized with a size of 1 and added to the registry for debuging purposes. The `carQueue` is loaded with either 0 or 1 and determines if a new car should be added at the end of the traffic flow. The `trafficLightQueue` is loaded with the current state of the traffic light and is used to synchronize light changes. The `adcQueue` is loaded with an integer value between 0 to 6 that determines the intensity of traffic and the interval between light changes. Lastly, Task handles are initialized with the same priority and the scheduler is started.
![main]

### PRV Setup

This function initializes, configures, and starts all of the hardware interfaces used by the program.
![prv]

### Task 1

The `trafficFlowAdjustmentTask` reads values from the ADC's buffer which is itself receiving voltage from the potentiometer.
![task1]

### Task 2

![task2]

### Task 3

![task3]

### Task 4

![task4]

### Breadboard

![breadboard]

### Circuit Diagram

![circuit-diagram]

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- DISCUSSION -->
## Discussion


<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- LIMITATIONS & POTENTIAL IMPROVEMENTS -->
## Limitations and Potential Improvements


<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- CONTACT -->
## Contact

Salem Ait Ami - [LinkedIn](https://www.linkedin.com/in/salemaitami/) - [salemaitami@uvic.ca](salemaitami@uvic.ca)

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- MARKDOWN LINKS & IMAGES -->
[breadboard]: images/breadboard.png
[circuit-diagram]: images/circuit_diagram.png
[prv]: images/prv_setup_hardware.png
[includes]: images/includes.png
[main]: images/maintls.png
[task1]: images/task1.png
[task2]: images/task2.png
[task3]: images/task3.png
[task4]: images/task4.png
