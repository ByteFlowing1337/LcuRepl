CC := gcc
CFLAGS := -Wall -Wextra -I./include -MMD -MP
LDFLAGS := -s -lwinhttp -lntdll -lreadline

TARGET := LcuRepl.exe

SRC := src/cmdline.c src/repl.c src/pid.c src/token.c src/request.c src/main.c
OBJS := $(SRC:.c=.o)
DEPS := $(OBJS:.o=.d)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	@if exist src\*.o del /Q /F src\*.o 2>NUL || true
	@if exist src\*.d del /Q /F src\*.d 2>NUL || true
	@if exist $(TARGET) del /Q /F $(TARGET) 2>NUL || true