PROGNAME = crc
PROGLIB = crclib
COMPILER = /usr/bin/gcc
COMP_FLAGS = -Wall -std=gnu17
DBG_FLAGS := $(COMP_FLAGS) -g -O0
DIST_FLAGS := $(COMP_FLAGS) -O3

all: $(PROGNAME)
debug: $(PROGNAME)

$(PROGNAME): $(PROGNAME).c $(PROGLIB).o
	$(COMPILER) $(DBG_FLAGS) -o $@ $^ 

$(PROGLIB).o: $(PROGLIB).c $(PROGLIB).h
	$(COMPILER) $(DBG_FLAGS) -o $@ -c $<

dist: $(PROGNAME).c $(PROGLIB).c $(PROGLIB).h
	$(COMPILER) $(DIST_FLAGS) -o $(PROGNAME) $(PROGNAME).c $(PROGLIB).c 

distclean: $(PROGNAME).c $(PROGLIB).c $(PROGLIB).h
	rm $(PROGNAME) *.o
	$(COMPILER)) $(DIST_FLAGS) -o $(PROGNAME) $(PROGNAME).c $(PROGLIB).c 

clean:
	rm $(PROGNAME) *.o
