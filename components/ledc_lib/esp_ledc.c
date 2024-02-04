#include "esp_ledc.h"
#include <stdio.h>
#include "driver/ledc.h"
#include "esp_err.h"

#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_CHANNEL            LEDC_CHANNEL_0
#define LEDC_DUTY_RES           LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define LEDC_FREQUENCY          (5000) // Frequency in Hertz. Set frequency at 5 kHz

void esp_ledc_set_duty(int channel, int duty)
{
    if (duty < 0) // sua lai
    {
        ledc_set_duty(LEDC_MODE, channel, 0);
        ledc_update_duty(LEDC_MODE, channel);
        return;
    }
    else if (duty >8192)
    {
        ledc_set_duty(LEDC_MODE, channel, 8192);
        ledc_update_duty(LEDC_MODE,channel);
        return;
    }
    ledc_set_duty(LEDC_MODE, channel, duty);
    ledc_update_duty(LEDC_MODE,channel);
}

void esp_ledc_init(int ledc_timer_num, int ledc_channel_num, int pin)
{
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE,
        .timer_num        = (ledc_timer_t)ledc_timer_num,
        .duty_resolution  = LEDC_DUTY_RES,
        .freq_hz          = LEDC_FREQUENCY,  // Set output frequency at 5 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_MODE,
        .channel        = ledc_channel_num,
        .timer_sel      = (ledc_timer_t)ledc_timer_num,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = pin,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}