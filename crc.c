/*
 * Program to generate CRC value given a polynomial and other parameters
 * against data from a file or from stdin.
 * Polynomial can be specified up to 128 bits in a hexadecimal format.
 *
 * (c) T K See Tho 2020-12-27
 */

#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "crclib.h"

#define MAXCRCSTR   ((MAXCRCBYTES * 2) + 3) // input string max length
                                            // cater for odd poly widths and string terminator

const char SWVersion[] = "5.5 (2021-01-18)";

// The popular (but rather weak) check data for CRC
const uint8_t TestData[] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};

/*
 * Print title and version
 */
void print_title() {
    printf("SeeTho's Generic CRC Code Generator (2020-12-27)\n");
    printf("Revision %s\n\n", SWVersion);
}

/*
 * Print usage/help
 */
void print_usage(char *progname) {
    print_title();
    printf("Usage: %s -pGPOLY [-iINIT] [-fINFILE | -t | -l | -h]\n\n", progname);
    printf("-p  <GPOLY>: Generator polynomial (in Koopman notation, max 128 bits).\n");
    printf("-i   <INIT>: Initial value of CRC register (msb left justified).\n");
    printf("-f <INFILE>: Input data file (default from stdin).\n");
    printf("-t Set to run on test string \"123456789\" (0x313233343536373839)\n");
    printf("-l Set to generate look-up-table\n");
    printf("-h Print this usage message.\n");
}

/*
 * Print results
 */
void print_results(CRC_EVAL_TYP *gd) {
    char str[(MAXCRCBYTES*2)+1] = ""; // make a temp string with enough space

    printf("Gen. Polynomial Degree:       %d (1 + %d bits)\n", gd->gp_width, gd->gp_width);
    reg2str(str, gd->gp_koopman, gd->reg_width);
    printf("Gen. Polynomial (Koopman):    0x%s\n", str);
    reg2str(str, gd->gp_normal, gd->reg_width);
    printf("Gen. Polynomial (Normal):     0x%s\n", str);
    reg2str(str, gd->gp_msb, gd->reg_width);
    printf("Gen. Polynomial (Direct msb): 0x%s\n", str);
    reg2str(str, gd->gp_lsb, gd->reg_width);
    printf("Gen. Polynomial (Direct lsb): 0x%s\n", str);
    reg2str(str, gd->init, gd->reg_width);
    printf("Initial CRC Register Value:   0x%s\n", str);
    reg2str(str, gd->dir_init, gd->reg_width);
    printf("Equivalent Direct Init Value: 0x%s\n", str);
    reg2str(str, gd->aug_init, gd->reg_width);
    printf("Equivalent Aug'd Init Value:  0x%s\n", str);
    reg2str(str, gd->crc_msb, gd->reg_width);
    printf("CRC (Direct msb-st data):     0x%s\n", str);
    reg2str(str, gd->crc_lsb, gd->reg_width);
    printf("CRC (Direct lsb-1st data):    0x%s\n", str);
}

/*
 * Generate look up table for the given polynomial.
 * Both msb and lsb tables.
 */
bool generate_lut(CRC_EVAL_TYP *gd) {
    FILE *fp;               // output file ptr
    char fname[256] = "";   // filename to write to
    char str[(MAXCRCBYTES*2)+1] = ""; // make a temp string with enough space
    int cnt = 0;

    print_title();

    // msb-1st lut
    reg2str(str, gd->gp_msb, gd->reg_width);
    sprintf(fname, "CRC%d_0x1_%s_msb.lut", gd->gp_width, str);
    if (NULL == (fp = fopen(fname, "w"))) {
        printf("Output file error: %s\n", fname);
        return false;
    }
    fprintf(fp, "// msb-1st look-up-table for generator polynomial 0x1_%s\n", str);
    for (int data = 0; data < 256; data++) {
        clear_reg(gd->crc_msb, gd->reg_width);
        gd->crc_msb[0] = (uint8_t)data;
        process_byte_msb(gd->crc_msb, gd->gp_msb, gd->reg_width);
        reg2str(str, gd->crc_msb, gd->reg_width);
        fprintf(fp, "0x%s", str);
        if (7 == cnt) {
            fprintf(fp, ",\n");
            cnt = 0;
        } else {
            fprintf(fp, ", ");
            cnt++;
        }
    }
    fclose(fp);
    printf("Look-up-table (msb-1st) generated to file %s\n", fname);

    // lsb-1st lut
    reg2str(str, gd->gp_msb, gd->reg_width);
    sprintf(fname, "CRC%d_0x1_%s_lsb.lut", gd->gp_width, str);
    if (NULL == (fp = fopen(fname, "w"))) {
        printf("Output file error: %s\n", fname);
        return false;
    }
    cnt = 0;
    fprintf(fp, "\n// lsb-1st look-up-table for generator polynomial 0x1_%s\n", str);
    for (int data = 0; data < 256; data++) {
        clear_reg(gd->crc_lsb, gd->reg_width);
        gd->crc_lsb[0] = (uint8_t)data;
        process_byte_lsb(gd->crc_lsb, gd->gp_lsb, gd->reg_width);
        reg2str(str, gd->crc_lsb, gd->reg_width);
        fprintf(fp, "0x%s", str);
        if (7 == cnt) {
            fprintf(fp, ",\n");
            cnt = 0;
        } else {
            fprintf(fp, ", ");
            cnt++;
        }
    }
    fclose(fp);
    printf("Look-up-table (lsb-1st) generated to file %s\n", fname);
    return true;
}

