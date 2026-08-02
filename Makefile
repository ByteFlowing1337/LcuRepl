CC := GCC

CFLAGS := -Wall -Wextra -I./include
LDFLAGS := -s -lwinhttp -lntdll -lreadline

TARGET := LcuRepl.exe

SRC := src/cmdline.c src/repl.c src/pid.c src/token.c src/request.c src/main.c
OBJS := $(SRC:.c=.o)
CLEAN_OBJS := $(subst /,\,$(OBJS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	del /Q /F $(CLEAN_OBJS) $(TARGET) 2>NUL || exit 0