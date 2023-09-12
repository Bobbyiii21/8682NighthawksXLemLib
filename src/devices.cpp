#include "main.h"
#include <cstddef>


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

//LemLib configuaration
///@todo #4 Measure the Robot

lemlib::Drivetrain_t drivetrain {
	// Left MotorGroup
	&leftDrive,
	// Right MotorGroup 
	&rightDrive,
	//Track Width
	11,
	//Wheel diameter
	3.25,
	//Wheel RPM
	360
};

lemlib::TrackingWheel leftTracker{
	&leftDrive,
	3.25,
	5.5,
	360
};

lemlib::TrackingWheel rightTracker{
	&rightDrive,
	3.25,
	5.5,
	360
};

lemlib::ChassisController_t lateralController{
	//Kp
	50,
	//Kd
	20,
	//Small Error
	1,
	//Small Error Timeout
	100,
	//Large Error
	3,
	//Large Error Timeout
	500,
	//Slew
	20
};

lemlib::ChassisController_t angularController {
	//Kp
	10,
	//Kd
	30,
	//Small Error
	1,
	//Small Error Timeout
	100,
	//Large Error
	3,
	//Large Error Timeout
	500,
	//Slew
	20
};

lemlib::OdomSensors_t sensors{
	&leftTracker,
	&rightTracker,
	nullptr,
	nullptr,
	&inertial
};

lemlib::Chassis chassis(drivetrain,lateralController,angularController,sensors);
