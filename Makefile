# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/16 12:58:52 by rmarrero          #+#    #+#              #
#    Updated: 2025/02/17 11:55:01 by rmarrero         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #
# --- Mandatory --- #
NAME = minirt
SRC_DIR = ./src/
OBJ_DIR = ./obj

SRCS =	$(SRC_DIR)fract-ol.c $(SRC_DIR)parse.c $(SRC_DIR)window.c $(SRC_DIR)keyboard.c \
		$(SRC_DIR)fractals/julia.c $(SRC_DIR)fractals/mandelbrot.c $(SRC_DIR)fractals/burning_ship.c

OBJS = $(SRCS:$(SRC_DIR)%.c=$(OBJ_DIR)/%.o)
HEADER = ./include/fractol.h
CC = gcc
CFLAGS = -Wall -Werror -Wextra -I./include
RM = rm -rf
LIBFT = ./libft
EX_LIB = $(LIBFT)/libft.a
MLX = ./minilibx
ifeq ($(UNAME_S), Linux)
	MLX_FLAGS = -lXext -lX11 -lm
else
	MLX_FLAGS = -framework OpenGL -framework AppKit
endif

RED     = \033[31m
GREEN   = \033[32m
YELLOW  = \033[33m
BLUE    = \033[34m
RESET   = \033[0m

all: libs $(NAME)

libs:
	@make -C $(LIBFT)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR) \
	$(OBJ_DIR)/fractals

$(OBJ_DIR)/%.o: $(SRC_DIR)%.c $(HEADER) Makefile | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(EX_LIB) $(OBJS) $(HEADER)
	@echo "$(GREEN)Compilando MiniLibX...$(RESET)"
	make -C $(MLX)
	@echo "$(GREEN)Compilando $(NAME)...$(RESET)"
	$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT) -lft -L$(MLX) -lmlx $(MLX_FLAGS) -o $(NAME)
	@echo "$(BLUE)"
	@echo "$(YELLOW)           ($(RESET)__$(YELLOW))\           $(RESET)"
	@echo "$(YELLOW)           ($(RESET)oo$(YELLOW))\\________  $(RESET)"
	@echo "$(RESET)           /|| \\        \\ $(NAME) ready$(RESET)"
	@echo "$(RESET)              ||------w | $(RESET)"
	@echo "$(RESET)              ||       || $(RESET)"
	@echo "$(YELLOW)THE COW MAKES MUUUUUUUUUU!$(RESET)"
	@echo "$(RESET)"

clean:
	@echo "$(GREEN)Eliminando archivos objeto...$(RESET)"
	$(RM) $(OBJ_DIR)
	@make clean -C $(LIBFT)
	@make clean -C $(MLX)

fclean: clean
	@echo "$(GREEN)Eliminando ejecutable y librerías...$(RESET)"
	$(RM) -f $(NAME)
	@make fclean -C $(LIBFT)

re: fclean all

.PHONY: all clean fclean re libs