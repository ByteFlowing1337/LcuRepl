CC := gcc
CFLAGS := -Wall -Wextra -O2 -I./include -I./src/isocline -MMD -MP
LDFLAGS := -s
LIBS := -lwinhttp -lntdll

TARGET := LcuRepl.exe

SRC := $(wildcard src/*.c) src/isocline/isocline.c
OBJS := $(SRC:.c=.o)
DEPS := $(OBJS:.o=.d)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	@if exist src\*.o del /Q /F src\*.o 2>NUL || true
	@if exist src\*.d del /Q /F src\*.d 2>NUL || true
	@if exist $(TARGET) del /Q /F $(TARGET) 2>NUL || true