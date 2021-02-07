#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define LUT_SIZE    16

void print_usage(char *progname) {
    printf("%s LUT_Filename\n", progname);
    printf("Data is expected from STDIN.\n");
    printf("  e.g. %s 0x8810_16_lsb.lut <check.dat\n", progname);
}

long get_filesize(FILE *fp) {
    long size = 0;

    if (0 == fseek(fp, 0, SEEK_END)) {
        size = ftell(fp);
        rewind(fp);
    }
    return size;
}

void proc16_lsb(uint8_t *lut, uint8_t *crc, uint8_t sz, uint8_t data) {
    uint8_t *lptr = NULL;

    *crc ^= data;
    for (int i = 0; i < 2; i++) {
        lptr = lut + (((*crc) & 0x0F) * sz);
        for (int j = 0; j < sz; j++) {
            *(crc+j) = (*(crc+j)>>4) ^ *(lptr+j);
            if ((j+1) < sz)
                *(crc+j) ^= *(crc+j+1)<<4;
        }
    }
}

void do_crc(uint8_t *lut, uint8_t *crc, uint8_t sz) {
    uint8_t data;

    while (1 == fread(&data, 1, 1, stdin))
        proc16_lsb(lut, crc, sz, data);
}

void print_crc(uint8_t *crc, uint8_t sz) {
    for (int i = 0; i < sz; i++)
        printf("%02X", *(crc+i));
    printf("\n");
}

int main(int argc, char *argv[]) {
    char *lut_fname = NULL;
    FILE *lfp = NULL;
    long fsize = 0;
    uint8_t *lut = NULL;
    uint8_t reg_size = 0;
    long rsize = 0;
    uint8_t *crc = NULL;

    lut_fname = argv[1];
    if (NULL != (lfp = fopen(lut_fname, "rb"))) {
    } else {
        printf("Error opening file %s\n", lut_fname);
        print_usage(argv[0]);
        return 1;
    }

    fsize = get_filesize(lfp);
    reg_size = fsize/LUT_SIZE;
    if (NULL != (lut = calloc(LUT_SIZE, reg_size))) {
        if (LUT_SIZE != (rsize = fread(lut, reg_size, LUT_SIZE, lfp))) {
            printf("LUT file read error!\n");
            fclose(lfp);
            return 1;
        }
        if (NULL == (crc = calloc(1, reg_size))) {
            printf("CRC register memory allocation error!\n");
            free(lut);
            fclose(lfp);
            return 1;
        }
        do_crc(lut, crc, reg_size);
        print_crc(crc, reg_size);
        free(lut);
        free(crc);
        fclose(lfp);
    } else {
        printf("LUT memory allocation error.\n");
        fclose(lfp);
        return 1;
    }

    return 0;
}
