#ifndef RT_H
# define RT_H

// --- 0. Includes de Librerías ---
# include "../minilibx/mlx.h"    // Minilibx library for window and events
# include "../src/libft/libft.h" // Your personal library
# include <math.h>               // For mathematical operations (sin, cos, etc.)
# include <pthread.h>            // For multithreading functions
# include <stdlib.h>             // For malloc, free, and exit
# include <X11/keysym.h>         // For keycode definitions
# include <errno.h>

// --- 1. Constantes y Macros ---
# define EPSILON 1e-6          // Pequeño valor para evitar errores de punto flotante
#ifndef M_PI
# define M_PI 3.14159265358979323846
#endif
#define EISDIR      21
# define KEY_W XK_w            // Tecla 'W' para movimiento
# define KEY_A XK_a            // Tecla 'A' para movimiento
# define KEY_S XK_s            // Tecla 'S' para movimiento
# define KEY_D XK_d            // Tecla 'D' para movimiento
# define KEY_ESC XK_Escape     // Tecla 'Esc' para salir

// --- 2. Macros de Errores ---
# define ERR_TOO_MANY_ARGS "Error\nToo many arguments.\n"
# define ERR_INVALID_FILE "Error\nInvalid file or path.\n"
# define ERR_INVALID_EXT "Error\nInvalid file extension. Use .rt\n"
# define ERR_INVALID_LINE "Error\nInvalid line in .rt file.\n"
# define ERR_MISSING_ELEMENT "Error\nMissing mandatory element (A, C).\n"
# define ERR_DUPLICATE_ELEMENT "Error\nDuplicate mandatory element (A, C).\n"
# define ERR_INVALID_PARAMS "Error\nInvalid parameters for element.\n"
# define ERR_MEMORY "Error\nMemory allocation failed.\n"

// --- 3. Tipos de Datos y Estructuras Fundamentales ---
// --- Basado en minirt_vec_types.h de bwerner ---
typedef struct s_vec2
{
    double          x;
    double          y;
}                   t_vec2;

typedef union u_vec3
{
    struct
    {
        double      x;
        double      y;
        double      z;
    };
    struct
    {
        double      r;
        double      g;
        double      b;
    };
}                   t_vec3;

typedef t_vec3      t_color;

typedef struct s_ray
{
    t_vec3          origin;
    t_vec3          direction;
}                   t_ray;

// --- 4. Estructuras de Materiales y Superficies ---
typedef enum e_texture_type
{
    TEX_SOLID,
    TEX_CHECKERBOARD,
    TEX_IMAGE
}                   t_texture_type;

typedef struct s_texture
{
    t_texture_type      type;
    char                *file_path; // Solo para TEX_IMAGE
    t_vec3              color1;     // Color principal (para todos los tipos)
    t_vec3              color2;     // Para TEX_CHECKERBOARD
    double              scale;      // Para TEX_CHECKERBOARD
}                       t_texture;

typedef struct s_material
{
    // Propiedades básicas (como en tu versión)
    t_vec3          base_color;
    double          mirror_ratio;
    
    // Propiedades de superficie y bonificaciones
    t_vec3          emission_color; // Para objetos que emiten luz
    double          emission_strength;
    double          metallic;       // Reflectividad
    double          roughness;      // Suavidad/dureza de la reflexión
    double          ior;            // Índice de refracción (para transparencia)
    double          transmission;   // Cantidad de luz que pasa a través
    t_texture       color_texture;  // Textura para el color
    t_texture       normal_map;     // Mapa de relieve para detalles de superficie
    double          normal_strength;
}                       t_material;

// Información de un impacto (hit)
typedef struct s_hit_record
{
    t_vec3          point;
    t_vec3          normal;
    double          t;
    t_ray           ray;
    struct s_object *object;
}                   t_hit_record;

// --- 5. Estructuras de Elementos de la Escena ---

// Cámara
typedef struct s_camera
{
    t_vec3          position;
    t_vec3          orientation;
    double          fov;
    t_vec3          forward;
    t_vec3          right;
    t_vec3          up;
}                   t_camera;

// Luces
typedef struct s_ambient_light
{
    double          ratio;
    t_vec3          color;
}                   t_ambient_light;

