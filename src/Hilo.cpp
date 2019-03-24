#include <iostream>
#include <thread>

class Hilo{
private:
	int id;
	int nlinea_inicio;
	int nlinea_final;
	int nlinea_encontrado;
	String linea_encontrada;

public:
	Hilo(int id, int nlinea_inicial, int nlinea_final);
	getLineaEncontrada();
	getNumeroLineaEncontrada();
}; 

Hilo::Hilo(int id, int nlinea_inicial, int nlinea_final):
	id(id),
	nlinea_inicial(nlinea_inicial),
	nlinea_final(nlinea_final){};
	
Hilo::getLineaEncontrada(){
	return(linea_encontrada);
}

Hilo::getNumeroLineaEncontrada(){
	return(nlinea_encontrado);
}
