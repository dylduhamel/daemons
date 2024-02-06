CC=gcc

CFLAGS=-Wall -pedantic -std=gnu99

TARGET=filemonitor

all: $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) `pkg-config --cflags --libs libnotify` $(TARGET).c -o build/$(TARGET)d
