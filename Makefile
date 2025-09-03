# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rmarrero <rmarrero@student.42barcelona.com>+#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/16 12:58:52 by rmarrero          #+#    #+#              #
#    Updated: 2025/08/18 03:45:00 by rmarrero         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# --- Project Configuration ---
NAME = minirt
CC = gcc
RM = rm -rf

# --- Directories ---
SRC_DIR = ./src
OBJ_DIR = ./.obj
INC_DIR = ./include
LIBFT_DIR = ./src/libft
MLX_DIR = ./MLX42

# --- Source Files ---
# List all your .c files here. This list MUST be updated manually.
SRCS = \
    $(SRC_DIR)/main.c \
    $(SRC_DIR)/render.c \
    $(SRC_DIR)/utils/init.c \
    $(SRC_DIR)/utils/ft_esc_cal.c \
    $(SRC_DIR)/utils/ft_vec_cal.c \
    $(SRC_DIR)/utils/nthreads.c \
    $(SRC_DIR)/windows/window.c \
    $(SRC_DIR)/parser/parser.c \
    $(SRC_DIR)/parser/parser_utils.c \
    $(SRC_DIR)/parser/parser_elements.c \
    $(SRC_DIR)/assets/ft_ray.c \
    $(SRC_DIR)/parser/parser_utils_materials.c

# --- Object Files ---
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# --- Header Files ---
HEADERS = -I$(INC_DIR) -I$(MLX_DIR)/include

# --- Compiler Flags ---
# -Werror -Wextra -Wall: Mandatory flags for 42 projects
# -Ofast: Optional optimization flag
CFLAGS = -fsanitize=address,leak -g -O2#-Wall -Wextra -Werror -Ofast 

# --- Library Flags for MLX42 ---
# -L$(LIBFT_DIR) -lft: Link your Libft
# -L$(MLX_DIR)/build: Set the library path for MLX42
# -lmlx42 -ldl -lglfw -pthread -lm: Link MLX42 and its dependencies
LIBS = -L$(LIBFT_DIR) -lft -L$(MLX_DIR)/build -lmlx42 -ldl -lglfw -pthread -lm

# --- Color Output ---
GREEN  = \033[32m
YELLOW = \033[33m
BLUE   = \033[34m
RED    = \033[31m
RESET  = \033[0m

# --- Main Targets ---
.PHONY: all clean fclean re libmlx

all: libmlx $(NAME)

# Target to compile MLX42
libmlx:
	@echo "$(YELLOW)Compiling MLX42...$(RESET)"
	@cmake $(MLX_DIR) -B $(MLX_DIR)/build -DCMAKE_BUILD_TYPE=Release
	@make -C $(MLX_DIR)/build -j4

# Link all object files and libraries to create the final executable
$(NAME): $(OBJS)
	@echo "$(YELLOW)Compiling and linking Libft...$(RESET)"
	@make -C $(LIBFT_DIR)
	@echo "$(BLUE)Linking $(NAME)...$(RESET)"
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) $(HEADERS) -o $(NAME)
	@echo "$(GREEN)MiniRT compiled successfully! 🎉$(RESET)"

# --- Compilation Rule ---
# Rule to compile a .c file into a .o file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "$(BLUE)Compiling $<...$(RESET)"
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(HEADERS) -c $< -o $@

# Create object directories if they don't exist
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# --- Clean Targets ---
clean:
	@echo "$(YELLOW)Cleaning object files...$(RESET)"
	@$(RM) $(OBJ_DIR)
	@make clean -C $(LIBFT_DIR)
	# The MLX42 build directory is removed here, no need to run 'make clean'
	@$(RM) -rf $(MLX_DIR)/build

fclean: clean
	@echo "$(YELLOW)Cleaning executable...$(RESET)"
	@$(RM) $(NAME)
	@make fclean -C $(LIBFT_DIR)
	# The MLX42 build directory is removed here
	@$(RM) -rf $(MLX_DIR)/build

re: fclean all