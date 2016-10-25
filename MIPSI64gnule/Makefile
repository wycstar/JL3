# Wind River Workbench generated Makefile.
# Do not edit!!!
#
# The file ".wrmakefile" is the template used by the Wind River Workbench to
# generate the makefiles of this project. Add user-specific build targets and
# make rules only(!) in this project's ".wrmakefile" file. These will then be
# automatically dumped into the makefiles.

WIND_HOME := $(subst \,/,$(WIND_HOME))
WIND_BASE := $(subst \,/,$(WIND_BASE))
WIND_USR := $(subst \,/,$(WIND_USR))
WRVX_COMPBASE := $(subst \,/,$(WRVX_COMPBASE))

all : pre_build main_all post_build

_clean ::
	@echo "make: removing targets and objects of `pwd`"

TRACE=0
TRACEON=$(TRACE:0=@)
TRACE_FLAG=$(TRACEON:1=)

MAKEFILE := Makefile

FLEXIBLE_BUILD := 1

BUILD_SPEC = MIPSI64gnule
DEBUG_MODE = 1
ifeq ($(DEBUG_MODE),1)
MODE_DIR := Debug
else
MODE_DIR := NonDebug
endif
OBJ_DIR := .
WS_ROOT_DIR := C:/WindRiver/workspace
PRJ_ROOT_DIR := $(WS_ROOT_DIR)/J3Modify



#Global Build Macros
PROJECT_TYPE = DKM
DEFINES = 
EXPAND_DBG = 0


#BuildSpec specific Build Macros
VX_CPU_FAMILY = mips
CPU = MIPSI64
TOOL_FAMILY = gnu
TOOL = gnule
TOOL_PATH = 
CC_ARCH_SPEC = -G 0 -mno-branch-likely -mips64 -mabi=n32 -mgp64 -EL
VSB_DIR = $(WIND_BASE)/target/lib
VSB_CONFIG_FILE = $(VSB_DIR)/h/config/vsbConfig.h
LIBPATH = 
LIBS = C:/WindRiver/workspace/J3Modify/Lib/io_lib.a C:/WindRiver/workspace/J3Modify/Lib/can_lib.a

IDE_INCLUDES = -I$(WIND_BASE)/target/h -IC:/WindRiver/components/windml-5.2/h -I$(WIND_BASE)/target/h/wrn/coreip 

IDE_LIBRARIES = 



#BuildTool flags
ifeq ($(DEBUG_MODE),1)
DEBUGFLAGS_C-Compiler = -g
DEBUGFLAGS_C++-Compiler = -g
DEBUGFLAGS_Linker = -g
DEBUGFLAGS_Partial-Image-Linker = 
DEBUGFLAGS_Librarian = 
DEBUGFLAGS_Assembler = -g
else
DEBUGFLAGS_C-Compiler = -O2
DEBUGFLAGS_C++-Compiler = -O2
DEBUGFLAGS_Linker = -O2
DEBUGFLAGS_Partial-Image-Linker = 
DEBUGFLAGS_Librarian = 
DEBUGFLAGS_Assembler = -O2
endif


#Project Targets
PROJECT_TARGETS = J3Modify/$(MODE_DIR)/J3Modify.out \
	 J3Modify_partialImage/$(MODE_DIR)/J3Modify_partialImage.o


#Rules

