
t_ray	generate_antialiased_ray(t_subpixel_data sp, t_scene *scene)
{
	t_ray		ray;
	t_ray_data	data;
	t_vec3		direction;

	data = calculate_ray_data(sp, scene);
	ray.origin = scene->camera.position;
	direction = vec3_add(vec3_add(data.viewport_center,
				vec3_mul(data.camera_right, 2.0 * tan(data.fov_rad / 2.0)
					* data.aspect_ratio * data.x_coord_normalized)),
			vec3_mul(data.camera_up, 2.0 * tan(data.fov_rad / 2.0)
				* data.y_coord_normalized));
	ray.direction = vec3_sub(direction, ray.origin);
	ray.direction = vec3_normalize(ray.direction);
	return (ray);
}
```
- **Gestión de la Escena:** El código incluye funciones para gestionar la escena de forma dinámica, permitiendo la adición y liberación de objetos y luces de la memoria.

***

### Estructuras y Funciones de Vectores

El código utiliza estructuras para vectores 3D y 2D, que son los cimientos de todas las operaciones geométricas. A continuación, se detallan las estructuras y se muestran ejemplos de cómo se utilizan las funciones asociadas en el código del proyecto.

#### Estructuras de Vector

##### `t_vec3`
Esta estructura representa un vector o un punto en un espacio 3D.
```c
typedef struct s_vec3
{
    double x;
    double y;
    double z;
} t_vec3;
````

##### `t_vec2`

Esta estructura representa un vector o una coordenada en un espacio 2D, comúnmente utilizada para el mapeo de texturas o las coordenadas de la pantalla.

```c
typedef struct s_vec2
{
    double u;
    double v;
} t_vec2;
```

#### Fragmentos de Código de Funciones de Vectores (`t_vec3`)

Aunque el código de implementación completo de las funciones no se encuentra disponible, los siguientes fragmentos de código muestran su uso en el proyecto:

##### `vec3_init`

Se usa para inicializar un vector con valores específicos.

```c
t_ambient_light ambient_light_init(double ratio, t_vec3 color)
{
    t_ambient_light al;
    al.ratio = ratio;
    al.color = color;
    return (al);
}
```

##### `vec3_normalize`

Normaliza un vector para que su longitud sea 1, lo cual es crucial para las direcciones de los rayos y las normales de las superficies.

```c
t_ray ray_init(t_vec3 origin, t_vec3 direction)
{
    t_ray r;
    r.origin = origin;
    r.direction = vec3_normalize(direction);
    return (r);
}
```

##### `vec3_sub`

Resta un vector de otro.

```c
data->scene.camera.forward = vec3_normalize(vec3_sub(center, data->scene.camera.position));
```

##### `vec3_mul`

Multiplica un vector por un escalar.

```c
return (vec3_add(r.origin, vec3_mul(r.direction, t)));
```

##### `vec3_cross`

Calcula el producto cruz entre dos vectores.

```c
data->scene.camera.right = vec3_normalize(vec3_cross( data->scene.camera.forward, (t_vec3){0, 1, 0}));
```

##### `vec3_dot`

Calcula el producto punto entre dos vectores.

```c
denom = vec3_dot(ray->direction, cy->axis);
```

##### `vec3_reflect`

Se usa para calcular el vector de reflexión.

```c
// reflect_dir = vec3_reflect(vec3_mul(to_light, -1.0), effective_normal);
```

-----

## Funciones Matemáticas y Fórmulas (Desde Mi Punto de Vista)