typedef struct s_light
{
    t_vec3          position;
    double          brightness;
    t_vec3          color;
}                   t_light;

// Tipos de objetos geométricos
typedef enum e_object_type
{
    SPHERE,
    PLANE,
    CYLINDER,
    CONE,
    HYPERBOLOID,
    PARABOLOID,
}                   t_object_type;

// Esfera
typedef struct s_sphere
{
    t_vec3          center;
    double          radius;
    t_material      material;
}                   t_sphere;

// Plano
typedef struct s_plane
{
    t_vec3          position;
    t_vec3          normal;
    t_material      material;
}                   t_plane;

// Cilindro
typedef struct s_cylinder
{
    t_vec3          position;
    t_vec3          axis;
    double          radius;
    double          height;
    t_material      material;
}                   t_cylinder;

// Cono
typedef struct s_cone
{
    t_vec3              position;
    t_vec3              axis;
    double              radius;
    double              height;
    t_material          material;
}                       t_cone;

// Hiperboloide
typedef struct s_hyperboloid
{
    t_vec3              position;
    t_vec3              axis;
    double              radius_a;
    double              radius_b;
    double              height;
    t_material          material;
}                       t_hyperboloid;

// Paraboloide
typedef struct s_parab
{
    t_vec3          position;
    t_vec3          axis;
    double          focal_lenght;
    double          height;
    t_material      material;
}                   t_parab;

// Objeto genérico
typedef struct s_object
{
    t_object_type   type;
    void            *data;
}                   t_object;

// --- 6. Estructuras de Control del Programa ---
typedef struct s_scene
{
    t_ambient_light     ambient;
    t_camera            camera;
    void                *lights;
    void                *objects;
    int                 width;
    int                 height;
    int                 has_camera;
    int                 has_ambient;
    t_color         background_color;
}                       t_scene;

typedef struct s_img
{
    char            *addr;
    void            *img_ptr;
    int             bpp;
    int             line_len;
    int             endian;
    int             width;
    int             height;
}                   t_img;

typedef struct s_mlx
{
    void            *mlx_ptr;
    void            *win_ptr;
    t_img           img;
}                   t_mlx;

typedef struct s_data
{
    t_mlx           mlx;
    t_scene         scene;
    int             num_threads;
    int             rendered_rows;
    int             show_progress;
    pthread_mutex_t progress_mutex;
}                   t_data;

typedef struct s_thread_data
{
    int             id;
    int             start_row;
    int             end_row;
    t_data          *global_data;
}                   t_thread_data;

// --- 7. Prototipos de Funciones de Renderizado ---
void                    mlx_setup(t_data *data);
int                     close_window(t_data *data);
int                     key_hook(int keycode, t_data *data);
int                     mouse_press(int button, int x, int y, t_data *data);
int                     mouse_release(int button, int x, int y, t_data *data);
int                     mouse_move(int x, int y, t_data *data);
void                    render_threaded(t_data *data);
void                    *thread_render_rows(void *arg);
void                    put_pixel_to_img(t_img *img, int x, int y, int color);

// --- 8. Prototipos de Funciones del Ray Tracer ---
t_ray                   generate_ray(int x, int y, t_scene *scene);
t_hit_record            find_closest_hit(t_ray *ray, t_scene *scene);
t_color                 calculate_light(t_hit_record *rec, t_scene *scene, t_ray *ray);
int                     color_to_int(t_color color);
t_vec3                  get_texture_color(t_hit_record *rec);
t_vec3                  apply_bump_map(t_hit_record *rec);

// --- 9. Prototipos de Funciones de Intersección ---
int                     intersect_sphere(t_ray *ray, t_sphere *sp, t_hit_record *rec);
int                     intersect_plane(t_ray *ray, t_plane *pl, t_hit_record *rec);
int                     intersect_cylinder(t_ray *ray, t_cylinder *cy, t_hit_record *rec);
int                     intersect_cone(t_ray *ray, t_cone *co, t_hit_record *rec);
int                     intersect_hyperboloid(t_ray *ray, t_hyperboloid *hb, t_hit_record *rec);
int                     intersect_paraboloid(t_ray *ray, t_parab *pa, t_hit_record *rec);

