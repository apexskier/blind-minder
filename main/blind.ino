// https://learn.adafruit.com/analog-feedback-servos/using-feedback

#define TOLERANCE 2

Blind::Blind() {}

Blind::Blind(int servo_pin, read_servo_val_func read_func, int max_read, int min_read, int max_pos, int min_pos) {
    this->servo_pin = servo_pin;
    this->read_func = read_func;

    this->target_angle = 0;
    this->obstruction_detected = false;
    this->moving_to_target = false;

    this->max_read = max_read;
    this->min_read = min_read;
    this->max_pos = max_pos;
    this->min_pos = min_pos;
    
    Serial.println(F("Servo initialized"));
    this->print_status();
}


Blind::Blind(int servo_pin, read_servo_val_func read_func) {
    this->servo_pin = servo_pin;
    this->read_func = read_func;

    this->target_angle = 0;
    this->obstruction_detected = false;
    this->moving_to_target = false;

    this->max_read = 850;
    this->min_read = 40;
    this->max_pos = 2150;
    this->min_pos = 10;

    this->calibrate();
    
    Serial.println(F("Servo initialized"));
    this->print_status();
};

void Blind::print_status() {
    Serial.print(F("  write pin: ")); Serial.println(this->servo_pin);
    Serial.print(F("  initial angle: ")); Serial.println(this->target_angle);
    Serial.print(F("  obstruction detected: ")); Serial.println(this->obstruction_detected ? F("true") : F("false"));
    Serial.print(F("  moving: ")); Serial.println(this->moving_to_target ? F("true") : F("false"));
    Serial.print(F("  current angle: ")); Serial.println(this->read());
}

void Blind::calibrate() {
    int last_read;
    int min_read = 20000;
    int max_read = 0;
    int min_pos = 20000;
    int max_pos = 0;
    bool found_min_pos = false;
    this->servo.attach(this->servo_pin, 0, 3000);

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
    Serial.print(this->servo_pin);
    Serial.println(F(")"));
    Serial.print(F("  min_read: ")); Serial.println(min_read);
    Serial.print(F("  max_read: ")); Serial.println(max_read);
    Serial.print(F("  min_pos: ")); Serial.println(min_pos);
    Serial.print(F("  max_pos: ")); Serial.println(max_pos);

    this->servo.detach();
}

void Blind::loop() {
    //Serial.println("in blind loop.");
    int val = this->read();
    int diff = val - this->target_angle;
//    if (this->moving_to_target) {
//        Serial.print("in blind loop. READ: ");
//        Serial.println(val);
//    }
    if (this->moving_to_target && (abs(diff) < TOLERANCE)) {
        this->moving_to_target = false;
        this->servo.detach();
    }
}

void Blind::seek(int deg) {
    this->target_angle = deg;
    this->moving_to_target = true;
    this->servo.attach(this->servo_pin, this->min_pos, this->max_pos);
    this->servo.writeMicroseconds(map(deg, 0, 120, this->min_pos, this->max_pos));
    //this->servo.write(deg);
}

void Blind::stop() {
    this->target_angle = this->read();
    this->moving_to_target = false;
    this->servo.detach();
}

int Blind::read() {
    return map(this->read_func(), this->min_read, this->max_read, 0, 120);
}
