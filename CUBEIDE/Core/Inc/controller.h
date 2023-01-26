/*
 * controller.h
 *
 *  Created on: 7 sty 2023
 *      Author: Nikod
 */

#ifndef INC_CONTROLLER_H_
#define INC_CONTROLLER_H_

#include "main.h"

struct Controller{
	float MAX_CONTROL_SIGNAL;
	float MIN_CONTROL_SIGNAL;
	float START_CONTROL_SIGNAL;
	float MIN_ERROR;
	float MAX_ERROR;
	float previous_error;
	float ki;
	float kp;
	float integrator;
};

void controller_init(struct Controller* controller, float MAX_CONTROL_SIGNAL, float MIN_CONTROL_SIGNAL, float START_CONTROL_SIGNAL,
		float MIN_ERROR, float MAX_ERROR, float ki, float kp);
float controller_control_signal(struct Controller* controller, float error);


#endif /* INC_CONTROLLER_H_ */
