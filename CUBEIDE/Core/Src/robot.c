#include "robot.h"

void robot_init(struct Robot* robot, float position, float orientation){
	robot->previous_position = position;
	robot->position = position;
	robot->orientation = orientation;
	robot->previous_orientation = orientation;
	robot->linear_velocity = 0;
	robot->angular_velocity = 0;
}

void robot_linear_update(struct Robot* robot, float measurement, float time){
	robot->position = measurement;
	robot->linear_velocity = (robot->position - robot->previous_position )/time;
	robot->previous_position = robot->position;
}

void robot_angular_update(struct Robot* robot, float measurement, float time){
	robot->orientation = measurement;
	robot->angular_velocity = (robot->previous_orientation - robot->orientation)/time;
	robot->previous_orientation = robot->orientation;
}

void robot_set_end_position(struct Robot* robot, float end_position)
{
	robot->end_position = end_position;
}

float robot_error(struct Robot* robot)
{
	return robot->end_position - robot->position;
}
