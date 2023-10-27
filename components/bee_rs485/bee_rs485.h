/*****************************************************************************
 *
 * @file 	bee_rs485.h
 * @author 	tuha
 * @date 	6 Sep 2023
 * @brief	module for project rs485 communication
 *
 ***************************************************************************/

#ifndef BEE_RS485
#define BEE_RS485

#define TX_PIN   (17)
#define RX_PIN   (16)

// RTS for RS485 Half-Duplex Mode manages DE/~RE
#define RTS_PIN   (18)

// CTS is not used in RS485 Half-Duplex Mode
#define CTS_PIN   (UART_PIN_NO_CHANGE)

#define BUF_SIZE        (4096)
#define BAUD_RATE       (9600)

// Read packet timeout
#define PACKET_READ_TICS     (100 / portTICK_PERIOD_MS)
#define RX_TASK_STACK_SIZE   (4096)
#define RX_TASK_PRIO         (10)
#define UART_PORT_2          (2)

// Timeout threshold for UART = number of symbols (~10 tics) with unchanged state on receive pin
#define RX_READ_TOUT          (3) // 3.5T * 8 = 28 ticks, TOUT=3 -> ~24..33 ticks

#define CRC8_POLYNOMIAL 0x31

#define ADDRESS_SLAVE_1 0X01
#define ADDRESS_SLAVE_2 0X02
#define ADDRESS_SLAVE_3 0X03

typedef struct
{
    uint32_t voltage3pha;
    uint32_t voltageL1;
    uint32_t voltageL2;
    uint32_t voltageL3;

    uint32_t voltageL1L2;
    uint32_t voltageL3L2;
    uint32_t voltageL3L1;

    uint32_t current3pha;
    uint32_t currentL1;
    uint32_t currentL2;
    uint32_t currentL3;
    uint32_t currentN;

    uint32_t actpower3pha;
    uint32_t actpowerL1;
    uint32_t actpowerL2;
    uint32_t actpowerL3;

    uint32_t ractpower3pha;
    uint32_t ractpowerL1;
    uint32_t ractpowerL2;
    uint32_t ractpowerL3;

    uint32_t aprtpower3pha;
    uint32_t aprtpowerL1;
    uint32_t aprtpowerL2;
    uint32_t aprtpowerL3;
    
    uint16_t Frequency;
} data_3pha_t;

void rs485_init();

void RX_task(void *pvParameters);

void TX(const int port, const char* str, uint8_t length);

char* read_holding_registers(uint8_t slave_addr);


#endif

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/