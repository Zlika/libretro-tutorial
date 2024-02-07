TARGET := mon_emu_libretro.so
CFLAGS := -fPIC -Wall -std=c99
INCDIRS := -I.
LDFLAGS := -lc -lm -shared -version-script=link.T -no-undefined

SRC=$(wildcard *.c)
OBJECTS := $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(INCDIRS) -c -o $@ $<

clean:
	rm -f $(OBJECTS)
	rm -f $(TARGET)

.PHONY: all clean