# J3Modify
ifeq ($(DEBUG_MODE),1)
J3Modify/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -g
J3Modify/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -g
J3Modify/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -g
J3Modify/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
J3Modify/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
J3Modify/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -g
else
J3Modify/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -O2
J3Modify/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -O2
J3Modify/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -O2
J3Modify/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
J3Modify/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
J3Modify/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -O2
endif
J3Modify/$(MODE_DIR)/% : IDE_INCLUDES = -I$(WIND_BASE)/target/h -IC:/WindRiver/components/windml-5.2/h -I$(WIND_BASE)/target/h/wrn/coreip 
J3Modify/$(MODE_DIR)/% : IDE_LIBRARIES = 
J3Modify/$(MODE_DIR)/% : PROJECT_TYPE = DKM
J3Modify/$(MODE_DIR)/% : DEFINES = 
J3Modify/$(MODE_DIR)/% : EXPAND_DBG = 0
J3Modify/$(MODE_DIR)/% : VX_CPU_FAMILY = mips
J3Modify/$(MODE_DIR)/% : CPU = MIPSI64
J3Modify/$(MODE_DIR)/% : TOOL_FAMILY = gnu
J3Modify/$(MODE_DIR)/% : TOOL = gnule
J3Modify/$(MODE_DIR)/% : TOOL_PATH = 
J3Modify/$(MODE_DIR)/% : CC_ARCH_SPEC = -G 0 -mno-branch-likely -mips64 -mabi=n32 -mgp64 -EL
J3Modify/$(MODE_DIR)/% : VSB_DIR = $(WIND_BASE)/target/lib
J3Modify/$(MODE_DIR)/% : VSB_CONFIG_FILE = $(VSB_DIR)/h/config/vsbConfig.h
J3Modify/$(MODE_DIR)/% : LIBPATH = 
J3Modify/$(MODE_DIR)/% : LIBS = C:/WindRiver/workspace/J3Modify/Lib/io_lib.a C:/WindRiver/workspace/J3Modify/Lib/can_lib.a
J3Modify/$(MODE_DIR)/% : OBJ_DIR := J3Modify/$(MODE_DIR)

OBJECTS_J3Modify = J3Modify_partialImage/$(MODE_DIR)/J3Modify_partialImage.o

J3Modify/$(MODE_DIR)/J3Modify.out : $(OBJECTS_J3Modify)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@";rm -f "$@";nmmips $(OBJECTS_J3Modify) $(IDE_LIBRARIES) | tclsh $(WIND_BASE)/host/resource/hutils/tcl/munch.tcl -c mips > $(OBJ_DIR)/ctdt.c; $(TOOL_PATH)ccmips $(DEBUGFLAGS_Linker) $(CC_ARCH_SPEC) -fdollars-in-identifiers -Wall  $(ADDED_CFLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL  -D_VSB_CONFIG_FILE=\"$(VSB_CONFIG_FILE)\" -D_WRS_MIPS_N32_ABI -DMIPSEL  $(DEFINES)  -o $(OBJ_DIR)/ctdt.o -c $(OBJ_DIR)/ctdt.c; $(TOOL_PATH)ccmips -r -nostdlib -Wl,-EL -Wl,-X -T $(WIND_BASE)/target/h/tool/gnu/ldscripts/link.OUT -o "$@" $(OBJ_DIR)/ctdt.o $(OBJECTS_J3Modify) $(IDE_LIBRARIES) $(LIBPATH) $(LIBS) $(ADDED_LIBPATH) $(ADDED_LIBS) && if [ "$(EXPAND_DBG)" = "1" ]; then plink "$@";fi

J3Modify/$(MODE_DIR)/J3Modify_compile_file : $(FILE) ;

_clean :: J3Modify/$(MODE_DIR)/J3Modify_clean

J3Modify/$(MODE_DIR)/J3Modify_clean : 
	$(TRACE_FLAG)if [ -d "J3Modify" ]; then cd "J3Modify"; rm -rf $(MODE_DIR); fi


