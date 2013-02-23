##################################################
# User settings are validated in the region below;
##################################################

# Include in user settings;
include Setting.mk
# This list conducts 'make' to generate definition flags.
SETTING_ALL = motion scheduler network pack traffic node logger

# Validate all user settings with the above way;
define validate
$(if $(filter $($(1)),$($(1)_all)),,$(error Invalid $(1) '$($(1))'; must be one of: $($(1)_all)))
endef
$(foreach SETTING,$(SETTING_ALL),$(call validate,$(SETTING)))

# Create symbolic links now;
$(foreach SETTING,$(SETTING_ALL),$(shell [[ -f $(SETTING)/$($(SETTING)).h ]] && ln -s -f $($(SETTING)).h $(SETTING)/$(SETTING).h))

##################################################
# Parameters are set in the region below;
##################################################

# Generate user setting flag for compilation;
ALL_SETTING_FLAG = $(foreach SETTING,$(SETTING_ALL),-DH$(SETTING)=$($(SETTING)))

# Set basic parameters;
V = @
CC = g++
CC_FLAG = -c -O3 -I. -Wall $(ALL_SETTING_FLAG) -std=c++0x
LD = g++
LD_LIB = -lm -lrt -lboost_program_options
RM = rm -rf
MKDIR = mkdir -p
DE = g++ -MM
MAKE = make -s
ECHO = echo

# Our final target;
TARGET = ../umnets

# Source, obj and dependent files, separated with a space;
SRC = $(shell find . -name '*.cc')
OBJ = $(SRC:.cc=.o)
DEP = $(SRC:.cc=.d)

##################################################
# The rules come in the region below;
##################################################

# Default target: generate the executable;
$(TARGET): $(OBJ)
	$(V)$(ECHO) LD: $^
	$(V)$(LD) $^ $(LD_LIB) -o $@

ifneq ($(MAKECMDGOALS),clean)
-include $(DEP)
endif

$(OBJ): Setting.mk

$(DEP): Setting.mk

# Initial way to generate dependency files.
%.d: %.cc
	$(V)$(ECHO) DEP: $<
	$(V)$(DE) $(CC_FLAG) -MT '$*.o' -o $@ $<

# Generate obj files.
%.o: %.cc
	$(V)$(ECHO) CC: $<
	$(V)$(CC) $(CC_FLAG) -o $@ $<

# Clean: obj, dependency, link and executable files;
.PHONY: clean
clean:
	$(V)$(RM) $(shell find . -type l)
	$(V)$(RM) $(shell find . -name '*.o')
	$(V)$(RM) $(shell find . -name '*.d')
	$(V)$(RM) $(TARGET)

