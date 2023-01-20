#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "main.h"
#include "stdio.h"
#include "stdlib.h"

struct Motor{
	TIM_HandleTypeDef* htim;
	uint32_t PWM_CHANNEL;
	GPIO_TypeDef* FORWARD_PORT;
	GPIO_TypeDef* BACKWARD_PORT;
	uint16_t FORWARD_PIN;
	uint16_t BACKWARD_PIN;

	float duty;
};

void motor_init(struct Motor* motor, TIM_HandleTypeDef* htim,uint32_t PWM_CHANNEL, GPIO_TypeDef* FORWARD_PORT, GPIO_TypeDef* BACKWARD_PORT, uint16_t FORWARD_PIN, uint16_t BACKWARD_PIN);
//void motor_update_duty(struct Motor* motor, float duty);
void motor_set_direction(struct Motor* motor, _Bool direction);
void motor_stop(struct Motor* motor);
void motor_move(struct Motor* motor, float  duty);

#endif /* INC_MOTOR_H_ */
