UNAME_S    := $(shell uname -s | tr '[:upper:]' '[:lower:]')
UNAME_M    := $(shell uname -m)
CPLC       ?= cplc
CPL_TARGET := out-cpl
CPL_SRC    := main.cpl src/token.cpl src/interpreter.cpl
INCLUDE    := include

.PHONY: all clean rebuild

all: $(CPL_TARGET)

$(CPL_TARGET): $(CPL_SRC) include/token_h.cpl include/interpreter_h.cpl
	$(CPLC) -O3 --linker-no-pie -I $(INCLUDE) --output $(CPL_TARGET) $(CPL_SRC)

clean:
	rm -f $(TARGET) $(CPL_TARGET)

rebuild: clean all
