#define TOLERANCE 0.001

Blind::Blind(int pin) {
    this->pin = pin;
    this->servo.attach(pin);
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
    return map(analogRead(this->pin), 0, ANALOG_MAX, -179, 179);
}
