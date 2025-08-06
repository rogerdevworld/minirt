# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/16 12:58:52 by rmarrero          #+#    #+#              #
#    Updated: 2025/07/31 02:08:00 by rmarrero         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# --- Project Setup ---
NAME = minirt
CC = gcc
RM = rm -rf

# --- Directories ---
SRC_DIR = ./src
OBJ_DIR = ./.obj
INC_DIR = ./include

# --- Source Files ---
# List all your .c files here. Adjust paths based on your actual file structure.
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
	$(SRC_DIR)/parser/parser_elements.c 
# $(SRC_DIR)/assets/ft_generate_ray.c
# --- Object Files ---
# Generate object file paths from source file paths
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# --- Headers ---
# All headers for the project
HEADERS = $(INC_DIR)/minirt.h

# --- Compiler Flags ---
# -Wall, -Wextra, -Werror: Standard warnings and errors as mandatory by 42
# -I$(INC_DIR): Include directory for your project headers
CFLAGS =  -pthread -fsanitize=address,leak# -Wall -Werror -Wextra -I$(INC_DIR) -g# -g for debugging

# --- External Libraries ---
LIBFT_PATH = ./src/libft
MLX_PATH = ./minilibx
EX_LIB = $(LIBFT_PATH)/libft.a

# --- Minilibx Specific Flags ---
# Detect OS to use correct Minilibx flags
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
    # -lXext -lX11: X Window System libraries required by MiniLibX on Linux
    # -lm: Math library (for sqrt, sin, cos, etc.)
    # -pthread: POSIX threads library for multithreading
    MLX_FLAGS = -lXext -lX11 -lm -pthread
else ifeq ($(UNAME_S), Darwin) # macOS
    # -framework OpenGL -framework AppKit: macOS specific frameworks for MiniLibX
    # -lm: Math library
    # -pthread: POSIX threads library
    MLX_FLAGS = -framework OpenGL -framework AppKit -lm -pthread
endif

# --- Colors for Makefile Output ---
RED     = \033[31m
GREEN   = \033[32m
YELLOW  = \033[33m
BLUE    = \033[34m
RESET   = \033[0m

# --- Targets ---
.PHONY: all clean fclean re libs

all: libs $(NAME)

# Compile libft first
libs:
	@echo "$(GREEN)Compiling Libft...$(RESET)"
	@make -C $(LIBFT_PATH)

# Create object directories if they don't exist
$(OBJ_DIR):
	@echo "$(YELLOW)Creating object directories...$(RESET)"
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/parser
	@mkdir -p $(OBJ_DIR)/utils
	@mkdir -p $(OBJ_DIR)/windows
	@mkdir -p $(OBJ_DIR)/assets
	# @mkdir -p $(OBJ_DIR)/intersections # Uncomment if you add these files
	# @mkdir -p $(OBJ_DIR)/lighting     # Uncomment if you add these files
	# @mkdir -p $(OBJ_DIR)/camera       # Uncomment if you add these files

# Rule to compile a .c file into a .o file
# It uses pattern rules to handle source files in subdirectories
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) Makefile | $(OBJ_DIR)
	@echo "$(BLUE)Compiling $<...$(RESET)"
	$(CC) $(CFLAGS) -c $< -o $@

# Main executable target
$(NAME): $(EX_LIB) $(OBJS) $(HEADERS)
	@echo "$(GREEN)Compiling MiniLibX...$(RESET)"
	@make -C $(MLX_PATH)
	@echo "$(GREEN)Linking $(NAME)...$(RESET)"
	$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_PATH) -lft -L$(MLX_PATH) -lmlx $(MLX_FLAGS) -o $(NAME)
	@echo "                                                                                "
	@echo "                                                                                "
	@echo "# **************************************************************************** #"
	@echo "#                                                                              #"
	@echo "#                                                         :::      ::::::::    #"
	@echo "#    MiniRT                                             :+:      :+:    :+:    #"
	@echo "#                                                     +:+ +:+         +:+      #"
	@echo "#    By: rmarrero rmarrero@student.42barcelona.com  +#+  +:+       +#+         #"
	@echo "#                                                 +#+#+#+#+#+   +#+            #"
	@echo "#    Created: 2024/09/16 12:58:52 by rmarrero          #+#    #+#              #"
	@echo "#    Updated: 2025/07/29 13:04:23 by rmarrero         ###   ########.fr        #"
	@echo "#                                                                              #"
	@echo "# **************************************************************************** #"
	@echo "                                                                                "
	@echo "                                                                                "
	@echo "$(RESET)"

# Clean target: remove object files and compiled libs
clean:
	@echo "$(GREEN)Cleaning object files...$(RESET)"
	$(RM) $(OBJ_DIR)
	@make clean -C $(LIBFT_PATH)
	@make clean -C $(MLX_PATH)

# Fclean target: remove executable and all compiled files
fclean: clean
	@echo "$(GREEN)Cleaning executable...$(RESET)"
	$(RM) -f $(NAME)
	@make fclean -C $(LIBFT_PATH)

# Re-compiles everything
re: fclean all