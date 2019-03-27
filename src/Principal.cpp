#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>

std::mutex sem;
std::vector<std::thread> v_hilos;

struct resultados{
	int id_hilo;
	std::string palabra_encontrada;
	int numero_linea;
	int inicio_fragmento;
	int fin_fragmento;
	std::string linea;
};

std::vector<resultados> v_resultados;

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

void buscarPalabra(std::ifstream &fs, std::string keyword,int linea_inicial, int linea_final, int id_hilo) {
	std::string linea;
	int repeticiones = 0;
	int contador =0;
	resultados res;
	while (getline(fs, linea)) {
		contador ++;
		if(contador>= linea_inicial && contador<=linea_final){
			if (linea.find(keyword) != std::string::npos) {
				sem.lock();
				res.id_hilo = id_hilo;
				res.inicio_fragmento = linea_inicial;
				res.fin_fragmento = linea_final;
				res.numero_linea = contador;
				res.palabra_encontrada = keyword;
				res.linea = linea;
				v_resultados.push_back(res);
				repeticiones++;
				sem.unlock();
			}
		}
	}
	if(repeticiones == 0){
		std::cout << "Hilo: " << id_hilo <<" Palabra: "<< keyword << " No encontrada" << std::endl;
	}else{
		std::cout << "Hilo: " << id_hilo << " Palabra: "<< keyword << " Encontrada: "<< repeticiones << " veces." << std::endl;
	}
}

/* F4: imprimir los resultados de la busqueda por pantalla */
void imprimeResultados(std::vector<int> aux){
	resultados res;
	std::cout << "--------------" << '\n';
	for(unsigned int i = 0; i < v_resultados.size();i++){
		res = v_resultados.at(i);
		std::cout << "Resultado: " << '\n';
		std::cout << "Hilo: " << res.id_hilo << '\n';
		std::cout << "Palabra encontrada: " << res.palabra_encontrada << '\n';
		std::cout << "Fragmento: " << res.inicio_fragmento <<" -> " << res.fin_fragmento <<'\n';
		std::cout << "Linea nº: " << res.numero_linea <<'\n';
		std::cout << "Linea completa: " << res.linea <<'\n';
		std::cout << "--------------" << '\n';
	}
}

/* F5: crea los hilos de busqueda */
void creaHilos(std::vector<int> numHilos,  std::string keyword, std::string nombrearchivo){
	std::ifstream archivo;
	int id_hilo = 0;
	for (unsigned int i = 0; i < numHilos.size(); i+=2) {
		archivo = abrirArchivo(nombrearchivo);
		std::thread hilo(buscarPalabra,std::ref(archivo), keyword,numHilos.at(i),numHilos.at(i+1),id_hilo);
		hilo.join();
		id_hilo++;
	}
}

int main(int argc, char *argv[]) {
	std::ifstream archivo = abrirArchivo(argv[1]);
	std::vector<int> v_lineas = obtenerLineas(archivo, atoi(argv[3]));
	creaHilos(v_lineas,"medida",argv[1]);
	imprimeResultados(v_lineas);
	return EXIT_SUCCESS;
}
