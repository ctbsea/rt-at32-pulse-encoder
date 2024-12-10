#include <rtthread.h>
#include <stdlib.h>
#include "drv_common.h"
#include "drv_usart.h"
#include "at32_msp.h"

void at32_msp_pulse_encode_init(void *instance)
{
#if defined (BSP_USING_PULSE_ENCODER3)
    gpio_init_type gpio_init_struct;
    tmr_type *tmr_x = (tmr_type *)instance;

    gpio_default_para_init(&gpio_init_struct);
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    #ifdef BSP_USING_PULSE_ENCODER3
    if(tmr_x == TMR3)
    {
        /* tmr3 clock enable */
        crm_periph_clock_enable(CRM_TMR3_PERIPH_CLOCK, TRUE);
        /* gpioe clock enable */
        crm_periph_clock_enable(CRM_GPIOE_PERIPH_CLOCK, TRUE);

        /* gpioa configuration: tmr3 channel3 and channel4 as alternate function push-pull */
        gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
        gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
        gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
        gpio_init_struct.gpio_pins = GPIO_PINS_3 | GPIO_PINS_4;
        gpio_init(GPIOE, &gpio_init_struct);

        gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE3, GPIO_MUX_2);
        gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE4, GPIO_MUX_2);
    }
#endif
    /* add others */
#endif
}
