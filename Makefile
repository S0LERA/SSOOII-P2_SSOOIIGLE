compilar:
	g++ ./src/Principal.cpp -o ./src/Principal -pthread -std=c++11

prueba:
	./src/Principal prueba2.txt "medida" 4

clean:
	rm ./src/Principal
	rm ./src/Principal.o
