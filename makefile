# ===============================================
# Configuración de Variables
# ===============================================
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
LDFLAGS = -lrt -pthread  # Librerías para IPC POSIX (sem_open)

# Bandera -I: Indica dónde buscar los archivos .h
INCLUDE_DIR = ./include
INCLUDE_FLAG = -I$(INCLUDE_DIR)

# Directorios de Código Fuente
SRC_DIR = ./src
PROC_DIR = ./procesos  # <-- CAMBIO 1: La carpeta 'procesos' ahora está en la raíz

# ----------------------------------------------------
# 1. ARCHIVOS DEL CORE (Todo lo que está en src)
# ----------------------------------------------------
CORE_SOURCES = $(wildcard $(SRC_DIR)/*.c)
CORE_SOURCES += $(wildcard $(SRC_DIR)/*/*.c)

CORE_OBJECTS = $(CORE_SOURCES:.c=.o)

# ASUMIMOS que el 'main' del planificador está en src/round_robin.c
# Objetos de Utilidad (Todo el CORE EXCEPTO round_robin.o, necesarios para los procesos de usuario)
UTILITY_OBJECTS = $(filter-out $(SRC_DIR)/round_robin.o, $(CORE_OBJECTS))

TARGET = planificador_rr

# ----------------------------------------------------
# 2. ARCHIVOS DE PROCESOS DE USUARIO
# ----------------------------------------------------
PROCESS_SOURCES = $(wildcard $(PROC_DIR)/*.c)
USER_TARGETS = $(notdir $(PROCESS_SOURCES:.c=)) # Ejecutables: prueba1, prueba2, etc.

# ===============================================
# Reglas de Compilación
# ===============================================

# Regla principal: Compila todo
all: $(TARGET) user_procs

# Regla para compilar cada archivo .c en .o (aplica al CORE)
%.o: %.c
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) $(INCLUDE_FLAG) -c $< -o $@

# ----------------- REGLA DEL PLANIFICADOR -----------------
$(TARGET): $(CORE_OBJECTS)
	@echo "Enlazando el ejecutable del Planificador $(TARGET)..."
	$(CC) $(CORE_OBJECTS) -o $(TARGET) $(LDFLAGS)

# ----------------- REGLAS DE PROCESOS DE USUARIO -----------------

# Objetivo para compilar todos los procesos de usuario
user_procs: $(USER_TARGETS)
	@echo "Todos los procesos de usuario han sido compilados."

# Regla de Patrón: Compila cada proceso de usuario por separado.
# El ejecutable ($@, ej: prueba1) depende de su .c y de los objetos de utilidad.
%: $(PROC_DIR)/%.c $(UTILITY_OBJECTS)
	@echo "Creando ejecutable de proceso de usuario: $@"
	$(CC) $(CFLAGS) $(INCLUDE_FLAG) $< $(UTILITY_OBJECTS) -o $@ $(LDFLAGS)

# ----------------- LIMPIEZA Y RECONSTRUCCIÓN -----------------

clean:
	@echo "Limpiando archivos objeto, ejecutable principal y procesos de usuario..."
	rm -f $(CORE_OBJECTS) $(TARGET) $(USER_TARGETS)

rebuild: clean all

.PHONY: all clean rebuild user_procs