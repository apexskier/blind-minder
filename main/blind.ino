// https://learn.adafruit.com/analog-feedback-servos/using-feedback

#define TOLERANCE 4
#define SPEED 20 // 90 === no speed

Blind::Blind() {}

Blind::Blind(
    int write_servo_pin,
    read_servo_val_func read_func,
    int max_read,
    int min_read,
    int max_pos,
    int min_pos
) {
    this->write_servo_pin = write_servo_pin;
    this->read_func = read_func;

    this->target_angle = 0;
    this->obstruction_detected = false;
    this->status = stopped;

    this->max_read = max_read;
    this->min_read = min_read;
    this->max_pos = max_pos;
    this->min_pos = min_pos;

    Serial.println(F("Servo initialized"));
    this->print_status();
}


Blind::Blind(int write_servo_pin, read_servo_val_func read_func) {
    this->write_servo_pin = write_servo_pin;
    this->read_func = read_func;

    this->target_angle = 0;
    this->obstruction_detected = false;
    this->status = stopped;

    this->max_read = 850;
    this->min_read = 40;
    this->max_pos = 2150;
    this->min_pos = 10;

    this->calibrate();

    Serial.println(F("Servo initialized"));
    this->print_status();
};

String Blind::get_status() {
    switch (this->status) {
        case positive:
            return "positive";
        case negative:
            return "negative";
        case stopped:
            return "stopped";
    }
}

void Blind::print_status() {
    Serial.print(F("  write pin: ")); Serial.println(this->write_servo_pin);
    Serial.print(F("  initial angle: ")); Serial.println(this->target_angle);
    Serial.print(F("  obstruction detected: ")); Serial.println(this->obstruction_detected ? F("true") : F("false"));
    Serial.print(F("  status: ")); Serial.println(this->get_status());
    Serial.print(F("  current angle: ")); Serial.println(this->read());
}

void Blind::calibrate() {
    int last_read;
    int min_read = 20000;
    int max_read = 0;
    int min_pos = 20000;
    int max_pos = 0;
    bool found_min_pos = false;
    this->servo.attach(this->write_servo_pin, 0, 3000);

    servo.writeMicroseconds(0);
    delay(1000); // make sure it has time to get there and settle

    for (int i = 0; i <= 3000; i += 10) {
        servo.writeMicroseconds(i);
        delay(20);
        int val = this->read_func();
        if (i != 0) {
            if (val > max_read) max_read = val;
            if (val < min_read) min_read = val;
            if (abs(val - last_read) > 10) {
                if (!found_min_pos) {
                    min_pos = i;
                    found_min_pos = true;
                }
                max_pos = i;
            }
        }
        last_read = val;
    }

    Serial.print(F("Blind calibrated ("));
    Serial.print(this->write_servo_pin);
    Serial.println(F(")"));
    Serial.print(F("  min_read: ")); Serial.println(min_read);
    Serial.print(F("  max_read: ")); Serial.println(max_read);
    Serial.print(F("  min_pos: ")); Serial.println(min_pos);
    Serial.print(F("  max_pos: ")); Serial.println(max_pos);

    this->servo.detach();
}

void Blind::loop() {
    if (this->status != stopped) {
        int val = this->read();
        int diff = val - this->target_angle;
        if (abs(diff) < TOLERANCE) {
            this->stop();
        }
    }
}

bool Blind::seek(int deg) {
    if (this->status == stopped) {
        this->target_angle = deg;
        this->servo.attach(this->write_servo_pin);
        int val = this->read();
        if (val < deg) {
            this->status = positive;
            this->servo.write(90 + SPEED);
        } else if (val > deg) {
            this->status = negative;
            this->servo.write(90 - SPEED);
        } // else already at the specified angle
        return true;
    }
    return false;
}

void Blind::stop() {
    this->status = stopped;
    this->servo.detach();
}

int Blind::read() {
    return map(this->read_func(), this->min_read, this->max_read, 0, 120);
}
