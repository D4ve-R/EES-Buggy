CC=g++

SDIR=src
SDIRL=$(SDIR)/lib
IDIR=inc
ODIR=build

OBJ=buggy.o hcsr04.o gy521.o hcsr04_log.o gy521_log.o pwm.o i2cdeviceWP.o adafruitmotorhat.o adafruitdcmotor.o led.o

CFLAGS=-std=c++11 -Wall -I$(IDIR)

LIBS=-lwiringPi -lm -pthread

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

buggy.o: hcsr04.o gy521.o hcsr04_log.o gy521_log.o adafruitmotorhat.o led.o
	$(CC) -c $(SDIRL)/buggy.cpp -o $@ $(CFLAGS)

gy521.o: i2cdeviceWP.o
	$(CC) -c $(SDIRL)/sensors/gy521.cpp -o $@ $(CFLAGS)

gy521i_log.o: i2cdeviceWP.o
	$(CC) -c $(SDIRL)/sensors/gy521_log.cpp -o $@ $(CFLAGS)

adafruitmotorhat.o: pwm.o  adafruitdcmotor.o
	$(CC) -c $(SDIRL)/adafruit/adafruitmotorhat.cpp -o $@ $(CFLAGS)

adafruitdcmotor.o: pwm.o
	$(CC) -c $(SDIRL)/adafruit/adafruitdcmotor.cpp -o $@ $(CFLAGS)

pwm.o: i2cdeviceWP.o
	$(CC) -c $(SDIRL)/adafruit/pwm.cpp -o $@ $(CFLAGS)

led.o:
	$(CC) -c $(SDIRL)/led.cpp -o $@ $(CFLAGS)

i2cdeviceWP.o:
	$(CC) -c $(SDIRL)/i2cdeviceWP.cpp -o $@ $(CFLAGS)

hcsr04.o:
	$(CC) -c $(SDIRL)/sensors/hcsr04.cpp -o $@ $(CFLAGS)

hcsr04_log.o:
	$(CC) -c $(SDIRL)/sensors/hcsr04_log.cpp -o $@ $(CFLAGS)

inputcontroller.o:
	$(CC) -c $(SDIRL)/inputcontroller.cpp -o $@ $(CFLAGS)

clean: 
	rm -rf $(ODIR)/*
