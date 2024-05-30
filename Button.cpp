#include "Button.h"
#include <Arduino.h>

Button::Button(int pin) :
    _pin(pin)
{
}

bool Button::is_pressed()
{
    return (bool) digitalRead(_pin);
}

void Button::wait_for_release()
{
    while (true) {
        delay(10); // 10ms after down
        if (!is_pressed()) {
            delay(10); // 10ms after down
            return;
        }
    }
}
