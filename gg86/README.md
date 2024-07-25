# Customized QMK firmware for Gopolar GG86

Based off HorrorTroll's implementation in QMK's main repository.

## Modifications

- Strip Gopolar's OLED boot sequence and RGB matrix animations.
- OLED displays only BongoCat + WPM meter. BongoCat code is slightly compressed compared to the regular implementation.
- Blue shine on the caps lock key and scroll lock key when caps lock and scroll lock are activated respectively.
- FN+Scroll Lock to toggle layer 2. On layer 2,
  - SOCD cleaning for A and D keys.
  - Hold down Caps Lock to rapid fire mouse 1.
  - Hit Scroll Lock again to toggle back to layer 0.
