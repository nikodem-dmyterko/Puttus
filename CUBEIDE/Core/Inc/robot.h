#ifndef INC_ROBOT_H_
#define INC_ROBOT_H_

#include "main.h"

struct Robot{
	float position;
	float previous_position;
	float orientation;
	float previous_orientation;
	float linear_velocity;
	float angular_velocity;
	float end_position;
};

void robot_init(struct Robot* robot, float position, float orientation);
void robot_linear_update(struct Robot* robot, float measurement, float time);
void robot_angular_update(struct Robot* robot, float mesurement, float time);
void robot_set_end_position(struct Robot* robot, float end_position);
float robot_error(struct Robot* robot);

#endif /* INC_ROBOT_H_ */
