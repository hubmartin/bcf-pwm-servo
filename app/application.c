#include <application.h>

#include <bc_timer.h>

bc_led_t led;
bc_button_t button;

void button_event_handler(bc_button_t *self, bc_button_event_t event, void *event_param)
{
    (void) self;
    (void) event_param;

    // Servo values in microseconds
    static const uint16_t servo_table[] = {550, 1000, 1500, 2300};

    // PWM Duty 0 - 255
    static const uint16_t pwm_table[] = {0, 100, 200, 255};
    static uint8_t cycle_index = 0;

    if (event == BC_BUTTON_EVENT_PRESS)
    {
        bc_led_pulse(&led, 100);

        // Divide by 5 = 5 us
        bc_pwm_set(BC_PWM_P1, servo_table[cycle_index] / 5);

        // PWM Duty is 0-255, do not divide values
        bc_pwm_set(BC_PWM_P6, pwm_table[cycle_index]);
        bc_pwm_set(BC_PWM_P12, pwm_table[cycle_index]);

        // Increment counter for next value
        cycle_index++;
        if (cycle_index == (sizeof(servo_table) / sizeof(uint16_t)))
        {
            cycle_index = 0;
        }
    }
}

void application_init(void)
{
    // Initialize LED
    bc_led_init(&led, BC_GPIO_LED, false, false);
    bc_led_pulse(&led, 100);

    bc_button_init(&button, BC_GPIO_BUTTON,  BC_GPIO_PULL_DOWN, 0);
    bc_button_set_event_handler(&button, button_event_handler, NULL);

    bc_log_init(BC_LOG_LEVEL_DEBUG, BC_LOG_TIMESTAMP_REL);

/*
    // Disable because of Power Module's relay
    bc_pwm_init(BC_GPIO_P0);
    bc_pwm_set(BC_GPIO_P0, 10);
    bc_pwm_enable(BC_GPIO_P0);
*/

    // TIM2
    bc_pwm_init(BC_PWM_P1);
    // Reconfigure TIM2 for servo pulses
    // This affects channels P0-P3 period
    bc_pwm_tim_configure(BC_PWM_TIM2_P0_P1_P2_P3, 5, 255 * 16);
    bc_pwm_set(BC_PWM_P1, 110);
    bc_pwm_enable(BC_PWM_P1);

    bc_pwm_init(BC_PWM_P2);
    bc_pwm_set(BC_PWM_P2, 80);
    bc_pwm_enable(BC_PWM_P2);

    bc_pwm_init(BC_PWM_P3);
    bc_pwm_set(BC_PWM_P3, 120);
    bc_pwm_enable(BC_PWM_P3);

    // TIM3
    bc_pwm_init(BC_PWM_P6);
    bc_pwm_set(BC_PWM_P6, 180);
    bc_pwm_enable(BC_PWM_P6);

    bc_pwm_init(BC_PWM_P7);
    bc_pwm_set(BC_PWM_P7, 210);
    bc_pwm_enable(BC_PWM_P7);

    bc_pwm_init(BC_PWM_P8);
    bc_pwm_set(BC_PWM_P8, 255);
    bc_pwm_enable(BC_PWM_P8);

    // TIM21
    bc_pwm_init(BC_PWM_P12);
    bc_pwm_set(BC_PWM_P12, 10);
    bc_pwm_enable(BC_PWM_P12);

    bc_pwm_init(BC_PWM_P14);
    bc_pwm_set(BC_PWM_P14, 128);
    bc_pwm_enable(BC_PWM_P14);

    bc_timer_init();

    bc_gpio_init(BC_GPIO_P9);
    bc_gpio_set_mode(BC_GPIO_P9, BC_GPIO_MODE_OUTPUT);
}


void application_task()
{
    // bc_delay test loop
    bc_gpio_set_output(BC_GPIO_P9, true);

    bc_timer_start();
    bc_timer_delay(5000);
    bc_timer_stop();

    bc_gpio_set_output(BC_GPIO_P9, false);

    bc_scheduler_plan_current_relative(50);
}
