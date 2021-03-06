CRC code and LUT generator for polynomials up to 128 bits.


Copyright
=========
(c) T K See Tho 2020-12-27
tkseetho@gmail.com 
GPG public key signature F489 A852 333D 5FBD CF6E  26DF AD5A B2C4 24BC 86B9

This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
https://creativecommons.org/licenses/by-nc-sa/4.0/


Usage
=====
    Usage: crc -pGPOLY [-iINIT] [-fINFILE | -t | -l | -L | -h]

        -p  <GPOLY>: Generator polynomial (in Koopman notation, max 128 bits).
        -i   <INIT>: Initial value of CRC register (high-order bit justified).
        -f <INFILE>: Input data file (default from stdin).
        -t           Set to run on test string "123456789" (0x313233343536373839).
        -l           Set to generate 16-element Look-Up-Table.
        -L           Set to generate 256-element Look-Up-Table.
        -h           Print this usage message.


History
=======
2021-02-07
    Improved the crc functions in testcrc.c for efficiency and elegance.
    Added crcl256.c program to test 256-element LUT binary data files.
    Added crcl16msb.c and crcl16lsb.c to test 16-element LUT binary data files.
    Modified test Makefile.
    
2021-02-06 (6.2.0)
    Change LUT data array declaration as a header file (with .h extension).
    Also output LUT as binary data file (with .lut extension). This will be useful for programs that use it's own data structure format for the LUT.

2021-02-04 (r6.1.1)
    Modified code of register bit shifting functions in crclib.c to be more readable.

2021-02-03
    Removed __ from some directive identifiers in test program. Shouldn't begin any identifiers with underscore.

2021-02-02 (r6.1.0)
    LUT values formatted as an array of array of bytes; i.e. uint8_t lut[][] {...
    Programs using the LUT will only have to include the file and use the arrays lut_msb or lut_lsb.
    This makes the use of LUT easier, especially the 16-element lsb LUT where the nibbles have to be processed in reversed order.
    Re-coding all the LUT test programs to make it more generic; so only minor modifications is required to adapt it to different LUTs.
    There are 2 flavours: one for 16-element LUTs and another for 256-element LUTs. These can be further optimised if needed.
    
2021-02-01 (r6.0.0)
    Updated 256 element LUT output filename to reflect Koopman style polynomial naming.
    Change option switch from -l to -L to generate 256 element LUT.
    Use -l option to generate 16-element LUT. Added process_nibble_msb() and process_nibble_lsb() functions to crclib.c.
    Moved LUTs and test programs into the ../test subdirectory.
    Updated usage message.

2021-01-31 (r5.5.1)
    Tested CRC-15/CAN LUT.
    Corrected some typos and improved some descriptions.

2021-01-30
    Generated LUTs for CRC-4/ITU G.704, CRC-5/ITU G.704, CRC-5/USB,
    CRC-16/CCITT and some small test codes to verify them.

2021-01-18 (r5.5)
    Consolidated stlib.c/stlib.h into crclib.c/crclib.h.
    Tweaked qup_msb, qdn_msb, qup_lsb, qdn_lsb to shift n number of bits.
    Removed removeleadzeros() function which is useless.
    Moved TestData out of the test function so that it may be available for 
        other functions.
    Preparing to use Git as a version control tool, and GitHub to place
        the project. It can be shared with others from there.

2021-01-14 (r5.4)
    Change some result display formats. The 'Normal' GPOLY is actually closer
        to the mathematical expression of a polynomial. I will now use the
        term 'Direct msb' to refer to the msb aligned GPOLY ready for use
        directly. The other version is of course 'Direct lsb'.
    Removed the (forgotten) -w option from optarg list.

2021-01-13 (r5.3)
    Fixed bug in str2reg() to cater for odd number of hex character in the 
        string.
    Fixed a bug that did not recognise single hex digit polynomial string 
        properly.
    Do more checking of INIT string. It must be the same length as the GPOLY,
        msb justified. It will be used as is.
    Tidy up some terminology and the display of parameter values and results.

2021-01-12 (r5.2)
    Put in conversion from Koopman to normal notation.
    Generator polynomial is now displayed in 3 different notations:
        1. The full version with implicit high bit
        2. The normal version (lsb justified)
        3. The Koopman version

2021-01-11 (r5.1)
    Added option to generate look up table for a particular generator 
        polynomial. Generated tables tested OK, so the values generated for msb
        and lsb poly are good.
    Cleanup some code and comments.
    Remove the option to specify gpoly width. The Koopman notation has the 
        advantage of not needing it. You can always count it while shifting 
        out the high bit.

2021-01-10 (r5.0)
    Use a single structure to hold the essential registers and parameter;
        making function calls cleaner.
    Forget about the Rocksoft model and do it my way.
    Moved CRC routines to separate library crclib.c and crclib.h.
    Tidy up functions in stlib.c.
    Use Koopman notation only for generator polynomial. This makes parsing the
        polynomial consistent.
    Include option to run on test string "123456789".
    Added calculation of equivalent init values.
    Removed output data file. Not practical since it's doing multiple CRC 
        generation methods at once.

2021-01-07 (r4.2)
    Changed usage message and result reporting to note the incorrect Rocksoft
        model use of Refout (Revout) and XORout thus displaying incorrect 
        reversed results.
    Code cleanup.

2021-01-06 (r4.1)
    Added -b option to specify polynomial bit widths.
    Pad poly with 0 in front if odd number of nibbles.
    Added -k option to specify Koopman notation polys.
    Change bit shifting of register to treat the CRC register as a bit queue 
        that moves forward or backward.
    Code cleanup.

2021-01-05 (r4.0)
    Change terminology - "non-direct" method to "augmented" method.
    Modify data structures making augmented method more efficient and more
        similar in how the direct method is done.
    Change some variable and constant names to make them more representative of
        what they do.
    Corrected some incorrect prompts.
    Added xor_regs() function.
    Don't reflect Init when Refin - shouldn't be necessary.
    Remove debugging prn() function. Use GDB instead!
    "Refout" should be "Revout" - reverse byte order. Not reflected.
    Fixed input and output data filename string reference.
    Code cleanup and comments.

2020-12-31 (r3.2)
    Added print help option.
    Moved some functions to stlib.c.
    Code clean up and adding comments.

2020-12-30 (r2.1)
    Added option to write data + CRC value to a file for verification.
    CRC value is appended in correct order and reflection according to the 
        given parameters to generate it.

2020-12-28 (r.2.0)
    Move some functions to a separate library.
    Reflect poly instead of all data bytes to do RefIn mode.
    Clean up code.
    So far it tested ok against polynomials of a multiple of 8-bits.

2020-12-27 (r1.0)
    Calculate CRC value for given data.

