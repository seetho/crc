PROGS = crc4 crc15 crc16 crc32
COMPILER = /usr/bin/gcc
COMP_FLAGS = -Wall -std=c17
DBG_FLAGS := $(COMP_FLAGS) -g -O0
DIST_FLAGS := $(COMP_FLAGS) -O3

all: $(PROGS)

crc4: 
	$(COMPILER) $(DBG_FLAGS) -D __CRC4__ -o $@ testcrc.c 

crc15: 
	$(COMPILER) $(DBG_FLAGS) -D __CRC15__ -o $@ testcrc.c 

crc16: 
	$(COMPILER) $(DBG_FLAGS) -D __CRC16__ -o $@ testcrc.c 

crc32: 
	$(COMPILER) $(DBG_FLAGS) -D __CRC32__ -o $@ testcrc.c 

clean:
	rm $(PROGS)
