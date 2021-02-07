MAINPROG = testcrc
PROG4 = crc4
PROG15 = crc15
PROG16 = crc16
PROG32 = crc32
COMPILER = /usr/bin/gcc
COMP_FLAGS = -Wall -std=c17
DBG_FLAGS := $(COMP_FLAGS) -g -O0
DIST_FLAGS := $(COMP_FLAGS) -O3

all: $(PROG4) $(PROG15) $(PROG16) $(PROG32)

$(PROG4): $(MAINPROG).c 
	$(COMPILER) $(DBG_FLAGS) -D COMP_CRC4 -o $@ $<

$(PROG15): $(MAINPROG).c 
	$(COMPILER) $(DBG_FLAGS) -D COMP_CRC15 -o $@ $< 

$(PROG16): $(MAINPROG).c
	$(COMPILER) $(DBG_FLAGS) -D COMP_CRC16 -o $@ $<

$(PROG32): $(MAINPROG).c
	$(COMPILER) $(DBG_FLAGS) -D COMP_CRC32 -o $@ $< 

clean:
	rm $(PROG4)
	rm $(PROG15) 
	rm $(PROG16)
	rm $(PROG32)
