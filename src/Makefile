$(TARGET): $(OBJ)
	$(V)$(ECHO) LD: $^
	$(V)$(LD) $^ $(LD_LIB) -o $@

-include $(DEP)

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

