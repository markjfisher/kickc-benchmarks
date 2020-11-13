###############################################################################
### Generic Makefile for kickc projects based on cc65 template              ###
###############################################################################

###############################################################################
### In order to override defaults - values can be assigned to the variables ###
###############################################################################

# Set sources to compile, pass in as a variable to override default
# e.g. "make SOURCES=src/foo.c"
# Default: all c files in SRCDIR
SOURCES :=

# Additional C compiler flags and options.
# Default: none
CFLAGS  = -Ocoalesce -Sl

# KickC Target name, will look in TARGET_DIR if set and on default system target path for tgt file.
# This will set the target for ALL applications in the src/ dir. To individually set, use #pragma target(xxx) in source code.
# Default: none
TARGET :=

# Target folder for custom target definitions.
# Default: target
TARGET_DIR :=

# Path(s) to additional libraries required for additional code.
# Default: lib. Additionally any *.lib dir will be add if found
LIBS :=

# Path(s) to additional header files.
# Default: include. Additionally any *.include dir will be add if found
INCLUDES :=

# Additional assembler flags and options that will be passed to kick assembler.
# Default: none
ASMFLAGS :=

# Verbosity flags for kickc
VERBOSITY := -vasmout

# Emulator command, supported includes: wine_altirra, ... TODO add others if windows support reinstated (e.g. win_atari800, win_altirra)
# Default: wine_altirra (the wine emulator on linux to run altirra)
EMUCMD :=

# Set the emulator target to run. No default. Point it to absolute path of binary the emulator needs to run
EMURUN :=

# Path to the directory containing main C source, and additional if not in lib subfolders.
# Default: src
SRCDIR :=

# Path to the directory where asm and other build artifact files will be stored.
# Default: build
BUILDDIR :=

# Path to the bin dir where latest copy of the output is saved
# Default: bin
BINDIR :=

# Program extension. Do not include the fullstop.
# Default: xex
OUT_EXT :=

###################################################################################
####  DO NOT EDIT BELOW THIS LINE, UNLESS YOU REALLY KNOW WHAT YOU ARE DOING!  ####
###################################################################################

###############################################################################
###  Defaults to be used if nothing defined in the editable sections above  ###
###############################################################################

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
win_atari800 := atari800 -windowed -xl -pal -nopatchall -run
wine_altirra := wine $(ALTIRRA_HOME)/Altirra64.exe

ifeq ($(EMUCMD),)
  EMUCMD := wine_altirra
endif

MKDIR = mkdir -p $1
RMDIR = rmdir $1
RMFILES = $(RM) $1

COMMA := ,
SPACE := $(N/A) $(N/A)
define NEWLINE


endef
# Note: Do not remove any of the two empty lines above !

# If unset, set SOURCES to all c files in src dir. They must have main() methods or they will fail to compile
ifeq ($(SOURCES),)
  SOURCES := $(wildcard $(SRCDIR)/*.c)
endif
BINS := $(subst $(SRCDIR),$(BUILDDIR),$(SOURCES:.c=.$(OUT_EXT)))

# Add to LIBS something like 'foo.lib src/bar.lib'.
LIBS += $(wildcard $(SRCDIR)/*.lib)
LIBS += $(wildcard *.lib)

# Add to LIBS something like 'foo.include src/bar.include'.
INCLUDES += $(wildcard $(SRCDIR)/*.include)
INCLUDES += $(wildcard *.include)

LIBS_PARAM := $(foreach l,$(subst $(COMMA),$(SPACE),$(LIBS)),-L=$l)
INCLUDES_PARAM := $(foreach i,$(subst $(COMMA),$(SPACE),$(INCLUDES)),-I=$i)
ASMFLAGS_PARAM := $(foreach a,$(subst $(COMMA),$(SPACE),$(ASMFLAGS)),-Xassembler="$a")
ifneq ($(TARGET),)
  TARGET_PARAM := -target=$(TARGET)
endif


# Start of targets

.SUFFIXES:
.PHONY: all clean wine_altirra

all: $(BINS)

$(BUILDDIR):
	$(call MKDIR,$@)

$(BINDIR):
	$(call MKDIR,$@)

vpath %.c $(SRCDIR)

$(BUILDDIR)/%.$(OUT_EXT): $(SRCDIR)/%.c | $(BUILDDIR)
	@echo "======================================================================"
	@echo "Building $@"
	@echo "======================================================================"
	$(CC) -a $(LIBS_PARAM) $(VERBOSITY) $(INCLUDES_PARAM) $(ASMFLAGS_PARAM) $(TARGET_PARAM) -targetdir=$(TARGET_DIR) $(CFLAGS) -odir=$(BUILDDIR) -o $(notdir $@) $< && cp $(BUILDDIR)/$(notdir $@) $(BINDIR)/
	@echo "Creating Altirra debug file: $(@:.$(OUT_EXT)=.atdbg)"
	@echo ".sourcemode on" > $(@:.$(OUT_EXT)=.atdbg)
	@echo ".bc" >> $(@:.$(OUT_EXT)=.atdbg)
	@echo ".unloadsym kerneldb" >> $(@:.$(OUT_EXT)=.atdbg)
	@grep '^al ' $(@:.$(OUT_EXT)=.vs) | sed 's#al C:##g' | awk '{printf("%s %s\n", substr($$2, 2), $$1)}' | sort | awk '{printf("ya %s %06s\n", $$1, $$2)}' >> $(@:.$(OUT_EXT)=.atdbg)
	@grep "^break" $(@:.$(OUT_EXT)=.vs) | sed 's#break #bp #g' >> $(@:.$(OUT_EXT)=.atdbg)

# requires ALTIRRA_HOME set, and wine on the path.
wine_altirra:
	$($(EMUCMD)) "Z:$(subst /,\,$(EMURUN))"

clean:
	@$(call RMFILES,$(BUILDDIR)/*)
