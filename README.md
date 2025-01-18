# Calendar Monitor

This project is a Python application that monitors calendar events and manages a Bluetooth Low Energy (BLE) device to control a light based on those events.

## Overview

The `CalendarMonitor` class in `src/calendar_monitor.py` handles the connection to a BLE device and updates the light status according to the calendar events. The application runs asynchronously, allowing it to monitor events and respond to user commands simultaneously.

## Features

- Monitors calendar events for specified notifications.
- Connects to a BLE device to control light status.
- Provides user commands to turn off the light or quit the application.

## Setup Instructions

1. Clone the repository:
   ```
   git clone https://github.com/yourusername/calendar-monitor.git
   cd calendar-monitor
   ```

2. Install the required dependencies:
   ```
   pip install -r requirements.txt
   ```

## Usage

To run the calendar monitor, execute the following command:
```
python src/calendar_monitor.py
```

While the application is running, you can use the following commands:
- Press `o` to turn off the light.
- Press `q` to quit the application.

- ## Arduino Code

The Arduino code for this project is located in the `arduino code` directory.

### Setup Instructions

1. Open the Arduino IDE.
2. Open the `.ino` file located in the `arduino code` directory.
3. Connect your Arduino board to your computer.
4. Select the appropriate board and port in the Arduino IDE.
5. Upload the code to the Arduino board.

## License

This project is licensed under the MIT License. See the LICENSE file for more details.