# J3Modify_partialImage
ifeq ($(DEBUG_MODE),1)
J3Modify_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -g
J3Modify_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -g
J3Modify_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -g
J3Modify_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
J3Modify_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
J3Modify_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -g
else
J3Modify_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -O2
J3Modify_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -O2
J3Modify_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -O2
J3Modify_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
J3Modify_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
J3Modify_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -O2
endif
J3Modify_partialImage/$(MODE_DIR)/% : IDE_INCLUDES = -I$(WIND_BASE)/target/h -IC:/WindRiver/components/windml-5.2/h -I$(WIND_BASE)/target/h/wrn/coreip 
J3Modify_partialImage/$(MODE_DIR)/% : IDE_LIBRARIES = 
J3Modify_partialImage/$(MODE_DIR)/% : PROJECT_TYPE = DKM
J3Modify_partialImage/$(MODE_DIR)/% : DEFINES = 
J3Modify_partialImage/$(MODE_DIR)/% : EXPAND_DBG = 0
J3Modify_partialImage/$(MODE_DIR)/% : VX_CPU_FAMILY = mips
J3Modify_partialImage/$(MODE_DIR)/% : CPU = MIPSI64
J3Modify_partialImage/$(MODE_DIR)/% : TOOL_FAMILY = gnu
J3Modify_partialImage/$(MODE_DIR)/% : TOOL = gnule
J3Modify_partialImage/$(MODE_DIR)/% : TOOL_PATH = 
J3Modify_partialImage/$(MODE_DIR)/% : CC_ARCH_SPEC = -G 0 -mno-branch-likely -mips64 -mabi=n32 -mgp64 -EL
J3Modify_partialImage/$(MODE_DIR)/% : VSB_DIR = $(WIND_BASE)/target/lib
J3Modify_partialImage/$(MODE_DIR)/% : VSB_CONFIG_FILE = $(VSB_DIR)/h/config/vsbConfig.h
J3Modify_partialImage/$(MODE_DIR)/% : LIBPATH = 
J3Modify_partialImage/$(MODE_DIR)/% : LIBS = C:/WindRiver/workspace/J3Modify/Lib/io_lib.a C:/WindRiver/workspace/J3Modify/Lib/can_lib.a
J3Modify_partialImage/$(MODE_DIR)/% : OBJ_DIR := J3Modify_partialImage/$(MODE_DIR)

J3Modify_partialImage/$(MODE_DIR)/Objects/J3Modify/can.o : $(PRJ_ROOT_DIR)/can.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccmips $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -fno-builtin -Wall   -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL  -D_VSB_CONFIG_FILE=\"$(VSB_CONFIG_FILE)\" -D_WRS_MIPS_N32_ABI -DMIPSEL  $(DEFINES) -o "$@" -c "$<"


J3Modify_partialImage/$(MODE_DIR)/Objects/J3Modify/control.o : $(PRJ_ROOT_DIR)/control.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccmips $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -fno-builtin -Wall   -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL  -D_VSB_CONFIG_FILE=\"$(VSB_CONFIG_FILE)\" -D_WRS_MIPS_N32_ABI -DMIPSEL  $(DEFINES) -o "$@" -c "$<"


J3Modify_partialImage/$(MODE_DIR)/Objects/J3Modify/digitalIO.o : $(PRJ_ROOT_DIR)/digitalIO.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccmips $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -fno-builtin -Wall   -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL  -D_VSB_CONFIG_FILE=\"$(VSB_CONFIG_FILE)\" -D_WRS_MIPS_N32_ABI -DMIPSEL  $(DEFINES) -o "$@" -c "$<"


J3Modify_partialImage/$(MODE_DIR)/Objects/J3Modify/file.o : $(PRJ_ROOT_DIR)/file.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccmips $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -fno-builtin -Wall   -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL  -D_VSB_CONFIG_FILE=\"$(VSB_CONFIG_FILE)\" -D_WRS_MIPS_N32_ABI -DMIPSEL  $(DEFINES) -o "$@" -c "$<"


J3Modify_partialImage/$(MODE_DIR)/Objects/J3Modify/main.o : $(PRJ_ROOT_DIR)/main.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccmips $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -fno-builtin -Wall   -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL  -D_VSB_CONFIG_FILE=\"$(VSB_CONFIG_FILE)\" -D_WRS_MIPS_N32_ABI -DMIPSEL  $(DEFINES) -o "$@" -c "$<"


J3Modify_partialImage/$(MODE_DIR)/Objects/J3Modify/net.o : $(PRJ_ROOT_DIR)/net.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccmips $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -fno-builtin -Wall   -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL  -D_VSB_CONFIG_FILE=\"$(VSB_CONFIG_FILE)\" -D_WRS_MIPS_N32_ABI -DMIPSEL  $(DEFINES) -o "$@" -c "$<"


J3Modify_partialImage/$(MODE_DIR)/Objects/J3Modify/ui.o : $(PRJ_ROOT_DIR)/ui.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccmips $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -fno-builtin -Wall   -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL  -D_VSB_CONFIG_FILE=\"$(VSB_CONFIG_FILE)\" -D_WRS_MIPS_N32_ABI -DMIPSEL  $(DEFINES) -o "$@" -c "$<"