// --- 10. Prototipos de Funciones del Parser ---
void                    apply_object_modifiers(t_object *obj, char **tokens, int start_idx);
int                     open_filename(const char *filename);
int                     valid_extension_rt(const char *file_name);
void                    validate_file(int fd, const char *file_name);
double                  parse_fov(char *str);
double                  parse_angle(char *str);
double                  parse_positive_double(char *str);
void                    parse_rt_file(t_scene *scene, const char *file_path);
void                    parse_ambient(t_scene *scene, char **tokens);
void                    parse_camera(t_scene *scene, char **tokens);
void                    parse_light(t_scene *scene, char **tokens);
void                    parse_sphere(t_scene *scene, char **tokens);
void                    parse_plane(t_scene *scene, char **tokens);
void                    parse_cylinder(t_scene *scene, char **tokens);
void                    parse_cone(t_scene *scene, char **tokens);
void                    parse_hyperboloid(t_scene *scene, char **tokens);
void                    parse_paraboloid(t_scene *scene, char **tokens);
t_vec3                  parse_vec3(char *str);
t_vec3                  parse_vec3_color(char *str);
t_vec3                  parse_vec3_normalized(char *str);
t_object                *create_object(t_object_type type, void *data);

// --- 11. Prototipos de Funciones de Inicialización ---
t_vec3                  vec3_init(double x, double y, double z);
t_ray                   ray_init(t_vec3 origin, t_vec3 direction);
t_camera                camera_init(t_vec3 position, t_vec3 orientation, double fov);
t_ambient_light         ambient_light_init(double ratio, t_vec3 color);
t_light                 light_init(t_vec3 position, double brightness, t_vec3 color);
t_material              material_init(t_vec3 base_color, double mirror_ratio);
t_sphere                sphere_init(t_vec3 center, double radius, t_material material);
t_plane                 plane_init(t_vec3 position, t_vec3 normal, t_material material);
t_cylinder              cylinder_init(t_vec3 position, t_vec3 axis, double radius, double height, t_material material);
t_cone                  cone_init(t_vec3 position, t_vec3 axis, double radius, double height, t_material material);
t_hyperboloid           hyperboloid_init(t_vec3 position, t_vec3 axis, double radius_a, double radius_b, double height, t_material material);
t_parab                 parab_init(t_vec3 position, t_vec3 axis, double focal_lenght, double height, t_material material);

// --- 12. Prototipos de Funciones de Utilidades ---
t_vec3                  vec3_add(t_vec3 v1, t_vec3 v2);
t_vec3                  vec3_sub(t_vec3 v1, t_vec3 v2);
t_vec3                  vec3_mul(t_vec3 v, double scalar);
t_vec3                  vec3_div(t_vec3 v, double scalar);
t_vec3                  vec3_mult_vec(t_vec3 v1, t_vec3 v2);
double                  vec3_dot(t_vec3 v1, t_vec3 v2);
t_vec3                  vec3_cross(t_vec3 v1, t_vec3 v2);
double                  vec3_length(t_vec3 v);
t_vec3                  vec3_normalize(t_vec3 v);
t_vec3                  ray_at(t_ray r, double t);
t_vec3                  vec3_reflect(t_vec3 v, t_vec3 n);
t_vec3                  rotate_vector(t_vec3 v, t_vec3 axis, double angle);

// --- 13. Prototipos de Funciones Auxiliares ---
void                    validate_args(int argc, char **argv);
int                     is_valid_float(char *str);
int                     is_valid_int(char *str);
int                     is_valid_color_component(char *str);
int                     is_normalized_vector(t_vec3 vec);
void                    ft_error_exit(const char *msg);
void                    add_object_to_scene(t_scene *scene, t_object *obj);
void                    add_light_to_scene(t_scene *scene, t_light *light);
int                     ft_strarr_len(char **arr);
double                  ft_atod(const char *str);
void                    ft_free_str_array(char **arr);
void                    free_tokens(char **tokens);
void                    free_scene_data(t_scene *scene);
void                    cleanup_program(t_data *data);
int                     is_in_shadow(t_ray *shadow_ray, t_scene *scene, t_light *light);
void                    init_scene(t_scene *scene);

#endif