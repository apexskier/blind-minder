#define TOLERANCE 0.001

Blind::Blind(int servo_pin, read_servo_val_func read_func) {
    this->servo_pin = servo_pin;
    this->read_func = read_func;

    this->servo.attach(servo_pin);
};

void Blind::loop() {
    int diff = this->read() - this->target_angle;
    if (moving_to_target && (abs(diff) > TOLERANCE)) {
        this->moving_to_target = false;
    }
}

void Blind::set(int degrees) {
    this->target_angle = degrees;
    this->moving_to_target = true;
}

void Blind::stop() {
    this->target_angle = this->read();
    this->moving_to_target = false;
}

int Blind::read() {
    return map(this->read_func(), 0, ANALOG_MAX, -179, 179);
}
