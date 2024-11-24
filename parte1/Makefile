# Definimos las variables
CXX = g++                # Usamos g++ como compilador
CXXFLAGS = -std=c++11 -Wall -pthread  # Flags para el compilador (C++11 y soporte para hilos)
SRC = main_parte1.cpp            # El archivo principal
OBJ = main_parte1.o              # Solo necesitamos el archivo objeto de main.cpp
EXEC = main_parte1                # Nombre del ejecutable

# Regla para compilar el ejecutable final
$(EXEC): $(OBJ)
	$(CXX) $(OBJ) -o $(EXEC)

# Regla para compilar main.cpp
main.o: main.cpp circular_queue.h monitor.h
	$(CXX) $(CXXFLAGS) -c main_parte1.cpp

# Limpiar archivos generados (objetos y ejecutables)
clean:
	rm -f $(OBJ) $(EXEC)

# Regla predeterminada (se ejecuta cuando solo se ejecuta `make`)
all: $(EXEC)
