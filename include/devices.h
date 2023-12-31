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
extern void wingValve(bool state);
extern void intake(bool state);
extern void intakeUntilObject();



extern void ButtonY_Callback();
extern void ButtonA_Callback();
extern void ButtonB_Callback();
extern void ButtonX_Callback();
extern void ButtonUp_Callback();

extern lemlib::Chassis chassis;