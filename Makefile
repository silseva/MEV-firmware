##
## Makefile for Miosix embedded OS
##
MAKEFILE_VERSION := 1.09
GCCMAJOR := $(shell arm-miosix-eabi-gcc --version | \
                    perl -e '$$_=<>;/\(GCC\) (\d+)/;print "$$1"')
## Path to kernel directory (edited by init_project_out_of_git_repo.pl)
KPATH := lib/miosix-kernel/miosix
## Path to config directory (edited by init_project_out_of_git_repo.pl)
CONFPATH := lib/
include $(CONFPATH)/config/Makefile.inc

##
## List here subdirectories which contains makefiles
##
SUBDIRS := $(KPATH) lib/mxgui

##
## List here your source files (both .s, .c and .cpp)
##
SRC_COMMON :=                           \
src/graphics/icons/warning_50x50.cpp    \
src/graphics/ConfirmBox.cpp             \
src/graphics/DisplayBox.cpp             \
src/graphics/Rectangle.cpp              \
src/graphics/TextBox.cpp                \
src/graphics/Button.cpp                 \
src/graphics/Keypad.cpp                 \
src/drivers/display_stm32.cpp           \
src/drivers/calibration.cpp             \
src/drivers/ADC122S021.cpp              \
src/drivers/flash.cpp                   \
src/common/PidRegulator.cpp             \
src/common/Persistence.cpp

SRC_BED :=                              \
src/Bed/UI/UiStateMain.cpp              \
src/Bed/UI/UiStateInputValue.cpp        \
src/Bed/UI/UiStateCalSensors.cpp        \
src/Bed/UI/UiStateSetup.cpp             \
src/Bed/AnalogSensors.cpp               \
src/Bed/ValveController.cpp             \
src/Bed/SensorSampler.cpp               \
src/bedMain.cpp

SRC_BJ :=                               \
src/BellJar/UI/UiStateConfAut.cpp       \
src/BellJar/UI/UiStateConfMan.cpp       \
src/BellJar/UI/UiStateConfSp.cpp        \
src/BellJar/UI/UiStateMain.cpp          \
src/BellJar/UI/UiStateInputVal.cpp      \
src/BellJar/UI/UiStateConfigPid.cpp     \
src/BellJar/UI/UiStateConfigInput.cpp   \
src/BellJar/LevelController.cpp         \
src/drivers/Blower.cpp                  \
src/bjMain.cpp

SRC_CALIB :=                            \
src/Bed/AnalogSensors.cpp               \
src/Bed/ValveController.cpp             \
src/Bed/SensorSampler.cpp               \
src/calibMain.cpp

# SRC := $(SRC_BED) $(SRC_COMMON)
# SRC := $(SRC_BJ)  $(SRC_COMMON)
SRC := $(SRC_CALIB) $(SRC_COMMON)

##
## List here additional static libraries with relative path
##
LIBS := lib/mxgui/libmxgui.a

##
## List here additional include directories (in the form -Iinclude_dir)
##
INCLUDE_DIRS := -Ilib/mxgui -Isrc

##############################################################################
## You should not need to modify anything below                             ##
##############################################################################

ifeq ("$(VERBOSE)","1")
Q :=
ECHO := @true
else
Q := @
ECHO := @echo
endif

## Replaces both "foo.cpp"-->"foo.o" and "foo.c"-->"foo.o"
OBJ := $(addsuffix .o, $(basename $(SRC)))

## Includes the miosix base directory for C/C++
## Always include CONFPATH first, as it overrides the config file location
CXXFLAGS := $(CXXFLAGS_BASE) -I$(CONFPATH) -I$(CONFPATH)/config/$(BOARD_INC)  \
            -I. -I$(KPATH) -I$(KPATH)/arch/common -I$(KPATH)/$(ARCH_INC)      \
            -I$(KPATH)/$(BOARD_INC) $(INCLUDE_DIRS)
CFLAGS   := $(CFLAGS_BASE)   -I$(CONFPATH) -I$(CONFPATH)/config/$(BOARD_INC)  \
            -I. -I$(KPATH) -I$(KPATH)/arch/common -I$(KPATH)/$(ARCH_INC)      \
            -I$(KPATH)/$(BOARD_INC) $(INCLUDE_DIRS)
AFLAGS   := $(AFLAGS_BASE)
LFLAGS   := $(LFLAGS_BASE)
DFLAGS   := -MMD -MP

## libmiosix.a is among stdlibs because needs to be within start/end group
STDLIBS  := -lmiosix -lstdc++ -lc -lm -lgcc

ifneq ($(GCCMAJOR),4)
	STDLIBS += -latomic
endif

LINK_LIBS := $(LIBS) -L$(KPATH) -Wl,--start-group $(STDLIBS) -Wl,--end-group

all: all-recursive main

clean: clean-recursive clean-topdir

program:
	$(PROGRAM_CMDLINE)

all-recursive:
	$(foreach i,$(SUBDIRS),$(MAKE) -C $(i)                               \
	  KPATH=$(shell perl $(KPATH)/_tools/relpath.pl $(i) $(KPATH))       \
	  CONFPATH=$(shell perl $(KPATH)/_tools/relpath.pl $(i) $(CONFPATH)) \
	  || exit 1;)

clean-recursive:
	$(foreach i,$(SUBDIRS),$(MAKE) -C $(i)                               \
	  KPATH=$(shell perl $(KPATH)/_tools/relpath.pl $(i) $(KPATH))       \
	  CONFPATH=$(shell perl $(KPATH)/_tools/relpath.pl $(i) $(CONFPATH)) \
	  clean || exit 1;)

clean-topdir:
	-rm -f $(OBJ) main.elf main.hex main.bin main.map $(OBJ:.o=.d)

main: main.elf
	$(ECHO) "[CP  ] main.hex"
	$(Q)$(CP) -O ihex   main.elf main.hex
	$(ECHO) "[CP  ] main.bin"
	$(Q)$(CP) -O binary main.elf main.bin
	$(Q)$(SZ) main.elf

main.elf: $(OBJ) all-recursive
	$(ECHO) "[LD  ] main.elf"
	$(Q)$(CXX) $(LFLAGS) -o main.elf $(OBJ) $(KPATH)/$(BOOT_FILE) $(LINK_LIBS)

%.o: %.s
	$(ECHO) "[AS  ] $<"
	$(Q)$(AS)  $(AFLAGS) $< -o $@

%.o : %.c
	$(ECHO) "[CC  ] $<"
	$(Q)$(CC)  $(DFLAGS) $(CFLAGS) $< -o $@

%.o : %.cpp
	$(ECHO) "[CXX ] $<"
	$(Q)$(CXX) $(DFLAGS) $(CXXFLAGS) $< -o $@

#pull in dependecy info for existing .o files
-include $(OBJ:.o=.d)
