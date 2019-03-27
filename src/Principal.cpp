#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <sstream>
#include <iterator>
#include <cstring>

struct resultados{
	int id_hilo;
	std::string palabra_encontrada;
	std::string palabra_anterior;
	std::string palabra_posterior;
	int numero_linea;
	int inicio_fragmento;
	int fin_fragmento;
	std::string linea;
};

struct signos_puntuacion: std::ctype<char>
{
	signos_puntuacion(): std::ctype<char>(get_tabla()) {}

	static std::ctype_base::mask const* get_tabla()
	{
		typedef std::ctype<char> cctype;
		static const cctype::mask *const_rc= cctype::classic_table();

		static cctype::mask rc[cctype::table_size];
		std::memcpy(rc, const_rc, cctype::table_size * sizeof(cctype::mask));

		rc[','] = std::ctype_base::space;
		rc['.'] = std::ctype_base::space;
		rc['!'] = std::ctype_base::space;
		rc['?'] = std::ctype_base::space;
		return &rc[0];
	}
};

std::mutex sem;
std::vector<std::thread> v_hilos;
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

/* FAUX Convertir string a vector */
std::vector<std::string> vectorLinea(std::string s){
	std::stringstream ss(s);
	ss.imbue(std::locale(std::locale(), new signos_puntuacion()));
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> vstrings(begin, end);
	return vstrings;
}

/* F3: buscar una palabra en un fichero */

void buscarPalabra(std::ifstream &fs, std::string keyword,int linea_inicial, int linea_final, int id_hilo) {
	std::vector<std::string> vector_linea;
	std::string linea;
	int repeticiones = 0;
	int contador =0;
	resultados res;
	while (getline(fs, linea)) {																			//Obtener lineas del archivo
		vector_linea = vectorLinea(linea);															//Pasar linea a vector
		contador ++;
		if(contador>= linea_inicial && contador<=linea_final){ 					//Para posicionar a cada hilo en su sitio
			for (unsigned int i = 0; i < vector_linea.size(); i++) {			//Recorrer el vactor con las palabras de la linea
				if (vector_linea.at(i) == keyword) {												//Buscar la palabra en la linea
					res.id_hilo = id_hilo;
					res.inicio_fragmento = linea_inicial;
					res.fin_fragmento = linea_final;
					res.numero_linea = contador;
					res.palabra_encontrada = keyword;
					res.palabra_anterior = vector_linea.at(i-1);
					res.palabra_posterior = vector_linea.at(i+1);
					res.linea = linea;
					sem.lock();
					v_resultados.push_back(res);
					repeticiones++;
					sem.unlock();
				}
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
	creaHilos(v_lineas,"hola",argv[1]);
	imprimeResultados(v_lineas);
	return EXIT_SUCCESS;
}
