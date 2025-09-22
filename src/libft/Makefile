# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rmarrero <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/16 12:58:52 by rmarrero          #+#    #+#              #
#    Updated: 2025/06/30 15:53:55 by rmarrero         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# --- Directorios de los módulos externos --- #
PRINTF_DIR = libft_modules/ft_printf
GNL_DIR = libft_modules/gnl

# --- Directorio para los archivos de objeto --- #
OBJ_DIR = .obj

# --- Archivos de origen ---
SRC = ft_isascii.c ft_memcpy.c ft_putstr_fd.c ft_strlcat.c ft_strrchr.c ft_atoi.c ft_isdigit.c \
ft_memmove.c ft_strlcpy.c ft_strtrim.c ft_bzero.c ft_isprint.c ft_memset.c ft_strchr.c \
ft_strlen.c ft_calloc.c ft_putchar_fd.c ft_strdup.c ft_strmapi.c ft_tolower.c ft_substr.c \
ft_isalnum.c ft_memchr.c ft_putendl_fd.c ft_striteri.c ft_strncmp.c ft_toupper.c ft_isalpha.c \
ft_memcmp.c ft_putnbr_fd.c ft_strjoin.c ft_strnstr.c ft_split.c ft_itoa.c

BSRC = ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c ft_lstlast_bonus.c ft_lstadd_back_bonus.c \
ft_lstdelone_bonus.c ft_lstclear_bonus.c ft_lstiter_bonus.c ft_lstmap_bonus.c

# GNL_SRC debe tener las rutas completas para que el 'make' pueda encontrarlos
GNL_SRC = $(GNL_DIR)/get_next_line_bonus.c $(GNL_DIR)/get_next_line_utils_bonus.c

# --- Archivos de objeto ---
# Concatenamos todas las listas de origen y luego generamos los objetos
ALL_SRCS = $(SRC) $(BSRC) $(GNL_SRC)
ALL_OBJS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(notdir $(ALL_SRCS)))

# --- Nombres de las bibliotecas --- #
NAME = libft.a
FT_PRINTF_NAME = $(PRINTF_DIR)/libftprintf.a

# --- Compiladores y flags --- #
CC = cc
CFLAGS = -Wall -Werror -Wextra -I.  # Added -I. to find libft.h
RM = rm -f
AR = ar rcs

# --- Colores de la terminal --- #
RED      = \033[31m
GREEN    = \033[32m
YELLOW   = \033[33m
BLUE     = \033[34m
RESET    = \033[0m

# --- Reglas principales --- #
.PHONY: all bonus clean fclean re repos libft_build printf_build

all: repos libft_build printf_build

# 📚 Regla para compilar Libft.a (incluyendo GNL y bonus)
libft_build: $(ALL_OBJS) $(FT_PRINTF_NAME)
	@echo "$(GREEN)Compilando la librería principal $(NAME)...$(RESET)"
	$(AR) $(NAME) $(ALL_OBJS)
	@echo "$(GREEN)Añadiendo ft_printf.a a $(NAME)...$(RESET)"
	$(AR) $(NAME) $(PRINTF_DIR)/libftprintf.a
	@echo "$(BLUE)"
	@echo "     ( __)\      "
	@echo "     ( oo)\________ "
	@echo "     /|| \         \\"
	@echo "       ||------w |"
	@echo "       ||        ||"
	@echo "$(YELLOW)¡LA VACA HACE MUUUUUUUUUU!$(RESET)"
	@echo "$(RESET)"

# 🚀 Regla para compilar ft_printf.a
printf_build: $(FT_PRINTF_NAME)
	@echo "$(GREEN)Librería de ft_printf.a compilada.$(RESET)"

$(FT_PRINTF_NAME):
	@echo "$(GREEN)Compilando ft_printf usando su Makefile...$(RESET)"
	$(MAKE) -C $(PRINTF_DIR) CFLAGS="$(CFLAGS) -I../.." all bonus

# --- Reglas de compilación de objetos --- #
# Regla para los objetos de Libft que están en la raíz
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# ⚠️ FIX: Nueva regla explícita para los objetos de GNL
$(OBJ_DIR)/get_next_line_bonus.o: $(GNL_DIR)/get_next_line_bonus.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/get_next_line_utils_bonus.o: $(GNL_DIR)/get_next_line_utils_bonus.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# --- Regla para descargar los repositorios --- #
repos:
	@echo "$(YELLOW)Verificando y clonando repositorios...$(RESET)"
	@if [ ! -d "$(PRINTF_DIR)" ]; then \
		echo "$(GREEN)Clonando ft_printf...$(RESET)"; \
		git clone https://github.com/rogerdevworld/ft_printf.git $(PRINTF_DIR); \
	fi
	@if [ ! -d "$(GNL_DIR)" ]; then \
		echo "$(GREEN)Clonando get_next_line...$(RESET)"; \
		git clone https://github.com/rogerdevworld/get_next_line.git $(GNL_DIR); \
	fi

# --- Reglas de limpieza --- #
clean:
	@echo "$(YELLOW)Borrando archivos objeto...$(RESET)"
	$(RM) -r $(OBJ_DIR)
	$(MAKE) -C $(PRINTF_DIR) clean

fclean: clean
	@echo "$(YELLOW)Borrando todas las librerías y directorios...$(RESET)"
	$(RM) $(NAME) $(FT_PRINTF_NAME)
	$(MAKE) -C $(PRINTF_DIR) fclean
	$(RM) -r $(PRINTF_DIR) $(GNL_DIR) libft_modules
	
re: fclean all