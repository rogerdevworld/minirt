/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:24:12 by rmarrero          #+#    #+#             */
/*   Updated: 2025/07/23 14:24:32 by rmarrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "../minilibx/mlx.h"    // Adjust path as needed for your Minilibx
# include "../src/libft/libft.h" // Adjust path as needed for your libft
# include <math.h>
# include <pthread.h> // For pthreads functions
# include <stdlib.h>  // For malloc, free, etc.

// --- Constants ---
# define EPSILON 1e-6
// Small value to avoid floating-point errors in comparisons
# define M_PI 3.14159265358979323846
// Definition of PI if not in math.h or for C99/C11

// --- 1. Vectors and Points ---
typedef struct s_vec3
{
	double				x;
	double				y;
	double				z;
}						t_vec3;

typedef t_vec3 t_color;

// --- 2. Ray ---
typedef struct s_ray
{
	t_vec3				origin;
	t_vec3 direction; // Always normalized
}						t_ray;

// --- 3. Camera ---
typedef struct s_camera
{
	t_vec3				position;
	t_vec3				orientation;
	double				fov;
}						t_camera;

// --- 4. Lights ---
typedef struct s_ambient_light
{
	double				ratio;
	t_vec3				color;
}						t_ambient_light;

// --- 5. Geometric Objects ---
// In include/minirt.h, somewhere before t_object struct
typedef enum e_object_type {
    OBJ_SPHERE,   // <-- Make sure these are exactly as written here
    OBJ_PLANE,
    OBJ_CYLINDER,
    // OBJ_CONE // Add if you implement cones
} t_object_type;

typedef struct s_sphere
{
	t_vec3				center;
	double				radius;
}						t_sphere;

// In include/minirt.h

// Plano
typedef struct s_plane {
    t_vec3 point;    // <--- THIS LINE IS STILL MISSING OR MISSPELLED! Make sure it's here.
    t_vec3 normal;   // Vector normalizado
} t_plane;
typedef struct s_cylinder
{
	t_vec3				position;
	t_vec3				axis;
	double				radius;
	double				height;
}						t_cylinder;


// Luz Puntual (Light)
typedef struct s_light {
    t_vec3 position;
    double brightness; // 0.0-1.0
    t_color color;     // 0.0-1.0
    struct s_light *next; // <--- ADD THIS LINE: For linked list
} t_light;

// ... (other object types like t_sphere, t_plane, t_cylinder) ...

// Objeto genérico
typedef struct s_object {
    t_object_type type;
    void *data; // Pointer to the specific struct (t_sphere, t_plane, t_cylinder)
    t_color color; // Object color (0.0-1.0)
    struct s_object *next; // <--- ADD THIS LINE: For linked list
} t_object;


// --- 6. Intersection Information (Hit Record) ---
typedef struct s_hit_record
{
	t_vec3				point;
	t_vec3				normal;
	double				t;
	t_object			*object;
}						t_hit_record;

// In include/minirt.h

// --- 7. Scene ---
typedef struct s_scene
{
	t_ambient_light		ambient;
	t_camera			camera;
	void *lights;  // Usaremos una lista enlazada (t_list*) para las luces
	void *objects; // Usaremos una lista enlazada (t_list*) para los objetos
	int					width;
	int					height;
	int has_camera;  // Para asegurar que solo haya una cámara
	int has_ambient; // Para asegurar que solo haya una luz ambiental
}						t_scene;

// --- 8. Minilibx Window and Image Management ---
// In include/minirt.h (around where t_mlx is defined)

typedef struct s_img {
    void    *img_ptr;   // Puntero a la imagen en la memoria de MLX
    char    *addr;      // Puntero al primer byte de los datos de píxeles
    int     bpp;        // Bits por píxel (bits per pixel)
    int     line_len;   // Longitud de una línea en bytes (size_line)
    int     endian;     // Orden de los bytes (endian)
    int     width;      // <--- ADD THIS LINE
    int     height;     // <--- ADD THIS LINE
} t_img;

typedef struct s_mlx
{
	void				*mlx_ptr;
	void				*win_ptr;
	t_img				img;
}						t_mlx;

// --- 9. Thread-Specific Data ---
typedef struct s_data	t_data;
// Forward declaration for t_thread_data to use t_data

typedef struct s_thread_data
{
	int id;           // <--- Ensure this is present
	int start_row;    // <--- Ensure this is present
	int end_row;      // <--- Ensure this is present
	t_data *global_data;
		// <--- Ensure this is present (uses the forward declaration)
}						t_thread_data;

// --- 10. Combined Scene and MLX Data (Global context) ---
// Define t_data after all other structs it might contain
// include/minirt.h
// Dentro de tu struct t_data:

typedef struct s_data
{
	t_mlx				mlx;
	t_scene				scene;
	int					num_threads;
	// --- Añade estas líneas ---
	int rendered_rows;              // Contador de filas ya procesadas
	pthread_mutex_t progress_mutex;
		// Mutex para proteger el acceso a rendered_rows
	// ------------------------
}						t_data;
// --- MLX initialization and management (from mlx_utils.c) ---
void					mlx_setup(t_data *data);
int						close_window(t_data *data);
int						key_hook(int keycode, t_data *data);
void					put_pixel_to_img(t_img *img, int x, int y, int color);

// --- Rendering (main rendering function now manages threads,
//	from render.c) ---
void					render_threaded(t_data *data);
void	*thread_render_rows(void *arg); // Function executed by each thread

// --- Initialization functions (from init_primitives.c) ---
t_vec3					vec3_init(double x, double y, double z);
t_ray					ray_init(t_vec3 origin, t_vec3 direction);
t_camera				camera_init(t_vec3 position, t_vec3 orientation,
							double fov);
t_ambient_light			ambient_light_init(double ratio, t_vec3 color);
t_light					light_init(t_vec3 position, double brightness,
							t_vec3 color);
t_sphere				sphere_init(t_vec3 center, double radius);
t_plane					plane_init(t_vec3 position, t_vec3 normal);
t_cylinder				cylinder_init(t_vec3 position, t_vec3 axis,
							double radius, double height);

// --- Vector operations (from vec_operations.c) ---
t_vec3					vec3_add(t_vec3 v1, t_vec3 v2);
t_vec3					vec3_sub(t_vec3 v1, t_vec3 v2);
t_vec3					vec3_mul(t_vec3 v, double scalar);
t_vec3					vec3_div(t_vec3 v, double scalar);
t_vec3	vec3_mult_vec(t_vec3 v1, t_vec3 v2); // Component-wise multiplication
double					vec3_dot(t_vec3 v1, t_vec3 v2);
t_vec3					vec3_cross(t_vec3 v1, t_vec3 v2);
double					vec3_length(t_vec3 v);
t_vec3					vec3_normalize(t_vec3 v);
t_vec3					ray_at(t_ray r, double t);
t_vec3					vec3_reflect(t_vec3 v, t_vec3 n);

// --- Intersection functions (from intersections/*.c) ---
// (You'll define these as you implement them)
// int intersect_sphere(t_ray *ray, t_sphere *sp, t_hit_record *rec);
// int intersect_plane(t_ray *ray, t_plane *pl, t_hit_record *rec);
// int intersect_cylinder(t_ray *ray, t_cylinder *cy, t_hit_record *rec);

// --- Rendering functions (from render.c) ---
// t_vec3 trace_ray(t_ray *ray, t_scene *scene); // This might be more complex,
	//maybe inside thread_render_rows
// t_vec3 calculate_light(t_hit_record *rec, t_scene *scene);

// include/parser.h
// --- Error codes / Messages ---

# include <fcntl.h>
# include <unistd.h>

// --- Error Messages (Defines) ---
# define ERR_TOO_MANY_ARGS "Error\nToo many arguments.\n"
# define ERR_INVALID_FILE "Error\nInvalid file or path.\n"
# define ERR_INVALID_EXT "Error\nInvalid file extension. Use .rt\n"
# define ERR_INVALID_LINE "Error\nInvalid line in .rt file.\n"
# define ERR_MISSING_ELEMENT "Error\nMissing mandatory element (A, C).\n"
# define ERR_DUPLICATE_ELEMENT "Error\nDuplicate mandatory element (A, C).\n"
# define ERR_INVALID_PARAMS "Error\nInvalid parameters for element.\n"
# define ERR_MEMORY "Error\nMemory allocation failed.\n"

// --- Main Parser Functions ---
void					parse_rt_file(const char *file_path, t_data *data);

// --- Validation & Helper Functions ---
void					validate_args(int argc, char **argv);
int						is_valid_float(char *str);
int						is_valid_int(char *str);
int	is_valid_color_component(char *str);
		// Checks if a component is between 0-255
int	is_normalized_vector(t_vec3 vec);   
		// Checks if vector components are between -1.0 and 1.0

// Functions for parsing common data types from strings
t_vec3					parse_vector(char *str, char *err_msg);
t_vec3					parse_color(char *str, char *err_msg);
double					parse_double(char *str, char *err_msg);
double	parse_fov(char *str, char *err_msg);  
		// Specific for camera FOV validation (0-180)
double	parse_ratio(char *str, char *err_msg);
		// Specific for ambient light ratio (0.0-1.0)
t_object				*create_object(t_object_type type, void *data,
							t_vec3 color);

// This is the new helper function for counting elements in a char** array
int						count_tokens(char **tokens);

// --- Functions to Parse Specific Scene Elements ---
void					parse_ambient_light(char **tokens, t_data *data);
void					parse_camera(char **tokens, t_data *data);
void					parse_light(char **tokens, t_data *data);
void					parse_sphere(char **tokens, t_data *data);
void					parse_plane(char **tokens, t_data *data);
void					parse_cylinder(char **tokens, t_data *data);

// --- Error Handling & Cleanup Functions ---
void					ft_error_exit(const char *msg);
void	free_tokens(char **tokens);     
		// For freeing char** arrays (like from ft_split)
void	free_scene_data(t_scene *scene);
		// Function to free linked lists of objects/lights
int color_to_int(t_color color); // Add this line!
void put_pixel_to_img(t_img *img, int x, int y, int color);
void    ft_free_str_array(char **arr);
void    parse_ambient_light(char **tokens, t_data *data);
int parse_scene(const char *file_path, t_data *data);

#endif // MINIRT_H