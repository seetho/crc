#include <stdio.h>
#include <stdint.h>

const uint16_t lut_msb[256] = {
#include "CRC15_0x1_8B32_msb.lut"
};

const uint16_t lut_lsb[256] = {
#include "CRC15_0x1_8B32_lsb.lut"
};

const uint16_t TestData[9] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39 };

int main(void) {
    uint16_t crc = 0x0000;
    int idx = 0;

    printf("Test CRC-15/CAN GPoly 0xC599 msb-1st data.\n");
    printf("Init CRC: 0x%04X\n", crc);
    for (int i = 0; i < 9; i++) {
        printf("(Data<<8) 0x%02X00 XOR (CRC) 0x%04X ", TestData[i], crc); 
        crc ^= (TestData[i] << 8);
        idx = (crc >> 8);
        printf("-> (CRC) 0x%04X -> LUT[%02X] -> 0x%04X XOR (CRC<<8) 0x%04X ", crc, idx, lut_msb[idx], (crc << 8) & 0xFFFF);
        crc = ((crc << 8) ^ lut_msb[idx]);
        printf("-> (CRC) 0x%04X\n", crc);
    }

    crc = 0x0000;
    idx = 0;
    printf("\nTest CRC-15/CAN GPoly 0xC599 lsb-1st data.\n");
    printf("Init CRC: 0x%04X\n", crc);
    for (int i = 0; i < 9; i++) {
        printf("(Data<<8) 0x%02X00 XOR (CRC) 0x%04X ", TestData[i], crc); 
        crc ^= (TestData[i] << 8);
        idx = (crc >> 8);
        printf("-> (CRC) 0x%04X -> LUT[%02X] -> 0x%04X XOR (CRC<<8) 0x%04X ", crc, idx, lut_msb[idx], (crc << 8) & 0xFFFF);
        crc = ((crc << 8) ^ lut_lsb[idx]);
        printf("-> (CRC) 0x%04X\n", crc);
    }

    return 0;
}
