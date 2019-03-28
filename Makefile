compilar:
	g++ ./src/SSOOGLE.cpp -o ./src/SSOOGLE -pthread -std=c++11

SS00GLE:
	cp $(FICHERO).txt src
	(cd src && ./SSOOGLE $(FICHERO).txt $(PALABRA) $(HILOS)) | tee resultados.txt
	rm ./src/$(FICHERO).txt

clean:
	rm ./src/SSOOGLE
	rm resultados.txt