Desde la perspectiva de un desarrollador, un proyecto como `mini rt` es un fascinante ejercicio de matemática aplicada. A continuación, se detalla cómo se aplican los conceptos matemáticos clave en el código:

  - **Uso de la librería `math` (`cos`, `tan`, `atan`, `atan2`, `M_PI`, etc.):** Se utilizan funciones como `atan2` y constantes como `M_PI` para realizar cálculos complejos, especialmente en el mapeo de texturas. Por ejemplo, en una función como `get_uv_sphere`, estas se emplean para mapear un punto 3D en la superficie de una esfera a una coordenada 2D para aplicar una textura. Esto implica convertir un punto de coordenadas cartesianas a esféricas para obtener las coordenadas UV correctas.
  - **Fórmulas de Intersección:** Aunque la lógica de intersección principal no es completamente visible en los fragmentos de código proporcionados, la estructura sugiere que funciones como `intersect` contienen fórmulas fundamentales de álgebra lineal y geometría. Estas fórmulas resuelven el punto de intersección entre un rayo y una primitiva geométrica (por ejemplo, una esfera o un plano). Esto implica resolver ecuaciones cuadráticas para esferas o ecuaciones de plano para los planos.
  - **Normalización de Vectores:** Un concepto crítico visto en todo el código (por ejemplo, en `ray_init`, `camera_init`, `plane_init`, etc.) es la normalización de vectores. Un vector normalizado (un vector con una longitud de 1) simplifica los cálculos de dirección y luz, ya que es más fácil calcular productos punto y otras operaciones sin que se vean afectadas por la magnitud del vector.
  - **Producto Punto (`vec3_dot`):** El producto punto es una herramienta esencial para la iluminación y las sombras. Determina el ángulo entre dos vectores. Un producto punto positivo significa que los vectores apuntan en una dirección similar (por ejemplo, una superficie que mira hacia una fuente de luz), mientras que uno negativo significa que se están alejando.
### Funciones Matemáticas y Álgebra del Proyecto

Desde la perspectiva de un desarrollador, el proyecto `mini rt` es un fascinante ejercicio de matemática aplicada. Se basa en una serie de fórmulas de álgebra lineal y trigonometría para simular la física de la luz y la geometría 3D. A continuación, se detalla cómo se aplican estos conceptos clave en el código.

---

### Uso de la Librería `math`

El proyecto utiliza funciones de la librería estándar `math.h` para realizar cálculos complejos que van más allá de las operaciones vectoriales básicas.

- **`sin` y `cos`:** Estas funciones son esenciales para la rotación de vectores y la manipulación de la cámara. Se utilizan para girar puntos o direcciones alrededor de un eje, basándose en un ángulo dado. Por ejemplo, en el código para rotar un vector, se aplicarían las fórmulas de rotación que dependen del seno y coseno del ángulo.
- **`tan`:** La función tangente se utiliza para definir la proyección de los rayos en el plano de la cámara, como se ve en la función `generate_antialiased_ray`. La fórmula `2.0 * tan(fov_rad / 2.0)` es una parte fundamental de la geometría de la cámara, que relaciona el ángulo de visión con el tamaño del plano de la imagen.
- **`atan` y `atan2`:** La función `atan2` (arcotangente con dos argumentos) es particularmente útil para el mapeo de texturas en objetos como esferas. Permite convertir un punto 3D en la superficie de la esfera en coordenadas 2D (U,V) para la textura. La fórmula utiliza `atan2` para calcular el ángulo polar y el ángulo acimutal del punto, lo que se traduce en las coordenadas de la textura.
- **`M_PI`:** Esta constante representa el valor de Pi (π). Se utiliza para conversiones de ángulos (por ejemplo, de grados a radianes) y en cualquier cálculo que involucre círculos o esferas, como el volumen, el área o, en este caso, el mapeo de texturas.

---

### Álgebra Lineal en Ray Tracing

El código del proyecto implementa conceptos clave del álgebra lineal para simular el comportamiento de la luz y las interacciones geométricas.

#### **Fórmulas de Intersección**

La parte más crucial de un motor de trazado de rayos es determinar si un rayo intersecta con un objeto y dónde. Esto se logra resolviendo ecuaciones matemáticas:

- **Intersección Rayo-Esfera:** Un rayo puede representarse como una ecuación paramétrica `P(t) = O + tD`, donde `O` es el origen y `D` es la dirección. Una esfera se define por la ecuación `(P - C) · (P - C) = r²`, donde `C` es el centro y `r` es el radio. Para encontrar la intersección, se sustituye la ecuación del rayo en la ecuación de la esfera y se resuelve para `t`. Esto resulta en una ecuación cuadrática (`at² + bt + c = 0`), cuya solución te indica si hay una, dos o ninguna intersección.
- **Intersección Rayo-Plano:** Un plano se define por un punto en él y un vector normal. La intersección se encuentra resolviendo la ecuación `(P(t) - Q) · N = 0`, donde `Q` es un punto en el plano y `N` es su vector normal. Esta es una ecuación lineal simple que se resuelve para `t`, lo que indica la distancia a la que se encuentra la intersección.

