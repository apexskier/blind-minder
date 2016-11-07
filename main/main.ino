#include <Wire.h>
#include <Adafruit_ADS1015.h>

#define NUM_BLINDS 2
#define ANALOG_MAX 1024
// TODO
#define SERVO_A_PIN 7
#define SERVO_B_PIN 8
#define SERVO_A_SENSOR_PIN 0
#define SERVO_B_SENSOR_PIN 1
#define LIGHT_SENSOR_PIN 2

#include "blind.h"
#include "light_sensor.h"
#include "routes.h"
#include "wifi.h"

Adafruit_ADS1015 analog_pins;

int read_servo_a_val() {
    analog_pins.readADC_SingleEnded(SERVO_A_SENSOR_PIN);
}
int read_servo_b_val() {
    analog_pins.readADC_SingleEnded(SERVO_B_SENSOR_PIN);
}

Blind blinds[NUM_BLINDS] = {
    Blind(SERVO_A_PIN, read_servo_a_val),
    Blind(SERVO_B_PIN, read_servo_b_val)
};


void setup() {
    Serial.begin(115200);
    Serial.println();
    Serial.println();

    analogWriteRange(ANALOG_MAX);
    // analogReference(EXTERNAL); // ??? make sure this is necessary

    analog_pins.begin();

    wifi_setup();
    routes_setup();

    luminance = light_sensor_read();
}

void loop() {
    int i;

    luminance = light_sensor_read();

    for (i = 0; i <= NUM_BLINDS; i++) {
        blinds[i].loop();
    }
}
