

#BSP PATH
# Output directories
#ARM_GCC_TOOLCHAIN=C:\cygwin\home\Arduino-1.5.2-nuvoton\build\windows\work\hardware\tools\g++_arm_none_eabi\bin
#PROJECT_BASE_PATH=C:\Users\SChung\Desktop\system
#CORE=cortex-m0
#CHIP=M051Series
#OUTPUT_BIN=.\
# ---------------------------------------------------------------------------------------

# Tool suffix when cross-compiling
CROSS_COMPILE = $(ARM_GCC_TOOLCHAIN)/arm-none-eabi-

# Compilation tools
AR = $(CROSS_COMPILE)ar
CC = $(CROSS_COMPILE)gcc
AS = $(CROSS_COMPILE)as
NM = $(CROSS_COMPILE)nm
ifeq ($(OS),Windows_NT)
RM=cs-rm -Rf
#RM=del /s /f
else
RM=rm -Rf
endif

SEP=/

# ---------------------------------------------------------------------------------------
# C Flags

CFLAGS += -Wall -Wchar-subscripts -Wcomment -Wformat=2 -Wimplicit-int
CFLAGS += -Werror-implicit-function-declaration -Wmain -Wparentheses
CFLAGS += -Wsequence-point -Wreturn-type -Wswitch -Wtrigraphs -Wunused
CFLAGS += -Wuninitialized -Wunknown-pragmas -Wfloat-equal -Wundef
CFLAGS += -Wshadow -Wpointer-arith -Wbad-function-cast -Wwrite-strings
CFLAGS += -Wsign-compare -Waggregate-return -Wstrict-prototypes
CFLAGS += -Wmissing-prototypes -Wmissing-declarations
CFLAGS += -Wformat -Wmissing-format-attribute -Wno-deprecated-declarations
CFLAGS += -Wpacked -Wredundant-decls -Wnested-externs -Winline -Wlong-long
CFLAGS += -Wunreachable-code
CFLAGS += -Wcast-align
#CFLAGS += -Wmissing-noreturn
#CFLAGS += -Wconversion

# To reduce application size use only integer printf function.
CFLAGS += -Dprintf=iprintf

CFLAGS += --param max-inline-insns-single=500 -mcpu=$(CORE) -mthumb -mlong-calls -ffunction-sections -fdata-sections -std=c99
CFLAGS += $(OPTIMIZATION) $(INCLUDES) -D$(CHIP)


# ---------------------------------------------------------------------------------------
# ASM Flags

ASFLAGS = -mcpu=$(CORE) -mthumb -Wall -a -g $(INCLUDES)


# ---------------------------------------------------------------------------------------
#BSP compiler 
CMSIS_ARM_PATH=$(PROJECT_BASE_PATH)/CMSIS/Include
CMSIS_DEVICE_PATH=$(PROJECT_BASE_PATH)/Device
CMSIS_STDDRIVER_PATH=$(PROJECT_BASE_PATH)/StdDriver

#-------------------------------------------------------------------------------
# Files
#-------------------------------------------------------------------------------

vpath %.h $(CMSIS_DEVICE_PATH)/Nuvoton/$(CHIP)/Include $(CMSIS_ARM_PATH) $(CMSIS_STDDRIVER_PATH)/inc
vpath %.c $(CMSIS_DEVICE_PATH)/Nuvoton/$(CHIP)/Source/ $(CMSIS_DEVICE_PATH)/Nuvoton/$(CHIP)/Source/GCC $(CMSIS_ARM_PATH) $(CMSIS_STDDRIVER_PATH)/src

VPATH+=$(CMSIS_DEVICE_PATH)/Nuvoton/$(CHIP)/Source/GCC
VPATH+=$(CMSIS_DEVICE_PATH)/Nuvoton/$(CHIP)/Source/
VPATH+=$(CMSIS_ARM_PATH)
VPATH+=$(CMSIS_STDDRIVER_PATH)/src

INCLUDES = -I$(CMSIS_DEVICE_PATH)/Nuvoton/$(CHIP)/Include
INCLUDES += -I$(CMSIS_ARM_PATH)
INCLUDES += -I$(CMSIS_STDDRIVER_PATH)/inc

#-------------------------------------------------------------------------------
ifdef DEBUG
OPTIMIZATION = -g -O0 -DDEBUG
else
OPTIMIZATION = -Os
endif