#### **Normalización de Vectores (`vec3_normalize`)**

Normalizar un vector significa escalarlo para que su **longitud o magnitud sea 1**. Esto es crítico porque simplifica los cálculos de la dirección sin que la magnitud afecte el resultado. En el ray tracing, los vectores de dirección de los rayos y los vectores normales de las superficies deben estar normalizados para que las fórmulas de iluminación, como el producto punto, funcionen correctamente.

#### **Producto Punto (`vec3_dot`)**

El producto punto es una operación fundamental en el renderizado 3D. El resultado es un escalar que indica la relación angular entre dos vectores.

- **Iluminación:** El producto punto entre el vector normal de una superficie y el vector que apunta hacia una fuente de luz determina cuánta luz recibe esa superficie. Si el resultado es positivo y cercano a 1, la luz incide directamente. Si es cero, la luz es paralela a la superficie. Si es negativo, la luz está incidiendo en la cara opuesta.
- **Sombras:** Se usa para determinar si un objeto se encuentra en la sombra, calculando la distancia a la que un rayo de sombra se encuentra con un objeto. La fórmula `is_in_shadow` utiliza el producto punto para saber si el rayo de sombra alcanza una superficie antes que la luz.
-----

## Mejoras y Optimizaciones de Calidad

Para un renderizado de alta calidad, el rendimiento y la fidelidad visual son clave. El código sugiere varias técnicas:

  - **Procesamiento Paralelo:** La función `get_num_processors` indica el uso potencial de múltiples hilos (Pthreads) para paralelizar el proceso de trazado de rayos. Al distribuir el trabajo de renderizar la imagen entre varios núcleos de la CPU, el motor puede lograr tiempos de renderizado más rápidos, especialmente para escenas complejas.
  - **Anti-Aliasing de Subpíxeles:** Esta técnica, también conocida como supersampling, consiste en lanzar múltiples rayos por píxel y promediar los resultados para suavizar los bordes irregulares o "dentados". Aunque no se muestra directamente en los fragmentos de código, esta sería una característica crucial para mejorar la calidad visual de la imagen final.
### Mejoras y Optimizaciones de Calidad

Para un renderizado de alta calidad, el rendimiento y la fidelidad visual son clave. Tu código implementa técnicas avanzadas para lograrlo, como el procesamiento paralelo y el anti-aliasing.

-----

### Procesamiento Paralelo (Multihilo)

El proyecto utiliza **Pthreads** para distribuir la carga de trabajo de renderizado entre múltiples núcleos de la CPU, permitiendo que la imagen se genere de forma mucho más rápida. Esto se logra dividiendo la imagen en filas y asignando bloques de filas a cada hilo.

#### Funciones Clave

1.  **`render_thread_func`:** Esta es la función principal que cada hilo ejecuta de forma independiente. Su trabajo es recorrer una sección específica de filas de la imagen, renderizar cada píxel y luego actualizar el progreso del renderizado.

    ```c
    void *render_thread_func(void *arg)
    {
        t_thread_data *thread_data;
        t_data *data;
        int y;

        thread_data = (t_thread_data *)arg;
        data = thread_data->global_data;
        y = thread_data->start_row;
        while (y < thread_data->end_row)
        {
            render_row(data, y);
            pthread_mutex_lock(&data->progress_mutex);
            data->rendered_rows++;
            pthread_mutex_unlock(&data->progress_mutex);
            y++;
        }
        return (NULL);
    }
    ```

2.  **`create_and_run_threads`:** Esta función orquesta la creación de los hilos. Determina cuántos hilos crear, calcula la cantidad de filas que cada hilo debe procesar (`rows_per_thread`) y luego usa la función `pthread_create` para lanzar cada hilo. También se usa un **mutex** (`data->progress_mutex`) para evitar que múltiples hilos actualicen el contador de progreso al mismo tiempo, lo que podría causar errores de sincronización.

    ```c
    void create_and_run_threads(t_data *data, pthread_t *threads, t_thread_data *thread_data)
    {
        t_thread_init_data init_d;
        int rows_per_thread;
        int start_row;

        init_d.td = thread_data;
        init_d.rows_per_thread = data->scene.height / data->num_threads;
        init_d.i = 0;
        start_row = 0;
        while (init_d.i < data->num_threads)
        {
            init_d.start_row = &start_row;
            init_thread_data(&init_d, data);
            if (pthread_create(&threads[init_d.i], NULL, render_thread_func, &thread_data[init_d.i]) != 0)
            {
                printf("Error\nFallo al crear el hilo %d.\n", init_d.i);
                while (--init_d.i >= 0)
                    pthread_join(threads[init_d.i], NULL);
                pthread_mutex_destroy(&data->progress_mutex);
                exit(1);
            }
            init_d.i++;
        }
    }
    ```