/*
 * Read data stream from file and calculate CRC value.
 */
void test_data_crc(CRC_EVAL_TYP *gd) {
    for (int i = 0; i < 9; i++) {
        gd->crc_msb[0] ^= TestData[i];
        process_byte_msb(gd->crc_msb, gd->gp_msb, gd->reg_width);
        gd->crc_lsb[0] ^= TestData[i];
        process_byte_lsb(gd->crc_lsb, gd->gp_lsb, gd->reg_width);
    }

    // show results
    print_title();
    printf("CRC code generation on test string \"123456789\"\n");
    print_results(gd);
}

/*
 * Process data from named file
 */
bool data_file_crc(CRC_EVAL_TYP *gd, char *infname) {
    FILE *fp;
    uint8_t data;
    unsigned long count = 0;

    if (NULL != (fp = fopen(infname, "r"))) {
        while (1 == fread(&data, 1, 1, fp)) {
            gd->crc_msb[0] ^= data;
            process_byte_msb(gd->crc_msb, gd->gp_msb, gd->reg_width);
            gd->crc_lsb[0] ^= data;
            process_byte_lsb(gd->crc_lsb, gd->gp_lsb, gd->reg_width);
            count++;
        }
        fclose(fp);

        //show results
        print_title();
        printf("CRC code generation on data from file %s\n", infname);
        printf("%lu bytes of data processed.\n", count);
        print_results(gd);

        return true;
    } else {
        printf("Error opening data file %s\n", infname);
        return false;
    }
}

/*
 * Process data from stdin
 */
void stdin_crc(CRC_EVAL_TYP *gd) {
    uint8_t data;
    unsigned long count = 0;

    while (1 == fread(&data, 1, 1, stdin)) {
        gd->crc_msb[0] ^= data;
        process_byte_msb(gd->crc_msb, gd->gp_msb, gd->reg_width);
        gd->crc_lsb[0] ^= data;
        process_byte_lsb(gd->crc_lsb, gd->gp_lsb, gd->reg_width);
        count++;
    }

    //show results
    print_title();
    printf("CRC code generation on data from input stream (stdin).\n");
    printf("%lu bytes of data processed.\n", count);
    print_results(gd);
}

/*
 * calculate the equivalent init value for direct method given aug init value
 * this is basically generating crc with the direct method init value
 */
void get_dir_init(uint8_t *crc, uint8_t *gp, uint8_t size) {
    for (int i = 0; i < size; i++) {
        process_byte_msb(crc, gp, size);
    }
}

/*
 * calculate the equivalent init value for aug method given direct init value
 * this if the reverse process of going from crc value to init value
 */
void get_aug_init(uint8_t *crc, uint8_t *gp, uint8_t size, uint8_t bitsize) {
    uint8_t mask = 0x01;

    mask <<= (7 - ((bitsize - 1) % 8));
    for (int i = 0; i < size; i++) {
        rev_process_msb(crc, gp, size, mask);
    }
}

/*
 * Do some copying and forming registers before doing the crc
 */
