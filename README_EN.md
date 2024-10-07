# PROJETE2405

- ***[Leia em Português](https://github.com/11-pog/PROJETE2405ESP/blob/main/README.md)***

This is the code for my project presented at the science fair of ETEFMC, called PROJETE.

The fair is already over, so there’s no real reason for me to make changes to this absolute mess of a code (though I’ll probably end up doing it anyway, or maybe not idk).

The project was an **automatic sheep feeder** with some functionalities: two graph showing the **humidity** and the **feed level remaining** in the dispenser storage. It was also possible to schedule three different times for the feeder to dispense the appropriate amount of feed for the sheep. This was all made possible through [This Website](https://github.com/JuanCB1/site-com-contador), developed by [Another Team Member](https://github.com/JuanCB1), and integrated by me into the microcontroller using the **MQTT** protocol.

(There's also a camera feature on the site, but I didn’t mention it here because it has nothing to do with the ESP, and it also wasn’t my part of the project).

## Some notes

- The code is written in **C++** and runs on the **ESP32**.
- I used PlatformIO in VSCode.
- The feed quantity configuration **does not** control the exact amount of feed released. It simply spins for a specific amount of time without precisely measuring the quantity dispensed.

## Features

- A *DHT22* sensor was used to measure the humidity in the feed storage, and an *HC-SR04 ultrasonic sensor* was used to monitor the feed level.
- The motor for the feeder was a *Micro Motor DC 12V 300rpm with N20 Gearbox*.
- If im not mistaken, an *ESP32 ESP-WROOM-32* was used.
- The pins were:

  - 5 for the Trigger, and 18 for the Echo of the ultrasonic sensor.
  - 27 for the DHT22 data.
  - 22 for the motor.
  - These pins can be edited in `ProjectSettings.cpp/.h` and `SerialUtils.h`, respectively.

## The End

![NEON GENESIS PROJETE 2K24](https://github.com/11-pog/PROJETE2405ESP/blob/main/THE%20END%20OF%20PROJETE.png?raw=true)

PROJETE will happen again in 2025 lmao

Note: This is only here for me to one day look back at it and say "Holy hell this project sucked"

Another note: This README was made with the help of my boy ChatGpt (Because i suck at writing, sorry).

Third note: Ignore the image.
