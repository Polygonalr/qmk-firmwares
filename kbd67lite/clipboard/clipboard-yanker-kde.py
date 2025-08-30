# Vibe-translated by Qwen3-235B-A22B-2507 from clipboard-yanker-win.py
import sys
import hid
from pynput import keyboard
import pyperclip  # Cross-platform clipboard access

# Only for kbd67mkiirgb, modify below values if to use with other keyboards
product_id = 0x1225
vendor_id = 0x4B42

usage_page = 0xFF60
usage = 0x61
report_length = 32


def get_raw_hid_interface():
    device_interfaces = hid.enumerate(vendor_id, product_id)
    raw_hid_interfaces = [
        i for i in device_interfaces
        if i['usage_page'] == usage_page and i['usage'] == usage
    ]

    if len(raw_hid_interfaces) == 0:
        print("No compatible device found.")
        return None

    interface = hid.Device(path=raw_hid_interfaces[0]['path'])

    print(f"Manufacturer: {interface.manufacturer}")
    print(f"Product: {interface.product}")

    return interface


def send_buffer(data):
    MAX_REQ_SIZE = 512
    interface = get_raw_hid_interface()

    if interface is None:
        print("No compatible device found. Exiting.")
        sys.exit(1)

    data_start = 0
    while data_start < len(data) and data_start <= MAX_REQ_SIZE:
        request_data = [0x00] * (report_length + 1)  # Report ID + 32 bytes
        data_end = min(len(data), data_start + 32)
        request_data[1:(data_end - data_start + 1)] = data[data_start:data_end]
        request_report = bytes(request_data)
        if len(request_report) != 33:
            print(f"Error: Invalid report length {len(request_report)}")
            break
        try:
            interface.write(request_report)
        except Exception as e:
            print(f"Error sending data: {e}")
        data_start += 32

    interface.close()


def on_press(key):
    try:
        # Use F21 — ensure it's triggered via keyboard or KDE custom shortcut
        if key == keyboard.Key.f21:
            print("F21 pressed – yanking clipboard content...")
            s = pyperclip.paste()
            if s:
                print(f"Yanked: {repr(s)}")

                # Convert string to byte array
                yank_buffer = bytearray(s.encode('utf-8'))
                send_buffer(yank_buffer)
            else:
                print("Clipboard is empty.")
    except Exception as e:
        print(f"Error: {e}")


# Start listening for keypresses
with keyboard.Listener(on_press=on_press) as listener:
    listener.join()