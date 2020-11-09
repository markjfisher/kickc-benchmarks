###############################################################################
### Generic Makefile for kickc projects based on cc65 template              ###
###############################################################################

###############################################################################
### In order to override defaults - values can be assigned to the variables ###
###############################################################################

# Name of the final, single-file executable, and where main is located. No extension.
# The kickc process loads all files in together to perform full optimisation rather than
# compiling each separately and linking.
PROGRAM := suite

# Program extension. Do not include the fullstop.
# Default: xex
OUT_EXT :=

# KickC Target name, will look in TARGET_DIR if set and on default system target path for tgt file.
TARGET := benchmark

# Target folder for custom target definitions.
# Default: target
TARGET_DIR :=

# Additional C compiler flags and options.
# Default: none
CFLAGS  = -Ocoalesce -Sl

# Path(s) to additional libraries required for additional code.
# Default: lib. Additionally any *.lib dir will be add if found
LIBS :=

# Path(s) to additional header files.
# Default: include. Additionally any *.include dir will be add if found
INCLUDES :=

# Additional assembler flags and options that will be passed to kick assembler.
# Default: none
ASFLAGS :=

# Path to the directory containing main C source, and additional if not in lib subfolders.
# Default: src
SRCDIR :=

# Path to the directory where asm and other build artifact files will be stored.
# Default: build
BUILDDIR :=

# Path to the bin dir where latest copy of the output is saved
# Default: bin
BINDIR :=

# Command used to run the emulator. TODO
# Default: depending on target platform. For default (c64) target: x64 -kernal kernal -VICIIdsize -autoload
EMUCMD :=

# Options state file name. You should not need to change this, but for those
# rare cases when you feel you really need to name it differently - here you are
STATEFILE := Makefile.options

###################################################################################
####  DO NOT EDIT BELOW THIS LINE, UNLESS YOU REALLY KNOW WHAT YOU ARE DOING!  ####
###################################################################################

###############################################################################
###  Defaults to be used if nothing defined in the editable sections above  ###
###############################################################################

# Set TARGET to override.
ifeq ($(TARGET),)
  TARGET := atarixl
endif

# Set TARGET_DIR to override.
ifeq ($(TARGET_DIR),)
  TARGET_DIR := target
endif

# Set OUT_EXT to override.
ifeq ($(OUT_EXT),)
  OUT_EXT := xex
endif

# Presume the C and asm source files to be located in the subdirectory 'src'.
ifeq ($(SRCDIR),)
  SRCDIR := src
endif

# Default LIBS dir
ifeq ($(LIBS),)
  LIBS := lib
endif

# Default LIBS dir
ifeq ($(INCLUDES),)
  INCLUDES := include
endif

# Presume the asm files to be located in the subdirectory
# 'build' (which will be created). Set ASMDIR to override.
ifeq ($(BUILDDIR),)
  BUILDDIR := build
endif

# Presume the asm files to be located in the subdirectory
# 'build' (which will be created). Set ASMDIR to override.
ifeq ($(BINDIR),)
  BINDIR := bin
endif

# Work out the kickc compiler
ifeq ($(OS),Windows_NT)
  KICKC_SCRIPT := kickc.bat
else
  KICKC_SCRIPT := kickc.sh
endif

# On Windows it is mandatory to have KICKC_HOME set. So do not unnecessarily
# rely on kickc.bat/sh being added to the PATH in this scenario.
ifdef KICKC_HOME
  CC := $(KICKC_HOME)/bin/$(KICKC_SCRIPT)
else
  CC := $(KICKC_SCRIPT)
endif

# Default emulator commands and options for particular targets.
# Set EMUCMD to override.
atari_EMUCMD := atari800 -windowed -xl -pal -nopatchall -run

ifeq ($(EMUCMD),)
  EMUCMD = $(atari_EMUCMD)
endif

###############################################################################
### The magic begins                                                        ###
###############################################################################

# The "Native Win32" GNU Make contains quite some workarounds to get along with
# cmd.exe as shell. However it does not provide means to determine that it does
# actually activate those workarounds. Especially does $(SHELL) NOT contain the
# value 'cmd.exe'. So the usual way to determine if cmd.exe is being used is to
# execute the command 'echo' without any parameters. Only cmd.exe will return a
# non-empy string - saying 'ECHO is on/off'.
#
# Many "Native Win32" prorams accept '/' as directory delimiter just fine. How-
# ever the internal commands of cmd.exe generally require '\' to be used.
#
# cmd.exe has an internal command 'mkdir' that doesn't understand nor require a
# '-p' to create parent directories as needed.
#
# cmd.exe has an internal command 'del' that reports a syntax error if executed
# without any file so make sure to call it only if there's an actual argument.
ifeq ($(shell echo),)
  MKDIR = mkdir -p $1
  RMDIR = rmdir $1
  RMFILES = $(RM) $1
