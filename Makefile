CC=g++

SDIR=src
IDIR=inc
ODIR=build

CFLAGS=-std=c++11 -Wall -I $(IDIR)

LIBS=-lwiringPi

OBJ= $(wildcard src/*.cpp)
LIST= $(OBJ:.cpp=.o)

main: main.o 
	$(CC) main.o buggy.o ultrasonic.o gy521.o  pwm.o i2cdeviceWP.o adafruitmotorhat.o adafruitdcmotor.o -o $(ODIR)/$@ $(LIBS) $(CFLAGS)
	rm *.o

main.o: buggy.o 
	$(CC) -c $(SDIR)/main.cpp -o $@ $(CFLAGS)

buggy.o: ultrasonic.o gy521.o adafruitmotorhat.o
	$(CC) -c $(SDIR)/buggy.cpp -o $@ $(CFLAGS)

gy521.o: i2cdeviceWP.o
	$(CC) -c $(SDIR)/gy521.cpp -o $@ $(CFLAGS)

adafruitmotorhat.o: pwm.o  adafruitdcmotor.o
	g++ -c $(SDIR)/adafruitmotorhat.cpp -o $@ $(CFLAGS)

adafruitdcmotor.o: pwm.o
	g++ -c $(SDIR)/adafruitdcmotor.cpp -o $@ $(CFLAGS)

pwm.o: i2cdeviceWP.o
	$(CC) -c $(SDIR)/pwm.cpp -o $@ $(CFLAGS)

i2cdeviceWP.o:
	$(CC) -c $(SDIR)/i2cdeviceWP.cpp -o $@ $(CFLAGS)

ultrasonic.o:
	$(CC) -c $(SDIR)/ultrasonic.cpp -o $@ $(CFLAGS)


clean: 
	rm -rf $(ODIR)/*
