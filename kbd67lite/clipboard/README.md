# Custom KBD67 Lite R1 keymap with clipboard feature

- Base firmware: `kbdfans/kbd67/mkiirgb/v2`
- Includes TaUnTcAsE (toggle with FN + CapsLck)
- Tapdancing for easier access to F-row keys (Tap left Alt + `1 to =` to input `F1 to F12`)
  - Tap, release and then hold left Alt before pressing `1 to =` to input `Alt + F1 to F12`
- Unlike the normal keymap, this keymap includes a clipboard feature. For this reason, VIA cannot be used to update the keymap.
  - Only works with Windows & Linux at the moment, though the clipboard yanker scripts can be updated to work with other OS as well.
  - `clipboard-yanker-xxx.py` must be running in the background for the OS to be able to send data to the keyboard.
  - `FN + C` to send OS clipboard data to the keyboard, `FN + V` to send the saved clipboard data back to the OS.

## Linux (Debian) specific instructions

```
sudo apt install python3-pip libhidapi-hidraw0 libhidapi-libusb0 xclip
pip3 install hidapi pyperclip pynput
```

Might need to update permissions for accessing HID device too.

```
sudo nano /etc/udev/rules.d/99-kbd67mkii.rules
SUBSYSTEM=="hidraw", ATTRS{idVendor}=="4b42", ATTRS{idProduct}=="1225", MODE="0666"
sudo udevadm control --reload-rules
sudo udevadm trigger
```

Replug the keyboard afterwards.