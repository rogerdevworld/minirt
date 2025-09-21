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
SRCS = \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/clean/clean_data.c \
	$(SRC_DIR)/clean/clean_data_2.c \
	$(SRC_DIR)/inits/init_basics.c \
	$(SRC_DIR)/inits/init_color.c \
	$(SRC_DIR)/inits/init_elements.c \
	$(SRC_DIR)/inits/init_figures.c \
	$(SRC_DIR)/parser/ft_error.c \
	$(SRC_DIR)/parser/parser.c \
	$(SRC_DIR)/parser/parser_elements_2.c \
	$(SRC_DIR)/parser/parser_elements.c \
	$(SRC_DIR)/parser/parser_utils_2.c \
	$(SRC_DIR)/parser/parser_utils_3.c \
	$(SRC_DIR)/parser/parser_utils.c \
	$(SRC_DIR)/parser/parser_utils_materials_2.c \
	$(SRC_DIR)/parser/parser_utils_materials.c \
	$(SRC_DIR)/render/ft_color.c \
	$(SRC_DIR)/render/ft_normal_mapping.c \
	$(SRC_DIR)/render/ft_ray.c \
	$(SRC_DIR)/render/render.c \
	$(SRC_DIR)/render/thread.c \
	$(SRC_DIR)/render/animation/animate_lights.c \
	$(SRC_DIR)/render/animation/auto_rotate.c \
	$(SRC_DIR)/render/intersect/ft_cone.c \
	$(SRC_DIR)/render/intersect/ft_cylinder.c \
	$(SRC_DIR)/render/intersect/ft_hyperboloid.c \
	$(SRC_DIR)/render/intersect/ft_paraboloid.c \
	$(SRC_DIR)/render/intersect/ft_plane.c \
	$(SRC_DIR)/render/intersect/ft_sphere.c \
	$(SRC_DIR)/render/light/ft_light.c \
	$(SRC_DIR)/render/light/ft_light_utils.c \
	$(SRC_DIR)/render/light/ft_specular_light.c \
	$(SRC_DIR)/render/mapping/ft_texture.c \
	$(SRC_DIR)/render/mapping/get_uv_cone.c \
	$(SRC_DIR)/render/mapping/get_uv_cylinder.c \
	$(SRC_DIR)/render/mapping/get_uv_hyperboloid.c \
	$(SRC_DIR)/render/mapping/get_uv_paraboloid.c \
	$(SRC_DIR)/render/mapping/get_uv_plane.c \
	$(SRC_DIR)/render/mapping/get_uv_sphere.c \
	$(SRC_DIR)/utils/nthreads.c \
	$(SRC_DIR)/utils/ray_at.c \
	$(SRC_DIR)/vector/cal.c \
	$(SRC_DIR)/vector/ft_tan.c \
	$(SRC_DIR)/vector/ft_asin.c \
	$(SRC_DIR)/vector/ft_atan.c \
	$(SRC_DIR)/vector/ft_atan2.c \
	$(SRC_DIR)/vector/ft_abs.c \
	$(SRC_DIR)/vector/ft_fmin.c \
	$(SRC_DIR)/vector/ft_fmod.c \
	$(SRC_DIR)/vector/ft_pow.c \
	$(SRC_DIR)/vector/vec2/vec2_init.c \
	$(SRC_DIR)/vector/vec2/vec2_normalize.c \
	$(SRC_DIR)/vector/vec3/ft_rotate_vector.c \
	$(SRC_DIR)/vector/vec3/vec3_add.c \
	$(SRC_DIR)/vector/vec3/vec3_cross.c \
	$(SRC_DIR)/vector/vec3/vec3_div.c \
	$(SRC_DIR)/vector/vec3/vec3_dot.c \
	$(SRC_DIR)/vector/vec3/vec3_init.c \
	$(SRC_DIR)/vector/vec3/vec3_length.c \
	$(SRC_DIR)/vector/vec3/vec3_length_squared.c \
	$(SRC_DIR)/vector/vec3/vec3_mul.c \
	$(SRC_DIR)/vector/vec3/vec3_mult_vec.c \
	$(SRC_DIR)/vector/vec3/vec3_normalize.c \
	$(SRC_DIR)/vector/vec3/vec3_reflect.c \
	$(SRC_DIR)/vector/vec3/vec3_sub.c \
	$(SRC_DIR)/windows/hook.c \
	$(SRC_DIR)/windows/mouse.c \
	$(SRC_DIR)/windows/window.c

# --- Object Files ---
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# --- Header Files ---
HEADERS = -I$(INC_DIR) -I$(MLX_DIR)/include

# --- Compiler Flags ---
# -Werror -Wextra -Wall: Mandatory flags for 42 projects
# -Ofast: Optional optimization flag
CFLAGS =  -Ofast -fsanitize=address,leak -g -O3 -march=native -flto #-Wall -Wextra -Werror

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

# --- Compilation Rule ---
# Rule to compile a .c file into a .o file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "$(BLUE)Compiling $<...$(RESET)"
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(HEADERS) -c $< -o $@

# Create object directories if they don't exist
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/render

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