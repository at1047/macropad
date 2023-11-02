# Macropad

Mainly a proof of concept/sanity check for diode-based keyboard matrix, but also uses an ESP 8266 chip with WiFi capabilities to control various IoT devices around the house. A little down the line, I decided I wanted the ability for it to interface with my computer as an HID (Human Interface Device, i.e. USB input) to send macro commands, but an ESP 8266 chip does not support being an HID. The alternative Pro Micro chip, which can be an HID, in turn, does not support WiFi capabilities. My solution for this is to have the ESP 8266 chip be the main board, and interface with a Pro Micro decoder, which translates Serial communication from the ESP 8266 into Keystrokes that will be sent to the computer.

![alt text](https://github.com/at1047/macropad/blob/main/Assets/Macropad%20v19.png)
