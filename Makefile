all:
	gcc -c -std=c99 -o lsquaredc.o lsquaredc.c
	g++ -c -std=c++0x -o I2C_Functions.o I2C_Functions.cpp
	g++ -c -std=c++0x -o UHF_Transceiver.o UHF_Transceiver.cpp
	g++ -c -std=c++0x -o Packager.o Packager.cpp
	g++ -c -std=c++0x -o Handler.o Handler.cpp
	g++ -c -std=c++0x -o Interpreter.o Interpreter.cpp
	g++ -c -std=c++0x -o Radio.o Radio.cpp
	g++ -c -std=c++0x -o main.o main.cpp
	g++ -o testapp lsquaredc.o I2C_Functions.o UHF_Transceiver.o Packager.o Handler.o Interpreter.o Radio.o main.o
	
main: 
	g++ -c -std=c++0x -o main.o main.cpp
	g++ -o testapp lsquaredc.o I2C_Functions.o UHF_Transceiver.o Packager.o Handler.o Interpreter.o Radio.o main.o

radio:
	g++ -c -std=c++0x -o Radio.o Radio.cpp
	g++ -c -std=c++0x -o main.o main.cpp
	g++ -o testapp lsquaredc.o I2C_Functions.o UHF_Transceiver.o Packager.o Handler.o Interpreter.o Radio.o main.o

clean:
	rm *.o
	rm -f testapp