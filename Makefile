TARGET := mon_emu_libretro.so
CFLAGS := -fPIC -Wall -std=c99
INCDIRS := -I.
SHARED := -shared -Wl,-version-script=link.T -Wl,-no-undefined

SRC=$(wildcard *.c)
OBJECTS := $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $(SHARED) $(OBJECTS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) $(INCDIRS)

clean:
	rm -f $(OBJECTS)
	rm -f $(TARGET)

.PHONY: clean

