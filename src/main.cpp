#include "main.h"
#include "autoSelect/selection.h"
#include "pros/misc.h"

using selector::auton;

int competitionSet = 0;

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
/* void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
} */

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize()
{
	// pros::lcd::initialize();
	// pros::lcd::set_text(1, "Hello PROS User!");

	// pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() { selector::init(); }

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous()
{
	switch (auton)
	{
	case -2: // Blue Right
		/* code */
		break;
	case -1: // Blue Left
		/* code */
		break;
	case 0: // Skills
		drive_test();
		break;
	case 1: // Red Left
		/* code */
		break;
	case 2: // Red Right
		auton_redRight();
		break;
	default:
		break;
	}
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol()
{
	int cata_mode = 0;
	int piston_mode = 0;
	// For Bottom PID
	double bottom_kp = 0.7;
	double bottom_ki = 0.1;
	double bottom_kd = 0.5;
	double bottom_derivative;
	double bottom_previous_error = 0;
	double bottom_integral = 0;
	double bottom_integral_limit = 35;
	double bottom_max_pct = 115;
	double bottom_error;
	// For Top PID
	double top_kp = 0.2;
	double top_ki = 0.15;
	double top_kd = 0.1;
	double top_derivative;
	double top_previous_error = 0;
	double top_integral = 0;
	double top_integral_limit = 35;
	double top_max_pct = 100;

	while (true)
	{
		// controller callbacks for wing and jack
		if (master.get_digital_new_press(DIGITAL_Y))
			ButtonY_Callback();
		if (master.get_digital_new_press(DIGITAL_A))
			ButtonA_Callback();
		if (master.get_digital_new_press(DIGITAL_B))
			ButtonB_Callback();
		if (master.get_digital_new_press(DIGITAL_X))
			ButtonX_Callback();
		if (master.get_digital_new_press(DIGITAL_UP))
			ButtonUp_Callback();

		// int leftDriveSpeed = master.get_analog(ANALOG_LEFT_Y) + master.get_analog(ANALOG_RIGHT_X);
		// int rightDriveSpeed = master.get_analog(ANALOG_LEFT_Y) - master.get_analog(ANALOG_RIGHT_X);

		// int deadband = 5;

		// if (abs(leftDriveSpeed) < deadband)
		// {
		// 	leftDriveSpeed = 0;
		// }
		// if (abs(rightDriveSpeed) < deadband)
		// {
		// 	rightDriveSpeed = 0;
		// }

		// leftDrive.move(leftDriveSpeed);
		// rightDrive.move(rightDriveSpeed);

		chassis.arcade(master.get_analog(ANALOG_LEFT_Y), master.get_analog(ANALOG_RIGHT_X));

		// intake

		if (master.get_digital(DIGITAL_L1))
		{
			intakeMotor.move_velocity(400);
		}
		else if (master.get_digital(DIGITAL_L2))
		{
			intakeMotor.move_velocity(-600);
		}
		else
		{
			intakeMotor.move_velocity(0);
		}

		// fun stuff

		switch (cata_mode)
		{
		case 0:
			if (master.get_digital(DIGITAL_R1))
			{
				cataMotor.move_velocity(100);
			}
			else if (master.get_digital(DIGITAL_DOWN))
			{
				cata_mode = 1;
			}
			else if (master.get_digital(DIGITAL_RIGHT))
			{
				cata_mode = 2;
			}
			else
			{
				cataMotor.brake();
				printf("R1 not pressed\n");
			}
			break;
		case 1:

			if (master.get_digital(DIGITAL_R1))
			{
				cataMotor.move_velocity(100 * .25);
			}
			else if (master.get_digital(DIGITAL_R2))
			{
				cata_mode = 0;
			}
			else if (master.get_digital(DIGITAL_RIGHT))
			{
				cata_mode = 2;
			}
			else if (distance.get() > 80)
			{

				bottom_error = 80 - distance.get();

				if (fabs(bottom_error) <= 8)
				{
					double bottom_previous_error = 0;
					double bottom_integral = 0;
					cataMotor.brake();
					break;
				}

				bottom_derivative = bottom_error - bottom_previous_error;
				bottom_integral = bottom_integral + bottom_ki * bottom_error;
				if (bottom_integral > bottom_integral_limit)
					bottom_integral = bottom_integral_limit;
				if (bottom_integral < -bottom_integral_limit)
					bottom_integral = -bottom_integral_limit;

				double speed = (bottom_kp * bottom_error) + (bottom_ki * bottom_integral) + (bottom_kd * bottom_derivative);
				// clamp max speed to 100
				if (speed > bottom_max_pct)
					speed = bottom_max_pct;
				if (speed < -bottom_max_pct)
					speed = -bottom_max_pct;

				// cataMotor.spin(reverse, speed, percent);
				cataMotor.move(-(127 * (speed / 100)));

				bottom_previous_error = bottom_error;

				printf("Error: %f\n", bottom_error);

				// int cata_height =  Distance.objectDistance(mm);

				// if (cata_height > 60) {
				//   cata_height = 60;
				// }

				// cataMotor.spin(forward,exponential(cata_height, 125, 2.7), percent);
				//         printf("Distance: %f\n", Distance.objectDistance(mm));
			}
			else
			{
				cataMotor.brake();
				printf("Target reached\n");
			}
			break;

		case 2:

			if (master.get_digital(DIGITAL_R1))
			{
				cataMotor.move(127 * .25);
			}

			else if (master.get_digital(DIGITAL_R2))
			{
				cata_mode = 0;
			}
			else if (master.get_digital(DIGITAL_DOWN))
			{
				cata_mode = 1;
			}
			else if (distance.get() < 125 || distance.get() > 130)
			{
				double error = 128 - distance.get();

				if (fabs(error) < 10)
				{
					double top_previous_error = 0;
					double top_integral = 0;
					break;
				}

				top_derivative = error - top_previous_error;
				top_integral = top_integral + top_ki * error;
				if (top_integral > top_integral_limit)
					top_integral = top_integral_limit;
				if (top_integral < -top_integral_limit)
					top_integral = -top_integral_limit;

				double speed = (top_kp * error) + (top_ki * top_integral) + (top_kd * top_derivative);
				// clamp max speed to 100
				if (speed > top_max_pct)
					speed = top_max_pct;
				if (speed < -top_max_pct)
					speed = -top_max_pct;

				// cataMotor.spin(reverse, speed, percent);
				cataMotor.move(-(127 * (speed / 100)));
				top_previous_error = error;

				// // int cata_height =  Distance.objectDistance(mm);

				// // if (cata_height < 60) {
				// //   cata_height = 60;
				// // }
				// double cata_height =  Distance.objectDistance(mm);

				//  cataMotor.spin(forward, exponential(cata_height, 100, 2.8), percent);
				printf("Distance: %d\n", distance.get());
			}
			else
			{
				cataMotor.brake();
				printf("Target reached\n");
			}
			break;
		}

		pros::delay(20);
	}
}
