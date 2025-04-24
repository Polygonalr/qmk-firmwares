# Custom KBD67 Lite R1 keymap with clipboard feature

- Base firmware: `kbdfans/kbd67/mkiirgb/v2`
- Includes TaUnTcAsE (toggle with FN + CapsLck)
- Tapdancing for easier access to F-row keys (Tap left Alt + `1 to =` to input `F1 to F12`)
  - Tap, release and then hold left Alt before pressing `1 to =` to input `Alt + F1 to F12`
- Unlike the normal keymap, this keymap includes a clipboard feature. For this reason, VIA cannot be used to update the keymap.
  - Only works with Windows at the moment, though `clipboard-yanker.py` can be updated to work with other OS as well.
  - `clipboard-yanker.py` must be running in the background for the OS to be able to send data to the keyboard.
  - `FN + C` to send OS clipboard data to the keyboard, `FN + V` to send the saved clipboard data back to the OS.
