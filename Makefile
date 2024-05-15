# Nombre del ejecutable
NAME = philo

# Compilador y flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread -O3

# Archivos fuente
SRCS = dinner.c \
       getters_setters.c \
       init.c \
       main.c \
       monitor.c \
       parsing.c \
       safe_functions.c \
       synchro_utils.c \
       utils.c \
       write.c

# Archivos objeto
OBJS = $(SRCS:.c=.o)

# Reglas principales
all: $(NAME)

# Regla para compilar el ejecutable
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

# Regla para compilar archivos .c a .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpiar archivos objeto
clean:
	rm -f $(OBJS)

# Limpiar todo, incluyendo el ejecutable
fclean: clean
	rm -f $(NAME)

# Regla para recompilar desde cero
re: fclean all

# Definir phony targets
.PHONY: all clean fclean re

