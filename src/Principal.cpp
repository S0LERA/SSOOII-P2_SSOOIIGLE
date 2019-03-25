#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <thread>

std::vector<std::thread> v_hilos;

/* F1: Abrir Archivo */
std::ifstream abrirArchivo(std::string nombre_archivo) {
	std::ifstream fs;
	fs.open(nombre_archivo);
	return fs;
}

/* F2: Obtener lineas archivo */
std::vector<int> obtenerLineas(std::ifstream &fs, int n_hilos){
	fs.clear();
	fs.seekg(0);
	std::vector<int> v_lineas;
	int lineas = 0;
	int particion = 0;
	int linea_f = 0;
	int linea_i = 1;

	while(fs.good()){
		if(fs.get()=='\n'){
			lineas++;
		}
	}

	particion = lineas/n_hilos;

	for (int i = 0; i < n_hilos; i++) {
		v_lineas.push_back(linea_i);
		linea_f = linea_i + particion;
		v_lineas.push_back(linea_f);
		linea_i = linea_f+1;
	}

	if((lineas%n_hilos)!=0){
		v_lineas.pop_back();
		v_lineas.push_back(lineas);
	}


	return v_lineas;
}

/* F3: buscar una palabra en un fichero */

void buscarPalabra(std::ifstream &fs, std::string keyword,int linea_inicial, int linea_final) {
	fs.clear();
	fs.seekg(0);
	std::string linea;
	int repeticiones = 0;
	int contador =0;
	while (getline(fs, linea)) {
		contador ++;
		if(contador>= linea_inicial && contador<=linea_final){
			if (linea.find(keyword) != std::string::npos) {
				repeticiones++;
			}
		}
	}
	if(repeticiones == 0){
		std::cout << "Palabra: "<< keyword << "No encontrada" << std::endl;
	}else{
		std::cout << "Palabra: "<< keyword << " Encontrada: "<< repeticiones << " veces." << std::endl;
	}
}

/* F4: imprimir los resultados de la busqueda por pantalla */
void imprimeResultados(std::vector<int> aux){
	int id_hilo = 0;
	for (int i = 0; i < aux.size(); i+=2) {
		std::cout << "Hilo " << id_hilo << ", lineas:" << aux.at(i) << " , " << aux.at(i+1) << std::endl;
		id_hilo++;
	}
}

/* F5: crea los hilos de busqueda

void creaHilos(int numHilos){
	for (int i = 0; i < numHilos; i++) {
		std::thread hilo(buscarPalabra);
		hilo.join();
	}
}
*/

int main(int argc, char *argv[]) {
	std::ifstream archivo = abrirArchivo(argv[1]);
	std::vector<int> v_lineas = obtenerLineas(archivo, atoi(argv[3]));
	std::thread hilo(buscarPalabra,std::ref(archivo), argv[2],0,30);
	hilo.join();
	imprimeResultados(v_lineas);
	return EXIT_SUCCESS;
}
