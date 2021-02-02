/*
 * CRC code and LUT generator for polynomials up to 128 bits.
 *
 * (c) T K See Tho 2020-12-27
 * tkseetho@gmail.com 
 * GPG public key signature F489 A852 333D 5FBD CF6E  26DF AD5A B2C4 24BC 86B9
 *
 * This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
 * https://creativecommons.org/licenses/by-nc-sa/4.0/
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>

#include "crclib.h"

/*
 * Convert polynomial from Koopman to direct msb justified notation
 */
uint8_t koopman2direct(uint8_t *dreg, uint8_t *kreg, uint8_t size) {
    uint8_t gpw = size * 8; // polynomial width (bits)

    copy_reg(dreg, kreg, size);
    if (dreg[0] & 0x80) {           // if msb = '1'
        qup_msb(dreg, size, 1);     // shift it out
        dreg[size-1] |= 0x01;       // put '1' in the lsb
    } else {
        qup_msb(dreg, size, 1);
        dreg[size-1] |= 0x01;       // put '1' in the lsb
        gpw--;
        while (!(dreg[0] & 0x80)) { // look for the '1' msb
            qup_msb(dreg, size, 1);
            gpw--;  
        }
        qup_msb(dreg, size, 1);     // shift it out
    }
    return gpw; // this is the polynomial width
}

/*
 * Convert hexadecimal string to data register format
 */
void str2reg(uint8_t *reg, char *str, uint8_t len) {
    int i = 0;

    for (i = 0; i < len; i++)
        reg[i/2] = (reg[i/2] << ((0 != (i%2))? 4: 0)) | hex2int(str[i]);
    if (i%2)
        reg[i/2] <<= 4;
}

/*
 * Convert data register to hexadecimal string
 */
void reg2str(char *str, uint8_t *reg, uint8_t len) {
    for (int i = 0; i < len; i++) 
        sprintf(&str[i*2], "%02X", reg[i]);
}

/*
 * Clear CRC_EVAL_TYP instance and get it ready for use
 */
void clear_crc_eval_data(CRC_EVAL_TYP *gd) {
    gd->gp_width = 0;
    gd->reg_width = 0;
    clear_reg(gd->init, MAXCRCBYTES);
    clear_reg(gd->dir_init, MAXCRCBYTES);
    clear_reg(gd->aug_init, MAXCRCBYTES);
    clear_reg(gd->gp_koopman, MAXCRCBYTES);
    clear_reg(gd->gp_normal, MAXCRCBYTES);
    clear_reg(gd->gp_msb, MAXCRCBYTES);
    clear_reg(gd->gp_lsb, MAXCRCBYTES);
    clear_reg(gd->crc_msb, MAXCRCBYTES);
    clear_reg(gd->crc_lsb, MAXCRCBYTES);
}

/*
 * XOR a register with another.
 * Result goes back into the dest_reg argument.
 */
void xor_reg(uint8_t *dest_reg, uint8_t *xor_reg, uint8_t size) {
    for (int i = 0; i < size; i++)
        dest_reg[i] ^= xor_reg[i];
}

/*
 * Invert all bits of a register.
 */
void invert_reg(uint8_t *reg, uint8_t size) {
    for (int i = 0; i < size; i++) 
        reg[i] ^= 0xFF;
}

/*
 * Clear all bits of register to 0
 */
void clear_reg(uint8_t *reg, uint8_t size) {
    for (int i = 0; i < size; i++) 
        reg[i] = 0x00;
}

/*
 * Set all bits of register to 1
 */
void set_reg(uint8_t *reg, uint8_t size) {
    for (int i = 0; i < size; i++) 
        reg[i] = 0xFF;
}

/*
 * Copy one data register to another
 */
void copy_reg(uint8_t *dest, uint8_t *src, uint8_t size) {
    for (int i = 0; i < size; i++)
        dest[i] = src[i];
}

/*
 * Reverse bit order of the individual bytes in a register
 */
void revbyte_reg(uint8_t *reg, uint8_t size) {
    for (int i = 0; i < size; i++ )
        reg[i] = revbyte(reg[i]);
}

/*
 * Shift msb-first register up n bits
 */
void qup_msb(uint8_t *reg, uint8_t size, uint8_t n) {
    while (n-- > 0) {
        for (int i = 0; i < size-1; i++)
            reg[i] = (reg[i] << 1) | ((reg[i+1] & 0x80)? 0x01: 0x00);
        reg[size-1] <<= 1;
    }
}

/*
 * Shift msb-first register down n bits
 */
