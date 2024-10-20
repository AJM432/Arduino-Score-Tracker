![Logo](https://github.com/user-attachments/assets/4cdb3e66-0bf5-4e4e-92b4-fc69b3fb4f57)

A general purpose Arduino score tracker that uses an IR remote to keep track of scores on a LCD display.


![](https://img.shields.io/badge/Arduino-00979D?style=for-the-badge&logo=Arduino&logoColor=white)

## Demo
https://github.com/user-attachments/assets/2c63fc98-199b-4376-a7fb-062b0c539bf4

## Usage
To set this up, connect your components according to the pin mappings below and upload the code to your Arduino UNO using the Arduino IDE.

### Pin Mappings
|Part|Pin|
|----|---|
|LCD VSS|GND|
|LCD VDD|+5V|
|LCD V0|PIN 6|
|LCD RS|PIN 7|
|LCD RW|GND|
|LCD E|PIN 8|
|LCD D4|PIN 9|
|LCD D5|PIN 10|
|LCD D6|PIN 11|
|LCD D7|PIN 12|
|LCD A|+5V|
|LCD K|GND|
|LED|PIN 4|
|1kΩ Resistor|In series with LED|
|Passive Buzzer|PIN 2|
|IR Receiver|PIN 5|

## Components
|Part| Function|
|---------|---------|
|Arduino UNO R3|Microcontroller|
|LCD1602 Module (with pin header)|Displays Score|
|Remote Control|Send commands to the microcontroller|
|IR Receiver Module (VS1838B)|Recieves commands from remote|
|Passive Buzzer|Buzzes when game begins/ends/updates|
|Green LED|Flashes when game begins/ends/updates|
|1kΩ Resistor|In series with LED to limit current|

## Help Menu
|Command|Button|
|-------|--------|
|Help Menu|9|
|Go Down|DOWN|
|Go Up|UP|
|Team 1 Scores|VOL+|
|Team 1 Loses|PAUSE|
|Team 2 Scores|FUNC/STOP|
|Team 2 Loses|FAST FORWARD|
|Play Game|FAST BACK|
|Finish Game|VOL-|
|Toggle Buzzer|EQ|
|Toggle LED|ST/REPT|
|Increment Contrast|8|
|Decrement Contrast|7|

## License

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)
## 🔗 Links
[![portfolio](https://img.shields.io/badge/my_portfolio-000?style=for-the-badge&logo=ko-fi&logoColor=white)](https://alvinmatthew.com/)