#-------------------------------------------------------------------------------
ifdef DEBUG
OUTPUT_OBJ=debug
OUTPUT_LIB=libchip_$(CHIP)_gcc_dbg.a
else
OUTPUT_OBJ=release
OUTPUT_LIB=libchip_$(CHIP)_gcc_rel.a
endif

OUTPUT_PATH=$(OUTPUT_BIN)/$(OUTPUT_OBJ)_$(CHIP)

#-------------------------------------------------------------------------------
# C source files and objects
#-------------------------------------------------------------------------------
C_SRC=$(wildcard $(CMSIS_ARM_PATH)/*.c)
C_SRC+=$(wildcard $(CMSIS_DEVICE_PATH)/Nuvoton/$(CHIP)/Source/*.c)
C_SRC+=$(wildcard $(CMSIS_STDDRIVER_PATH)/src/*.c)
C_SRC+=$(wildcard $(CMSIS_DEVICE_PATH)/Nuvoton/$(CHIP)/Source/GCC/*.c)


C_OBJ_TEMP=$(patsubst %.c, %.o, $(notdir $(C_SRC)))

# during development, remove some files
C_OBJ_FILTER=

C_OBJ=$(filter-out $(C_OBJ_FILTER), $(C_OBJ_TEMP))

#-------------------------------------------------------------------------------
# Assembler source files and objects
#-------------------------------------------------------------------------------
A_SRC=$(wildcard $(CMSIS_DEVICE_PATH)/Nuvoton/$(CHIP)/Source/GCC/*.s)

A_OBJ_TEMP=$(patsubst %.s, %.o, $(notdir $(A_SRC)))

# during development, remove some files
A_OBJ_FILTER=

A_OBJ=$(filter-out $(A_OBJ_FILTER), $(A_OBJ_TEMP))

#-------------------------------------------------------------------------------
# Rules
#-------------------------------------------------------------------------------
all: $(CHIP)

$(CHIP): create_output $(OUTPUT_LIB)

.PHONY: create_output
create_output:
	@echo ------------------------------------------------------------
	@echo ARM_GCC_TOOLCHAIN=$(ARM_GCC_TOOLCHAIN)
	@echo PROJECT_BASE_PATH=$(PROJECT_BASE_PATH)
	@echo CORE =$(CORE)
	@echo CHIP=$(CHIP)
	@echo OUTPUT_BIN=$(OUTPUT_BIN)
	@echo ------------------------------------------------------------	
	@echo Compiler Information :
$(addprefix $(OUTPUT_PATH)/,$(C_OBJ)): $(OUTPUT_PATH)/%.o: %.c
	@"$(CC)" -fms-extensions -c $(CFLAGS) $< -o $@ 

$(addprefix $(OUTPUT_PATH)/,$(A_OBJ)): $(OUTPUT_PATH)/%.o: %.s
	@"$(AS)" $(ASFLAGS) $< -o $@
	
$(OUTPUT_LIB): $(addprefix $(OUTPUT_PATH)/, $(C_OBJ)) $(addprefix $(OUTPUT_PATH)/, $(A_OBJ))
	@"$(AR)" -r "$(OUTPUT_BIN)/$@" $^
	@"$(NM)" "$(OUTPUT_BIN)/$@" > "$(OUTPUT_BIN)/$@.txt"

.PHONY: clean
clean:
	@echo ------------------------------------------------------------------------------------
	@echo --- Cleaning $(CHIP) files $(OUTPUT_PATH) $(subst /,$(SEP),$(OUTPUT_BIN)/$(OUTPUT_LIB))
	-@$(RM) $(OUTPUT_PATH) 1>$(DEV_NUL) 2>&1
	-@$(RM) $(subst /,$(SEP),$(OUTPUT_BIN)/$(OUTPUT_LIB)) 1>$(DEV_NUL) 2>&1
	-@$(RM) $(subst /,$(SEP),$(OUTPUT_BIN)/$(OUTPUT_LIB)).txt 1>$(DEV_NUL) 2>&1
	@echo ------------------------------------------------------------------------------------

# dependencies
$(addprefix $(OUTPUT_PATH)/,$(C_OBJ)): $(OUTPUT_PATH)/%.o: $(wildcard $(CMSIS_DEVICE_PATH)/Nuvoton/$(CHIP)/Include/*.h) $(wildcard $(CMSIS_BASE_PATH)/*.h)