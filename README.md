# Water-Level-Control-ESP32

## Introduction
This repository contains a code which controls a tank level having any liquid by alternating the voltage of a pump between 0V and 3.3V, where in 0V the pump is shut and in 3.3V the pump opens, filling the tank with the liquid.

This project was made utilizing the ESP32 board, responsible for compilling the code and automating the pump control.

## How it Works?
Inicially we would have an empty tank of 18.4cm, where the level of liquid inside of it is controlled by two pumps, one responsible for leaking the liquid to another tank (PUMP 1) and another responsible for filling the tank with the liquid (PUMP 2).

To this project it was established that the level of liquid inside the tank must vary between 8cm and 12cm, not leaving this interval of 4cm.

Therefore in order for the closed system to function the PUMP 1 is constantly open, leaking the liquid, while the PUMP 2 is open and closed respecting the logic present in the code, where this pump has a bigger flow rate than the PUMP 1, meaning if both are open it will fill anyway.

The measures responsible for the controlling of the level of liquid are made by two equipements: a Ultrassonic Sensor HC-SR04 and a MPU6050 Accelerometer with Gyroscope. The accelerometer simulates a floater inside the tank where it controls the level of liquid inside the tank by its position, while the Ultrassonic sensor measures the size in cm of the liquid level inside the tank. In addition the level of liquid inside the tank can be acertained by four LEDs (Red, Green, Blue and Yellow), where the Red means that the liquid level is not where its ideally expected to be, the Green means that the level is where its supposed to be, the Yellow means that the PUMP 1 is open and the Blue that PUMP 2 is open.

## The Logic behind the Code
As said before the tank begins empty, and so both yellow, blue and red LEDs are activated and a voltage of 3.3V is applied to PUMP 2. So the level of liquid inside the tank starts rising and upon entering the interval of 8 and 12 cm the red LED turns off and the green is lit, signifying that the liquid level of the tank has reached the ideal. 

After this point the liquid continues to enter the tank until it passes 12cm where PUMP 2 receives a voltage of 0V and is turned off, meaning that the blue LED is turned off and the liquid level will now reduce until it reaches 8cm again and starts filling again, repeating the process over and over mantaining the level of liquid constant in the 4cm interval.

## Dependencies
In order for the project to work effectively you would need the equipment:
> - Ultrassonic Sensor HC-SR04,
> - MPU6050 Accelerometer with Gyroscope,
> - ESP32;

and the following libraries:

>Libraries
> - Adafruit_MPU6050.h,
> - Adafruit_Sensor.h,
> - Wire.h;

## Simulation and montage
![montagem](https://user-images.githubusercontent.com/110485566/184224668-bef73f1d-1ed5-498b-82c0-3788e161532a.PNG)
Simulation a image aquired from Wokwi
Simulation Link: https://wokwi.com/projects/338209863448396370

## Behavior Graph
![graph](https://user-images.githubusercontent.com/110485566/184224301-19949364-7122-476a-b750-5d33e43f4582.jpg)
