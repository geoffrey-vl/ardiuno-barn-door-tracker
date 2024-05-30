#include "SpeedControl.h"
#include <math.h>
#include <Arduino.h>

const double EARTH_ROTATION_SPEED = 0.250684477; // degrees/min

SpeedControl::SpeedControl(double distance_hinge_to_rod, double rod_pitch, int stepper_steps_per_rotation, SpeedControlResult results[], int results_size) :
    _distance_hinge_to_rod(distance_hinge_to_rod),
    _rod_pitch(rod_pitch),
    _stepper_steps_per_rotation(stepper_steps_per_rotation),
    _results(results),
    _nr_of_results(results_size)
{
}

void SpeedControl::precompute()
{
    // calc 1...end
    for (int i = 0; i < _nr_of_results; i++) {
        double prev_distance_opposite_side = 0;
        if (i != 0) 
            prev_distance_opposite_side = _results[i-1].distance_opposite_side;
        calc_result(_results[i], i+1, prev_distance_opposite_side);
    }
}

void SpeedControl::calc_result(SpeedControlResult& result, int time_since_start, double previous_distance_opposite_side)
{
    result.min_since_start = time_since_start; // minutes
    result.angle = EARTH_ROTATION_SPEED * time_since_start; // degrees
    double tangens_ratio = tan(result.angle * M_PI / 180);
    result.distance_opposite_side = _distance_hinge_to_rod * tangens_ratio; // meter
    double speed = (result.distance_opposite_side - previous_distance_opposite_side) * 1000; // mm/min
    result.rotating_speed_rod = speed / _rod_pitch; // rotations per minute
    result.steps_per_minute = int(result.rotating_speed_rod * _stepper_steps_per_rotation);

    result.print();
}

void SpeedControlResult::print()
{
    Serial.print("Time: ");
    Serial.print(this->min_since_start);
    Serial.print(", angle: ");
    Serial.print(this->angle);
    Serial.print(", opposite side: ");
    Serial.print(this->distance_opposite_side);
    Serial.print(", rotating speed rod: ");
    Serial.print(this->rotating_speed_rod);
    Serial.print(", steps_per_minute: ");
    Serial.println(this->steps_per_minute);
}
