#ifndef INC_HCSR04_H_
#define INC_HCSR04_H_

#include "main.h"

typedef uint32_t TIM_Channel;

struct us_sensor_str{
	TIM_HandleTypeDef *htim_echo;
	TIM_HandleTypeDef *htim_trig;
	TIM_Channel trig_channel;

	volatile float distance_cm;
};

void hc_sr04_init(struct us_sensor_str *us_sensor, TIM_HandleTypeDef *htim_echo, TIM_HandleTypeDef *htim_trig, TIM_Channel channel);
float hc_sr04_convert_us_to_cm(float distance_us);

#endif /* INC_HCSR04_H_ */
