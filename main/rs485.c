#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/uart.h"
#include "freertos/queue.h"
#include "esp_log.h"

#include "bee_rs485.h"
#include "esp_system.h"

static const char *TAG = "uart_events";
static QueueHandle_t uart0_queue;

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
        vTaskDelay(pdMS_TO_TICKS(2000));
        }  
    }
}


#if 0
void print_json_task(void *pvParameters)
{
    for(;;)
    {
        char *json_str = pack_json_3pha_data();
        printf("str json: %s\n", json_str);
        printf("strlen: %d\n", strlen(json_str));
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}
#endif

static void uart_event_task(void *pvParameters)
{
    uart_event_t event;
    uint8_t* dtmp_uart1 = (uint8_t*) malloc(256);
    for(;;) {
        //Waiting for UART event.
        if(xQueueReceive(uart0_queue, (void * )&event, (TickType_t)portMAX_DELAY))
        {
            bzero(dtmp_uart1, 256);
            uart_read_bytes(1, dtmp_uart1, event.size, portMAX_DELAY);
            printf("%s\n", dtmp_uart1);
        }
    }
    free(dtmp_uart1);
    dtmp_uart1 = NULL;
    vTaskDelete(NULL);
}

static void uart_tx_task(void *pvParameters)
{
    for(;;)
    {
        uart_write_bytes(1, "hello", 6);
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void app_main(void)
{
    rs485_init();
    xTaskCreatePinnedToCore(RX_task, "RX_task", RX_TASK_STACK_SIZE * 2, NULL, RX_TASK_PRIO, NULL, 1);
    xTaskCreate(TX_task, "TX_task", 4096 * 2, NULL, 31, NULL);
    //xTaskCreate(print_json_task, "print_json_task", 2048, NULL, 10, NULL);

    esp_log_level_set(TAG, ESP_LOG_INFO);

    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    //Install UART driver, and get the queue.
    uart_driver_install(1, BUF_SIZE * 2, BUF_SIZE * 2, 20, &uart0_queue, 0);
    uart_param_config(1, &uart_config);

    //Set UART log level
    esp_log_level_set(TAG, ESP_LOG_INFO);
    //Set UART pins (using UART0 default pins ie no changes.)
    uart_set_pin(1, 26, 25, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    //Create a task to handler UART event from ISR
    xTaskCreate(uart_event_task, "uart_event_task", 2048, NULL, 12, NULL);
    xTaskCreate(uart_tx_task, "uart_tx_task", 2048, NULL, 11, NULL);
}
