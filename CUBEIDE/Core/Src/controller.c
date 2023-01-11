#include "controller.h"

void controller_init(struct Controller* controller, float MAX_CONTROL_SIGNAL, float MIN_CONTROL_SIGNAL, float START_CONTROL_SIGNAL, float MIN_ERROR, float MAX_ERROR)
{
	controller->MAX_CONTROL_SIGNAL = MAX_CONTROL_SIGNAL;
	controller->MIN_CONTROL_SIGNAL = MIN_CONTROL_SIGNAL;
	controller->START_CONTROL_SIGNAL = START_CONTROL_SIGNAL;
	controller->MIN_ERROR = MIN_ERROR;
	controller->MAX_ERROR = MAX_ERROR;
	controller->previous_error = 0;
}

float controller_control_signal(struct Controller* controller, float error)
{
	//float dot_error = error-controller->previous_error;
	controller->previous_error = error;
	if(error < -controller->MAX_ERROR)
	{
		return controller->MAX_CONTROL_SIGNAL;
	}
	else if(error < -controller->MIN_ERROR)
	{
		return (controller->MAX_CONTROL_SIGNAL/controller->MAX_ERROR)*-error + controller->START_CONTROL_SIGNAL;
	}
	else if(error < controller->MIN_ERROR)
	{
		return 0;
	}
	else if(error < controller->MAX_ERROR)
	{
		return (controller->MAX_CONTROL_SIGNAL/controller->MAX_ERROR)*-error - controller->START_CONTROL_SIGNAL;
	}
	else
	{
		return -controller->MAX_CONTROL_SIGNAL;
	}
}
