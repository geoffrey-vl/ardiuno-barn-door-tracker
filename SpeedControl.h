#ifndef SPEED_CONTROL_H
#define SPEED_CONTROL_H

struct SpeedControlResult {
    int min_since_start; // minutes
    double angle; // degrees
    double distance_opposite_side; // meter
    double rotating_speed_rod; // rotations per minute
    int steps_per_minute; // the number of steps the stepper motor needs to take within a minute

    void print();
};

class SpeedControl {
public:
    SpeedControl(double distance_hinge_to_rod, double rod_pitch, int stepper_steps_per_rotation, SpeedControlResult results[], int nr_of_results);
    void precompute();
    
private:
    double _distance_hinge_to_rod;
    double _rod_pitch;
    int _stepper_steps_per_rotation;
    SpeedControlResult* _results;
    int _nr_of_results;

    void calc_result(SpeedControlResult& result, int time_since_start, double previous_distance_opposite_side);
    void print_result(SpeedControlResult& result);
};


#endif // SPEED_CONTROL_H
