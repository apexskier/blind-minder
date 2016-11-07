// https://learn.adafruit.com/adafruit-ga1a12s202-log-scale-analog-light-sensor/use-it

#define LOG_RANGE 5.0 // 3.3v = 10^5 lux

float light_sensor_read() {
    int raw = analog_pins.readADC_SingleEnded(LIGHT_SENSOR_PIN);
    float log_lux = raw * LOG_RANGE / ANALOG_MAX;
    return pow(10, log_lux);
}
