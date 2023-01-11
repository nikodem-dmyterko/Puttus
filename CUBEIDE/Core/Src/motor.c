#include "motor.h"

void motor_init(struct Motor* motor,TIM_HandleTypeDef* htim, uint32_t PWM_CHANNEL, GPIO_TypeDef* FORWARD_PORT, GPIO_TypeDef* BACKWARD_PORT, uint16_t FORWARD_PIN, uint16_t BACKWARD_PIN){
	static _Bool tim = 0;
	motor->duty = 0;
	motor->htim = htim;
	motor->PWM_CHANNEL = PWM_CHANNEL;
	motor->FORWARD_PORT = FORWARD_PORT;
	motor->BACKWARD_PORT = BACKWARD_PORT;
	motor->FORWARD_PIN = FORWARD_PIN;
	motor->BACKWARD_PIN = BACKWARD_PIN;

//	if(!tim)
//	{
//		HAL_TIM_Base_Start_IT(motor->htim);
//		tim = !tim;
//	}

	HAL_TIM_PWM_Start(motor->htim, PWM_CHANNEL);

	motor_stop(motor);
}

void motor_update_duty(struct Motor* motor, float duty)
{
	motor->duty = duty;
	__HAL_TIM_SET_COMPARE(motor->htim, motor->PWM_CHANNEL, abs(duty));
}

void motor_set_direction(struct Motor* motor, _Bool direction)
{
	if(direction)
	{
		HAL_GPIO_WritePin(motor->FORWARD_PORT, motor->FORWARD_PIN, 0);
		HAL_GPIO_WritePin(motor->BACKWARD_PORT, motor->BACKWARD_PIN, 1);
	}
	else
	{
		HAL_GPIO_WritePin(motor->FORWARD_PORT, motor->FORWARD_PIN, 1);
		HAL_GPIO_WritePin(motor->BACKWARD_PORT, motor->BACKWARD_PIN, 0);
	}
}

void motor_stop(struct Motor* motor)
{
	HAL_GPIO_WritePin(motor->FORWARD_PORT, motor->FORWARD_PIN, 0);
	HAL_GPIO_WritePin(motor->BACKWARD_PORT, motor->BACKWARD_PIN, 0);
	__HAL_TIM_SET_COMPARE(motor->htim, motor->PWM_CHANNEL, 0);
}

void motor_move(struct Motor* motor[], float duty)
{
	for(int i = 0; i < sizeof(motor)/sizeof(motor[0]); i++)
	{
		motor[i]->duty = duty;
		if(motor[i]->duty < 0)
		{
			motor_set_direction(motor[i], 1);
			__HAL_TIM_SET_COMPARE(motor[i]->htim, motor[i]->PWM_CHANNEL, abs(motor[i]->duty));
		}
		if(motor[i]->duty > 0)
		{
			motor_set_direction(motor[i], 0);
			__HAL_TIM_SET_COMPARE(motor[i]->htim, motor[i]->PWM_CHANNEL, motor[i]->duty);
		}
		if(motor[i]->duty == 0)
		{
			motor_stop(motor[i]);
			__HAL_TIM_SET_COMPARE(motor[i]->htim, motor[i]->PWM_CHANNEL, motor[i]->duty);
		}
	}
}
