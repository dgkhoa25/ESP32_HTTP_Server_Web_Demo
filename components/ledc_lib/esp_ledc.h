#ifndef ESP_LEDC_H
#define ESP_LEDC_H

void esp_ledc_init(int ledc_timer, int ledc_channel, int pin);
void esp_ledc_set_duty(int channel, int duty);
#endif
