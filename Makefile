# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rmarrero <rmarrero@student.42barcelona.com>+#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/16 12:58:52 by rmarrero          #+#    #+#              #
#    Updated: 2025/08/17 23:45:00 by rmarrero         ###   ########.fr        #
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
LIBFT_DIR = ./libft
MLX_DIR = ./minilibx

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
# List all your .h files here. This list MUST be updated manually.
HEADERS = \
    $(INC_DIR)/minirt.h

# --- Compiler Flags ---
# -Wall, -Wextra, -Werror: Mandatory flags for 42 projects
# -I$(INC_DIR): Include directory for your project headers
# -g -fsanitize=address: Optional flags for debugging
CFLAGS = -Wall -Wextra -Werror -I$(INC_DIR) -pthread #-g -fsanitize=address

# --- Library Flags ---
# Detect OS to use the correct MiniLibX flags
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
    # -lXext -lX11: X Window System libraries on Linux.
    # -lm: Math library.
    # -lmlx: Minilibx library. The linker will find it in system paths.
    MLX_FLAGS = -lXext -lX11 -lm
    MLX_LIB = -L$(MLX_DIR) -lmlx
else ifeq ($(UNAME_S), Darwin) # macOS
    MLX_FLAGS = -framework OpenGL -framework AppKit -lm
    MLX_LIB = -L$(MLX_DIR) -lmlx
endif

# --- Color Output ---
GREEN  = \033[32m
YELLOW = \033[33m
BLUE   = \033[34m
RED    = \033[31m
RESET  = \033[0m

# --- Main Targets ---
.PHONY: all clean fclean re bonus

all: $(NAME)

# Link all object files and libraries to create the final executable
$(NAME): $(OBJS) $(HEADERS)
	@echo "$(YELLOW)Compiling and linking Libft...$(RESET)"
	@make -C $(LIBFT_DIR)
	@echo "$(YELLOW)Compiling and linking MiniLibX...$(RESET)"
	@make -C $(MLX_DIR)
	@echo "$(BLUE)Linking $(NAME)...$(RESET)"
	$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_DIR) -lft $(MLX_LIB) $(MLX_FLAGS) -o $(NAME)
	@echo "$(GREEN)MiniRT compiled successfully! 🎉$(RESET)"

# --- Compilation Rules ---
# Rule to compile a .c file into a .o file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "$(BLUE)Compiling $<...$(RESET)"
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Create object directories if they don't exist
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# --- Clean Targets ---
clean:
	@echo "$(YELLOW)Cleaning object files...$(RESET)"
	@$(RM) $(OBJ_DIR)
	@make clean -C $(LIBFT_DIR)
	@make clean -C $(MLX_DIR)

fclean: clean
	@echo "$(YELLOW)Cleaning executable...$(RESET)"
	@$(RM) $(NAME)
	@make fclean -C $(LIBFT_DIR)
	@make fclean -C $(MLX_DIR)

re: fclean all