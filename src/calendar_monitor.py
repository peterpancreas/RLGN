# FILE: /calendar-monitor/calendar-monitor/src/calendar_monitor.py
import asyncio
import msvcrt

NOTIFICATION_TAG = "YourNotificationTag"  # Replace with your actual notification tag

class CalendarMonitor:
    def __init__(self):
        self.ble_client = None  # Initialize your BLE client here

    async def monitor_calendar(self):
        while True:
            event_status = await self.check_calendar_events()
            await self.update_light_status(event_status)
            await self.check_for_commands()

    async def check_calendar_events(self):
        # Logic to check calendar events
        pass

    async def update_light_status(self, event_status):
        # Logic to update light status based on event status
        pass

    async def force_light_off(self):
        # Logic to force the light off
        pass

    async def check_for_commands(self):
        for _ in range(60):  # Check every second for a minute
            if msvcrt.kbhit():
                char = msvcrt.getch().decode('utf-8').lower()
                if char == 'o':
                    await self.force_light_off()
                elif char == 'q':
                    return
            await asyncio.sleep(1)

async def main():
    monitor = CalendarMonitor()
    print(f"Starting calendar monitor... Looking for events with {NOTIFICATION_TAG}")
    print("Press 'o' to turn off the light, 'q' to quit")
    
    try:
        await monitor.monitor_calendar()
    except KeyboardInterrupt:
        print("\nShutting down...")
    finally:
        if monitor.ble_client and monitor.ble_client.is_connected:
            await monitor.force_light_off()
            await monitor.ble_client.disconnect()
        print("Goodbye!")

if __name__ == "__main__":
    asyncio.run(main())