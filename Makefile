#---------------------------------
# INITIALISATION DES VARIABLES 
#---------------------------------

# Indiquer le compilateur
CXX= gcc

# Les chemins ou se trouvent les fichiers a inclure
INCLUDE= -I/usr/include

# Options de compilation.
CXXFLAGS=  $(INCLUDE) -c `pkg-config --cflags gtk+-2.0`

# Les chemins ou se trouvent les librairies
LIBRARY_PATH= -L/usr/lib -pthread

# Options pour le linker.
LDFLAGS= $(LIBRARY_PATH) -g -o

# Les librairies avec lesquelles on va effectuer l'édition de liens
LIBS= `pkg-config --libs --cflags gtk+-2.0` -lm

# Les fichiers sources de l'application
FILES= main.c callback.c core.c file.c generate.c print.c window.c

#-----------
# LES CIBLES
#-----------
Sudoku :  $(FILES:.c=.o)
	$(CXX) $(LDFLAGS) Sudoku $(FILES:.c=.o) $(LIBS)

.PHONY : clean
clean:
	/bin/rm $(FILES:.c=.o) Sudoku

#-----------------------------------------------------------------------------
# LES REGLES DE DEPENDANCE
#-----------------------------------------------------------------------------
main.o:main.c callback.h core.h file.h generate.h print.h window.h 
callback.o:callback.c callback.h
core.o:core.c core.h
file.o:file.c file.h
generate.o:generate.c generate.h
print.o:print.c print.h
window.o:window.c window.h

#---------------------------------
# REGLES DE COMPILATION IMPLICITES
#---------------------------------
%.o : %.c ; $(CXX) $(CXXFLAGS) $*.c
