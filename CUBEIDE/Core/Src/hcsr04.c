#include "hcsr04.h"

#define HC_SR04_US_TO_CM_CONVERTER	58

void hc_sr04_init(struct us_sensor_str *us_sensor, TIM_HandleTypeDef *htim_echo, TIM_HandleTypeDef *htim_trig, TIM_Channel trig_channel)
{
	us_sensor->htim_echo = htim_echo;
	us_sensor->htim_trig = htim_trig;
	us_sensor->trig_channel = trig_channel;

	HAL_TIM_IC_Start_IT(us_sensor->htim_echo, TIM_CHANNEL_1 | TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(us_sensor->htim_trig, us_sensor->trig_channel);
}

float hc_sr04_convert_us_to_cm(float distance_us)
{
	return (distance_us / HC_SR04_US_TO_CM_CONVERTER);
}
