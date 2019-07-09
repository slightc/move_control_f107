#ifndef ROBOMODULE_CAN
#define ROBOMODULE_CAN

#include "stm32f1xx.h"
#include <stm32f1xx_hal.h>

#include "robomodule_type.h"

void robomodule_set_can_handle(CAN_HandleTypeDef *that_can_handle);
uint8_t robomodule_can_reset(uint8_t group, uint8_t number, uint32_t timeout);
uint8_t robomodule_can_set_mode(uint8_t group, uint8_t number, 
                                    uint8_t mode, uint32_t timeout);
uint8_t robomodule_can_openloop_mode(uint8_t group, uint8_t number, 
                                        int16_t pwm, uint32_t timeout);
uint8_t robomodule_can_current_mode(uint8_t group, uint8_t number, 
                        int16_t max_pwm, int16_t current, uint32_t timeout);
uint8_t robomodule_can_velocity_mode(uint8_t group, uint8_t number, 
                        int16_t max_pwm, int16_t velocity, uint32_t timeout);
uint8_t robomodule_can_position_mode(uint8_t group, uint8_t number, 
                        int16_t max_pwm, int32_t position, uint32_t timeout);
#endif