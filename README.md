Interrupt-Driven Security System (4⭐ Distinction)
Overview:
This project shows a fully interrupt driven security system using Arduino Uno and demostrates Pin change interrupt, timer interrupt and most importantly sense think act arcitecture.

Hardware Components Used:
Door Sensor (push button) - D8
Motion Sensor - D9
Tilt Sensor - A2
Alarm LED - D13
Status LED - D12

System Behavior :
- If any sensor is triggered then Alarm LED turns ON
- timer1 interupts runs every 1 sencond and every 1 second LED blinks
- All sensors are handled using Pin change interrupts.
- ISRs only set flags and logic runs in the main loop.

How to Run:
1. Upload .ino file to Arduino Uno.
2. Open serial monitor.
3. Trigger sensors to observe interrupt driven behavior.
4. Observe concurrent timer heartbeat.
