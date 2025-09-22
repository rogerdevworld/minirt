/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 18:38:47 by rmarrero          #+#    #+#             */
/*   Updated: 2025/09/22 18:38:51 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef LIBFT_H
# define LIBFT_H

# include <fcntl.h>
# include <limits.h>
# include <stdarg.h>
# include <stddef.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
// # include "libft_modules/ft_printf/include/ft_printf_bonus.h"

# ifndef SIZE_MAX
#  define SIZE_MAX 4294967295
# endif

/*
** --- Structures for Specific Projects (e.g., Vector Math) ---
*/
typedef struct s_vec3
{
	double			x;
	double			y;
	double			z;
}					t_vec3;

/*
** --- Mandatory Libft Functions ---
*/

// --- Memory Manipulation ---
void				*ft_memset(void *s, int c, size_t n);
void				ft_bzero(void *s, size_t n);
void				*ft_memcpy(void *dest, const void *src, size_t n);
void				*ft_memmove(void *dest, const void *src, size_t n);
void				*ft_memchr(const void *s, int c, size_t n);
int					ft_memcmp(const void *s1, const void *s2, size_t n);

// --- Character Classification Functions ---
int					ft_isalpha(int c);
int					ft_isdigit(int c);
int					ft_isalnum(int c);
int					ft_isascii(int c);
int					ft_isprint(int c);
int					ft_isspace(int c);

// --- String Manipulation ---
size_t				ft_strlen(const char *str);
size_t				ft_strlcpy(char *dest, const char *src, size_t size);
size_t				ft_strlcat(char *dest, char *src, unsigned int size);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
int					ft_strcmp(const char *s1, const char *s2);
char				*ft_strchr(const char *s, int c);
char				*ft_strrchr(const char *str, char c);
char				*ft_strnstr(const char *big, const char *little,
						size_t len);
char				*ft_strdup(const char *s);
char				*ft_substr(char const *s, unsigned int start, size_t len);
char				*ft_strjoin(char const *s1, char const *s2);
char				*ft_strtrim(char const *s1, char const *set);
char				**ft_split(char const *s, char c);

// --- Character and Number Conversions ---
int					ft_toupper(int c);
int					ft_tolower(int c);
int					ft_atoi(const char *nptr);
long				ft_atol(const char *nptr);
double				ft_atof(const char *str);
char				*ft_itoa(int n);
void				*ft_calloc(size_t count, size_t size);
void				*ft_realloc(void *ptr, size_t new_size);

// --- File I/O ---
void				ft_putchar_fd(char c, int fd);
void				ft_putstr_fd(char *s, int fd);
void				ft_putendl_fd(char *s, int fd);
void				ft_putnbr_fd(int n, int fd);

// --- Additional Functions (Part 2) ---
char				*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void				ft_striteri(char *s, void (*f)(unsigned int, char *));

/*
** --- Linked List Functions (Bonus) ---
*/
typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

t_list				*ft_lstnew(void *content);
void				ft_lstadd_front(t_list **lst, t_list *new);
int					ft_lstsize(t_list *lst);
t_list				*ft_lstlast(t_list *lst);
void				ft_lstadd_back(t_list **lst, t_list *new);
void				ft_lstdelone(t_list *lst, void (*del)(void *));
void				ft_lstclear(t_list **lst, void (*del)(void *));
void				ft_lstiter(t_list *lst, void (*f)(void *));
t_list				*ft_lstmap(t_list *lst, void *(*f)(void *),
						void (*del)(void *));

// # include "libft_modules/gnl/get_next_line_bonus.h"

/*
** --- Utility Functions (Specific Projects) ---
*/

// --- Mathematical Functions ---
int					ft_sqrt(int nb);
double				ft_fabs_double(double x);

// --- Environment Handling Functions ---
char				*ft_getenv(char *name, char **env);

/*
** --- Vector Math Functions ---
*/
t_vec3				vec3_add(t_vec3 v1, t_vec3 v2);
t_vec3				vec3_sub(t_vec3 v1, t_vec3 v2);
t_vec3				vec3_mul(t_vec3 v, double scalar);
t_vec3				vec3_div(t_vec3 v, double scalar);
t_vec3				vec3_mult_vec(t_vec3 v1, t_vec3 v2);
double				vec3_dot(t_vec3 v1, t_vec3 v2);
t_vec3				vec3_cross(t_vec3 v1, t_vec3 v2);
double				vec3_length(t_vec3 v);
t_vec3				vec3_normalize(t_vec3 v);
t_vec3				vec3_reflect(t_vec3 v, t_vec3 n);
t_vec3				rotate_vector(t_vec3 v, t_vec3 axis, double angle);

#endif
