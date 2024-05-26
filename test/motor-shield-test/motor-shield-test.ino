

int PIN_DIR_A = 11;
int PIN_STEP_A = 12;
int PIN_ENABLE_A = 13;

int PIN_DIR_B = 8;
int PIN_STEP_B = 9;
int PIN_ENABLE_B = 10;

int PIN_DIR_C = 5;
int PIN_STEP_C = 6;
int PIN_ENABLE_C = 7;

int direction = HIGH;

void setup()
{
    pinMode(PIN_DIR_A, OUTPUT);
    pinMode(PIN_STEP_A, OUTPUT);
    pinMode(PIN_ENABLE_A, OUTPUT);

    pinMode(PIN_DIR_B, OUTPUT);
    pinMode(PIN_STEP_B, OUTPUT);
    pinMode(PIN_ENABLE_B, OUTPUT);

    pinMode(PIN_DIR_C, OUTPUT);
    pinMode(PIN_STEP_C, OUTPUT);
    pinMode(PIN_ENABLE_C, OUTPUT);

    // enable stepper board
    digitalWrite(PIN_ENABLE_A, LOW);
    digitalWrite(PIN_ENABLE_B, LOW);
    digitalWrite(PIN_ENABLE_C, LOW);

    // set direction
    digitalWrite(PIN_DIR_A, direction);
    digitalWrite(PIN_DIR_B, direction);
    digitalWrite(PIN_DIR_C, direction);
}

void step(int pin)
{
    digitalWrite(pin, LOW);
    delayMicroseconds(500);
    digitalWrite(pin, HIGH);
    delayMicroseconds(500);
}

void toggleDirection()
{
    if (direction == LOW) direction = HIGH;
    else direction = LOW;
    digitalWrite(PIN_DIR_A, direction);
    digitalWrite(PIN_DIR_B, direction);
    digitalWrite(PIN_DIR_C, direction);
}

void loop()
{
    delay(3000);
    for (int i = 0; i < 200; i++) {
        delay(100);
        step(PIN_STEP_A);
        step(PIN_STEP_B);
        step(PIN_STEP_C);
    }
    toggleDirection();
}
