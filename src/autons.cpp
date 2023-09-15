#include "display/lv_misc/lv_task.h"
#include "main.h"

///@brief autonomous function for testing
void auton_lemLibTest(){

}

///@brief placehold autonomous function
void placeholder(){

}

///@brief autonomous function for red right
void auton_redRight(){

chassis.setPose({95.4, -133.585, 340});
chassis.turnTo(60, 0, 1, true,false, 117,true);
chassis.moveTo(60, 0, 1, true, false, 117, true);
// chassis.waitUntilDist(10);
intakeUntilObject();
chassis.moveTo(60, -25, 1, true, false, 117, true);





}

void drive_test(){
    chassis.setPose({0,0,0});
    chassis.moveTo(10,0,0,1000,false,true, 0, true,127,true);

}