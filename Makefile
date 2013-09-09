SHELL = /bin/bash

##################################################
# User settings are validated in the region below.
##################################################

# Include in user settings.
include Setting.mk
# This list conducts 'make' to generate definition flags.
export SETTING_ALL = motion scheduler network packet traffic node logger

# Validate all user settings with the above way.
define validate
$(if $(filter $($(1)),$($(1)_all)),,$(error Invalid $(1) '$($(1))'; must be one of: $($(1)_all)))
endef
$(foreach SETTING,$(SETTING_ALL),$(call validate,$(SETTING)))

##################################################
# Parameters are set in the region below.
##################################################

# Generate user setting flag for compilation.
export ALL_SETTING_FLAG = $(foreach SETTING,$(SETTING_ALL),-DH$(SETTING)=$($(SETTING)))

# Set basic parameters.
export V = @
export CC = g++ -c
export DE = g++ -MM
export LD = g++
export DOXY = doxygen
export CC_FLAG = -O3 -Isrc -Wall $(ALL_SETTING_FLAG) -std=c++0x
export LD_LIB = -lm -lrt -lboost_program_options
export RM = rm -rf
export MKDIR = mkdir -p
export MAKE = make -s
export ECHO = echo

# Our final target.
export TARGET = umnets

# Source, obj and dependent files, separated with a space.
export SRC = $(foreach SETTING,$(SETTING_ALL),src/$(SETTING)/$($(SETTING)).cc)\
						 $(shell find src/driver -name "*.cc")\
						 $(shell find src/util -name "*.cc")
export OBJ = $(SRC:.cc=.o)
export DEP = $(SRC:.cc=.d)

##################################################
# The rules come in the region below.
##################################################

# Default target: generate the executable.
.PHONY: all
all: make_link
	$(V)$(MAKE) -f src/Makefile $(TARGET)

# Make symbolic links.
.PHONY: make_link
make_link: $(addsuffix .make_link,$(SETTING_ALL))

%.make_link:
	$(V)[[ -f src/$*/$($*).h ]] && ln -s -f $($*).h src/$*/$*.h

# Make doxygen docs.
.PHONY: docs
docs: make_link
	$(V)$(MKDIR) docs
	$(V)$(DOXY) Doxyfile > /dev/null

# Clean: obj, dependency, link, docs and executable files.
.PHONY: clean
clean:
	$(V)$(RM) $(shell find src -type l)
	$(V)$(RM) $(shell find src -name '*.o')
	$(V)$(RM) $(shell find src -name '*.d')
	$(V)$(RM) $(TARGET)
	$(V)$(RM) docs

