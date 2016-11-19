#include <Servo.h>

typedef int (*read_servo_val_func) ();

enum ServoStatus { positive, negative, stopped };

class Blind {
public:
    int target_angle; // flat, 90 deg => up, -90 deg => down. won't ever go all the way
    bool obstruction_detected;
    ServoStatus status;

    Blind();
    Blind(int servo_pin, read_servo_val_func read_func);
    Blind(int servo_pin, read_servo_val_func read_func, int max_read, int min_read, int max_pos, int min_pos);
    String get_status();
    void loop();
    bool seek(int degrees);
    void stop();
    int read();
private:
    Servo servo;
    int write_servo_pin;
    read_servo_val_func read_func;

    int max_read;
    int min_read;
    int max_pos;
    int min_pos;

    void calibrate();
    void print_status();
};
