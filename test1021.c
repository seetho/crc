#include <stdio.h>
#include <stdint.h>

const uint16_t lut_msb[256] = {
#include "CRC16_0x1_1021_msb.lut"
};

const uint16_t lut_lsb[256] = {
#include "CRC16_0x1_1021_lsb.lut"
};

const uint16_t TestData[9] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39 };

int main(void) {
    uint16_t crc = 0x0000;
    int idx = 0;

    printf("Test GPoly 0x1021 msb-1st data.\n");
    printf("Init: 0x%04X\n", crc);
    for (uint16_t i = 0; i < 9; i++) {
        printf("0x%02X: ", TestData[i]); 
        crc ^= (TestData[i] << 8);
        idx = (crc >> 8);
        printf("-> 0x%04X -> lut[%02X] ", crc, idx);
        crc = ((crc << 8) ^ lut_msb[idx]);
        printf("-> 0x%04X\n", crc);
    }

    crc = 0x0000;
    idx = 0;
    printf("Test GPoly 0x1021 lsb-1st data.\n");
    printf("Init: 0x%04X\n", crc);
    for (uint16_t i = 0; i < 9; i++) {
        printf("0x%02X: ", TestData[i]); 
        crc ^= (TestData[i] << 8);
        idx = (crc >> 8);
        printf("-> 0x%04X -> lut[%02X] ", crc, idx);
        crc = ((crc << 8) ^ lut_lsb[idx]);
        printf("-> 0x%04X\n", crc);
    }

    return 0;
}
