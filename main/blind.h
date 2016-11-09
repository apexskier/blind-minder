#include <Servo.h>

typedef int (*read_servo_val_func) ();

class Blind {
public:
    int target_angle; // flat, 90 deg => up, -90 deg => down. won't ever go all the way
    bool obstruction_detected;
    bool moving_to_target;

    Blind();
    Blind(int servo_pin, read_servo_val_func read_func);
    Blind(int servo_pin, read_servo_val_func read_func, int max_read, int min_read, int max_pos, int min_pos);
    void loop();
    void seek(int degrees);
    void stop();
    int read();
private:
    Servo servo;
    int servo_pin;
    read_servo_val_func read_func;

    int max_read;
    int min_read;
    int max_pos;
    int min_pos;

    void calibrate();
    void print_status();
};
