CC=g++

SDIR=src
IDIR=inc
ODIR=build

OBJ=buggy.o hcsr04.o gy521.o  pwm.o i2cdeviceWP.o adafruitmotorhat.o adafruitdcmotor.o led.o

CFLAGS=-std=c++11 -Wall -I$(IDIR)

LIBS=-lwiringPi -lm

main: main.o $(OBJ)
	$(CC) main.o $(OBJ) -o $(ODIR)/$@ $(LIBS) $(CFLAGS)
	rm *.o
	@echo created executable $@ in $(ODIR) 

wasd: wasd.o inputcontroller.o $(OBJ)
	$(CC) wasd.o $(OBJ) inputcontroller.o -o $(ODIR)/$@ $(LIBS) -lncurses $(CFLAGS) 
	rm *.o
	@echo created executable $@ in $(ODIR) 
	
main.o:  
	$(CC) -c $(SDIR)/main.cpp -o $@ $(CFLAGS)

wasd.o:
	$(CC) -c $(SDIR)/wasd.cpp -o $@ $(CFLAGS)

libbuggy.a: buggy.o
	ar -r -o $(ODIR)/libbuggy.a buggy.o hcsr04.o gy521.o  pwm.o i2cdeviceWP.o adafruitmotorhat.o adafruitdcmotor.o led.o

buggy.o: hcsr04.o gy521.o adafruitmotorhat.o led.o
	$(CC) -c $(SDIR)/buggy.cpp -o $@ $(CFLAGS)

gy521.o: i2cdeviceWP.o
	$(CC) -c $(SDIR)/gy521.cpp -o $@ $(CFLAGS)

adafruitmotorhat.o: pwm.o  adafruitdcmotor.o
	$(CC) -c $(SDIR)/adafruitmotorhat.cpp -o $@ $(CFLAGS)

adafruitdcmotor.o: pwm.o
	$(CC) -c $(SDIR)/adafruitdcmotor.cpp -o $@ $(CFLAGS)

pwm.o: i2cdeviceWP.o
	$(CC) -c $(SDIR)/pwm.cpp -o $@ $(CFLAGS)

led.o:
	$(CC) -c $(SDIR)/led.cpp -o $@ $(CFLAGS)

i2cdeviceWP.o:
	$(CC) -c $(SDIR)/i2cdeviceWP.cpp -o $@ $(CFLAGS)

hcsr04.o:
	$(CC) -c $(SDIR)/hcsr04.cpp -o $@ $(CFLAGS)

inputcontroller.o:
	$(CC) -c $(SDIR)/inputcontroller.cpp -o $@ $(CFLAGS)

clean: 
	rm -rf $(ODIR)/*