else
  MKDIR = mkdir $(subst /,\,$1)
  RMDIR = rmdir $(subst /,\,$1)
  RMFILES = $(if $1,del /f $(subst /,\,$1))
endif
COMMA := ,
SPACE := $(N/A) $(N/A)
define NEWLINE


endef
# Note: Do not remove any of the two empty lines above !

TARGETLIST := $(TARGET)

###############################################################
# START IF - keeping this for now to see if multiple targets makes sense
ifeq ($(words $(TARGETLIST)),1)

# Set SOURCES to main source file for kickc to then pull in other files from via includes.
SOURCES := $(PROGRAM).c

# Add to LIBS something like 'foo.lib src/bar.lib'.
LIBS += $(wildcard $(SRCDIR)/*.lib)
LIBS += $(wildcard *.lib)

# Add to LIBS something like 'foo.include src/bar.include'.
INCLUDES += $(wildcard $(SRCDIR)/*.include)
INCLUDES += $(wildcard *.include)

# Set PROGRAM_OUT to something like 'myprog.com'.
PROGRAM_OUT := $(PROGRAM).$(OUT_EXT)

.SUFFIXES:
.PHONY: all test clean copy_out

all: $(PROGRAM_OUT) copy_out

-include $(STATEFILE)

# If OPTIONS are given on the command line then save them to STATEFILE
# if (and only if) they have actually changed. But if OPTIONS are not
# given on the command line then load them from STATEFILE. Have object
# files depend on STATEFILE only if it actually exists.
ifeq ($(origin OPTIONS),command line)
  ifneq ($(OPTIONS),$(_OPTIONS_))
    ifeq ($(OPTIONS),)
      $(info Removing OPTIONS)
      $(shell $(RM) $(STATEFILE))
      $(eval $(STATEFILE):)
    else
      $(info Saving OPTIONS=$(OPTIONS))
      $(shell echo _OPTIONS_=$(OPTIONS) > $(STATEFILE))
    endif
    $(eval $(OBJECTS): $(STATEFILE))
  endif
else
  ifeq ($(origin _OPTIONS_),file)
    $(info Using saved OPTIONS=$(_OPTIONS_))
    OPTIONS = $(_OPTIONS_)
    $(eval $(OBJECTS): $(STATEFILE))
  endif
endif

# Transform the abstract OPTIONS to the actual cc65 options.
$(foreach o,$(subst $(COMMA),$(SPACE),$(OPTIONS)),$(eval $(_$o_)))

# Strip potential variant suffix from the actual kickc target.
KICKC_TARGET := $(firstword $(subst .,$(SPACE),$(TARGETLIST)))

LIBS_PARAM := $(foreach l,$(subst $(COMMA),$(SPACE),$(LIBS)),-L=$l)
INCLUDES_PARAM := $(foreach i,$(subst $(COMMA),$(SPACE),$(INCLUDES)),-I=$i)

# The remaining targets.
$(BUILDDIR):
	$(call MKDIR,$@)

$(BINDIR):
	$(call MKDIR,$@)

vpath %.c $(SRCDIR)

$(PROGRAM_OUT): $(SOURCES) | $(BUILDDIR)
	$(CC) -a $(LIBS_PARAM) $(INCLUDES_PARAM) -target=$(KICKC_TARGET) -targetdir=$(TARGET_DIR) $(CFLAGS) -odir=$(BUILDDIR) -o $@ $<

copy_out: | $(BINDIR)
	cp $(BUILDDIR)/$(PROGRAM_OUT) $(BINDIR)/

test: $(PROGRAM_OUT)
	$(PREEMUCMD)
	$(EMUCMD) $<
	$(POSTEMUCMD)

clean:
	$(call RMFILES,$(BUILDDIR)/*)

###############################################################
# ELSE
else # $(words $(TARGETLIST)),1

all test clean:
	$(foreach t,$(TARGETLIST),$(MAKE) TARGET=$t $@$(NEWLINE))

###############################################################
# ENDIF
endif # $(words $(TARGETLIST)),1

###################################################################
###  Place your additional targets in the additional Makefiles  ###
### in the same directory - their names have to end with ".mk"! ###
###################################################################
-include *.mk
