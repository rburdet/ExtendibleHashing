## Makefile TEMPORAL para compilacion de TP Grupal realizado en lenguaje C++

# CONFIGURACION
################
# Nombre del ejecutable.

# Extensión de los archivos a compilar (c para C, cpp o cc o cxx para C++).
extension = cpp

TESTS = BlockTable TableBlock
TMP_FILE_PREFIX = test_file_


# CONFIGURACION "AVANZADA"
###########################

# Opciones para el compilador C/C++ en modo ansi.
CFLAGS = -Wall -Werror -pedantic -pedantic-errors

# Para que explote lo mas posible
CFLAGS += -O3 -DNDEBUG

# Para valgrind o debug
CFLAGS += -ggdb -DDEBUG -fno-inline

# Opciones del enlazador.
#LDFLAGS =

# Estandar de C a usar
CSTD = c99

# Estandar de C++ a usar
CXXSTD = c++98

# Archivo log de Valgrind.
VGLOGFILE = valgrind.out

# Flags para el Valgrind
VGFLAGS = --tool=memcheck --trace-children=yes --track-fds=yes --time-stamp=yes --num-callers=20 --error-exitcode=42 --db-attach=no --leak-check=full --leak-resolution=med --log-file=$(VGLOGFILE)



# VARIABLES CALCULADAS A PARTIR DE LA CONFIGURACION
####################################################


# Pongo flags de C para C++ también
CXXFLAGS += $(CFLAGS)

# Uso enlazador de c++ si es código no C.
ifeq ($(extension), c)
CFLAGS += -std=$(CSTD)
LD = $(CC)
else
CXXFLAGS += -std=$(CXXSTD)
LD = $(CXX)
endif

# Si no especifica archivos, tomo todos.
fuentes ?= $(wildcard ../*.$(extension))
o_files = $(patsubst %.$(extension),%.o,$(fuentes))
o_files_no_main = $(filter-out ../interfaz_main.o,$(o_files))
tmp_file ?= $(wildcard $(TMP_FILE_PREFIX)*)

occ := $(CC)
ocxx := $(CXX)
orm := $(RM)
old := $(LD)
ifdef verbose
RM := $(RM) -v
else
CC =  @echo "  CC  $@"; $(occ)
CXX = @echo "  CXX $@"; $(ocxx)
RM =  @echo "  CLEAN"; $(orm)
LD =  @echo "  LD  $@"; $(old)
endif


# REGLAS
#########

.PHONY: all clean

all: $(TESTS)

$(TESTS): $(o_files_no_main)
	$(LD) $(LDFLAGS) $@.cpp $(o_files_no_main) $(LOADLIBS) $(LDLIBS) -o $@

clean:
	$(RM) $(TESTS) $(tmp_file) .HashBlock*

valgrind:
	valgrind $(VGFLAGS) ./$(target) 