void prepare_registers(CRC_EVAL_TYP *gd, char *gpstr, char *initstr) {
    int len = strlen(gpstr);

    // format all the generator polynomial forms and adjust to widths
    str2reg(gd->gp_koopman, gpstr, len);
    if (len%2) {
        gd->reg_width = (len/2) + 1;
        qdn_msb(gd->gp_koopman, gd->reg_width, 4);
    } else
        gd->reg_width = (len/2);
    gd->gp_width  = koopman2direct(gd->gp_msb, gd->gp_koopman, gd->reg_width);
    copy_reg(gd->gp_normal, gd->gp_msb, gd->reg_width);
    qdn_msb(gd->gp_normal, gd->reg_width, (gd->reg_width * 8) - gd->gp_width);
    copy_reg(gd->gp_lsb, gd->gp_msb, gd->reg_width);
    revbyte_reg(gd->gp_lsb, gd->reg_width);

    // copy init values to crc registers
    if (initstr)
        str2reg(gd->init, initstr, len);
    copy_reg(gd->dir_init, gd->init, gd->reg_width);
    copy_reg(gd->aug_init, gd->init, gd->reg_width);
    copy_reg(gd->crc_msb, gd->init, gd->reg_width);
    copy_reg(gd->crc_lsb, gd->init, gd->reg_width);
    revbyte_reg(gd->crc_lsb, gd->reg_width);

    // calculate equivalent init value for direct and augmented generation methods
    get_dir_init(gd->dir_init, gd->gp_msb, gd->reg_width);
    get_aug_init(gd->aug_init, gd->gp_msb, gd->reg_width, gd->gp_width);
}


int main(int argc, char *argv[]) {
    CRC_EVAL_TYP gendata;
    int opt = 0;                    // optarg cases
    bool arg_error = false;         // optarg error flag
    bool runtest = false;           // Gen CRC on test string "123456789" flag
    bool genlut = false;            // Gen LUT for GPOLY flag
    bool gotpoly = false;           // GPOLY specified flag
    bool doinit = false;            // Init value specified flag
    char *gpstr = NULL;             // pointer to GPOLY string in argv[]
    char *initstr = NULL;           // pointer to INIT string in argv[]
    char *datainfilename = NULL;    // pointer to data input filename string in argv[]
    bool data_from_file = false;    // read data from named file flag
    int len = 0;                    // scratch var

    // initialise main data structure first before use
    clear_crc_eval_data(&gendata);

    // Process commandline arguments
    while (-1 != (opt = getopt(argc, argv, ":tlhp:i:f:"))) {
        switch (opt) {
            case 't':   // run test string
                runtest = true;
                break;
            case 'l':   // generate lut
                genlut = true;
                break;
            case 'h':   // print usage
                print_usage(argv[0]);
                return 0;
            case 'p':   // the generator polynomial in hexadecimal format
                len = strlen(optarg);
                if ((1 > len) || (MAXCRCBITS < len*4) || (!isxstr(optarg, len))) {
                    printf("Invalid generator polynomial %s\n", optarg);
                    return 1;
                }
                gpstr = optarg;
                gotpoly = true;
                break;
            case 'i':   // Init value
                if (!isxstr(optarg, strlen(optarg))) {
                    printf("Invalid initial CRC register value %s\n", optarg);
                    return 1;
                }
                initstr = optarg;
                doinit = true;
                break;
            case 'f':   // proccess data from input file
                datainfilename = optarg;
                data_from_file = true;
                break;
            case ':':
            case '?':
            default:
                arg_error = true;
        }
    }
    
    // must have at least a valid polynomial and its degree (width)
    if (arg_error) {    
        print_usage(argv[0]);
        return 1;
    }

    // must have GPOLY
    if (!gotpoly) {
        printf("Generator polynomial must be specified!\n");
        return 1;
    }

    // check init string
    if (doinit)
        if (strlen(gpstr) != strlen(initstr)) {
            printf("Size of INIT does not match GPOLY!\n");
            return 1;
        }

    // prepare the registers
    prepare_registers(&gendata, gpstr, initstr);

    // do the crc generation
    if (runtest) {
        test_data_crc(&gendata);
    } else if (genlut) {
        if (!generate_lut(&gendata))
            return 1;
    } else if (data_from_file) {
        if (!data_file_crc(&gendata, datainfilename))
            return 1;
    } else
        stdin_crc(&gendata);

    return 0;
}
