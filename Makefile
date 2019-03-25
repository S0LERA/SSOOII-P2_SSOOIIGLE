compilar:
	g++ ./src/Principal.cpp -o ./src/Principal -pthread -std=c++11

prueba:
	./src/Principal prueba.txt hola 2

clean:
	rm ./src/Principal
	rm ./src/Principal.o