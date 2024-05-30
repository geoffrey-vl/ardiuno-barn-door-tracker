#ifndef A4988_H
#define A4988_H

enum class Direction {
    LEFT,
    RIGHT
};

class A4988 {
public:
    A4988(int pin_dir, int pin_step, int pin_enable, int steps_per_revolution);
    void init();
    void step();
    Direction direction();
    void set_direction(Direction dir);
    void enable(bool enable);
    int steps_per_resolution();

private:
    int _pin_dir;
    int _pin_step;
    int _pin_enable;
    Direction _direction;
    int _steps_per_resolution;

    void set_dir_pin();
};


#endif //A4988
