#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/uart.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "esp_mac.h"

#include "bee_rs485.h"

void TX_task(void *pvParameters)
{        
    char* str_tx = read_holding_registers(0x01);
    for(;;)
    {
        if (str_tx != NULL)
        {
        printf("str TX: ");
        for (int i = 0; i < 8; i++)
        {
            printf("%02X ", (unsigned char)str_tx[i]);
        }
        printf("\n");
        TX(2, str_tx, 8);
        vTaskDelay(pdMS_TO_TICKS(4000));
        }  
    }
}

void print_json_task(void *pvParameters)
{
    for(;;)
    {
        char *json_str = pack_json_3pha_data();
        printf("str json: %s\n", json_str);
        printf("strlen: %d\n", strlen(json_str));
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void app_main(void)
{
    rs485_init();
    xTaskCreate(RX_task, "RX_task", RX_TASK_STACK_SIZE * 2, NULL, RX_TASK_PRIO, NULL);
    xTaskCreate(TX_task, "TX_task", 4096 * 2, NULL, 31, NULL);
    //xTaskCreate(print_json_task, "print_json_task", 4096, NULL, 5, NULL);
}
