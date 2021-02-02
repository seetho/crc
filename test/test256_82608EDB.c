#include <stdio.h>
#include <stdint.h>

#define CRC_SIZE    4
#define TEST_SIZE   9

#include "CRC32_0x82608EDB_256_msb.lut"
#include "CRC32_0x82608EDB_256_lsb.lut"

const uint8_t TestData[TEST_SIZE] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};

void lut_test(uint8_t *crc, const uint8_t lut[][CRC_SIZE]) {
    uint8_t idx = 0;

    for (int i = 0; i < CRC_SIZE; i++)
        crc[i] = 0x00;
    for (int i = 0; i < TEST_SIZE; i++) {
        crc[0] ^= TestData[i];
        idx = crc[0];
        crc[0] = lut[idx][0];
        if (CRC_SIZE > 1) {
            for (int j = 1; j < CRC_SIZE; j++) {
                crc[j-1] = crc[j];
                crc[j] = 0x00;
            }
            for (int k = 0; k < CRC_SIZE; k++)
                crc[k] ^= lut[idx][k];
        }
    }
}


int main(void) {
    uint8_t crc[CRC_SIZE];

    printf("CRC-32 (0x82608EDB or 0x04C11DB7) with 256-element LUTs:\n");

    // msb-1st test
    lut_test(crc, lut_msb);
    printf("    msb-1st LUT check: 0x");
    for (int i = 0; i < CRC_SIZE; i++)
        printf("%02X", crc[i]);
    printf("\n");

    // lsb-1st test
    lut_test(crc, lut_lsb);
    printf("    lsb-1st LUT check: 0x");
    for (int i = 0; i < CRC_SIZE; i++)
        printf("%02X", crc[i]);
    printf("\n");

    return 0;
}
