#define NUM_BLINDS 2
#define ANALOG_MAX 1024
#define SERVO_PIN_A 8 // TODO
#define SERVO_PIN_B 9 // TODO

#include "blind.h"
#include "routes.h"
#include "wifi.h"


Blind blinds[NUM_BLINDS] = { Blind(SERVO_PIN_A), Blind(SERVO_PIN_B) };


void setup() {
    Serial.begin(115200);
    Serial.println();
    Serial.println();

    analogWriteRange(ANALOG_MAX);

    wifi_setup();
    routes_setup();
}

void loop() {
    int i;
    for (i = 0; i <= NUM_BLINDS; i++) {
        blinds[i].loop();
    }
}
