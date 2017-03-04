# CS165 MakeFile
# This file includes automatic dependency tracking
# Please see example of "utils" to see how to add additional file
# to your project

all: randmst

# C-compiler settings
CC = g++ 

# Default optimization level
O ?= 3

# Flags and other libraries
override CFLAGS += -Wall -Wextra -pedantic -pthread -O$(O) -I$(INCLUDES)
LIBS = 
INCLUDES = include


####### Automatic dependency magic #######
# Set-up dependency directory
DEPSDIR := .deps
BUILDSTAMP := $(DEPSDIR)/rebuildstamp
DEPFILES := $(wildcard $(DEPSDIR)/*.d)
ifneq ($(DEPFILES),)
include $(DEPFILES)
endif
DEPCFLAGS = -MD -MF $(DEPSDIR)/$*.d -MP

# Dependency compilation
ifneq ($(DEP_CC),$(CC) $(CFLAGS) $(DEPCFLAGS) $(O))
DEP_CC := $(shell mkdir -p $(DEPSDIR); echo >$(BUILDSTAMP); echo "DEP_CC:=$(CC) $(CFLAGS) $(DEPCFLAGS) $(O)" >$(DEPSDIR)/_cc.d)
endif

# Make sure dependency directories are generated
$(DEPSDIR)/stamp $(BUILDSTAMP):
	mkdir -p $(@D)
	touch $@

####### Automatic dependency magic #######

%.o : %.cpp $(BUILDSTAMP)
	$(CC) $(CFLAGS) $(DEPCFLAGS) -O$(O) -o $@ -c $<

##
# To include additional non-executable files (e.g. selects.c, utils.c, etc),
# you'll need to add an additional build dependency to the file that requires
# the new file.  For example, see that client and server both require utils.o
#
# If you create a new file such as selects.c, then you will need a "selects.o"
# dependency on the right side of whichever one requires the file.
##

randmst: main.o Graph.o
	$(CC) $(CFLAGS) $(DEPCFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f randmst *.o *~ *.bak core *.core
	rm -rf .deps

distclean: clean
	rm -rf $(DEPSDIR)
	rm -rf *.txt

.PHONY: all clean distclean
