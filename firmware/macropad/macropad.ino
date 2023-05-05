#include <FastLED.h>
#define NUM_LEDS 22 //19
#define LED_PIN 0
#define COLOR_ORDER GRB
#define CHIPSET WS2812B
#define BRIGHTNESS 200
#define VOLTS 5
#define MAX_AMPS 500

const unsigned long debounceDelay = 40;    // the debounce time; increase if the output flickers

CRGB rgb[NUM_LEDS];
int monitorLight = true;

byte rows[] = {14,16};
const int rowCount = sizeof(rows)/sizeof(rows[0]);

// JP2 and JP3 are outputs
byte cols[] = {12,13,2};
const int colCount = sizeof(cols)/sizeof(cols[0]);

byte keys[colCount][rowCount];

unsigned long lastDebounceTimeArr[colCount][rowCount];
byte lastButtonStateArr[colCount][rowCount];
byte buttonStateArr[colCount][rowCount];

void setup() {
    Serial.begin(9600);
    monitorLightSetup();
    matrixSetup();
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
        Serial.print(rows[x]); Serial.println(" as input-pullup");
        pinMode(rows[x], INPUT);
    }

    for (int x=0; x<colCount; x++) {
        Serial.print(cols[x]); Serial.println(" as input");
        pinMode(cols[x], INPUT_PULLUP);
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
            Serial.print(keys[colIndex][rowIndex]);
            if (colIndex < colCount)
                Serial.print(F(", "));
        }
        Serial.println("");
        Serial.print(monitorLight);
    }
    Serial.println("");
}

void updateStates() {
    if (!buttonStateArr[0][1]) {
        monitorLight = !monitorLight;
    }

}

void loop() {
    readMatrix();
    Serial.print(keys[0][1]);

    // int reading = keys[0][1];
    for (int rowIndex=0; rowIndex < rowCount; rowIndex++) {
        for (int colIndex=0; colIndex < colCount; colIndex++) {
            if (keys[colIndex][rowIndex] != lastButtonStateArr[colIndex][rowIndex]) {
                lastDebounceTimeArr[colIndex][rowIndex] = millis();
            }

            if ((millis() - lastDebounceTimeArr[colIndex][rowIndex]) > debounceDelay) {
                if (keys[colIndex][rowIndex] != buttonStateArr[colIndex][rowIndex]) {
                    buttonStateArr[colIndex][rowIndex] = keys[colIndex][rowIndex];
                    if (buttonStateArr[colIndex][rowIndex]) {
                    updateStates();
                    Serial.println("Updating states...");
                    Serial.print(buttonStateArr[colIndex][rowIndex]);
                    Serial.print(keys[colIndex][rowIndex]);
                    }
                }
            }
            lastButtonStateArr[colIndex][rowIndex] = keys[colIndex][rowIndex];
        }
    }



    // for (int rowIndex=0; rowIndex < rowCount; rowIndex++) {
    //     for (int colIndex=0; colIndex < colCount; colIndex++) {
    //         if ((millis() - lastDebounceTimeArr[colIndex][rowIndex]) > debounceDelay) {
    //             if (keys[colIndex][rowIndex] != buttonStateArr[colIndex][rowIndex]) {
    //                 buttonStateArr[colIndex][rowIndex] = keys[colIndex][rowIndex];
    //             }
    //         }
    //         lastButtonStateArr[colIndex][rowIndex] = keys[colIndex][rowIndex];

    //     }
    // }

    // if (reading != lastButtonState) {
    //     // reset the debouncing timer
    //     lastDebounceTime = millis();
    // }

    // if ((millis() - lastDebounceTime) > debounceDelay) {
    //     // whatever the reading is at, it's been there for longer than the debounce
    //     // delay, so take it as the actual current state:
    //     Serial.println("In debounce loop");

    //     // if the button state has changed:
    //     if (reading != buttonState) {
    //         buttonState = reading;

    //         Serial.print("New button state");
    //         Serial.println(buttonState);
    //         // only toggle the LED if the new button state is HIGH
    //         if (!buttonState) {
    //             monitorLight = !monitorLight;
    //         }
    //     }
    // }

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

