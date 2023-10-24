/*****************************************************************************
 *
 * @file 	bee_rs485.c
 * @author 	tuha
 * @date 	5 Sep 2023
 * @brief	module for project rs485 commuication
 *
 ***************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/uart.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "sdkconfig.h"

#include "bee_rs485.h"

#define TAG "RS485"

static QueueHandle_t uart_queue;

void rs485_init()
{
    const int uart_num = UART_PORT_2;
    uart_config_t uart_config =
    {
        .baud_rate = BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 122,
        .source_clk = UART_SCLK_DEFAULT,
    };

    // Set UART log level
    esp_log_level_set(TAG, ESP_LOG_INFO);

    ESP_LOGI(TAG, "Start RS485 application and configure UART.");

    ESP_ERROR_CHECK(uart_driver_install(uart_num, BUF_SIZE * 2, 0, 10, &uart_queue, 0));

    // Configure UART parameters
    ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));

    ESP_LOGI(TAG, "UART set pins, mode and install driver.");

    // Set UART pins
    ESP_ERROR_CHECK(uart_set_pin(uart_num, TX_PIN, RX_PIN, RTS_PIN, CTS_PIN));

    // Set RS485 half duplex mode
    ESP_ERROR_CHECK(uart_set_mode(uart_num, UART_MODE_RS485_HALF_DUPLEX));

    // Set read timeout of UART TOUT feature
    ESP_ERROR_CHECK(uart_set_rx_timeout(uart_num, RX_READ_TOUT));
}

void TX(const int port, const char* str, uint8_t length)
{
    if (uart_write_bytes(port, str, length) != length)
    {
        ESP_LOGE(TAG, "Send data critical failure.");
        // add your code to handle sending failure here
        abort();
    }
    //free(new_tx_str);
}

void RX_task(void *pvParameters)
{
    uart_event_t event;
    uint8_t* dtmp = (uint8_t*) malloc(BUF_SIZE);
    for(;;)
    {
        //Waiting for UART event.
        if(xQueueReceive(uart_queue, (void * )&event, (TickType_t)portMAX_DELAY))
        {
            bzero(dtmp, BUF_SIZE);

            ESP_LOGI(TAG, "[UART DATA]: %d", event.size);
            uart_read_bytes(UART_PORT_2, dtmp, event.size, portMAX_DELAY);

            // add your code to handle RX events

            //free(dtmp);
            dtmp = NULL;
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

char *tx_str_example(uint8_t address_slave, uint8_t function, uint8_t type_data)
{
    uint8_t tx_str[6];
    tx_str[0] = 0x55;
    tx_str[1] = address_slave;
    tx_str[2] = function;
    tx_str[3] = type_data;

    uint8_t low_byte_data = 0x10;
    uint8_t high_byte_data = 0x20;
    tx_str[4] = low_byte_data;
    tx_str[5] = high_byte_data;

    // Tính CRC của chuỗi tx_str.
    uint16_t crc = cal_crc16(tx_str, sizeof(tx_str));

    // Thêm CRC vào chuỗi tx_str.
    tx_str[6] = crc & 0xFF;       // Byte thấp của CRC
    tx_str[7] = (crc >> 8) & 0xFF;  // Byte cao của CRC

    // Sao chép chuỗi tx_str vào một vùng nhớ mới.
    char* new_tx_str = malloc(sizeof(tx_str) + 1);

    memcpy(new_tx_str, tx_str, sizeof(tx_str));

    return new_tx_str;
}

bool check_crc(const uint8_t* data, size_t len)
{
    if (len < 2)
    {
        // Ít nhất cần 2 byte để kiểm tra CRC.
        return false;
    }

    // Tính toán CRC từ dữ liệu nhận được, trừ 2 byte CRC cuối cùng.
    uint16_t received_crc = cal_crc16(data, len - 2);

    // So sánh với CRC được gửi trong chuỗi nhận được (2 byte cuối cùng).
    uint16_t expected_crc = (data[len - 2] | (data[len - 1] << 8));

    return received_crc == expected_crc;
}

uint16_t cal_crc16(const uint8_t *data, size_t len)
{
  uint16_t crc = 0xffff;
  for (size_t i = 0; i < len; i++)
  {
    crc = crc ^ data[i];
    for (int j = 0; j < 8; j++)
    {
      if ((crc & 1) == 0)
      {
        crc >>= 1;
      }
      else
      {
        crc = (crc >> 1) ^ 0xa001;
      }
    }
  }
  return crc;
}

// Hàm chuyển đổi từ float sang 2 byte cao và thấp
void floatToBytes(float floatValue, unsigned char *byteArray)
{
    // Sử dụng con trỏ để truy cập từng byte của giá trị float
    unsigned char *floatBytes = (unsigned char *)&floatValue;

    // Gán giá trị byte cao và byte thấp
    byteArray[0] = floatBytes[3];
    byteArray[1] = floatBytes[2];
}

// Hàm chuyển đổi từ 2 byte sang float
float bytesToFloat(const unsigned char *byteArray)
{
    float result;

    // Tạo một con trỏ để truy cập từng byte của giá trị float
    unsigned char *floatBytes = (unsigned char *)&result;

    // Gán giá trị byte cao và byte thấp
    floatBytes[3] = byteArray[0];
    floatBytes[2] = byteArray[1];

    return result;
}


/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