-----

### Anti-Aliasing de Subpíxeles

Esta técnica, también conocida como **supersampling**, es un método para mejorar la calidad visual de la imagen final. El código no lanza un solo rayo por píxel, sino varios "subrayos" dentro de cada píxel, promediando el color final. Esto suaviza los bordes irregulares ("dentados") y reduce el parpadeo en las animaciones.

#### Funciones Clave

1.  **`generate_antialiased_ray`:** La función que genera el rayo para un subpíxel específico. El cálculo de la dirección del rayo incluye un `offset` (`sp.sub_x` y `sp.sub_y`) que posiciona el rayo con precisión en el espacio del subpíxel.

    ```c
    t_ray	generate_antialiased_ray(t_subpixel_data sp, t_scene *scene)
    {
    	t_ray		ray;
    	t_ray_data	data;
    	t_vec3		direction;

    	data = calculate_ray_data(sp, scene);
    	ray.origin = scene->camera.position;
    	direction = vec3_add(vec3_add(data.viewport_center,
    				vec3_mul(data.camera_right, 2.0 * tan(data.fov_rad / 2.0)
    					* data.aspect_ratio * data.x_coord_normalized)),
    			vec3_mul(data.camera_up, 2.0 * tan(data.fov_rad / 2.0)
    				* data.y_coord_normalized));
    	ray.direction = vec3_sub(direction, ray.origin);
    	ray.direction = vec3_normalize(ray.direction);
    	return (ray);
    }
    ```

2.  **`render_pixel`:** Esta función se encarga de lanzar los múltiples rayos dentro de un píxel. Itera sobre los subpíxeles, suma el color de cada sub-rayo y luego promedia el resultado para obtener el color final del píxel, lo que suaviza los bordes.

    ```c
    t_color render_pixel(int x, int y, t_data *data)
    {
    	t_color final_color;
    	t_subpixel_data sp;
    	final_color = vec3_init(0.0, 0.0, 0.0);
    	sp.x = x;
    	sp.y = y;
    	sp.sub_y = 0;
    	while (sp.sub_y < SUBPIXEL_SAMPLES)
    	{
    		sp.sub_x = 0;
    		while (sp.sub_x < SUBPIXEL_SAMPLES)
    		{
    			final_color = vec3_add(final_color, calculate_subpixel_color(data, sp));
    			sp.sub_x++;
    		}
    		sp.sub_y++;
    	}
    	return (final_color);
    }
    ```
-----

## Vista Previa de la Escena

El mapeo de texturas y normales es fundamental para darle a tus objetos en `mini rt` una apariencia realista sin aumentar la complejidad geométrica. Tu proyecto maneja estos conceptos de manera modular a través de estructuras de datos y funciones específicas para cada tipo de figura.

-----

### Mapeo de Texturas (UV Mapping)

El mapeo de texturas en tu proyecto se basa en el principio de proyectar una imagen 2D sobre una superficie 3D. A cada punto de intersección de un rayo con un objeto, se le asigna una coordenada **UV** (`(u,v)`) que corresponde a un punto en la textura.

#### 1\. Estructura de Material (`t_material`)

La estructura `t_material` en tu código es el punto de partida para el manejo de texturas y mapas. Contiene los campos `has_texture` y `has_normal_map` para indicar si un objeto tiene estas propiedades, y punteros a las imágenes de la textura (`texture_img`) y el color (`color_img`).

```c
typedef struct s_material {
    ...
    bool has_texture;
    bool has_normal_map;
    mlx_texture_t *color_img;
    mlx_texture_t *texture_img;
    ...
} t_material;
```

