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
SRC_DIR = ./src/mandatory
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
	$(SRC_DIR)/render/intersect/ft_cylinder_utils.c \
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

# --- bonus --- #
B_NAME = minirt_bonus
BSRC_DIR = ./src/bonus/
B_OBJ_DIR = ./obj_bonus

BSRCS = \
	$(BSRC_DIR)/main_bonus.c \
	$(BSRC_DIR)/clean/clean_data_bonus.c \
	$(BSRC_DIR)/clean/clean_data_2_bonus.c \
	$(BSRC_DIR)/inits/init_basics_bonus.c \
	$(BSRC_DIR)/inits/init_color_bonus.c \
	$(BSRC_DIR)/inits/init_elements_bonus.c \
	$(BSRC_DIR)/inits/init_figures_bonus.c \
	$(BSRC_DIR)/parser/ft_error_bonus.c \
	$(BSRC_DIR)/parser/parser_bonus.c \
	$(BSRC_DIR)/parser/parser_elements_2_bonus.c \
	$(BSRC_DIR)/parser/parser_elements_bonus.c \
	$(BSRC_DIR)/parser/parser_utils_2_bonus.c \
	$(BSRC_DIR)/parser/parser_utils_3_bonus.c \
	$(BSRC_DIR)/parser/parser_utils_bonus.c \
	$(BSRC_DIR)/parser/parser_utils_materials_2_bonus.c \
	$(BSRC_DIR)/parser/parser_utils_materials_bonus.c \
	$(BSRC_DIR)/render/ft_color_bonus.c \
	$(BSRC_DIR)/render/ft_normal_mapping_bonus.c \
	$(BSRC_DIR)/render/ft_ray_bonus.c \
	$(BSRC_DIR)/render/render_bonus.c \
	$(BSRC_DIR)/render/thread_bonus.c \
	$(BSRC_DIR)/render/animation/animate_lights_bonus.c \
	$(BSRC_DIR)/render/animation/auto_rotate_bonus.c \
	$(BSRC_DIR)/render/intersect/ft_cone_bonus.c \
	$(BSRC_DIR)/render/intersect/ft_cylinder_bonus.c \
	$(BSRC_DIR)/render/intersect/ft_cylinder_utils_bonus.c \
	$(BSRC_DIR)/render/intersect/ft_hyperboloid_bonus.c \
	$(BSRC_DIR)/render/intersect/ft_paraboloid_bonus.c \
	$(BSRC_DIR)/render/intersect/ft_plane_bonus.c \
	$(BSRC_DIR)/render/intersect/ft_sphere_bonus.c \
	$(BSRC_DIR)/render/light/ft_light_bonus.c \
	$(BSRC_DIR)/render/light/ft_light_utils_bonus.c \
	$(BSRC_DIR)/render/light/ft_specular_light_bonus.c \
	$(BSRC_DIR)/render/mapping/ft_texture_bonus.c \
	$(BSRC_DIR)/render/mapping/get_uv_cone_bonus.c \
	$(BSRC_DIR)/render/mapping/get_uv_cylinder_bonus.c \
	$(BSRC_DIR)/render/mapping/get_uv_hyperboloid_bonus.c \
	$(BSRC_DIR)/render/mapping/get_uv_paraboloid_bonus.c \
	$(BSRC_DIR)/render/mapping/get_uv_plane_bonus.c \
	$(BSRC_DIR)/render/mapping/get_uv_sphere_bonus.c \
	$(BSRC_DIR)/utils/nthreads_bonus.c \
	$(BSRC_DIR)/utils/ray_at_bonus.c \
	$(BSRC_DIR)/vector/vec2/vec2_init_bonus.c \
	$(BSRC_DIR)/vector/vec2/vec2_normalize_bonus.c \
	$(BSRC_DIR)/vector/vec3/ft_rotate_vector_bonus.c \
	$(BSRC_DIR)/vector/vec3/vec3_add_bonus.c \
	$(BSRC_DIR)/vector/vec3/vec3_cross_bonus.c \
	$(BSRC_DIR)/vector/vec3/vec3_div_bonus.c \
	$(BSRC_DIR)/vector/vec3/vec3_dot_bonus.c \
	$(BSRC_DIR)/vector/vec3/vec3_init_bonus.c \
	$(BSRC_DIR)/vector/vec3/vec3_length_bonus.c \
	$(BSRC_DIR)/vector/vec3/vec3_length_squared_bonus.c \
	$(BSRC_DIR)/vector/vec3/vec3_mul_bonus.c \
	$(BSRC_DIR)/vector/vec3/vec3_mult_vec_bonus.c \
	$(BSRC_DIR)/vector/vec3/vec3_normalize_bonus.c \
	$(BSRC_DIR)/vector/vec3/vec3_reflect_bonus.c \
	$(BSRC_DIR)/vector/vec3/vec3_sub_bonus.c \
	$(BSRC_DIR)/windows/hook_bonus.c \
	$(BSRC_DIR)/windows/mouse_bonus.c \
	$(BSRC_DIR)/windows/window_bonus.c

