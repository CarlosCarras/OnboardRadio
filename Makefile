CC= gcc
CCC= g++

CFLAGS= -Wall
CPPFLAGS= $(CFLAGS)
# BINS= imu_test i2clib.a


all: test

main.o: main.cpp
	$(CCC) $(CPPFLAGS) -c main.cpp -o main.o

Radio.o: Radio.h Radio.cpp
	$(CCC) $(CPPFLAGS) -c Radio.cpp -o Radio.o

Interpreter.o: Interpreter.h Interpreter.cpp
	$(CCC) $(CPPFLAGS) -c Interpreter.cpp -o Interpreter.o

Handler.o: Handler.h Handler.cpp
	$(CCC) $(CPPFLAGS) -c Handler.cpp -o Handler.o

Packager.o: Packager.h Packager.cpp
	$(CCC) $(CPPFLAGS) -c Packager.cpp -o Packager.o

UHF_Transceiver.o: UHF_Transceiver.h UHF_Transceiver.cpp
	$(CCC) $(CPPFLAGS) -c UHF_Transceiver.cpp -o UHF_Transceiver.o

I2C_Functions.o: I2C_Functions.h I2C_Functions.cpp
	$(CCC) $(CPPFLAGS) -c I2C_Functions.cpp -o I2C_Functions.o

lsquaredc.o: lsquaredc.h lsquaredc.c
	$(CC) $(CFLAGS) -c lsquaredc.c -o lsquaredc.o

test: lsquaredc.o I2C_Functions.o UHF_Transceiver.o Handler.o Packager.o Interpreter.o Radio.o main.o
	$(CCC) $(CPPFLAGS) -o test main.o Radio.o Interpreter.o Handler.o Packager.o UHF_Transceiver.o I2C_Functions.o lsquaredc.o

# i2clib.a: libi2c.o
#	 ar rcs i2clib.a libi2c.o lsquaredc.o

clean:
	rm -rf *.o test