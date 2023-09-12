#include "api.h"
#include "lemlib/api.hpp"

extern pros::Controller master;
extern pros::Motor FrontRight;
extern pros::Motor MiddleRight;
extern pros::Motor BackRight;
extern pros::Motor_Group rightDrive;
extern pros::Motor FrontLeft;
extern pros::Motor MiddleLeft;
extern pros::Motor BackLeft;
extern pros::Motor_Group leftDrive;
extern pros::Motor cataMotor;
extern pros::Motor intakeMotor;
extern pros::ADIDigitalOut intake_air;
extern pros::ADIDigitalOut rear_jack;
extern pros::ADIDigitalOut left_wing;
extern pros::ADIDigitalOut right_wing;
extern pros::IMU inertial;
extern pros::Distance distance;

extern bool left_wing_toggle;
extern bool right_wing_toggle;
extern bool rear_jack_toggle;
extern bool intake_toggle;

void ButtonY_Callback();
void ButtonA_Callback();
void ButtonB_Callback();
void ButtonX_Callback();
void ButtonUp_Callback();