Estos campos permiten al motor de renderizado determinar en cada punto de intersección si debe obtener el color y la normal de un archivo de imagen en lugar de usar los valores predeterminados del objeto.

#### 2\. Funciones de Coordenadas UV por Figura

Tu código utiliza funciones dedicadas para calcular las coordenadas UV para cada figura geométrica. Estas funciones (`get_uv_sphere`, `get_uv_plane`, etc.) toman el punto de intersección y lo "desenvuelven" en el espacio 2D de la textura.

  - **Esfera:** El mapeo UV de una esfera típicamente utiliza coordenadas polares. Un punto en la superficie de la esfera se proyecta a coordenadas `(u, v)` de la siguiente manera:

      * `u` se calcula a partir del ángulo de la coordenada `x` en el plano horizontal (longitud).
      * `v` se calcula a partir del ángulo de la coordenada `y` vertical (latitud).

  - **Plano:** El mapeo UV para un plano es el más sencillo, ya que el plano ya es una superficie 2D. Las coordenadas UV se calculan directamente a partir de las coordenadas del punto de intersección en el plano, ajustadas por una escala para controlar la repetición de la textura.

  - **Cilindro:** El mapeo UV para un cilindro se descompone en tres partes: la superficie lateral y las dos tapas circulares.

      * **Superficie lateral:** El `u` se calcula en función del ángulo alrededor del eje del cilindro, y el `v` se calcula en función de la altura del punto de intersección.
      * **Tapas:** El mapeo para las tapas es similar al de un círculo, donde las coordenadas `(u, v)` se calculan desde el centro del círculo hacia afuera.

-----

### Mapeo de Texturas por Figura: Teoría y Código de Ejemplo

El proceso de mapeo UV es diferente para cada figura, ya que la forma en que se "desenvuelve" la superficie varía. A continuación se presentan las funciones de ejemplo para cada una, mostrando cómo se implementa la teoría.

#### **Mapeo de Esfera**

El mapeo de texturas para una esfera se basa en las coordenadas esféricas (latitud y longitud).

  * **Teoría:**
    Se utiliza la trigonometría para convertir las coordenadas cartesianas `(x, y, z)` del punto de impacto en la superficie de la esfera a coordenadas angulares. La coordenada `u` (longitud) se calcula usando la función `atan2`, que toma las coordenadas `x` y `z` para encontrar el ángulo alrededor del eje `y`. La coordenada `v` (latitud) se calcula a partir de la coordenada `y` normalizada del punto de impacto, usando la función `acos`.

  * **Función de Ejemplo:**

<!-- end list -->

```c
// t_hit_record es una estructura que contiene el punto de impacto y la normal
t_uv	get_uv_sphere(t_hit_record *rec)
{
	t_uv	uv;
	t_vec3	p;

	p = vec3_sub(rec->p, rec->object->center);
	p = vec3_div(p, rec->object->radius);
	uv.u = 1 - (atan2(p.z, p.x) + M_PI) / (2 * M_PI);
	uv.v = (acos(p.y) + M_PI / 2) / M_PI;
	return (uv);
}
```

#### **Mapeo de Plano**

El mapeo para un plano es el más sencillo, ya que el plano ya es una superficie 2D.

  * **Teoría:**
    Se proyecta el punto de intersección directamente sobre el plano y se utilizan sus coordenadas para mapearlas al rango `[0, 1]`. Para controlar la escala de la textura (que no se estire y se repita correctamente), se multiplican las coordenadas por un factor de escala.

  * **Función de Ejemplo:**

<!-- end list -->

```c
// t_hit_record contiene el punto de impacto
t_uv	get_uv_plane(t_hit_record *rec)
{
	t_uv	uv;
	double	u_scale;
	double	v_scale;

	u_scale = 1.0; // Ajusta este valor para controlar la repetición
	v_scale = 1.0;
	uv.u = fmod(rec->p.x * u_scale, 1.0);
	uv.v = fmod(rec->p.z * v_scale, 1.0);
	if (uv.u < 0)
		uv.u += 1.0;
	if (uv.v < 0)
		uv.v += 1.0;
	return (uv);
}
```

#### **Mapeo de Cilindro**

