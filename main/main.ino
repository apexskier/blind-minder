#include <Wire.h>
#include <Adafruit_ADS1015.h>

#define NUM_BLINDS 2
#define ANALOG_MAX 1024
#define MAX_DEGREES 120
#define SERVO_A_PIN 12
#define SERVO_B_PIN 13
#define SERVO_A_SENSOR_PIN 0
#define SERVO_B_SENSOR_PIN 1
#define LIGHT_SENSOR_PIN 3

#include "private.h"
#include "blind.h"
#include "light_sensor.h"
#include "routes.h"
#include "wifi.h"


// https://learn.adafruit.com/adafruit-4-channel-adc-breakouts/programming
Adafruit_ADS1015 analog_pins = Adafruit_ADS1015();

int read_servo_a_val() {
    return analog_pins.readADC_SingleEnded(SERVO_A_SENSOR_PIN);
}
int read_servo_b_val() {
    return analog_pins.readADC_SingleEnded(SERVO_B_SENSOR_PIN);
}

Blind blinds[NUM_BLINDS];


void setup() {
    Serial.begin(115200);
    Serial.println("+------------+");
    Serial.println("| booting up |");
    Serial.println("+------------+");

    analogWriteRange(ANALOG_MAX);

    delay(2000); // Wait a moment before starting up

    analog_pins.begin();

    Serial.println();
    blinds[0] = Blind(SERVO_A_PIN, read_servo_a_val, 850, 40, 2500, 500);
    blinds[1] = Blind(SERVO_B_PIN, read_servo_b_val, 850, 40, 2500, 500);

    wifi_setup();
    routes_setup();

    luminance = light_sensor_read();
}

void loop() {
    int i;

    luminance = light_sensor_read();

    routes_loop();

    for (i = 0; i < NUM_BLINDS; i++) {
        blinds[i].loop();
    }
}
