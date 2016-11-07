#include <Servo.h>

typedef int (*read_servo_val_func) ();

class Blind {
public:
    int target_angle; // flat, 90 deg => up, -90 deg => down. won't ever go all the way
    bool obstruction_detected;
    bool moving_to_target;

    Blind(int servo_pin, read_servo_val_func read_func);
    void loop();
    void set(int degrees);
    void stop();
    int read();
private:
    Servo servo;
    int servo_pin;
    read_servo_val_func read_func;
};
