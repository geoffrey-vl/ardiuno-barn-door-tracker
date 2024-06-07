#include "A4988.h"
#include "MotorShieldPinout.h"
#include "SpeedControl.h"
#include "Button.h"
#include <math.h>

// WANTAI_42BYGHW811
const int MOTOR_FULL_STEP_STEPS_PER_RESOLUTION = 200;

const double DISTANCE_HINGE_TO_PLANK = 0.012; // meter
const double DISTANCE_HINGE_TO_ROD = 0.317 + DISTANCE_HINGE_TO_PLANK; // meter
const double ROD_PITCH = 0.8; // mm, M5 see https://fullerfasteners.com/tech/basic-metric-thread-chart-m1-m100-2/

const int MAX_TRACK_MINUTES = 45;
SpeedControlResult speeds[MAX_TRACK_MINUTES] = {};

A4988 motor(PIN_DIR_MOTOR_C, PIN_STEP_MOTOR_C, PIN_ENABLE_MOTOR_C, MicrostepResolution::HALF, MOTOR_FULL_STEP_STEPS_PER_RESOLUTION);
SpeedControl speed_ctrl(DISTANCE_HINGE_TO_ROD, ROD_PITCH, motor.effective_steps_per_resolution(), speeds, MAX_TRACK_MINUTES);

Button sw1(SW1);
Button sw5(SW5);
Button sw6(SW6);

int steps_taken = 0;
int high_speed_delay_ms = 0;

void setup()
{
    Serial.begin(115200);
    Serial.println("Setup");
    motor.init();

    Serial.print("AP4988 effective steps per resolution: ");
    Serial.println(motor.effective_steps_per_resolution());

    pinMode(SW1, INPUT);
    pinMode(SW5, INPUT);
    pinMode(SW6, INPUT);

    speed_ctrl.precompute();

    // don't go full speed
    high_speed_delay_ms = 10;
    if (motor.microstepping_resolution() == MicrostepResolution::HALF) {
        high_speed_delay_ms = 5;
    }

    motor.enable(true);
    motor.set_direction(Direction::LEFT);
    Serial.println("Ready");
}

void toggleDirection()
{
    Direction dir = motor.direction();
    if (dir == Direction::LEFT) dir = Direction::RIGHT;
    else dir = Direction::LEFT;
    Serial.print("Toggling direction: ");
    Serial.println(dir == Direction::LEFT ? "left" : "right");
    motor.set_direction(dir);
}

void print_tracker_position(int minutes_passed, int steps_to_do, int step_timespan)
{
    Serial.print("MIN: ");
    Serial.print(minutes_passed);
    Serial.print(", STEPS: ");
    Serial.print(steps_to_do);
    Serial.print(", SPAN: ");
    Serial.println(step_timespan);
}

void compensate_earth()
{
    /**
     * Compensate Earth's rotation.
     * Known eror:
     * We're at roughly 2.65 rotations per minute for the rod
     * 2.625178456 * 200 = 525,0356912 steps per minute, value
     * slightly differantiates when time increases.
     * The error is 0.0356912 as we can not take those half steps;
     * so after less than 30 minutes the error has build up to approx 1
     * full step that we didn't take, or maybe 2. 
     * Test if this is acceptable.
     */
    Serial.println("Compensating Earth");
    const unsigned long MILLIS_PER_MINUTE = 60 * 1000UL;
    int minutes_passed = 0;
    steps_taken = 0;
    int steps_to_do = speeds[minutes_passed].steps_per_minute;
    int step_timespan = MILLIS_PER_MINUTE / steps_to_do;
    unsigned long start_time = millis();
    unsigned long end_of_minute_time = start_time + (MILLIS_PER_MINUTE * (minutes_passed + 1));
    print_tracker_position(minutes_passed, steps_to_do, step_timespan);
    while (true) {
        // check if a minute has passed (recalc if so)
        unsigned long now = millis();
        if (now >= end_of_minute_time) {
            minutes_passed++;
            if (minutes_passed > MAX_TRACK_MINUTES) {
                Serial.println("End reached");
                return;
            }
            steps_to_do = speeds[minutes_passed].steps_per_minute;
            step_timespan = MILLIS_PER_MINUTE / steps_to_do;
            end_of_minute_time = start_time + (MILLIS_PER_MINUTE * (minutes_passed + 1)); // recalc from start time to reduce error deviation over time
            // TODO: above trick doesn't work
            print_tracker_position(minutes_passed, steps_to_do, step_timespan);
        }

        // sleep and step
        delay(step_timespan);
        motor.step();
        steps_taken++;

        // check if user requested to stop
        if (sw1.is_pressed()) {
            sw1.wait_for_release();
            Serial.println("User interrupted");
            return;
        }
    }
}

void rewind()
{
    Serial.println("Rewinding");
    toggleDirection();
    while (steps_taken > 0) {
        if (steps_taken % 100 == 0) {
            Serial.println(steps_taken/100);
        }
        motor.step();
        delay(high_speed_delay_ms);
        steps_taken--;
    }
    Serial.println("");
    toggleDirection();
}

void wait_for_user_confirm()
{
    while (true) {
        // tracker reached end, wait for user to command 'rewind'
        delay(10);
        if (sw1.is_pressed()) {
            sw1.wait_for_release();
            return;
        }
    }
}

void manual_motor_control(Button& btn, Direction dir)
{
    Serial.print("Manual drive: ");
    Serial.println(dir == Direction::LEFT ? "left" : "right");
    motor.set_direction(dir);
    while (btn.is_pressed()) {
        motor.step();
        delay(high_speed_delay_ms);
    }
    Serial.println("Ready again");
}

void loop()
{
    if (sw1.is_pressed()) {
        sw1.wait_for_release();
        compensate_earth();
        wait_for_user_confirm();
        rewind();
        Serial.println("Ready again");
    }
    if (sw6.is_pressed()) {
        manual_motor_control(sw6, Direction::LEFT);
    }
    if (sw5.is_pressed()) {
        manual_motor_control(sw5, Direction::RIGHT);
    }
}
