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

    float floatValue = -124.5; // Giá trị float ban đầu

    unsigned char byteData[2]; // Mảng 2 byte để lưu giá trị float

    floatToBytes(floatValue, byteData);
    printf("Float value: %f\n", floatValue);
    printf("Byte 0: 0x%02X\n", byteData[0]);
    printf("Byte 1: 0x%02X\n", byteData[1]);

    float convertedFloat = bytesToFloat(byteData);
    printf("Converted Float value: %f\n", convertedFloat);

    return 0;
}
