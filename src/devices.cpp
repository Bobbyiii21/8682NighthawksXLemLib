#include "main.h"


using namespace pros;

// controller definition

Controller master(E_CONTROLLER_MASTER);

// motors

Motor FrontRight = Motor(17, MOTOR_GEARSET_06, false);
Motor MiddleRight = Motor(16, MOTOR_GEARSET_06, false);
Motor BackRight = Motor(15, MOTOR_GEARSET_06, false);
Motor_Group rightDrive = Motor_Group({FrontRight, MiddleRight, BackRight});
Motor FrontLeft = Motor(18, MOTOR_GEARSET_06, true);
Motor MiddleLeft = Motor(19, MOTOR_GEARSET_06, true);
Motor BackLeft = Motor(20, MOTOR_GEARSET_06, true);
Motor_Group leftDrive = Motor_Group({FrontLeft, MiddleLeft, BackLeft});
Motor cataMotor = Motor(14, MOTOR_GEARSET_36, true);
Motor intakeMotor = Motor(10, MOTOR_GEARSET_18, true);
ADIDigitalOut intake_air('H');
ADIDigitalOut rear_jack('G');
ADIDigitalOut left_wing('F');
ADIDigitalOut right_wing('E');

// sensors

IMU inertial(12);
Distance distance(13);

// pneumaic functions
void wingValve(bool state) {
  left_wing.set_value(state);
  right_wing.set_value(state);
}

// callback functions

bool left_wing_toggle = false;
bool right_wing_toggle = false;
bool rear_jack_toggle = false;
bool intake_toggle = false;

void ButtonY_Callback() {
  left_wing_toggle = !left_wing_toggle;
  left_wing.set_value(left_wing_toggle);
}
void ButtonA_Callback() {
  right_wing_toggle = !right_wing_toggle;
  right_wing.set_value(right_wing_toggle);
}
void ButtonB_Callback() {
  rear_jack_toggle = !rear_jack_toggle;
  rear_jack.set_value(rear_jack_toggle);
}
void ButtonX_Callback() {
  intake_toggle = !intake_toggle;
  intake_air.set_value(intake_toggle);
}
void ButtonUp_Callback() {
  right_wing_toggle = !right_wing_toggle;
  left_wing_toggle = right_wing_toggle;
  right_wing.set_value(right_wing_toggle);
  left_wing.set_value(left_wing_toggle);
}
