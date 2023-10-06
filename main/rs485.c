#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/uart.h"
#include "freertos/queue.h"

#include "bee_rs485.h"

void app_main(void)
{
    xTaskCreate(RX_task, "RX_task", RX_TASK_STACK_SIZE, NULL, RX_TASK_PRIO, NULL);
}
