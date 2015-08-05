################################################################################
# Project
################################################################################
PROJECT = canmon
TARGET  = atmega2560
F_CPU   = 16000000L


################################################################################
# Compiler Options
################################################################################
OPTIMIZATION = s
STANDARD     = c11


################################################################################
# Programmer Options
################################################################################
PROGRAMMER         = stk500v2
PROGRAMMER_PORT    = /dev/cu.usbmodem*
PROGRAMMER_OPTIONS = -b 115200


################################################################################
# Directories
################################################################################
BUILD_DIR     = ./build
INCLUDE_DIR   = ./include
SOURCE_DIR    = ./src


################################################################################
# Tools
################################################################################
CC      = avr-gcc
AR      = avr-ar
LD      = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE    = avr-size
AVRDUDE = avrdude
REMOVE  = rm


################################################################################
# Source Files
################################################################################
GCC_SOURCES = $(shell find $(SOURCE_DIR) -type f -name '*.c')

OBJECTS = $(GCC_SOURCES:.c=.o)


################################################################################
# Tool Flags
################################################################################
GCC_FLAGS  = -g -std=$(STANDARD) -mmcu=$(TARGET) -O$(OPTIMIZATION) -DF_CPU=$(F_CPU) -fpack-struct -fshort-enums -funsigned-bitfields -funsigned-char -Wall -Wstrict-prototypes
GCC_FLAGS += $(patsubst %,-I%,$(INCLUDE_DIR)) -I.

LD_FLAGS  = -mmcu=$(TARGET) -DF_CPU=$(F_CPU)
LD_FLAGS += $(patsubst %,-I%,$(INCLUDE_DIR)) -I.

AVRDUDE_FLAGS = -q -q -q -q -F -p $(TARGET) -c $(PROGRAMMER) -P $(PROGRAMMER_PORT) $(PROGRAMMER_OPTIONS)


############################################################
# Verbosity
############################################################
ifeq ($V, 1)
	VERBOSE =
else
	VERBOSE = @
endif


################################################################################
# Targets: Actions
################################################################################
.SUFFIXES: .c .eep .h .hex .o .out .s .S
.PHONY: all disasm hex upload clean

all: $(BUILD_DIR)/$(PROJECT).out

disasm: $(BUILD_DIR)/$(PROJECT).out
	@echo objdump $<
	$(VERBOSE) $(OBJDUMP) -D -S $< > $(BUILD_DIR)/$(PROJECT).s

hex: all $(BUILD_DIR)/$(PROJECT).hex $(BUILD_DIR)/$(PROJECT).eep

upload: hex
	$(VERBOSE) $(AVRDUDE) $(AVRDUDE_FLAGS) -U flash:w:$(BUILD_DIR)/$(PROJECT).hex # -U eeprom:w:$(BUILD_DIR)/$(PROJECT).eep

clean:
	$(VERBOSE) find . \( -type f -name '*.o' -o -name '*.s' -o -name '*.out' -o -name '*.hex' \) -exec $(REMOVE) {} \;


################################################################################
# Targets: Output
################################################################################
$(BUILD_DIR)/$(PROJECT).out: $(OBJECTS)
	@echo ld $@
	$(VERBOSE) $(LD) -o $@ $(OBJECTS) $(LD_FLAGS)

%.o: %.c
	@echo cc $<
	$(VERBOSE) $(CC) $(GCC_FLAGS) -c $< -o $@

%.hex: %.out
	@echo objecopy $<
	$(VERBOSE) $(OBJCOPY) -O ihex -R .eeprom $< $@
	
%.eep: %.out
	@echo objcopy $<
	$(VERBOSE) $(OBJCOPY) -O ihex -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 $< $@
