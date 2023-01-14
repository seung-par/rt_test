NAME = minirt

CC = cc
CFLAGS = -Wall -Wextra -Werror -g

INCLUDE = -I./includes -I./get_next_line -I./mlx -I./libft
LIBS = -L./mlx -lmlx -L./libft -lft

SRC_DIR = ./srcs/
GNL_DIR = ./get_next_line/
OBJ_DIR = ./objs/

SRC_LIST = main.c \
		   utils.c \
		   ray.c \
		   canvas.c \
		   scene.c \
		   print.c \
		   object_create.c \
		   normal.c \
		   hit_sphere.c \
		   hit.c \
		   object_utils.c \

GNL_LIST = get_next_line.c \
		   get_next_line_utils.c \

SRCS = $(addprefix $(SRC_DIR), $(SRC_LIST))
OBJS = $(SRCS:.c=.o)

GNL_SRCS = $(addprefix $(GNL_DIR), $(GNL_LIST))
GNL_OBJS = $(GNL_SRCS:.c=.o)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o  $@

$(NAME): $(OBJS) $(GNL_OBJS)
	$(MAKE) -C./mlx
	$(MAKE) -C./libft
	$(CC) $(CFLAGS) $(INCLUDE) $(LIBS) -o $(NAME) $(OBJS) $(GNL_OBJS) -framework OpenGL -framework Appkit

all: $(NAME)

clean:
	$(MAKE) -C./mlx clean
	$(MAKE) -C./libft clean
	rm -f $(OBJS) $(GNL_OBJS)

fclean: clean
	$(MAKE) -C./libft fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
