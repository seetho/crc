#include <stdio.h>
#include <stdint.h>

const uint8_t lut_msb[256] = {
#include "CRC5_0x1_58_msb.lut"
};

const uint8_t lut_lsb[256] = {
#include "CRC5_0x1_58_lsb.lut"
};

const uint8_t TestData[9] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39 };

int main(void) {
    uint8_t crc = 0x00;

    printf("Test CRC-5/ITU G.704 GPoly 101011b msb-1st data.\n");
    printf("Init CRC: 0x%02X\n", crc);
    for (int i = 0; i < 9; i++) {
        printf("(Data) 0x%02X XOR (CRC) 0x%02X ", TestData[i], crc); 
        crc ^= TestData[i];
        printf("-> (CRC) 0x%02X -> LUT[%02X] ", crc, crc);
        crc = lut_msb[crc];
        printf("-> (CRC) 0x%02X\n", crc);
    }

    crc = 0x00;
    printf("Test CRC-5/ITU G.704 GPoly 101011b lsb-1st data.\n");
    printf("Init CRC: 0x%04X\n", crc);
    for (int i = 0; i < 9; i++) {
        printf("(Data) 0x%02X XOR (CRC) 0x%02X ", TestData[i], crc); 
        crc ^= TestData[i];
        printf("-> (CRC) 0x%02X -> LUT[%02X] ", crc, crc);
        crc = lut_lsb[crc];
        printf("-> (CRC) 0x%02X\n", crc);
    }

    return 0;
}
