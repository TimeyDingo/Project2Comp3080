CC = gcc
CFLAGS = 
LIBS = -lpthread

TARGET = ReaderWriters

all: $(TARGET)

01_TEST: ./ReaderWriters ricr ricb roocr roocb wicr wicb woocr woocb nr nw


$(TARGET): ReaderWriters.c
	$(CC) $(CFLAGS) -o $(TARGET) ReaderWriters.c $(LIBS)

clean:
	rm -f $(TARGET)