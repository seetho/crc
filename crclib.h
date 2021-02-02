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

/*
 * Data structures and functions for CRC generation.
 *
 * All polynomials and crc registers are left justified, i.e. the front of the 
 * generation queue is at index 0 of the byte arrays.
 * If msb first processing the msb of byte 0 is the leading bit in the queue,
 * otherwise it is the lsb of byte 0 that is the leading bit.
 * Generator polynomials are presented without the implicit high bit; e.g. the
 * polynomial for CRC32 0x104C11DB7 should be specified as 04C11DB7, left
 * justified. In this case gp_width should be 32.
 */

#define MAXCRCBYTES 16                  // maximum polynomial size in bytes
#define MAXCRCBITS  (MAXCRCBYTES * 8)   // maximum polynomial size in bits

// Structure to hold all essential data of a CRC generation
typedef struct {
    uint8_t gp_width;                   // degree of g.polynomial, i.e. # of bits minus 1
    uint8_t reg_width;                  // crc and gp register width in bytes
    uint8_t init[MAXCRCBYTES];          // the initial crc register value, left justified
    uint8_t dir_init[MAXCRCBYTES];      // the equivalent initial value for direct method
    uint8_t aug_init[MAXCRCBYTES];      // the equivalent initial value for augmented 0s method
    uint8_t gp_koopman[MAXCRCBYTES];    // g.polynomial in koopman notation
    uint8_t gp_normal[MAXCRCBYTES];     // g.polynomial in normal notation (lsb justified)
    uint8_t gp_msb[MAXCRCBYTES];        // g.polynomial for msb first data processing
    uint8_t gp_lsb[MAXCRCBYTES];        // g.polynomial for lsb first data processing
    uint8_t crc_msb[MAXCRCBYTES];       // crc register for msb first data stream
    uint8_t crc_lsb[MAXCRCBYTES];       // crc register for lsb first data stream
} CRC_EVAL_TYP; 

// Look up table to reverse bit order of nibbles
static const uint8_t RevNibble[] = {
    0x00, 0x08, 0x04, 0x0C, 0x02, 0x0A, 0x06, 0x0E,
    0x01, 0x09, 0x05, 0x0D, 0x03, 0x0B, 0x07, 0x0F};

uint8_t koopman2direct(uint8_t *dreg, uint8_t *kreg, uint8_t size);
/* 
Convert polynomial from Koopman to direct msb justified notation
    *dreg: the direct (msb justified) register
    *kreg: the koopman register
    size: size of registers in bytes
*/


void str2reg(uint8_t *reg, char *str, uint8_t len);
/*
Convert hexadecimal string to data register format
    *reg: the register array
    *str: the string to convert from (char array)
    len: length of string
*/

void reg2str(char *str, uint8_t *reg, uint8_t len);
/*
Convert data register to hexadecimal string
    *str: the string to convert to (char array)
    *reg: the register array to convert from
    len: number of bytes in register array
*/

void clear_crc_eval_data(CRC_EVAL_TYP *gd);
/*
Clear CRC_EVAL_TYP instance and get it ready for use
    gd: ptr to a CRC_EVAL_TYP structure.
*/

void xor_reg(uint8_t *dest_reg, uint8_t *xor_reg, uint8_t size);
/*
XOR a register with another.
    *dest_reg: ptr to array with data to XOR.
    *xor_reg: ptr to array with 2nd XOR data.
    size: (number of bytes) in the register to XOR.
*/

void invert_reg(uint8_t *reg, uint8_t size);
/*
Invert all bits of a register.
    *reg: array to do bit invert
    size: number of elements (bytes)
*/

void clear_reg(uint8_t *reg, uint8_t size);
/*
Clear all bits of register to 0
    *reg: array to clear bits to all 0
    size: number of elements (bytes)
*/

void set_reg(uint8_t *reg, uint8_t size);
/*
Set all bits of register to 1
    *reg: array to set bits to all 1
    size: number of elements (bytes)
*/

void copy_reg(uint8_t *dest, uint8_t *src, uint8_t size);
/*
Copy one data register to another
    *dest: array to copy to
    *src: array to aopy from
    size: number of elements (bytes)
*/

void revbyte_reg(uint8_t *reg, uint8_t size);
/*
Reverse bit order of the individual bytes in a register
    *reg: array with bytes to reverse
    size: number of elements (bytes)
*/

void qup_msb(uint8_t *reg, uint8_t size, uint8_t n);
/*
Shift msb-first register up 1 bit
    *reg: array to do bit shift
    size: number of elements involves in the shift (bytes)
    n : number of bits to shift
*/

void qup_lsb(uint8_t *reg, uint8_t size, uint8_t n);
/*
Shift lsb-first register up 1 bit
    *reg: array to do bit shift
    size: number of elements involves in the shift (bytes)
    n : number of bits to shift
*/

void qdn_msb(uint8_t *reg, uint8_t size, uint8_t n);
/*
Shift msb-first register down 1 bit
    *reg: array to do bit shift
    size: number of elements involves in the shift (bytes)
    n : number of bits to shift
*/

void qdn_lsb(uint8_t *reg, uint8_t size, uint8_t n);
/*
Shift lsb-first register down 1 bit
    *reg: array to do bit shift
    size: number of elements involves in the shift (bytes)
    n : number of bits to shift
*/

void process_nibble_msb(uint8_t *crc, uint8_t *gp, uint8_t size);
/*
Process high-nibble of data byte msb first.
Check each data bit; if 1 then shift data and XOR with polynomial.
    *crc: crc register array
    *gp: generator polynomial array
    size: size of crc/polynomial array
*/

void process_byte_msb(uint8_t *crc, uint8_t *gp, uint8_t size);
/*
Process data byte msb first.
Check each data bit; if 1 then shift data and XOR with polynomial.
    *crc: crc register array
    *gp: generator polynomial array
    size: size of crc/polynomial array
*/

void process_nibble_lsb(uint8_t *crc, uint8_t *gp, uint8_t size);
/*
Process low-nibble of data byte lsb first.
Check each data bit; if 1 then shift data and XOR with polynomial.
    *crc: crc register array
    *gp: generator polynomial array
    size: size of crc/polynomial array
*/

void process_byte_lsb(uint8_t *crc, uint8_t *gp, uint8_t size);
/*
Process data byte lsb first.
Check each data bit; if 1 then shift data and XOR with polynomial.
    *crc: crc register array
    *gp: generator polynomial array
    size: size of crc/polynomial array
*/

void rev_process_msb(uint8_t *crc, uint8_t *gp, uint8_t size, uint8_t mask);
/*
Reverse process an init flag of direct method to init value of augmented data method
    *crc: crc register array
    *gp: generator polynomial array
    size: size of crc/polynomial array
    mask: mask to pick out lsb of right most array element
*/

void toupperstr(char *str, int len);
/*
Convert string to upper case
    *str: string to convert all character to uppercase
    len: length of str
*/

bool isxstr(char *str, int len);
/*
Check is string is a valid hexadecimal value string
    *str: hexadecimal character string to check
    len: length of str
*/

uint8_t revbyte(uint8_t b);
/*
Reverse the bit order of a given byte
    b: byte value
*/

int hex2int(char c);
/*
Convert a hexadecimal char to int.
Returns -1 if given an invalid char.
    c: valid hexadecimal character (upper or lowercase)
*/

