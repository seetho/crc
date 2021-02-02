PROGS_16 = test16_09 test16_62CC test16_8810 test16_82608EDB
PROGS_256 = test256_09 test256_62CC test256_8810 test256_82608EDB
COMPILER = /usr/bin/gcc
COMP_FLAGS = -Wall -std=c17
DBG_FLAGS := $(COMP_FLAGS) -g -O0
DIST_FLAGS := $(COMP_FLAGS) -O3

all: $(PROGS_16) $(PROGS_256)

test16_09: test16_09.c
	$(COMPILER) $(DIST_FLAGS) -o $@ $< 
test256_09: test256_09.c
	$(COMPILER) $(DIST_FLAGS) -o $@ $< 

test16_62CC: test16_62CC.c
	$(COMPILER) $(DIST_FLAGS) -o $@ $< 
test256_62CC: test256_62CC.c
	$(COMPILER) $(DIST_FLAGS) -o $@ $< 

test16_8810: test16_8810.c
	$(COMPILER) $(DIST_FLAGS) -o $@ $< 
test256_8810: test256_8810.c
	$(COMPILER) $(DIST_FLAGS) -o $@ $< 

test16_82608EDB: test16_82608EDB.c
	$(COMPILER) $(DIST_FLAGS) -o $@ $< 
test256_82608EDB: test256_82608EDB.c
	$(COMPILER) $(DIST_FLAGS) -o $@ $< 

clean:
	rm $(PROGS_16)
	rm $(PROGS_256)
