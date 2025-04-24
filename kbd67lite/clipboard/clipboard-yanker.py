import sys
import hid
from pynput import keyboard
import win32clipboard

# Only for kbd67mkiirgb, modify below values if to use with other keyboards
product_id = 0x1225
vendor_id = 0x4B42

usage_page    = 0xFF60
usage         = 0x61
report_length = 32

def get_raw_hid_interface():
    device_interfaces = hid.enumerate(vendor_id, product_id)
    raw_hid_interfaces = [i for i in device_interfaces if i['usage_page'] == usage_page and i['usage'] == usage]

    if len(raw_hid_interfaces) == 0:
        return None

    interface = hid.Device(path=raw_hid_interfaces[0]['path'])

    print(f"Manufacturer: {interface.manufacturer}")
    print(f"Product: {interface.product}")

    return interface

def send_buffer(data):
    MAX_REQ_SIZE = 512
    interface = get_raw_hid_interface()

    if interface is None:
        print("No compatible device found")
        sys.exit(1)

    data_start = 0
    while data_start < len(data) and data_start <= MAX_REQ_SIZE:
        request_data = [0x00] * (report_length + 1) # First byte is Report ID
        data_end = min(len(data), data_start+32)
        request_data[1:(data_end-data_start+1)] = data[data_start:data_end]
        request_report = bytes(request_data)
        assert(len(request_report) == 33)
        data_start += 32
        try:
            interface.write(request_report)
        except:
            print(f"Error sending data {request_data}")
    interface.close()

def on_press(key):
    if key == keyboard.Key.f21:
        # yank
        win32clipboard.OpenClipboard()
        s = win32clipboard.GetClipboardData()
        print(f"yanked {s}")

        yank_buffer = bytearray()
        yank_buffer.extend(map(ord, s))
        win32clipboard.CloseClipboard()
        send_buffer(yank_buffer)


with keyboard.Listener(on_press=on_press) as listener:
    listener.join()
