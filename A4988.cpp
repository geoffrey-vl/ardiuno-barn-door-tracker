#include "A4988.h"
#include <Arduino.h>

const int STEP_INTERVAL = 1;

A4988::A4988(int pin_dir, int pin_step, int pin_enable, int steps_per_resolution) :
    _pin_dir(pin_dir),
    _pin_step(pin_step),
    _pin_enable(pin_enable),
    _direction(Direction::LEFT),
    _steps_per_resolution(steps_per_resolution)
{
}

void A4988::init()
{
    pinMode(_pin_dir, OUTPUT);
    pinMode(_pin_step, OUTPUT);
    pinMode(_pin_enable, OUTPUT);

    // disable stepper board
    enable(false);

    set_dir_pin();
}

int A4988::steps_per_resolution()
{
    return _steps_per_resolution;
}

void A4988::step()
{
    digitalWrite(_pin_step, LOW);
    delayMicroseconds(STEP_INTERVAL);
    digitalWrite(_pin_step, HIGH);
    delayMicroseconds(STEP_INTERVAL);
}

Direction A4988::direction()
{
    return _direction;
}

void A4988::set_direction(Direction dir)
{
    if (_direction == dir) return;
    _direction = dir;
    set_dir_pin();
}

void A4988::enable(bool enable)
{
    digitalWrite(_pin_enable, enable ? LOW : HIGH);
}

void A4988::set_dir_pin()
{
    if (_direction == Direction::LEFT)
        digitalWrite(_pin_dir, LOW);
    else
        digitalWrite(_pin_dir, HIGH);
}
