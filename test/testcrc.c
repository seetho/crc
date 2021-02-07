#include <stdio.h>
#include <stdint.h>

// Select one of the following to compile for
//#define COMP_CRC4
//#define COMP_CRC15
//#define COMP_CRC16
//#define COMP_CRC32
// ...or pass it from gcc with the -D option; e.g. -D COMP_CRC32

#if defined COMP_CRC4
#define CRC_SIZE    1
#define CRC_TYPE    CRC-4
#define KPOLY       0x09
#define GPOLY       0x13

#elif defined COMP_CRC15
#define CRC_SIZE    2
#define CRC_TYPE    CRC-15
#define KPOLY       0x62CC
#define GPOLY       0xC599

#elif defined COMP_CRC16
#define CRC_SIZE    2
#define CRC_TYPE    CRC-16
#define KPOLY       0x8810
#define GPOLY       0x11021

#elif defined COMP_CRC32
#define CRC_SIZE    4
#define CRC_TYPE    CRC-32
#define KPOLY       0x82608EDB
#define GPOLY       0x104C11DB7
#endif

#define STR_(s)     #s
#define STR(s)      STR_(s)
#define NAME_(p,r)  p ## r
#define NAME(p,r)   NAME_(p, r)
#define TEST_SIZE   9

#include STR(NAME(KPOLY, _16_msb.h))
#include STR(NAME(KPOLY, _16_lsb.h))
#include STR(NAME(KPOLY, _256_msb.h))
#include STR(NAME(KPOLY, _256_lsb.h))


const uint8_t TestData[TEST_SIZE] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};

void lut256_test(uint8_t *crc, const uint8_t lut[][CRC_SIZE]) {
    uint8_t idx = 0;

    for (int i = 0; i < CRC_SIZE; i++)          // zero the crc register first
        crc[i] = 0x00;

    for (int i = 0; i < TEST_SIZE; i++) {       // run through all the test data
        idx = crc[0] ^ TestData[i];             // index into the LUT
        for (int j = 0; j < CRC_SIZE; j++) {    // each byte of LUT value XORed with next byte of crc
            crc[j] = lut[idx][j];               //      becomes the new crc value
            if ((j+1) < CRC_SIZE)
                crc[j] ^= crc[j+1];
        }
    }
}

void lut16_msb_test(uint8_t *crc, const uint8_t lut[][CRC_SIZE]) {
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

void lut16_lsb_test(uint8_t *crc, const uint8_t lut[][CRC_SIZE]) {
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

    printf(STR(CRC_TYPE) " " STR(KPOLY) " (" STR(GPOLY) ") with 256-element LUTs:\n");
    // msb-1st test
    lut256_test(crc, lut256_msb);
    printf("    msb-1st LUT check: 0x");
    for (int i = 0; i < CRC_SIZE; i++)
        printf("%02X", crc[i]);
    printf("\n");
    // lsb-1st test
    lut256_test(crc, lut256_lsb);
    printf("    lsb-1st LUT check: 0x");
    for (int i = 0; i < CRC_SIZE; i++)
        printf("%02X", crc[i]);
    printf("\n\n");

    printf(STR(CRC_TYPE) " " STR(KPOLY) " (" STR(GPOLY) ") with 16-element LUTs:\n");
    // msb-1st test
    lut16_msb_test(crc, lut16_msb);
    printf("    msb-1st LUT check: 0x");
    for (int i = 0; i < CRC_SIZE; i++)
        printf("%02X", crc[i]);
    printf("\n");
    // lsb-1st test
    lut16_lsb_test(crc, lut16_lsb);
    printf("    lsb-1st LUT check: 0x");
    for (int i = 0; i < CRC_SIZE; i++)
        printf("%02X", crc[i]);
    printf("\n");
    return 0;
}
