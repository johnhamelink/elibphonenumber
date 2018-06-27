# Based on c_src.mk from erlang.mk by Loic Hoguin <essen@ninenines.eu>

CURDIR := $(shell pwd)
BASEDIR := $(abspath $(CURDIR)/..)
PROJECT = phonenumber_util_nif

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
	LDFLAGS ?= -arch x86_64 -flat_namespace -undefined suppress
else ifeq ($(UNAME_SYS), FreeBSD)
	CC ?= cc
	CXXFLAGS ?= -O3 -finline-functions -Wall
else ifeq ($(UNAME_SYS), Linux)
	CC ?= gcc
	CXXFLAGS ?= -g -O3 -finline-functions -Wall
endif

ifeq ($(UNAME_SYS), Darwin)
	ifneq ($(HOMEBREW), "")
		HOMEBREW_PREFIX = $(shell brew --prefix)
		HOMEBREW_CFLAGS = -I$(HOMEBREW_PREFIX)/opt/boost/include -I$(HOMEBREW_PREFIX)/opt/icu4c/include -I$(HOMEBREW_PREFIX)/opt/re2/include -I$(HOMEBREW_PREFIX)/opt/protobuf/include -I$(HOMEBREW_PREFIX)/opt/libphonenumber/include
		HOMEBREW_LDFLAGS = -L$(HOMEBREW_PREFIX)/opt/boost/lib -L$(HOMEBREW_PREFIX)/opt/icu4c/lib -L$(HOMEBREW_PREFIX)/opt/re2/lib -L$(HOMEBREW_PREFIX)/opt/protobuf/lib -L$(HOMEBREW_PREFIX)/opt/libphonenumber/lib
		LDFLAGS += $(HOMEBREW_LDFLAGS)
		CXXFLAGS += $(HOMEBREW_CFLAGS)
	endif
endif

CXXFLAGS += -fPIC -I $(ERTS_INCLUDE_DIR) -I $(ERL_INTERFACE_INCLUDE_DIR)

LDLIBS += -L $(ERL_INTERFACE_LIB_DIR) -lerl_interface -lei  -lstdc++ -lboost_system -lboost_date_time -licui18n -licuuc -lprotobuf -lgeocoding -lphonenumber
LDFLAGS += -shared

# Verbosity.

cpp_verbose_0 = @echo " CPP   " $(?F);
cpp_verbose = $(cpp_verbose_$(V))

link_verbose_0 = @echo " LD    " $(@F);
link_verbose = $(link_verbose_$(V))

SOURCES = phonenumber_util_nif.cpp
OBJECTS = $(addsuffix .o, $(basename $(SOURCES)))

COMPILE_CPP = $(cpp_verbose) $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c

$(C_SRC_OUTPUT): $(OBJECTS)
	@mkdir -p $(BASEDIR)/priv/
	$(link_verbose) $(CC) $(OBJECTS) $(LDFLAGS) $(LDLIBS) -o $(C_SRC_OUTPUT)

phonenumber_util_nif.o: phonenumber_util_nif.cpp
	$(COMPILE_CPP) -DLPN_HASLOCALONLY $(OUTPUT_OPTION) phonenumber_util_nif.cpp || \
	$(COMPILE_CPP) $(OUTPUT_OPTION) phonenumber_util_nif.cpp

clean:
	@rm -f $(C_SRC_OUTPUT) $(OBJECTS)
