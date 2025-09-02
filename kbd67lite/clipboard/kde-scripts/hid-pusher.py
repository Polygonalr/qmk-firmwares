import sys
import hid

PIPE_PATH = "/tmp/clipboard_pipe"

# Only for kbd67mkiirgb, modify below values if to use with other keyboards
product_id = 0x1225
vendor_id = 0x4B42

usage_page = 0xFF60
usage = 0x61
report_length = 32


def get_raw_hid_interface():
    interfaces_list = hid.enumerate(vendor_id, product_id)
    interface = hid.device()
    interface.open_path(interfaces_list[1]['path'])

    print(f"Manufacturer: {interface.get_manufacturer_string()}")
    print(f"Product: {interface.get_product_string()}")

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
        print(request_data)
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


if __name__ == "__main__":
    with open(PIPE_PATH, 'r') as pipe:
        while True:
            data = pipe.readline()
            if data:
                send_buffer(data)
            else:
                exit(0)
