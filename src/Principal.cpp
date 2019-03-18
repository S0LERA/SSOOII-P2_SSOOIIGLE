#include <iostream>
#include <fstream>
#include <vector>

/* F1: Abrir Archivo */
std::ifstream abrirArchivo(std::string nombre_archivo) {
 std::ifstream fs;
 fs.open(nombre_archivo);
 return fs;
}

/* F2: Obtener lineas archivo */
std::vector<int> obtenerLineas(std::ifstream fs){
 std::vector<int> lineas;


 return lineas;
}
