#include "controller.h"

void controller_init(struct Controller* controller, float MAX_CONTROL_SIGNAL, float MIN_CONTROL_SIGNAL, float START_CONTROL_SIGNAL, float MIN_ERROR,
		float MAX_ERROR, float ki, float kp)
{
	controller->MAX_CONTROL_SIGNAL = MAX_CONTROL_SIGNAL;
	controller->MIN_CONTROL_SIGNAL = MIN_CONTROL_SIGNAL;
	controller->START_CONTROL_SIGNAL = START_CONTROL_SIGNAL;
	controller->MIN_ERROR = MIN_ERROR;
	controller->MAX_ERROR = MAX_ERROR;
	controller->previous_error = 0;
	controller->ki = ki;
	controller->kp = kp;
	controller->integrator = 0;
}

//float controller_control_signal(struct Controller* controller, float error)
//{
//	controller->previous_error = error;
//	if(error < -controller->MAX_ERROR)
//	{
//		return controller->MAX_CONTROL_SIGNAL;
//	}
//	else if(error < -controller->MIN_ERROR)
//	{
//		return (controller->MAX_CONTROL_SIGNAL/controller->MAX_ERROR)*-error + controller->START_CONTROL_SIGNAL;
//	}
//	else if(error < controller->MIN_ERROR)
//	{
//		return 0;
//	}
//	else if(error < controller->MAX_ERROR)
//	{
//		return (controller->MAX_CONTROL_SIGNAL/controller->MAX_ERROR)*-error - controller->START_CONTROL_SIGNAL;
//	}
//	else
//	{
//		return -controller->MAX_CONTROL_SIGNAL;
//	}
//}

float controller_control_signal(struct Controller* controller, float error)
{
	float sterowanie = 0;
	static float calka = 0;
	controller->previous_error = error;
	if(error < -controller->MAX_ERROR)
	{
		sterowanie += controller->MAX_CONTROL_SIGNAL - controller->START_CONTROL_SIGNAL;
	}
	else if(error < -controller->MIN_ERROR)
	{
		sterowanie += (controller->MAX_CONTROL_SIGNAL/controller->MAX_ERROR)*-error;
	}
	else if(error < controller->MIN_ERROR)
	{
		sterowanie += 0;
	}
	else if(error < controller->MAX_ERROR)
	{
		sterowanie += (controller->MAX_CONTROL_SIGNAL/controller->MAX_ERROR)*-error;
	}
	else
	{
		sterowanie += -controller->MAX_CONTROL_SIGNAL + controller->START_CONTROL_SIGNAL;
	}
	calka -= controller->ki*error;
	if(calka > controller->START_CONTROL_SIGNAL)
	{
		calka = controller->START_CONTROL_SIGNAL;
	}
	else if(calka < -controller->START_CONTROL_SIGNAL)
	{
		calka = -controller->START_CONTROL_SIGNAL;
	}
	sterowanie += calka;

	if(error > -controller->MIN_ERROR && error < controller->MIN_ERROR)
	{
		sterowanie = 0;
	}

	return sterowanie;

}

//float controller_control_signal(struct Controller* controller, float error)
//{
//	//człon proporcjonalny
//	float P = controller->kp * error;
//
//	//człon całkujący
//	controller->integrator += 0.5f * controller->ki * (error + controller->previous_error);
//
//
//	//STEROWANIE
//	float sterowanie = P + controller->integrator;
//
//	if(sterowanie > controller->MAX_CONTROL_SIGNAL)
//	{
//		sterowanie = controller->MAX_CONTROL_SIGNAL;
//	}
//	else if(sterowanie < controller->MIN_CONTROL_SIGNAL)
//	{
//		sterowanie = controller->MIN_CONTROL_SIGNAL;
//	}
//
//	//poprzedni uchyb
//	controller->previous_error = error;
//
//	//dead bounds
//	if(error < controller->MIN_ERROR && error > -controller->MIN_ERROR)
//	{
//		sterowanie = 0;
//	}
//
//	return -sterowanie;
//}
