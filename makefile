# ===============================================
# Configuración
# ===============================================
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
LDFLAGS = -lrt -pthread

INCLUDE_DIR = ./include
INCLUDE_FLAG = -I$(INCLUDE_DIR)

SRC_DIR = ./src
PROC_DIR = ./procesos

TARGET = planificador_rr

# ===============================================
# CORE DEL PLANIFICADOR
# ===============================================
CORE_SOURCES = $(wildcard $(SRC_DIR)/*.c)
CORE_OBJECTS = $(CORE_SOURCES:.c=.o)

# main del planificador está en round_robin.c
UTILITY_OBJECTS = $(filter-out $(SRC_DIR)/round_robin.o, $(CORE_OBJECTS))

# ===============================================
# PROCESOS DE USUARIO
# ===============================================
PROCESS_SOURCES = $(wildcard $(PROC_DIR)/*.c)
USER_TARGETS = $(notdir $(PROCESS_SOURCES:.c=))

# ===============================================
# REGLAS
# ===============================================

.PHONY: all clean user_procs rebuild

all: $(TARGET) user_procs

# -----------------------------------------------
# Compilación de objetos del core
# -----------------------------------------------
%.o: %.c
	@echo "Compilando $<"
	$(CC) $(CFLAGS) $(INCLUDE_FLAG) -c $< -o $@

# -----------------------------------------------
# Ejecutable del planificador
# -----------------------------------------------
$(TARGET): $(CORE_OBJECTS)
	@echo "Enlazando $(TARGET)"
	$(CC) $(CORE_OBJECTS) -o $(TARGET) $(LDFLAGS)

# -----------------------------------------------
# Procesos de usuario
# -----------------------------------------------
user_procs: $(USER_TARGETS)
	@echo "Procesos de usuario compilados"

$(USER_TARGETS): %: $(PROC_DIR)/%.c $(UTILITY_OBJECTS)
	@echo "Creando proceso de usuario $@"
	$(CC) $(CFLAGS) $(INCLUDE_FLAG) $< $(UTILITY_OBJECTS) -o $@ $(LDFLAGS)

# -----------------------------------------------
# Limpieza
# -----------------------------------------------
clean:
	@echo "Limpiando archivos objeto y ejecutables..."
	rm -f $(CORE_OBJECTS) $(TARGET) $(USER_TARGETS)

rebuild: clean all
