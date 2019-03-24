#include <iostream>
#include <fstream>
#include <vector>
#include <string>

/* F1: Abrir Archivo */
std::ifstream abrirArchivo(std::string nombre_archivo) {
	std::ifstream fs;
	fs.open(nombre_archivo);
	return fs;
}

/* F2: Obtener lineas archivo */
std::vector<int> obtenerLineas(std::ifstream &fs, int n_hilos){
	std::vector<int> v_lineas;
	int lineas = 0;
	int particion = 0;
	int particion_final = 0;
	int linea_f = 0;
	int linea_i = 1;
	
	while(fs.good()){
		if(fs.get()=='\n'){
			lineas++;
		}
	}
	
	particion = lineas/n_hilos;
	if((lineas%n_hilos) == 0){
		particion_final = particion;
	}else{
		particion_final = particion + 1;
	}
	
	for (int i = 0; i < n_hilos; i++) {
		v_lineas.push_back(linea_i);
		if(i == n_hilos -1){
			linea_f = linea_i + particion_final;
		}else{
			linea_f = linea_i + particion;
		}
		v_lineas.push_back(linea_f);
		linea_i = linea_f+1;
	}
	
	return v_lineas;
}

void imprimeResultados(std::vector<int> aux){
	int id_hilo = 0;
	for (int i = 0; i < aux.size(); i+=2) {
		std::cout << "Hilo" << id_hilo << ", lineas:" << aux.at(i) << " , " << aux.at(i+1) << std::endl;
		id_hilo++;
	}
}

int main(int argc, char *argv[]) {
	std::ifstream archivo = abrirArchivo("prueba.txt");
	std::vector<int> v_lineas = obtenerLineas(archivo, 4);
	imprimeResultados(v_lineas);
	return EXIT_SUCCESS;
}