El mapeo de un cilindro es una combinación de técnicas: una para la **superficie lateral** y otra para las **tapas** (superior e inferior).

  * **Teoría:**

      * **Cuerpo del Cilindro:** La coordenada `u` se calcula a partir del ángulo alrededor del eje central, de manera similar al mapeo de la esfera, y la coordenada `v` se basa en la altura del punto de intersección.
      * **Tapas del Cilindro:** Las tapas son círculos. Un punto en una de las tapas se proyecta como un disco en 2D, donde el centro de la tapa se mapea al centro de la textura y el borde de la tapa se mapea al borde de la textura.

  * **Función de Ejemplo:**

<!-- end list -->

```c
t_uv	get_uv_cylinder(t_hit_record *rec)
{
	t_uv	uv;
	t_vec3	h_vec;
	double	theta;
	double	h;
	
	// Si el rayo impacta las tapas del cilindro
	if (rec->is_cap)
	{
		uv.u = (rec->p.x - rec->object->center.x) / rec->object->diameter + 0.5;
		uv.v = (rec->p.z - rec->object->center.z) / rec->object->diameter + 0.5;
	}
	// Si el rayo impacta el cuerpo lateral del cilindro
	else
	{
		h_vec = vec3_sub(rec->p, rec->object->center);
		h = vec3_dot(h_vec, rec->object->orientation);
		theta = atan2(h_vec.z, h_vec.x);
		uv.u = 1 - (theta / (2 * M_PI));
		uv.v = h / rec->object->height;
	}
	return (uv);
}
```

-----

### Normal Mapping (Mapeo de Normales)

El **normal mapping** es una técnica que utiliza una textura especial (el mapa de normales) para modificar la dirección de las normales de la superficie en cada punto.  Esto crea la ilusión de detalles de relieve como baches o grietas, sin añadir polígonos a la geometría real del objeto. Tu proyecto implementa esta lógica en el archivo `ft_normal_mapping.c`.

#### 1\. Vectores Normales en el Código

En tu proyecto, la normal es un vector tridimensional que se utiliza en las funciones de iluminación.

  * `t_hit_record->normal`: La estructura `t_hit_record` almacena la normal geométrica del punto de intersección. Por ejemplo, en el caso de un plano, la normal es constante. Para una esfera, se calcula como el vector normalizado desde el centro hasta el punto de impacto.
  * `get_effective_normal()`: Aunque el código de esta función no está presente, su existencia es clave. Su función es determinar la normal que se utilizará en los cálculos de luz. Esta función probablemente comprueba si el objeto tiene un mapa de normales. Si lo tiene, lee el vector del mapa, lo normaliza, y lo convierte al espacio de la escena para reemplazar la normal geométrica. Si no hay mapa, devuelve la normal geométrica.

#### 2\. Cálculo de Iluminación

Una vez que se ha determinado la normal "efectiva" (ya sea del mapa de normales o la geométrica), se utiliza en los cálculos de iluminación.

  * **Luz Difusa:** La función `calculate_diffuse_light` calcula la intensidad de la luz difusa usando el producto punto entre la normal efectiva y el vector que apunta a la luz (`to_light`). Un valor más alto significa más luz, y viceversa.
  * **Luz Especular:** La función `calculate_specular` utiliza la normal efectiva para calcular los reflejos especulares, simulando los brillos en las superficies.

Estas técnicas combinadas te permiten crear escenas con objetos visualmente complejos y detallados de una manera muy eficiente.

```
3. Mapeo de Normales y Estructura de Archivos
El normal mapping es una técnica que utiliza una textura especial (el mapa de normales) para modificar la dirección de las normales de la superficie en cada punto.  Esto crea la ilusión de detalles de relieve, como baches o grietas, sin añadir polígonos a la geometría real.

La Organización de las Texturas
Tu proyecto tiene una estructura de archivos bien definida que refleja esta lógica:

scene/
├── maps/
│   ├── figures/
│   ├── mir/
│   └── spc/
└── texture/
    ├── ladrillo_color.png
    └── ladrillo_normal.png
scene/maps/: Contiene escenas diseñadas para probar una característica específica, como el reflejo (mir) o el brillo especular (spc).

scene/texture/: Almacena las imágenes que se usan como texturas. La convención de nombres (_color.png y _normal.png) es clave para que el motor de renderizado asocie correctamente la textura de color con su mapa de normales correspondiente.
```