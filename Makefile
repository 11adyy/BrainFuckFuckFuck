UNAME_S    := $(shell uname -s | tr '[:upper:]' '[:lower:]')
UNAME_M    := $(shell uname -m)
CPLC       ?= cplc
PYTHON     ?= python3
CPL_TARGET := a.out
CPLC_FLAGS ?= -O3 --linker-mode driver --linker gcc --linker-no-pie -Xlinker -nostartfiles
CPL_SRC    := main.cpl src/token.cpl src/interpreter.cpl
INCLUDE    := include

.PHONY: all clean rebuild test test-raw test-bfpp

all: $(CPL_TARGET)

$(CPL_TARGET): $(CPL_SRC) include/token_h.cpl include/interpreter_h.cpl Makefile
	$(CPLC) $(CPLC_FLAGS) -I $(INCLUDE) --output $(CPL_TARGET) $(CPL_SRC)

test: $(CPL_TARGET)
	$(PYTHON) tests/run_tests.py --no-build --binary ./$(CPL_TARGET)

test-raw: $(CPL_TARGET)
	$(PYTHON) tests/run_tests.py --no-build --binary ./$(CPL_TARGET) --prefix raw_

test-bfpp: $(CPL_TARGET)
	$(PYTHON) tests/run_tests.py --no-build --binary ./$(CPL_TARGET) --exclude-prefix raw_

clean:
	rm -f $(TARGET) $(CPL_TARGET)

rebuild: clean all
