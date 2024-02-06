CC=gcc

CFLAGS=-Wall -pedantic -std=gnu99

TARGET=filemonitor

all: $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) $(TARGET).c `pkg-config --cflags --libs libnotify` -o build/$(TARGET)d
