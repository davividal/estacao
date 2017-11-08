//
// Created by davi on 11/8/17.
//

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include "pressao.h"
#include "mqtt.h"

float pressure(void) {
    int file;
    int AC1;
    int AC2;
    int AC3;
    int AC4;
    int AC5;
    int AC6;
    int B1;
    int B2;
    int MC;
    int MD;

    char data[22] = {0};
    char config[2] = {0};
    char reg[1] = {0xF6};

    float X1;
    float X2;
    float B5;

    int UT;
    long UP;
    float B6;
    float X3;
    float B3;
    float B4;
    float B7;
    float pressure;

    // Create I2C bus
    char *bus = "/dev/i2c-1";
    if ((file = open(bus, O_RDWR)) < 0) {
        printf("Failed to open the bus. \n");
        exit(1);
    }
    // Get I2C device, VEML6070 I2C address is 0x77(119)
    ioctl(file, I2C_SLAVE, 0x77);

    // Calibration Cofficients stored in EEPROM of the device
    // Read 22 bytes of data from address 0xAA(170)
    read(file, data, 22);

    // Convert the data
    AC1 = data[0] * 256 + data[1];
    AC2 = data[2] * 256 + data[3];
    AC3 = data[4] * 256 + data[5];
    AC4 = data[6] * 256 + data[7];
    AC5 = data[8] * 256 + data[9];
    AC6 = data[10] * 256 + data[11];
    B1 = data[12] * 256 + data[13];
    B2 = data[14] * 256 + data[15];
    MC = data[18] * 256 + data[19];
    MD = data[20] * 256 + data[21];
    sleep(1);

    // Select measurement control register(0xF4)
    // Enable temperature measurement(0x2E)
    config[0] = 0xF4;
    config[1] = 0x2E;
    write(file, config, 2);
    sleep(1);

    // Read 2 bytes of data from register(0xF6)
    // temp msb, temp lsb
    write(file, reg, 1);
    if (read(file, data, 2) != 2) {
        printf("Erorr : Input/output Erorr \n");
        exit(1);
    }

    // Convert the data
    UT = (data[0] * 256 + data[1]);

    // Select measurement control register(0xf4)
    // Enable pressure measurement, OSS = 1(0x74)
    config[0] = 0xF4;
    config[1] = 0x74;
    write(file, config, 2);
    sleep(1);

    // Read 3 bytes of data from register(0xF6)
    // pres msb1, pres msb, pres lsb
    reg[0] = 0xF6;
    write(file, reg, 1);
    read(file, data, 3);

    // Convert the data
    UP = (data[0] * 65536 + (data[1] * 256) + data[2]) / 128;

    // Callibration for Temperature
    X1 = (UT - AC6) * AC5 / 32768;
    X2 = MC * 2048 / (X1 + MD);
    B5 = X1 + X2;

    // Calibration for Pressure
    B6 = B5 - 4000;
    X1 = (B2 * (B6 * B6 / 4096)) / 4096;
    X2 = AC2 * B6 / 2048;
    X3 = X1 + X2;
    B3 = (((AC1 * 4 + X3)) + 2) / 4;
    X1 = AC3 * B6 / 8192;
    X2 = (B1 * (B6 * B6 / 4096)) / 65536;
    X3 = ((X1 + X2) + 2) / 4;
    B4 = AC4 * (X3 + 32768) / 32768;
    B7 = ((UP - B3) * 50000);
    if (B7 < 0x80000000) {
        pressure = (B7 * 2) / B4;
    } else {
        pressure = (B7 / B4) * 2;
    }
    X1 = (pressure / 256) * (pressure / 256);
    X1 = (X1 * 3038) / 65536;
    X2 = ((7357) * pressure) / 65536;
    pressure = ((pressure + (X1 + X2 + 3791) / 15) / -1000);

    // Output data to screen
    return (pressure);
}

void *thread_pressao(void *pVoid) {
    float pressao;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (1) {
        pressao = pressure();
        mqtt_pub_double("pressao", pressao);
        printf("Pressão: %.1f hPa\n", pressao);
    }
#pragma clang diagnostic pop
}

void *pressao(void *pVoid) {
    pthread_t t1 = 0;

    pthread_create(&t1, NULL, thread_pressao, NULL);

    pthread_join(t1, NULL);
}