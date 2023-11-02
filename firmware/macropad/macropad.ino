#include <FastLED.h>
#define NUM_LEDS 22 //19
#define LED_PIN 0
#define COLOR_ORDER GRB
#define CHIPSET WS2812B
#define BRIGHTNESS 255
#define VOLTS 5
#define MAX_AMPS 500

// #include <Arduino.h>
// #include <ESP8266WiFi.h>
// #include <ESP8266HTTPClient.h>
// #include <WiFiClientSecureBearSSL.h>

const unsigned long debounceDelay = 20;    // the debounce time; increase if the output flickers

CRGB rgb[NUM_LEDS];
int monitorLight = 0;

byte rows[] = {14,16};
const int rowCount = sizeof(rows)/sizeof(rows[0]);

// JP2 and JP3 are outputs
byte cols[] = {12,13,2};
const int colCount = sizeof(cols)/sizeof(cols[0]);

byte keys[colCount][rowCount];
unsigned long lastDebounceTimeArr[colCount][rowCount];
byte lastButtonStateArr[colCount][rowCount];
byte buttonStateArr[colCount][rowCount];
byte toggleArr[colCount][rowCount];

void setup() {
    Serial.begin(9600);
    delay(1000);
    monitorLightSetup();
    matrixSetup();

    Serial.write(0x48);
    delay(500);
    Serial.write(0x48);
    delay(500);
}

void monitorLightSetup() {
    pinMode(LED_BUILTIN, OUTPUT);          // set arduino pin to output mode
    FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(rgb, NUM_LEDS);
    FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS, MAX_AMPS);
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.clear();
    FastLED.show();
}

void matrixSetup() {
    for(int x=0; x<rowCount; x++) {
        //Serial.print(rows[x]); Serial.println(" as input-pullup");
        pinMode(rows[x], INPUT);
    }

    for (int x=0; x<colCount; x++) {
        //Serial.print(cols[x]); Serial.println(" as input");
        pinMode(cols[x], INPUT_PULLUP);
    }

    for (int rowIndex=0; rowIndex<rowCount; rowIndex++) {
      for (int colIndex=0; colIndex<colCount; colIndex++) {
        lastButtonStateArr[colIndex][rowIndex] = 1;
      }
    }

}

void readMatrix() {
    // iterate the rows
    for (int rowIndex=0; rowIndex < rowCount; rowIndex++) {
        byte curRow = rows[rowIndex];
        pinMode(curRow, OUTPUT);
        digitalWrite(curRow, LOW);

        for (int colIndex=0; colIndex < colCount; colIndex++) {

            byte curCol = cols[colIndex];
            pinMode(curCol, INPUT_PULLUP);
            keys[colIndex][rowIndex] = digitalRead(curCol);
            pinMode(curCol, INPUT);
        }
        // disable the row
        digitalWrite(curRow, HIGH);
        pinMode(curRow, INPUT);
    }
}

void printMatrix() {
    for (int rowIndex=0; rowIndex < rowCount; rowIndex++) {
        if (rowIndex < 10)
            Serial.print(F("0"));
        Serial.print(rowIndex); Serial.print(F(": "));

        for (int colIndex=0; colIndex < colCount; colIndex++) {
              Serial.print(buttonStateArr[colIndex][rowIndex]);
            if (colIndex < colCount)
                Serial.print(F(", "));
        }
        Serial.println("");
        // Serial.print(monitorLight);
    }
    //Serial.println("");
}


void printStates() {
    for (int rowIndex=0; rowIndex < rowCount; rowIndex++) {
        if (rowIndex < 10)
            Serial.print(F("0"));
        Serial.print(rowIndex); Serial.print(F(": "));

        for (int colIndex=0; colIndex < colCount; colIndex++) {
              Serial.print(toggleArr[colIndex][rowIndex]);
            if (colIndex < colCount)
                Serial.print(F(", "));
        }
        Serial.println("");
        // Serial.print(monitorLight);
    }
    //Serial.println("");
}

void updateStates() {
    //printMatrix();
    if (!buttonStateArr[0][1]) { // Monitor Light Index
        toggleArr[0][1] = !toggleArr[0][1];
    }
    if (!buttonStateArr[1][1]) { // Monitor Light Index
        // Serial.print("Sending H");
        // Serial.print("Sending: ");
        Serial.write(0x48);
        delay(500);
    }

    // Serial.print("Updating States");
    updatePeripherals();
}

void updatePeripherals() {
    monitorLight = toggleArr[0][1];
}

void loop() {
    readMatrix();
    //Serial.print(keys[0][1]);

    // printMatrix();
    // int reading = keys[0][1];
    for (int rowIndex=0; rowIndex < rowCount; rowIndex++) {
        for (int colIndex=0; colIndex < colCount; colIndex++) {
            if (keys[colIndex][rowIndex] != lastButtonStateArr[colIndex][rowIndex]) {
                lastDebounceTimeArr[colIndex][rowIndex] = millis();
            }

            if ((millis() - lastDebounceTimeArr[colIndex][rowIndex]) > debounceDelay) {
                if (keys[colIndex][rowIndex] != buttonStateArr[colIndex][rowIndex]) {
                    buttonStateArr[colIndex][rowIndex] = keys[colIndex][rowIndex];
                    // buttonStateArr[colIndex][rowIndex] = !buttonStateArr[colIndex][rowIndex];


                    // Falling edge check, since ButtonState is 0 when connected
                    if (!buttonStateArr[colIndex][rowIndex]) {
                      updateStates();
                    // Serial.println("Updating states...");
                    // Serial.print(buttonStateArr[colIndex][rowIndex]);
                    // Serial.print(keys[colIndex][rowIndex]);
                    }
                }
            }
            lastButtonStateArr[colIndex][rowIndex] = keys[colIndex][rowIndex];
        }
    }

    if (monitorLight) {
        for (int i=0;i < NUM_LEDS; i++) {
            rgb[i] = CRGB(255, 147, 41);
        }
    } else {
        for (int i=0;i < NUM_LEDS; i++) {
            rgb[i] = CRGB(0, 0, 0);
        }
    }
    // lastButtonState = reading;
    FastLED.show();
}

