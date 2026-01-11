CC      := gcc
TARGET  := out
SRC     := main.c
INCLUDE := include

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
	-Wconversion

LDFLAGS := -flto

.PHONY: all clean rebuild

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)

rebuild: clean all
