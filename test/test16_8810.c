#include <stdio.h>
#include <stdint.h>

#define CRC_SIZE    2
#define TEST_SIZE   9

#include "CRC16_0x8810_16_msb.lut"
#include "CRC16_0x8810_16_lsb.lut"

const uint8_t TestData[TEST_SIZE] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};

void lut_test_msb(uint8_t *crc, const uint8_t lut[][CRC_SIZE]) {
    uint8_t idx = 0;

    for (int i = 0; i < CRC_SIZE; i++)
        crc[i] = 0x00;
    for (int i = 0; i < TEST_SIZE; i++) {
        crc[0] ^= TestData[i];
        idx = crc[0] >> 4;      // high-nibble is LUT index
        if (CRC_SIZE > 1) {     // if multibyte crc
            crc[0] <<= 4;       // shift everything 1 nibble left
            for (int j = 1; j < CRC_SIZE; j++) {
                crc[j-1] |= crc[j] >> 4;
                crc[j] <<= 4;
            }
            for (int k = 0; k < CRC_SIZE; k++)
                crc[k] ^= lut[idx][k];
            idx = crc[0] >> 4;  // previous low-nibble index to LUT 
            crc[0] <<= 4;       // shift everything 1 nibble left
            for (int j = 1; j < CRC_SIZE; j++) {
                crc[j-1] |= crc[j] >> 4;
                crc[j] <<= 4;
            }
            for (int k = 0; k < CRC_SIZE; k++)
                crc[k] ^= lut[idx][k];
        } else {    // single byte crc
            crc[0] = (crc[0] << 4) ^ lut[idx][0];   // first look up
            crc[0] = lut[crc[0] >> 4][0];           // 2nd look up
        }
    }
}

void lut_test_lsb(uint8_t *crc, const uint8_t lut[][CRC_SIZE]) {
    uint8_t idx = 0;

    for (int i = 0; i < CRC_SIZE; i++)
        crc[i] = 0x00;
    for (int i = 0; i < TEST_SIZE; i++) {
        crc[0] ^= TestData[i];
        idx = crc[0] & 0x0F;    // low-nibble is LUT index
        if (CRC_SIZE > 1) {     // multi-byte crc
            crc[0] >>= 4;       // shift everything 1 nibble left (low nibble first)
            for (int j = 1; j < CRC_SIZE; j++) {
                crc[j-1] |= crc[j] << 4;
                crc[j] >>= 4;
            }
            for (int k = 0; k < CRC_SIZE; k++)
                crc[k] ^= lut[idx][k];
            idx = crc[0] & 0x0F;    // previous high-nibble index to LUT
            crc[0] >>= 4;           // shift everything 1 nibble left (low nibble first)
            for (int j = 1; j < CRC_SIZE; j++) {
                crc[j-1] |= crc[j] << 4;
                crc[j] >>= 4;
            }
            for (int k = 0; k < CRC_SIZE; k++)
                crc[k] ^= lut[idx][k];
        } else {    // single byte crc
            crc[0] = (crc[0] >> 4) ^ lut[idx][0];   // first look up
            crc[0] = lut[crc[0] & 0x0F][0];         // 2nd look up
        }
    }
}


int main(void) {
    uint8_t crc[CRC_SIZE];

    printf("CRC-16/CCITT (0x8810 or 0x11021) with 16-element LUTs:\n");

    // msb-1st test
    lut_test_msb(crc, lut_msb);
    printf("    msb-1st LUT check: 0x");
    for (int i = 0; i < CRC_SIZE; i++)
        printf("%02X", crc[i]);
    printf("\n");

    // lsb-1st test
    lut_test_lsb(crc, lut_lsb);
    printf("    lsb-1st LUT check: 0x");
    for (int i = 0; i < CRC_SIZE; i++)
        printf("%02X", crc[i]);
    printf("\n");

    return 0;
}
