#include "motor.h"

void motor_init(struct Motor* motor,TIM_HandleTypeDef* htim, uint32_t PWM_CHANNEL, GPIO_TypeDef* FORWARD_PORT, GPIO_TypeDef* BACKWARD_PORT, uint16_t FORWARD_PIN, uint16_t BACKWARD_PIN){
	motor->duty = 0;
	motor->htim = htim;
	motor->PWM_CHANNEL = PWM_CHANNEL;
	motor->FORWARD_PORT = FORWARD_PORT;
	motor->BACKWARD_PORT = BACKWARD_PORT;
	motor->FORWARD_PIN = FORWARD_PIN;
	motor->BACKWARD_PIN = BACKWARD_PIN;

	HAL_TIM_Base_Start_IT(motor->htim);
	HAL_GPIO_WritePin(motor->FORWARD_PORT, motor->FORWARD_PIN, 0);
	HAL_GPIO_WritePin(motor->BACKWARD_PORT, motor->BACKWARD_PIN, 0);

	HAL_TIM_Base_Start_IT(motor->htim);
	HAL_TIM_PWM_Start(motor->htim, PWM_CHANNEL);
}

void motor_update_duty(struct Motor* motor, float duty)
{
	motor->duty = duty;
	__HAL_TIM_SET_COMPARE(motor->htim, motor->PWM_CHANNEL, duty);
}

void motor_set_direction(struct Motor* motor, _Bool direction)
{
	if(direction)
	{
		HAL_GPIO_WritePin(motor->FORWARD_PORT, motor->FORWARD_PIN, 1);
		HAL_GPIO_WritePin(motor->BACKWARD_PORT, motor->BACKWARD_PIN, 0);
	}
	else
	{
		HAL_GPIO_WritePin(motor->FORWARD_PORT, motor->FORWARD_PIN, 0);
		HAL_GPIO_WritePin(motor->BACKWARD_PORT, motor->BACKWARD_PIN, 1);
	}
}

void motor_stop(struct Motor* motor)
{
	HAL_GPIO_WritePin(motor->FORWARD_PORT, motor->FORWARD_PIN, 0);
	HAL_GPIO_WritePin(motor->BACKWARD_PORT, motor->BACKWARD_PIN, 0);
	__HAL_TIM_SET_COMPARE(motor->htim, motor->PWM_CHANNEL, 0);
}

void motor_move(struct Motor* motor, float duty)
{
	if(motor->duty > 0)
	{
		motor_set_direction(motor, 1);
		motor_update_duty(motor, abs(duty));
	}
	else if(motor->duty < 0)
	{
		motor_set_direction(motor, 0);
		motor_update_duty(motor, abs(duty));
	}
	if(motor->duty == 0)
	{
		motor_stop(motor);
		motor_update_duty(motor, 0);
	}
}
