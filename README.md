| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-C6 | ESP32-H2 | ESP32-S2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- | -------- | -------- | -------- |

# RS485 Communication Module

**Author:** tuha  
**Date:** 30 Sep 2023

This is a module for RS485 communication in an embedded project. It includes functions to read and write RS485 data and calculate CRC16 for Modbus communication.

## Table of Contents

- [Introduction](#introduction)
- [Requirements](#requirements)
- [How to Use](#how-to-use)
  - [Initialization](#initialization)
  - [Sending Data](#sending-data)
  - [Receiving Data](#receiving-data)
- [Data Structures](#data-structures)
- [Examples](#examples)
- [Example Output](#example-output)

## Introduction

This module provides a set of functions to work with RS485 communication, particularly in the context of Modbus. It can be used to send and receive data, calculate CRC16, and handle Modbus frames.

## Requirements

- ESP32 development board
- UART-to-RS485 adapter
- RS485 line drivers
- ESP-IDF development environment

## How to Use

### Initialization

To use this module, you need to initialize it. Here are the steps:

1. Include the necessary header files.
2. Configure the UART settings.
3. Set up the RS485 half-duplex mode.
4. Install the UART driver.
5. Initialize the RS485 module.

### Sending Data

To send data over RS485, you can use the `TX` function. Provide the UART port, data, and its length to this function. It will send the data over the RS485 network.

### Receiving Data

To receive data from the RS485 network, a separate task is created to listen for UART events. The received data is then processed, and CRC16 is verified to ensure data integrity.

## Data Structures

The module includes a data structure `data_3pha_t` to hold various parameters. You can access voltage, current, power, and frequency data using this structure.

## Examples

The code includes an example for reading holding registers. You can modify this example to suit your specific project needs.

## How to use example

### Hardware Required
PC + USB Serial adapter connected to USB port + RS485 line drivers + Espressif development board.
The MAX485 line driver is used for example below but other similar chips can be used as well.

#### RS485 example connection circuit schematic:
```
         VCC ---------------+                               +--------------- VCC
                            |                               |
                    +-------x-------+               +-------x-------+
         RXD <------| RO            |               |             RO|-----> RXD
                    |              B|---------------|B              |
         TXD ------>| DI  MAX485    |    \  /       |    MAX485   DI|<----- TXD
ESP32 BOARD         |               |   RS-485 side |               |  SERIAL ADAPTER SIDE
         RTS --+--->| DE            |    /  \       |             DE|---+
               |    |              A|---------------|A              |   |
               +----| /RE           |               |            /RE|---+-- RTS
                    +-------x-------+               +-------x-------+
                            |                               |
                           ---                             ---
```

#### Connect an external RS485 serial interface to an ESP32 board
Connect a USB-to-RS485 adapter to a computer, then connect the adapter's A/B output lines with the corresponding A/B output lines of the RS485 line driver connected to the ESP32 chip (see figure above).
```
  --------------------------------------------------------------------------------------------------
  |  UART Interface       | #define            | Default pin for ESP32 | External RS485 Driver Pin |
  | ----------------------|--------------------|-----------------------|---------------------------|
  | Transmit Data (TxD)   | CONFIG_MB_UART_TXD | GPIO17                | DI                        |
  | Receive Data (RxD)    | CONFIG_MB_UART_RXD | GPIO16                | RO                        |
  | Request To Send (RTS) | CONFIG_MB_UART_RTS | GPIO18                | ~RE/DE                    |
  | Ground                | n/a                | GND                   | GND                       |
  --------------------------------------------------------------------------------------------------
```
Note: Each target chip has different GPIO pins available for UART connection. Please refer to UART documentation for selected target for more information.

### Configure the project
```
idf.py menuconfig
```

### Build and Flash
Build the project and flash it to the board, then run monitor tool to view serial output:
```
idf.py -p PORT flash monitor
```

(To exit the serial monitor, type ``Ctrl-]``.)

See the Getting Started Guide for full steps to configure and use ESP-IDF to build projects.

## Example Output
Example output of the application:
```
str TX: 01 03 5B 00 00 33 16 FB
I (365) main_task: Returned from app_main()
str RX: 01 03 66 FF FF FF FF 00 00 08 CC FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF 00 00 00 00 00 00 00 00 FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF 00 00 00 00 00 00 00 00 7F FF FF FF 7F FF FF FF 00 00 00 00 00 00 00 00 7F FF FF FF 7F FF FF FF 00 00 00 00 00 00 00 00 7F FF FF FF 7F FF FF FF 13 83 9F F5
I (545) RS485: Slave address: 01
I (545) RS485: Funtion: Read holding registers
I (545) RS485: Byte count: 102
phase_voltage_3pha: FFFFFFFF
phase_voltage_l1: 2252
currentL1: 0
aprtpowerL1: 0
frequency: 4995
```
The received message is showed in hexadecimal form in the brackets.


