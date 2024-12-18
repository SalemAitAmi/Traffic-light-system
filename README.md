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

The Traffic Light System project effectively demonstrates the use of FreeRTOS to manage multiple concurrent tasks in an embedded system. By leveraging the STM32F405 microcontroller's capabilities and the FreeRTOS task management features, this project simulates a real-world traffic light control system.

The project is divided into four main tasks: traffic flow adjustment, traffic generation, traffic light state management, and system display. Each task is responsible for a specific aspect of the traffic system, and they communicate with each other using message queues. This modular approach makes the system easier to understand, debug, and extend.

The hardware setup, which includes the potentiometer, LEDs, and shift registers, provides a hands-on demonstration of how embedded systems can interact with the physical world. The potentiometer allows for dynamic adjustment of traffic intensity, showcasing how input from the real world can influence the system's behavior in real-time.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- LIMITATIONS & POTENTIAL IMPROVEMENTS -->
## Limitations and Potential Improvements

- **Queue Sizes:** The current implementation uses queues with a size of 1. This might lead to data loss if tasks attempt to write to a full queue. Increasing the queue sizes could help mitigate this issue.

- **Fixed Delays:** The tasks use fixed delays (vTaskDelay) to simulate real-time operation. However, this can lead to non-deterministic behavior if the system becomes overloaded. Implementing more sophisticated timing mechanisms or using hardware timers could improve accuracy.

- **Scalability:** The system is designed for a single-lane traffic intersection. Extending it to handle multiple lanes or more complex traffic scenarios would require significant changes to the task logic and inter-task communication.

- **Error Handling:** Currently, there is minimal error handling. Adding mechanisms to detect and respond to hardware failures, task overruns, or communication errors would increase system robustness.

- **Energy Consumption:** Continuous ADC conversions and high-frequency task switching may lead to higher power consumption. Implementing power-saving modes and optimizing task scheduling could make the system more energy-efficient.

- **Physical Interface:** The breadboard setup, while useful for prototyping, is not ideal for a final product. Designing a printed circuit board (PCB) and creating a more permanent physical setup would improve the system's reliability and durability.

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
