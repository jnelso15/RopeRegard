#include <Arduino.h>
bool ledMode = false;
void setup() {
    pinMode(LED_BUILTIN, OUTPUT);

}

void loop() {
    
    ledMode = !ledMode;

    digitalWrite(LED_BUILTIN, ledMode);
    delay(500);
}