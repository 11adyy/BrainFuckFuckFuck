UNAME_S    := $(shell uname -s | tr '[:upper:]' '[:lower:]')
UNAME_M    := $(shell uname -m)
CPLC       ?= ../ExampleCompiler/builds/$(UNAME_S)-$(UNAME_M)/cplc
TARGET     := out
CPL_TARGET := out-cpl
SRC        := main.c src/token.c src/interpreter.c
CPL_SRC    := main.cpl src/token.cpl src/interpreter.cpl
INCLUDE    := include

CFLAGS := \
	-O3 \
	-march=native \
	-mtune=native \
	-flto \
	-fomit-frame-pointer \
	-funroll-loops \
	-fstrict-aliasing \
	-fno-plt \
	-pipe \
	-I$(INCLUDE)

CFLAGS += \
	-Wall \
	-Wextra \
	-Wpedantic \
	-Wshadow \
	-Wconversion \
	-DDEBUG

LDFLAGS := -flto

.PHONY: all clean rebuild

all: $(CPL_TARGET)

$(CPL_TARGET): $(CPL_SRC) include/token_h.cpl include/interpreter_h.cpl
	$(CPLC) -I $(INCLUDE) --output $(CPL_TARGET) $(CPL_SRC)

clean:
	rm -f $(TARGET) $(CPL_TARGET)

rebuild: clean all
