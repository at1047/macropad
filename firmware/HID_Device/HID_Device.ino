/*
  Keyboard test

  For the Arduino Leonardo, Micro or Due

  Reads a byte from the serial port, sends a keystroke back.
  The sent keystroke is one higher than what's received, e.g. if you send a,
  you get b, send A you get B, and so forth.

  The circuit:
  - none

  created 21 Oct 2011
  modified 27 Mar 2012
  by Tom Igoe

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/KeyboardSerial
*/

#include "Keyboard.h"

void setup() {
  // initialize control over the keyboard:
  pinMode(2, INPUT_PULLUP);
  Keyboard.begin();
}

void loop() {
  // check for incoming serial data:
  if (digitalRead(2) == LOW) {
    //Send the message
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press('3');
      delay(100);
    Keyboard.releaseAll();
    delay(1000);
  }
}