J3Modify_partialImage/$(MODE_DIR)/Objects/J3Modify/userIncludes.o : $(PRJ_ROOT_DIR)/userIncludes.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccmips $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -fno-builtin -Wall   -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL  -D_VSB_CONFIG_FILE=\"$(VSB_CONFIG_FILE)\" -D_WRS_MIPS_N32_ABI -DMIPSEL  $(DEFINES) -o "$@" -c "$<"


OBJECTS_J3Modify_partialImage = J3Modify_partialImage/$(MODE_DIR)/Objects/J3Modify/can.o \
	 J3Modify_partialImage/$(MODE_DIR)/Objects/J3Modify/control.o \
	 J3Modify_partialImage/$(MODE_DIR)/Objects/J3Modify/digitalIO.o \
	 J3Modify_partialImage/$(MODE_DIR)/Objects/J3Modify/file.o \
	 J3Modify_partialImage/$(MODE_DIR)/Objects/J3Modify/main.o \
	 J3Modify_partialImage/$(MODE_DIR)/Objects/J3Modify/net.o \
	 J3Modify_partialImage/$(MODE_DIR)/Objects/J3Modify/ui.o \
	 J3Modify_partialImage/$(MODE_DIR)/Objects/J3Modify/userIncludes.o

J3Modify_partialImage/$(MODE_DIR)/J3Modify_partialImage.o : $(OBJECTS_J3Modify_partialImage)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)ccmips -r -nostdlib -Wl,-EL -Wl,-X  -o "$@" $(OBJECTS_J3Modify_partialImage) $(ADDED_OBJECTS) $(IDE_LIBRARIES) $(LIBPATH) $(LIBS) $(ADDED_LIBPATH) $(ADDED_LIBS) && if [ "$(EXPAND_DBG)" = "1" ]; then plink "$@";fi

J3Modify_partialImage/$(MODE_DIR)/J3Modify_partialImage_compile_file : $(FILE) ;

_clean :: J3Modify_partialImage/$(MODE_DIR)/J3Modify_partialImage_clean

J3Modify_partialImage/$(MODE_DIR)/J3Modify_partialImage_clean : 
	$(TRACE_FLAG)if [ -d "J3Modify_partialImage" ]; then cd "J3Modify_partialImage"; rm -rf $(MODE_DIR); fi

DEP_FILES := J3Modify_partialImage/$(MODE_DIR)/Objects/J3Modify/can.d J3Modify_partialImage/$(MODE_DIR)/Objects/J3Modify/control.d J3Modify_partialImage/$(MODE_DIR)/Objects/J3Modify/digitalIO.d \
	 J3Modify_partialImage/$(MODE_DIR)/Objects/J3Modify/file.d J3Modify_partialImage/$(MODE_DIR)/Objects/J3Modify/main.d J3Modify_partialImage/$(MODE_DIR)/Objects/J3Modify/net.d \
	 J3Modify_partialImage/$(MODE_DIR)/Objects/J3Modify/ui.d J3Modify_partialImage/$(MODE_DIR)/Objects/J3Modify/userIncludes.d
-include $(DEP_FILES)


WIND_SCOPETOOLS_BASE := $(subst \,/,$(WIND_SCOPETOOLS_BASE))

clean_scopetools :
	$(TRACE_FLAG)rm -rf $(PRJ_ROOT_DIR)/.coveragescope/db

CLEAN_STEP := clean_scopetools


-include $(PRJ_ROOT_DIR)/*.makefile

-include *.makefile

main_all : external_build $(PROJECT_TARGETS)
	@echo "make: built targets of `pwd`"

# entry point for extending the build
external_build ::
	@echo ""

# main entry point for pre processing prior to the build
pre_build :: $(PRE_BUILD_STEP) generate_sources
	@echo ""

# entry point for generating sources prior to the build
generate_sources ::
	@echo ""

# main entry point for post processing after the build
post_build :: $(POST_BUILD_STEP) deploy_output
	@echo ""

# entry point for deploying output after the build
deploy_output ::
	@echo ""

clean :: external_clean $(CLEAN_STEP) _clean

# entry point for extending the build clean
external_clean ::
	@echo ""