# --- Bonus Objects ---
BOBJS = $(patsubst $(BSRC_DIR)/%.c,$(B_OBJ_DIR)/%.o,$(BSRCS))

# --- Header Files ---
HEADERS = -I$(INC_DIR) -I$(MLX_DIR)/include

# --- Compiler Flags ---
# -Werror -Wextra -Wall: Mandatory flags for 42 projects
# -Ofast: Optional optimization flag
CFLAGS = -Wall -Wextra -Werror -Ofast -fsanitize=address,leak -g -O3 -march=native -flto #
# --- Library Flags for MLX42 ---
# -L$(LIBFT_DIR) -lft: Link your Libft
# -L$(MLX_DIR)/build: Set the library path for MLX42
# -lmlx42 -ldl -lglfw -pthread -lm: Link MLX42 and its dependencies
LIBS = -L$(LIBFT_DIR) -lft -L$(MLX_DIR)/build -lmlx42 -ldl -lglfw -pthread -lm

# --- libft --- #
LIBFT = ./src/libft
EX_LIB = $(LIBFT)/libft.a
B_LIBFT = ./src/libft
B_EX_LIB = $(B_LIBFT)/libft.a

# Conditional compilation for bonus or mandatory
ifdef BONUS
    TARGET_NAME = $(B_NAME)
    OBJECTS = $(BOBJS)
    HEADER = ./include/minirt_bonus.h
    LIB = $(B_EX_LIB)
    CURRENT_SRC_DIR = $(BSRC_DIR)
    CURRENT_OBJ_DIR = $(B_OBJ_DIR)
else
    TARGET_NAME = $(NAME)
    OBJECTS = $(OBJS)
    HEADER = ./include/minirt.h
    LIB = $(EX_LIB)
    CURRENT_SRC_DIR = $(SRC_DIR)
    CURRENT_OBJ_DIR = $(OBJ_DIR)
endif

# --- Color Output ---
GREEN  = \033[32m
YELLOW = \033[33m
BLUE   = \033[34m
RED    = \033[31m
RESET  = \033[0m

# --- Main Targets ---
.PHONY: all clean fclean re libmlx

#all: libmlx $(NAME)
all: libs libmlx $(TARGET_NAME)

libs:
	@echo "$(GREEN)Compilando libft...$(RESET)"
ifdef BONUS
	@make -C $(B_LIBFT)
else
	@make -C $(LIBFT)
endif

# Target to compile MLX42
libmlx:
	@echo "$(YELLOW)Compiling MLX42...$(RESET)"
	@cmake $(MLX_DIR) -B $(MLX_DIR)/build -DCMAKE_BUILD_TYPE=Release
	@make -C $(MLX_DIR)/build -j4

# Link all object files and libraries to create the final executable
$(NAME): $(OBJS) Makefile include/minirt.h
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

# Rule to compile bonus a .c file into a .o file
$(B_OBJ_DIR)/%.o: $(BSRC_DIR)/%.c | $(B_OBJ_DIR)
	@echo "$(BLUE)Compiling $<...$(RESET)"
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(HEADERS) -c $< -o $@

# $(NAME): $(OBJS) Makefile include/minirt.h
# 	@echo "$(YELLOW)Compiling and linking Libft...$(RESET)"
# 	@make -C $(LIBFT_DIR)
# 	@echo "$(BLUE)Linking $(NAME)...$(RESET)"
# 	$(CC) $(CFLAGS) $(OBJS) $(HEADERS) -o $(NAME) $(LIBS)


# Create object directories if they don't exist
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/render

$(B_OBJ_DIR):
	@mkdir -p $(B_OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/render

$(TARGET_NAME): $(LIB) $(OBJECTS) $(HEADER)
	@echo "$(GREEN)Compilando $(TARGET_NAME)...$(RESET)"
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET_NAME) $(LIB) $(LIBS)

bonus:
	@echo "$(GREEN)Compilando bonus...$(RESET)"
	@$(MAKE) BONUS=1 all --no-print-directory

# --- Clean Targets ---
clean:
	@echo "$(YELLOW)Cleaning object files...$(RESET)"
	@$(RM) $(OBJ_DIR) $(B_OBJ_DIR)
	@make clean -C $(LIBFT_DIR)
	@make clean -C $(B_LIBFT) 2>/dev/null || true
	@$(RM) -rf $(MLX_DIR)/build

fclean: clean
	@echo "$(YELLOW)Cleaning executable...$(RESET)"
	@$(RM) $(NAME) $(B_NAME)
	@make fclean -C $(LIBFT_DIR)
	# The MLX42 build directory is removed here
	@$(RM) -rf $(MLX_DIR)/build

re: fclean all