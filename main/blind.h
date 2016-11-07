#include <Servo.h>

class Blind {
public:
    int target_angle; // flat, 90 deg => up, -90 deg => down. won't ever go all the way
    bool obstruction_detected;
    bool moving_to_target;

    Blind(int pin);
    void loop();
    void set(int degrees);
    void stop();
    int read();
private:
    Servo servo;
    int pin;
};
