# Endgame 48 (v2)

Puchi-C based 40% keyboard with rotary encoder and I2C OLED

- Keyboard Maintainer: Andrew Dunai
- Hardware Supported: Puchi-C handwired

Make example for this keyboard (after setting up your build environment):

    make endgame48_v2:default:dfu

## Details

- Puchi-C based handwired keyboard
- 2x custom 1.25mm stainless steel plates
- Kailh Choc White (clicky)
- Rotary encoder
- 0.91" 128x32 I<sup>2</sup>C OLED
- WS2812 strip (12 LEDs)
- Handwiring is fun!

## Pinout

```
                 ┌────────┐
          ╭──────┤        ├──────╮
     RGB> ┿ D3   │        │  RAW ┿
          │      │        │      │
    PS2D> ┿ D2   └────────┘  GND ┿
          │                      │
          ┿ GND              RST ┿
          │      []      []      │
          ┿ GND              VCC ┿
          │                      │
     I2C> ┿ D1   ||| ||  ||   F4 ┿ <ROW5
          │                      │
     I2C> ┿ D0                F5 ┿ <COL1
          │          ╱╲          │
    ROW1> ┿ D4      ╱  ╲      F6 ┿ <COL2
          │        ╱    ╲        │
    ROW3> ┿ C6     ╲    ╱     F7 ┿ <COL3
          │         ╲  ╱         │
    COL7> ┿ D7       ╲╱       B1 ┿ <COL4
          │                      │
    ENCA> ┿ E6                B3 ┿ <COL5
          │                      │
    ENCB> ┿ B4                B2 ┿ <COL6
          │                      │
    ROW2> │ B5 B7 D5 C7 F1 F0 B6 ┿ <ROW6
          ╰────╂──╂──╂──╂──╂─────╯
               ┃  ┃     ┃  ┃
          ROW4 ┛  ┃     ┃  ┗ ROW8
                  ┃     ┃
             PS2C ┛     ┗ ROW7
```
