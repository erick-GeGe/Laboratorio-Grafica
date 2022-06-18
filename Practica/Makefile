NAME_PROJECT = openGL

SRC_DIR = src
INCLUDES = -I../dep/include
LIBS = -lopengl32 -lglfw3 -lgdi32 -lglu32 -lfreeglut

OBJS = $(SRC_DIR)\main.cpp $(SRC_DIR)\glad.c $(SRC_DIR)\glm.c
CFLAGS = $(INCLUDES) $(LIBS)

$(NAME_PROJECT) : $(OBJS)
	@g++ -o $(NAME_PROJECT) $(OBJS) $(INCLUDES) $(LIBS)
	$(NAME_PROJECT).exe

clean :
	@del $(NAME_PROJECT).exe