void qdn_msb(uint8_t *reg, uint8_t size, uint8_t n) {
    while (n-- > 0) {
        for (int i = size-1; i > 0; i--)
            reg[i] = (reg[i] >> 1) | ((reg[i-1] & 0x01)? 0x80: 0x00);
        reg[0] >>= 1;
    }
}

/*
 * Shift lsb-first register up n bits
 */
void qup_lsb(uint8_t *reg, uint8_t size, uint8_t n) {
    while (n-- > 0) {
        for (int i = 0; i < size-1; i++) 
            reg[i] = (reg[i] >> 1) | ((reg[i+1] & 0x01)? 0x80: 0x00);
        reg[size-1] >>= 1;
    }
}

/*
 * Shift lsb-first register down n bits
 */
void qdn_lsb(uint8_t *reg, uint8_t size, uint8_t n) {
    while (n-- > 0) {
        for (int i = size-1; i > 0; i--) 
            reg[i] = (reg[i] << 1) | ((reg[i-1] & 0x80)? 0x01: 0x00);
        reg[0] <<= 1;
    }
}

/*
 * Process high-nibble of data byte msb first.
 * Check each data bit; if 1 then shift data and XOR with polynomial.
 */
void process_nibble_msb(uint8_t *crc, uint8_t *gp, uint8_t size) {
    for (int i = 0; i < 4; i++) {
        if (crc[0] & 0x80) {    // shift up and do xor
            qup_msb(crc, size, 1);
            xor_reg(crc, gp, size);
        } else                  // just shift up
            qup_msb(crc, size, 1);
    }
}

/*
 * Process data byte msb first.
 * Check each data bit; if 1 then shift data and XOR with polynomial.
 */
void process_byte_msb(uint8_t *crc, uint8_t *gp, uint8_t size) {
    for (int i = 0; i < 8; i++) {
        if (crc[0] & 0x80) {    // shift up and do xor
            qup_msb(crc, size, 1);
            xor_reg(crc, gp, size);
        } else                  // just shift up
            qup_msb(crc, size, 1);
    }
}

/*
 * Process low-nibble of data byte lsb first.
 * Check each data bit; if 1 then shift data and XOR with polynomial.
 */
void process_nibble_lsb(uint8_t *crc, uint8_t *gp, uint8_t size) {
    for (int i = 0; i < 4; i++) {
        if (crc[0] & 0x01) {    // shift up and do xor
            qup_lsb(crc, size, 1);
            xor_reg(crc, gp, size);
        } else                  // just shift up
            qup_lsb(crc, size, 1);
    }
}

/*
 * Process data byte lsb first.
 * Check each data bit; if 1 then shift data and XOR with polynomial.
 */
void process_byte_lsb(uint8_t *crc, uint8_t *gp, uint8_t size) {
    for (int i = 0; i < 8; i++) {
        if (crc[0] & 0x01) {    // shift up and do xor
            qup_lsb(crc, size, 1);
            xor_reg(crc, gp, size);
        } else                  // just shift up
            qup_lsb(crc, size, 1);
    }
}

/*
 * Reverse process an init flag of direct method to init value of augmented data method
 */
void rev_process_msb(uint8_t *crc, uint8_t *gp, uint8_t size, uint8_t mask) {
    for (int i = 0; i < 8; i++) {
        if (crc[size-1] & mask) {   // do XOR then shift down
            xor_reg(crc, gp, size);
            crc[size-1] &= ~mask;   // remove bit lsb 1 first - don't leave junk just in case
            qdn_msb(crc, size, 1);
            crc[0] |= 0x80;         // put back a '1' in the msb position
        } else                      // just shift down
            qdn_msb(crc, size, 1);
    }
}

/*
 * Convert string to upper case
 */
void toupperstr(char *str, int len) {
    for (int i = 0; i < len; i++)
        str[i] = (char) toupper(str[i]);
}

/*
 * Check is string is a valid hexadecimal value string
 */
bool isxstr(char *str, int len) {
    for (int i = 0; i < len; i++)
        if (!isxdigit(str[i]))
            return false;
    return true;
}

/*
 * Reverse the bit order of a given byte
 */
uint8_t revbyte(uint8_t b) {
    return (RevNibble[b >> 4] | (RevNibble[b & 0x0F] << 4));
}

/*
 * Convert a hexadecimal char to int.
 * Returns -1 if given an invalid char.
 */
int hex2int(char c) {
    if (('0' <= c) && ('9' >= c))
        return (int)(c - '0');
    else if (('a' <= c) && ('f' >= c))
        return (int)(c - 'a' + 10);
    else if (('A' <= c) && ('F' >= c))
        return (int)(c - 'A' + 10);
    else 
        return (int)(-1);
}
