#ifndef A4988_H
#define A4988_H

enum class Direction {
    LEFT,
    RIGHT
};

enum class MicrostepResolution {
    FULL,
    HALF
};

class A4988 {
public:
    A4988(int pin_dir, int pin_step, int pin_enable, MicrostepResolution microstep_resolution, int motor_steps_per_resolution);
    void init();
    void step();
    Direction direction();
    void set_direction(Direction dir);
    void enable(bool enable);
    int effective_steps_per_resolution();
    MicrostepResolution microstepping_resolution();

private:
    int _pin_dir;
    int _pin_step;
    int _pin_enable;
    Direction _direction;
    MicrostepResolution _microstepping_resolution;
    int _motor_steps_per_resolution;

    void set_dir_pin();
};


#endif //A4988
