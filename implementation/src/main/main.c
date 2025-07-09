/*
    AUTHOR: Ronny N. Ismael
    DATE: 2025-07-0

    DESCRIPTION: Module that will control a dog door on a cat litter box
    to keep my dumb dog out of the litter box because he loves to eat the
    cat poop.
*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
// #include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "sdkconfig.h"

static const char *TAG = "dog_door";

/*
void pxTaskCode(void *pv) {
  while (1)
  {
    foo();
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}
*/

void app_main(void)
{
  ESP_LOGI(TAG, "ESP32 startup procedure beginning...");

  while (1)
  {
    /* xTaskCreate(TaskFunction_t pxTaskCode, const char *const pcName, const uint32_t usStackDepth, void *const pvParameters, UBaseType_t uxPriority, TaskHandle_t *const pxCreatedTask) */
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
