# Based on c_src.mk from erlang.mk by Loic Hoguin <essen@ninenines.eu>

CURDIR := $(shell pwd)
BASEDIR := $(abspath $(CURDIR)/..)

PROJECT = phonenumber_util_nif
#PROJECT ?= $(notdir $(BASEDIR))
#PROJECT := $(strip $(PROJECT))

ERTS_INCLUDE_DIR ?= $(shell erl -noshell -s init stop -eval "io:format(\"~s/erts-~s/include/\", [code:root_dir(), erlang:system_info(version)]).")
ERL_INTERFACE_INCLUDE_DIR ?= $(shell erl -noshell -s init stop -eval "io:format(\"~s\", [code:lib_dir(erl_interface, include)]).")
ERL_INTERFACE_LIB_DIR ?= $(shell erl -noshell -s init stop -eval "io:format(\"~s\", [code:lib_dir(erl_interface, lib)]).")

C_SRC_DIR = .
C_SRC_OUTPUT ?= ../priv/$(PROJECT).so

# System type and C compiler/flags.

UNAME_SYS := $(shell uname -s)
HOMEBREW  := $(shell { type brew; } 2>/dev/null)

ifeq ($(UNAME_SYS), Darwin)
	CC ?= cc
	CXXFLAGS ?= -O3 -arch x86_64 -Wall
	CFLAGS ?= -O3 -std=c99 -arch x86_64 -Wall -Wmissing-prototypes
	LDFLAGS ?= -arch x86_64 -flat_namespace -undefined suppress
else ifeq ($(UNAME_SYS), FreeBSD)
	CC ?= cc
	CFLAGS ?= -O3 -std=c99 -finline-functions -Wall -Wmissing-prototypes
	CXXFLAGS ?= -O3 -finline-functions -Wall
else ifeq ($(UNAME_SYS), Linux)
	CC ?= gcc
	CFLAGS ?= -O3 -std=c99 -finline-functions -Wall -Wmissing-prototypes
	CXXFLAGS ?= -g -O3 -finline-functions -Wall
endif

ifeq ($(UNAME_SYS), Darwin)
	ifneq ($(HOMEBREW), "")
		HOMEBREW_PREFIX = $(shell brew --prefix)
		HOMEBREW_CFLAGS = -I$(HOMEBREW_PREFIX)/opt/boost/include -I$(HOMEBREW_PREFIX)/opt/icu4c/include -I$(HOMEBREW_PREFIX)/opt/re2/include -I$(HOMEBREW_PREFIX)/opt/protobuf/include -I$(HOMEBREW_PREFIX)/opt/libphonenumber/include
		HOMEBREW_LDFLAGS = -L$(HOMEBREW_PREFIX)/opt/boost/lib -L$(HOMEBREW_PREFIX)/opt/icu4c/lib -L$(HOMEBREW_PREFIX)/opt/re2/lib -L$(HOMEBREW_PREFIX)/opt/protobuf/lib -L$(HOMEBREW_PREFIX)/opt/libphonenumber/lib
		CFLAGS += $(HOMEBREW_CFLAGS)
		LDFLAGS += $(HOMEBREW_LDFLAGS)
		CXXFLAGS += $(HOMEBREW_CFLAGS)
	endif
endif


CFLAGS += -fPIC -I $(ERTS_INCLUDE_DIR) -I $(ERL_INTERFACE_INCLUDE_DIR)
CXXFLAGS += -fPIC -I $(ERTS_INCLUDE_DIR) -I $(ERL_INTERFACE_INCLUDE_DIR)

LDLIBS += -L $(ERL_INTERFACE_LIB_DIR) -lerl_interface -lei  -lstdc++ -lboost_system -lboost_date_time -licui18n -licuuc -lprotobuf -lgeocoding -lphonenumber
LDFLAGS += -shared

# Verbosity.

c_verbose_0 = @echo " C     " $(?F);
c_verbose = $(c_verbose_$(V))

cpp_verbose_0 = @echo " CPP   " $(?F);
cpp_verbose = $(cpp_verbose_$(V))

link_verbose_0 = @echo " LD    " $(@F);
link_verbose = $(link_verbose_$(V))

SOURCES := $(shell find $(C_SRC_DIR) -type f \( -name "*.c" -o -name "*.C" -o -name "*.cc" -o -name "*.cpp" \))
OBJECTS = $(addsuffix .o, $(basename $(SOURCES)))

COMPILE_C = $(c_verbose) $(CC) $(CFLAGS) $(CPPFLAGS) -c
COMPILE_CPP = $(cpp_verbose) $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c

$(C_SRC_OUTPUT): $(OBJECTS)
	@mkdir -p $(BASEDIR)/priv/
	$(link_verbose) $(CC) $(OBJECTS) $(LDFLAGS) $(LDLIBS) -o $(C_SRC_OUTPUT)

%.o: %.c
	$(COMPILE_C) $(OUTPUT_OPTION) $<

%.o: %.cc
	$(COMPILE_CPP) $(OUTPUT_OPTION) $<

%.o: %.C
	$(COMPILE_CPP) $(OUTPUT_OPTION) $<

%.o: %.cpp
	$(COMPILE_CPP) $(OUTPUT_OPTION) $<

clean:
	@rm -f $(C_SRC_OUTPUT) $(OBJECTS)
