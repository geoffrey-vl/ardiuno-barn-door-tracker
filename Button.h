#ifndef BUTTON_H
#define BUTTON_H

class Button {
public:
    Button(int pin);

    bool is_pressed();
    void wait_for_release();
private:
    int _pin;
};

#endif // BUTTON_H
