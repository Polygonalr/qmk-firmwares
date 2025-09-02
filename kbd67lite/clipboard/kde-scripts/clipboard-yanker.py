import os
import sys
import subprocess
from time import sleep
from pynput import keyboard
import pyperclip

PIPE_PATH = "/tmp/clipboard_pipe"
PUSHER_SCRIPT = "./hid_pusher.py"

def on_press(key):
    try:
        # trigger if F21 is pressed
        if hasattr(key, 'vk') and key.vk == 269025193:
            print("F21 pressed - yanking clipboard content...")
            s = pyperclip.paste()
            if s:
                print(f"Yanked: {repr(s)}")
                with open(PIPE_PATH, 'w') as pipe:
                    pipe.write(s + '\n')
                    pipe.flush()
            else:
                print("Clipboard is empty.")
    except Exception as e:
        print(f"Error: {e}")


if __name__ == "__main__":
    python_executable = sys.executable
    process = subprocess.Popen(
        ['sudo', python_executable, PUSHER_SCRIPT],
        stdin=subprocess.PIPE,
        stdout=None,
        stderr=None
    )
    print("HID data pusher process launched.")
    sleep(1)

    # Start listening for keypresses
    with keyboard.Listener(on_press=on_press) as listener:
        listener.join()