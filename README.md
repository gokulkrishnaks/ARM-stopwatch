# ARM Stopwatch Using LPC2148 Microcontroller

This project demonstrates a Digital Stopwatch using an ARM7-based LPC2148 Microcontroller. The stopwatch features Start, Stop, and Reset functionalities, and displays elapsed time on a 16x2 LCD.

## Project Description
- Microcontroller: LPC2148 (ARM7TDMI-S Core)
- Features:
  - Start, Stop, and Reset controls using Push Buttons
  - Real-time time display on a 16x2 LCD
  - Timer0 interrupt-driven stopwatch functionality
  - Precision up to milliseconds
- Application: Digital Timers, Embedded Time-based Applications, Academic Projects.

## Hardware Requirements
- LPC2148 Development Board
- 16x2 LCD: HD44780 compatible
- Push Buttons (3 units): For Start, Stop, Reset operations
- Power Supply: 3.3V regulated (with USB-UART or external power)
- Breadboard, Jumper Wires, Resistors

## Software Requirements
- Keil uVision (for ARM C code compilation & HEX generation)
- Proteus Design Suite (for circuit simulation)
- Flash Magic (if flashing to hardware)

## Project Folder Structure
```
arm-stopwatch/
├── src/
│   └── main.c               # C Source Code
├── simulation/
│   └── project.pdsprj       # Proteus simulation file
└── README.md                # Project Overview & Instructions
```

## Steps to Compile HEX File in Keil
1. Open Keil uVision IDE.
2. Create a New Project:
   - Project > New Project
   - Name it: ARM_Stopwatch
3. Select Microcontroller:
   - From the device database, select NXP > LPC2148.
4. Add Source File to Project:
   - Right-click Source Group 1 > Add Existing Files to Group.
   - Browse and select `main.c` from `/src/`.
5. Configure Output HEX File:
   - Go to Project > Options for Target 'Target 1'.
   - Under Output tab, check "Create HEX File".
6. Build the Project:
   - Press F7 or go to Project > Build Target.
   - The generated HEX file will be located inside the project directory.

## Running the Simulation in Proteus
### Simulation Prerequisites:
- Ensure you have Proteus component libraries:
  - LPC2148 Microcontroller
  - HD44780 LCD (16x2)
  - Push Buttons
  - Resistors and Power Supply

### Simulation Steps:
1. Open Proteus Design Suite.
2. Open the Provided Simulation File:
   - Navigate to `/simulation/` folder.
   - Open `project.pdsprj`.
3. Load the HEX File to LPC2148:
   - Double-click on the LPC2148 symbol.
   - In the Program File field, browse and select the generated HEX file.
4. Run the Simulation:
   - Click **Play (Run Simulation)**.

### Operation Flow:
- **Start Button (P0.14)**: Starts the stopwatch.
- **Stop Button (P0.15)**: Pauses/Stops the stopwatch.
- **Reset Button (P0.16)**: Stops and resets the stopwatch time to 00:00:000.
- The elapsed time is displayed on the LCD in mm:ss:ms format.

## How the Code Works
- PLL is configured to run the system clock at 60MHz.
- LCD is interfaced in 4-bit mode for display operations.
- Timer0 is configured to trigger an interrupt every 1ms.
- Timer0 ISR increments the time counters (ms, sec, min) and updates the LCD display.
- Push Buttons are polled in a loop to control Start, Stop, and Reset functionalities.

## Flowchart
```
Start
  |
  |---> Initialize PLL (60MHz)
  |---> Initialize LCD
  |---> Configure Timer0 for 1ms interrupts
  |---> Enable VIC interrupt for Timer0
  |
Main Loop:
  |
  |---> Check Start Button (P0.14)
  |        |
  |        +--> Set Run Flag = 1
  |
  |---> Check Stop Button (P0.15)
  |        |
  |        +--> Clear Run Flag = 0
  |
  |---> Check Reset Button (P0.16)
  |        |
  |        +--> Reset ms, sec, min = 0
  |        +--> Clear Run Flag = 0

Timer0 ISR (Every 1ms):
  |
  |---> If Run Flag == 1:
  |         |
  |         +--> Increment ms
  |         +--> Update sec and min when ms >= 1000 or sec >= 60
  |         +--> Refresh LCD Display with new Time
  |
  |---> Clear Timer0 Interrupt Flag
  |---> Return to Main Loop
```

## Notes
- The LCD operates in 4-bit mode to save GPIO pins.
- The ISR is lightweight to ensure consistent timing.
- Buttons are polled instead of using interrupts for simplicity.
- Ensure debounce handling in hardware or via software if necessary.

## Troubleshooting
- **HEX File Not Loading in Proteus?**
  - Ensure correct HEX file generation and path in Proteus.
  - Verify LPC2148 part is correctly placed in Proteus.
- **LCD Not Displaying Correctly?**
  - Check LCD pin connections and contrast settings.
- **Button Response Delays?**
  - Ensure proper grounding and debounce mechanisms.

## License
This project is licensed under the MIT License. You are free to use, modify, and distribute this code and simulation files.

## Preview (Add Screenshots in /docs/)
- Simulation Circuit Diagram
- LCD showing Stopwatch Ready
- LCD displaying running time

## Author
Gokul Krishna K S
