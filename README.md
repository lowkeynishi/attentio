# Attentio

Attentio is a hardware companion project developed for the Interactive Systems module.  
It uses an Arduino Uno, a PIR sensor and an ultrasonic sensor to monitor user activity, and provides feedback via a buzzer and an RGB LED inside a custom 3D‑printed enclosure.

## Project context

- **Module:** Interactive Systems
- **Type:** University hardware prototype (Arduino + sensors + 3D‑printed body)
- **Goal:** Encourage sustained focus and good viewing distance from a monitor by sensing user movement and distance and giving immediate feedback.

## Interaction & behaviour

- **Focus monitoring:**  
  The PIR sensor detects motion in front of the device. Frequent or large movements can indicate loss of focus.

- **Distance monitoring:**  
  The ultrasonic sensor measures how far the user sits from the monitor/device. Values outside a target range can indicate unhealthy eye distance.

- **Feedback:**  
 The RGB LED shows the current status:
  - Blue during startup/warm-up or when the user is outside the ideal range
  - Green when the user is within a healthy distance range and activity is calm
  - Yellow when activity becomes moderately high
  - Red when the user is too close to the monitor or activity becomes too high
  - The buzzer turns on when the measured distance is too close to the monitor.

## Features

- Arduino Uno–based companion device with:
  - PIR motion sensor
  - Ultrasonic distance sensor
  - RGB LED module for visual feedback
  - Buzzer for audio alerts
- Simple breadboard wiring for the prototype stage.
- Custom enclosure designed in Fusion 360 and 3D‑printed with a Bambu Lab printer.

## Hardware components

- Arduino Uno
- PIR sensor
- Ultrasonic sensor (e.g. HC‑SR04)
- RGB LED module
- Buzzer
- Small breadboard
- Jumper wires
- USB cable for power and programming

## Pin configuration

- PIR sensor: pin 2
- RGB LED red: pin 12
- RGB LED green: pin 11
- RGB LED blue: pin 13
- Ultrasonic trig: pin 4
- Ultrasonic echo: pin 5
- Buzzer: pin 8  

## Software & tools used

- **Arduino IDE:** for writing and uploading the `attentio.ino` sketch to the Arduino Uno.
- **Fusion 360:** for designing the physical body/enclosure of the prototype.
- **Bambu Lab slicer/printer:** for slicing and 3D‑printing the Fusion 360 model.

## Logic overview

- The system starts with a 60-second warm-up period for the PIR sensor.
- Motion events increase an activity score.
- The activity score gradually decays over time.
- Distance is measured continuously using the ultrasonic sensor.
- A green-zone hysteresis is used to avoid unstable LED switching when the distance is near the acceptable boundary.
- If the user is too close to the monitor (below 30 cm), the LED turns red and the buzzer is activated.
- If activity is too high, the LED changes to yellow or red depending on the score.

## Getting started

## Setup and usage

1. Connect all components to the Arduino Uno according to the pin configuration.
2. Open the project in Arduino IDE.
3. Select the correct Arduino Uno board and COM port.
4. Upload the code to the board.
5. Place the prototype near a monitor.
6. Observe the LED color and buzzer feedback based on movement and viewing distance.

## Files

- `attentio.ino` – main Arduino sketch
- `models/` – Fusion 360 or STL files for the enclosure

## Academic context

This project was developed as part of the Interactive Systems module. It combines embedded sensing, feedback design, and physical prototyping in a companion device intended to support better study habits.

## License

MIT License

## Author

Developed by a group of Computer Science students at Technische Hochschule Brandenburg as part of the Interactive Systems module.
- Muhammad Danish
- Abdulai
- Benn Miguel
- Scott
