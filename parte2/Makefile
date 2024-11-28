# Definimos las variables
CXX = gcc                # Usamos gcc como compilador
CXXFLAGS = -Wall =Wextra -std=c11 -g  # Flags para el compilador (C11)
SRC = main_parte2.c funciones.c            # El archivo principal y funciones
OBJ = $(SRC:.c=.o) 
EXEC = main_parte2               # Nombre del ejecutable

# Regla para compilar el ejecutable final
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

# Regla para compilar main.c
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

# Limpiar archivos generados (objetos y ejecutables)
clean:
	rm -f $(OBJ) $(EXEC)

# Regla predeterminada (se ejecuta cuando solo se ejecuta `make`)
all: $(EXEC)