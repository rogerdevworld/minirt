Claro, aquí tienes toda la investigación en formato Markdown:

# Guía Detallada para el Desarrollo del Proyecto miniRT de la Escuela 42

## Introducción: El Mundo del Ray Tracing

El proyecto `miniRT` de la Escuela 42 introduce a los estudiantes en el fascinante campo de los gráficos por computadora, centrándose en la técnica de renderizado conocida como ray tracing. Esta metodología avanzada de generación de imágenes 3D simula de manera precisa el comportamiento de la luz, trazando rayos hipotéticos desde una cámara virtual y calculando sus interacciones con los objetos de una escena. El objetivo primordial es producir imágenes de un realismo notable, capturando fenómenos visuales complejos como sombras realistas, reflejos especulares y la refracción de la luz a través de materiales transparentes.

A diferencia de las técnicas de rasterización tradicionales, que proyectan modelos 3D directamente sobre una pantalla 2D, el ray tracing opera invirtiendo conceptualmente la trayectoria de la luz. En lugar de simular la luz que emana de las fuentes, traza los rayos "hacia atrás" desde el punto de vista de la cámara, a través de cada píxel del plano de la imagen, y hacia la escena 3D hasta que intersectan con fuentes de luz u objetos que reflejan o refractan la luz. Este enfoque de trazado inverso es considerablemente más eficiente computacionalmente, ya que evita simular la vasta mayoría de los rayos de luz emitidos por las fuentes que nunca alcanzarían la cámara en una escena típica.

Aunque intrínsecamente más intensivo en cálculos debido a su detallada simulación de la luz, el ray tracing produce imágenes de una calidad y realismo inigualables. Es fundamental comprender que existe una relación inherente entre el realismo visual y la carga computacional en los gráficos por computadora. Lograr una mayor fidelidad visual generalmente implica un costo computacional más elevado. Las aplicaciones gráficas contemporáneas, incluidos los videojuegos modernos, están adoptando cada vez más un enfoque de "renderizado híbrido". Esta estrategia combina inteligentemente las fortalezas del ray tracing con la velocidad de la rasterización tradicional, aprovechando los avances en el hardware para lograr tanto la fidelidad visual como el rendimiento en tiempo real. Para los estudiantes, esta comprensión enmarca el proyecto `miniRT` no solo como un ejercicio de implementación algorítmica, sino como una incursión inicial en el dominio crítico de la optimización del rendimiento dentro de tareas gráficas computacionalmente exigentes.

El proyecto `miniRT` en la Escuela 42 es una introducción fundamental a los principios de los gráficos 3D por computadora. Sumerge a los estudiantes en los conceptos centrales de los algoritmos de ray tracing y los fundamentos matemáticos esenciales requeridos para el renderizado. El proyecto exige la construcción de un renderizador 3D básico completamente en C. Esto implica varias etapas clave: parsear las descripciones de la escena desde un formato de archivo `.rt` personalizado, calcular con precisión las intersecciones de rayos con objetos y, finalmente, renderizar las imágenes resultantes para su visualización utilizando la biblioteca gráfica MiniLibX.

Los objetivos de aprendizaje son amplios y buscan desarrollar un conjunto de habilidades robusto:
*   **Dominio de las matemáticas de vectores 3D:** Esto incluye operaciones fundamentales para cálculos geométricos, como la suma de vectores, la resta, el producto escalar, el producto vectorial y la normalización.
*   **Comprensión del comportamiento de la luz y los modelos de sombreado:** Se espera que los estudiantes comprendan cómo la luz interactúa con las superficies e implementen modelos como el sombreado de Phong para simular una iluminación realista.
*   **Renderizado de primitivas geométricas básicas:** El proyecto requiere específicamente la capacidad de renderizar esferas, planos y cilindros.
*   **Interconexión con MiniLibX:** La competencia en el uso de la biblioteca MiniLibX para la creación de ventanas, la gestión de búferes de imagen, la manipulación de píxeles y el manejo de eventos es crucial.
*   **Parseo y validación de archivos:** El desarrollo de parsers robustos para interpretar archivos de descripción de escenas y validar su contenido es un componente significativo.
*   **Técnicas de optimización:** Aunque es un bonus, el proyecto fomenta la exploración de métodos para mejorar el rendimiento del renderizado, lo que sugiere los desafíos prácticos de la programación gráfica.

La extensa lista de objetivos del proyecto revela que `miniRT` es mucho más que una simple tarea de codificación; está diseñado como una introducción holística y multidisciplinaria a los gráficos por computadora. Esto implica que el éxito requiere no solo competencia en codificación, sino una comprensión profunda e integrada en varios dominios: matemáticas, física, estructuras de datos y programación de bajo nivel. El proyecto actúa como un paso fundamental, vinculando conceptos teóricos abstractos con resultados visuales tangibles, lo que es un sello distintivo del aprendizaje basado en proyectos de 42. Este enfoque integral prepara a los estudiantes para temas más avanzados en el campo al construir una base sólida e interconectada.

## 1. Arquitectura del Proyecto: Estableciendo los Cimientos

El diseño arquitectónico de `miniRT` es crucial para su desarrollo y mantenimiento. El proyecto se estructura en módulos bien definidos para gestionar la complejidad y promover la claridad del código.

### Diseño de Sistema de Alto Nivel (Parser, Motor de Ray Tracing, Interfaz MiniLibX)

Conceptualmente, el proyecto `miniRT` se puede dividir claramente en dos módulos principales e interdependientes: el **Parser** y el **Motor de Renderizado**. Esta separación de responsabilidades es fundamental para gestionar la complejidad. El flujo de ejecución general sigue una tubería distinta:
1.  **Ingesta de Archivos de Escena:** El programa comienza leyendo e interpretando el archivo de descripción de escena `.rt`.
2.  **Estructuración de Datos:** La información parseada se organiza y almacena meticulosamente dentro de una estructura de datos coherente en memoria que representa toda la escena.
3.  **Configuración del Entorno Gráfico:** De forma concurrente o subsiguiente, se inicializa la biblioteca gráfica MiniLibX y se crea la ventana de visualización.
4.  **Generación y Trazado de Rayos:** El núcleo del proceso de renderizado implica iterar a través de cada píxel de la imagen de salida. Para cada píxel, se genera un rayo único, que se origina desde la cámara virtual y se extiende hacia la escena 3D.
5.  **Intersección y Sombreado:** Este rayo se "traza" para determinar si intersecta con algún objeto en la escena. Si ocurre una intersección, el color en ese punto se calcula basándose en modelos de iluminación complejos y cálculos de sombras.
6.  **Dibujo de Píxeles:** El color calculado se escribe en el búfer de imagen de MiniLibX, "pintando" efectivamente el píxel.
7.  **Visualización e Interacción:** Finalmente, la imagen completamente renderizada se muestra en la ventana de MiniLibX, y el programa entra en un bucle de eventos para manejar las entradas del usuario.

La descripción de la escena en sí es un archivo de texto estructurado que contiene definiciones para la luz ambiental, las propiedades de la cámara, varias fuentes de luz y primitivas geométricas como esferas, planos y cilindros.

La división explícita de la arquitectura en "parser" y "motor" es una manifestación directa del principio de "separación de responsabilidades". Esta no es simplemente una sugerencia, sino una práctica fundamental para desarrollar software robusto y mantenible, especialmente dentro de las limitaciones de la Norminette de 42. Al aislar las responsabilidades de la interpretación de la entrada (parser) de los cálculos gráficos centrales (motor), el sistema se vuelve significativamente más fácil de desarrollar, depurar y extender. Por ejemplo, si se agregara un nuevo tipo de objeto geométrico (como un cono), las modificaciones se limitarían principalmente al parser (para leer el nuevo tipo de objeto) y a las funciones de intersección dentro del motor, sin requerir una revisión completa de toda la tubería de renderizado. Esta modularidad contribuye directamente a la claridad del código y a la manejabilidad del proyecto.

### Principios de Diseño Modular para Proyectos en C

La adhesión a la Norminette de 42 es de suma importancia y promueve intrínsecamente un diseño modular. La estricta regla que limita las funciones a un máximo de 25 líneas (excluyendo las llaves) obliga a los desarrolladores a descomponer tareas complejas en unidades más pequeñas, altamente enfocadas y fácilmente verificables. Esta restricción, aunque desafiante, es un potente impulsor para la creación de una base de código altamente modular y componible.

La Norminette, a menudo percibida por los estudiantes como un conjunto de reglas de estilo de codificación arbitrarias y restrictivas, en realidad cumple un propósito pedagógico y arquitectónico mucho más profundo. Sus limitaciones estrictas, en particular el límite de 25 líneas por función y la restricción en las declaraciones de variables, obligan implícitamente a los estudiantes a adoptar buenos principios de diseño modular. Para cumplir, es imperativo descomponer algoritmos complejos en numerosas funciones más pequeñas y de propósito único. Esta restricción, en lugar de obstaculizar la creatividad, cultiva una disciplina que conduce a un código más legible, testeable y mantenible, cualidades esenciales para proyectos de software de mayor envergadura. Así, la Norminette actúa como un ejecutor arquitectónico, guiando a los estudiantes hacia patrones de diseño de software intrínsecamente mejores al hacer imposible que el código monolítico o fuertemente acoplado pase la validación.

Una organización de archivos típica para proyectos en C de 42, y de hecho para proyectos en C bien estructurados en general, sigue un patrón jerárquico:
*   `src/`: Contiene los archivos de implementación (`.c`) para varios módulos (por ejemplo, `parser/`, `render/`, `utils/`, `main.c`).
*   `include/`: Alberga los archivos de cabecera (`.h`), que definen estructuras de datos (structs, typedefs) y prototipos de funciones, haciéndolos accesibles a través de diferentes archivos fuente.
*   `utils/`: A menudo un subdirectorio dentro de `src/` o `include/` dedicado a funciones de utilidad comunes, como operaciones de matemáticas vectoriales (`vec3_add`, `vec3_dot`, etc.) que se utilizan en todo el proyecto.[18, 19]

Esta estructura asegura una clara distinción entre la interfaz (cabeceras) y la implementación (archivos fuente), facilitando un desarrollo organizado y una navegación más sencilla por la base de código.

### Visión General del Grafo de Escena y Flujo de Datos

La estructura de datos central, a menudo denominada `t_scene`, es responsable de contener todos los elementos parseados que definen el entorno 3D.[7, 20] Esto incluye:
*   Configuraciones de luz ambiental global (`t_ambient`).
*   Configuración de la cámara (`t_camera`).
*   Una colección de fuentes de luz.
*   Una colección de objetos geométricos (esferas, planos, cilindros).
*   Fundamentalmente, los parámetros de resolución (`xres`, `yres`) también se almacenan aquí.
*   Para asegurar la integridad de la escena, se suelen utilizar banderas (por ejemplo, `res_init`, `al_init`, `cam_nb`) dentro de `t_scene` para rastrear si los elementos singulares (Ambient, Cámara, Luz) han sido inicializados, evitando múltiples declaraciones.

Un enfoque común y flexible para gestionar colecciones de objetos y luces es utilizar **listas enlazadas**. Esto se evidencia por la presencia de punteros `*next` dentro de las estructuras `t_light` y `t_cam` , y el parámetro `t_obj **list` que se pasa frecuentemente a las funciones de parseo. Esta asignación dinámica permite que la escena contenga un número arbitrario de luces y objetos, en lugar de estar limitada por arrays de tamaño fijo.

Para manejar la heterogeneidad de las primitivas geométricas (esferas, planos, cilindros) dentro de una única lista, se suele emplear una estructura `t_object` genérica. Esta `t_object` contendría una `union` (`u_figures`) que puede albergar los datos específicos para cada tipo de primitiva (por ejemplo, `t_sphere`, `t_plane`, `t_cylinder`), junto con atributos comunes como el color y las propiedades del material. Un campo `type` dentro de `t_object` indicaría entonces qué miembro de la unión está activo actualmente.

El diseño de `t_scene` con listas enlazadas para luces y objetos, combinado con la unión implícita dentro de un `t_object` genérico , es un patrón clásico y eficiente en C para gestionar datos heterogéneos. Este enfoque permite que el programa `miniRT` maneje escenas con un número y tipos variables de primitivas geométricas y fuentes de luz sin necesidad de preasignar arrays de tamaño fijo o implementar bucles de procesamiento separados para cada tipo de objeto. Promueve la reutilización y extensibilidad del código, haciendo que sea sencillo añadir nuevos tipos de objetos o fuentes de luz en el futuro simplemente extendiendo la unión y añadiendo las funciones de parseo e intersección correspondientes, en lugar de rediseñar todo el sistema de gestión de la escena. Esto demuestra una aplicación práctica de las capacidades de estructuración de datos de bajo nivel de C para aplicaciones gráficas flexibles y escalables.

## 2. Definición de Estructuras: Los Bloques Constructivos

La correcta definición de las estructuras de datos es la piedra angular de un proyecto de ray tracing eficiente y comprensible. Estas estructuras no solo almacenan la información de la escena, sino que también facilitan los complejos cálculos matemáticos necesarios para el renderizado.

### Tipos de Datos Fundamentales

#### Vectores (`t_vec3` o similar para puntos/direcciones/colores 3D)

Un bloque de construcción fundamental para cualquier aplicación de gráficos 3D es una estructura vectorial robusta. Para `miniRT`, un vector 3D (por ejemplo, `t_vec3` o `t_point3`) es esencial para representar:
*   **Puntos en el espacio 3D:** Como centros de objetos, posiciones de luces o coordenadas de la cámara.
*   **Direcciones:** Como direcciones de rayos, normales de superficie o vectores de luz.
*   **Colores RGB:** Donde cada componente (Rojo, Verde, Azul) puede tratarse como un escalar dentro del vector. Aunque algunos fragmentos mencionan `t_vec4` , lo que indica un posible uso de coordenadas homogéneas o un cuarto componente para propósitos específicos (como el alfa en el color, o un componente 'w' para puntos versus vectores), un `t_vec3` suele ser suficiente para la representación central de la geometría 3D y el color en un ray tracer básico.

La necesidad explícita de una estructura de vector 3D y un conjunto completo de operaciones vectoriales [4, 6] subraya que el álgebra lineal, específicamente las matemáticas vectoriales, constituye el lenguaje y la columna vertebral computacional de los gráficos 3D por computadora. Cada interacción geométrica, cada cálculo de luz y cada transformación espacial dentro del ray tracer dependen en gran medida de estas operaciones fundamentales. Sin una biblioteca de matemáticas vectoriales bien implementada y comprendida a fondo, los pasos subsiguientes (como las pruebas de intersección rayo-objeto, los cálculos de normales de superficie y las aplicaciones de modelos de iluminación) se vuelven abrumadoramente complejos y propensos a errores. Esto resalta la profunda interdependencia entre la teoría matemática abstracta y la implementación de programación concreta, enfatizando que el dominio de las matemáticas vectoriales es sinónimo de dominar los gráficos 3D.

#### Operaciones Vectoriales Básicas (Suma, Resta, Producto Escalar, Producto Vectorial, Normalización, Escala)

Se debe implementar un conjunto completo de operaciones vectoriales para facilitar todos los cálculos geométricos. Estas suelen incluir:
*   `vec3_add(v1, v2)`: Suma de vectores.[4, 6]
*   `vec3_sub(v1, v2)`: Resta de vectores.[4, 6]
*   `vec3_scale(v, scalar)`: Escalar un vector por un escalar.[4, 6]
*   `vec3_dot(v1, v2)`: Calcula el producto escalar, crucial para cálculos de ángulos, proyecciones e iluminación.[4, 6]
*   `vec3_cross(v1, v2)`: Calcula el producto vectorial, esencial para encontrar vectores perpendiculares y normales de superficie.
*   `vec3_normalize(v)`: Normaliza un vector a longitud unitaria, crítico para vectores de dirección y normales de superficie para asegurar cálculos consistentes, especialmente en modelos de iluminación.[6, 21]
*   `vec3_magnitude(v)`: Calcula la longitud de un vector.

### Estructuras de Elementos de Escena

#### Cámara (`t_camera`: posición, orientación, FOV)

La estructura `t_camera` define el punto de vista virtual desde el cual se observa y renderiza la escena 3D. Sus propiedades se parsean del elemento `C` en el archivo `.rt`.
*   Campos clave incluyen:
    *   `t_vec3 position`: Las coordenadas (x,y,z) del punto ocular de la cámara.
    *   `t_vec3 orientation`: Un vector 3D normalizado que indica hacia dónde mira la cámara, con componentes típicamente en el rango [-1,1].
    *   `int fov`: El campo de visión horizontal en grados, típicamente en el rango .
*   Para optimización, es muy beneficioso precalcular valores adicionales derivados de la cámara durante el parseo o la inicialización, como:
    *   `t_vec3 btm_left_cnr`: Las coordenadas 3D de la esquina inferior izquierda del viewport virtual (plano de proyección).
    *   `t_vec3 horiz`: Un vector que representa la extensión horizontal completa del viewport.
    *   `t_vec3 vert`: Un vector que representa la extensión vertical completa del viewport. Estos tres vectores, combinados con el origen de la cámara, son cruciales para generar rayos a través de cada píxel.

#### Fuente de Luz (`t_light`: posición, brillo, color)

La estructura `t_light` define una fuente de luz puntual que ilumina la escena. Sus propiedades se parsean del elemento `L` en el archivo `.rt`.
*   Campos clave incluyen:
    *   `t_vec3 position`: Las coordenadas (x,y,z) de la fuente de luz.
    *   `double bright_ratio`: La relación de brillo de la luz, típicamente en el rango [0.0, 1.0].
    *   `t_color3 light_color`: El color RGB de la fuente de luz, con componentes en el rango [0-255].
*   Si se admiten múltiples luces, esta estructura también requerirá un puntero `t_light *next` para enlazar las fuentes de luz en una lista.

#### Luz Ambiental (`t_ambient`: ratio, color)

La estructura `t_ambient` (o directamente dentro de `t_scene`) representa una fuente de luz global, no direccional, que proporciona un nivel base de iluminación a toda la escena, simulando la luz indirecta.[7, 22, 23] Sus propiedades se parsean del elemento `A`.
*   Campos clave incluyen:
    *   `double ratio`: La relación de iluminación ambiental, típicamente en el rango [0.0, 1.0].
    *   `t_color3 color`: El color RGB de la luz ambiental, con componentes en el rango [0-255].
*   Dado que la luz ambiental suele ser una única configuración global, a menudo reside directamente como un miembro dentro de la estructura `t_scene` en lugar de en una lista enlazada.[7, 20]

### Estructuras de Primitivas Geométricas

#### Esfera (`t_sphere`: centro, radio)

Representa un objeto esférico en la escena. Parseada del elemento `sp`.
*   Estructura: `typedef struct s_sphere { t_vec4 center; float radius; } t_sphere;`.
*   El archivo `.rt` especifica un `diameter`, que debe convertirse a `radius` durante el parseo, ya que el radio es matemáticamente más conveniente para los cálculos de intersección.

#### Plano (`t_plane`: punto, vector normal)

Representa una superficie plana infinita. Parseada del elemento `pl`.
*   Estructura: `typedef struct s_plane { t_vec4 point; t_vec4 vector; } t_plane;`.
*   `point` es cualquier punto que se encuentra en el plano, y `vector` es el vector normal 3D normalizado perpendicular al plano, que define su orientación.

#### Cilindro (`t_cylinder`: centro, vector de orientación, radio, altura)

Representa un objeto cilíndrico. Parseada del elemento `cy`.
*   Estructura: `typedef struct s_cylinder { t_vec4 center; t_vec4 n; float radius; float height; float angle; t_vec4 axis; } t_cylinder;`.
*   `center` típicamente se refiere a las coordenadas del centro inferior del cilindro. `n` (o `axis`) es el vector de orientación 3D normalizado que define el eje del cilindro. `radius` se deriva del `diameter` parseado, y `height` define su extensión a lo largo del eje. El campo `angle` mencionado en  podría ser un valor interno precalculado para transformaciones específicas o un artefacto de una implementación más compleja, pero la definición central se basa en el centro, la orientación, el radio y la altura.

### Estructuras de Rayos e Intersecciones

#### Rayo (`t_ray`: origen, dirección)

La entidad fundamental en el ray tracing, que representa una línea en el espacio 3D a lo largo de la cual viaja la luz.[6, 15, 24, 25]
*   Estructura: `typedef struct s_ray { t_vec4 orig; t_vec4 dir; } t_ray;`.
*   `orig`: El punto 3D desde el cual se origina el rayo (por ejemplo, la posición de la cámara para los rayos primarios, o un punto de intersección para los rayos de sombra/reflexión).
*   `dir`: El vector de dirección 3D normalizado a lo largo del cual viaja el rayo.

#### Información de Intersección (`t_intersection`: distancia `t`, punto de intersección, normal de superficie, objeto golpeado)

Cuando un rayo intersecta exitosamente un objeto, la información detallada sobre esa intersección es crítica para los cálculos de sombreado subsiguientes.[4, 25] Esta información se encapsula típicamente en una estructura.
*   Campos clave para `t_intersection` (o `t_hit_record`):
    *   `float t`: La distancia paramétrica a lo largo del rayo desde su origen hasta el punto de intersección. Este es el valor más crucial, ya que el `t` positivo más pequeño identifica el objeto visible.[6, 24, 26]
    *   `t_vec3 point`: Las coordenadas 3D precisas del punto de intersección en el espacio del mundo.[25, 26]
    *   `t_vec3 normal`: El vector normal de superficie 3D normalizado en el punto de intersección. Este vector es perpendicular a la superficie y apunta hacia afuera, esencial para los cálculos de iluminación.[21, 25, 26]
    *   `t_color3 color`: El color RGB base del objeto en el punto de intersección, tal como se define en el archivo `.rt`.
    *   `int object_type`: Una enumeración o identificador que indica el tipo de primitiva geométrica que fue golpeada (por ejemplo, `SPHERE`, `PLANE`, `CYLINDER`).
    *   `void *object_ptr`: Un puntero genérico a la estructura de objeto específica (`t_sphere *`, `t_plane *`, etc.) que fue golpeada. Esto permite el acceso a las propiedades únicas del objeto (por ejemplo, radio, altura, coeficientes de material) para el sombreado.

La recurrente relevancia del parámetro `t` (distancia a lo largo del rayo) en los cálculos de intersección [6, 24, 26] revela su papel fundamental más allá de ser un simple resultado matemático. En el ray tracing, el valor `t` es el principal árbitro de la visibilidad. Cuando un rayo intersecta múltiples objetos, el objeto correspondiente al valor `t` positivo más pequeño es el más cercano al origen del rayo y, por lo tanto, el visible. Esta comprensión es crítica para implementar correctamente la lógica de intersección, asegurando que los objetos en primer plano ocluyan correctamente a los que están detrás, y constituye la base para una percepción precisa de la profundidad en la escena renderizada.

### Estructura de Datos de Escena (`t_scene`)

Esta es la estructura general que encapsula toda la información parseada que define la escena 3D.[7, 20] Actúa como el repositorio central para todos los elementos de la escena, que luego se pasan al motor de renderizado.
*   Campos típicos dentro de `t_scene`:
    *   `t_ambient ambient_light`: Almacena las propiedades de la luz ambiental global.
    *   `t_camera camera`: Almacena las propiedades de la cámara principal.
    *   `t_light *lights`: Un puntero al inicio de una lista enlazada que contiene todas las fuentes de luz definidas en la escena.
    *   `t_object *objects`: Un puntero al inicio de una lista enlazada que contiene todos los objetos geométricos (esferas, planos, cilindros) en la escena.
    *   `int width`: La resolución horizontal de la imagen de salida.
    *   `int height`: La resolución vertical de la imagen de salida.
    *   `int res_init`, `int al_init`, `int cam_nb`: Banderas booleanas o contadores para asegurar que elementos singulares como la resolución, la luz ambiental y la cámara se definan exactamente una vez, según las reglas del archivo `.rt`.

La tabla a continuación resume las estructuras de datos esenciales para el proyecto `miniRT`.

| Nombre de la Estructura | Propósito | Campos Clave |
|---|---|---|
| `t_vec3` | Representa un punto 3D, dirección o color RGB. | `float x, y, z` |
| `t_ray` | Define un rayo en el espacio 3D. | `t_vec3 orig`, `t_vec3 dir` |
| `t_intersection` | Almacena información detallada de una intersección rayo-objeto. | `float t`, `t_vec3 point`, `t_vec3 normal`, `t_color3 color`, `int object_type`, `void *object_ptr` |
| `t_camera` | Configuración de la cámara virtual. | `t_vec3 position`, `t_vec3 orientation`, `int fov`, `t_vec3 btm_left_cnr`, `t_vec3 horiz`, `t_vec3 vert` |
| `t_light` | Define una fuente de luz puntual. | `t_vec3 position`, `double bright_ratio`, `t_color3 light_color`, `t_light *next` |
| `t_ambient` | Configuración de la luz ambiental global. | `double ratio`, `t_color3 color` |
| `t_sphere` | Representa una esfera. | `t_vec3 center`, `float radius` |
| `t_plane` | Representa un plano infinito. | `t_vec3 point`, `t_vec3 normal` |
| `t_cylinder` | Representa un cilindro. | `t_vec3 center`, `t_vec3 axis`, `float radius`, `float height` |
| `t_object` | Estructura genérica para primitivas geométricas. | `int type`, `t_color3 color`, `union u_figures { t_sphere sp; t_plane pl; t_cylinder cy; /*... */ } fig`, `t_object *next` |
| `t_scene` | Contenedor principal de todos los elementos de la escena. | `t_ambient ambient_light`, `t_camera camera`, `t_light *lights`, `t_object *objects`, `int width`, `int height`, `int res_init`, `int al_init`, `int cam_nb` |

## 3. Parser del Archivo.rt

El parser es el componente inicial y crítico del proyecto `miniRT`, encargado de interpretar el archivo de descripción de escena `.rt` y transformar su contenido textual en las estructuras de datos en memoria que el motor de renderizado utilizará.

### Formato del Archivo.rt

El archivo `.rt` es un formato de texto simple que describe la escena 3D. Cada elemento de la escena se define en una línea separada, y la información del elemento puede estar separada por uno o más espacios. Los elementos pueden definirse en cualquier orden, pero aquellos con una letra mayúscula (como `A` para luz ambiental, `C` para cámara, `L` para luz puntual) solo pueden declararse una vez en el archivo.

Los tipos de elementos esperados en un archivo `.rt` incluyen:
*   **Luz Ambiental (`A`):** Identificador `A`, seguido de la relación de iluminación ambiental (flotante entre 0.0 y 1.0) y los componentes de color RGB (enteros entre 0 y 255).
*   **Cámara (`C`):** Identificador `C`, seguido de las coordenadas (x,y,z) del punto de vista, un vector de orientación 3D normalizado (componentes entre -1 y 1), y el campo de visión horizontal en grados (entero entre 0 y 180).
*   **Luz Puntual (`L`):** Identificador `L`, seguido de las coordenadas (x,y,z) del punto de luz, la relación de brillo (flotante entre 0.0 y 1.0) y los componentes de color RGB (enteros entre 0 y 255).
*   **Esfera (`sp`):** Identificador `sp`, seguido de las coordenadas (x,y,z) del centro, el diámetro (flotante) y los componentes de color RGB (enteros entre 0 y 255).
*   **Plano (`pl`):** Identificador `pl`, seguido de las coordenadas (x,y,z) de un punto en el plano, un vector de orientación 3D normalizado (componentes entre -1 y 1) y los componentes de color RGB (enteros entre 0 y 255).
*   **Cilindro (`cy`):** Identificador `cy`, seguido de las coordenadas (x,y,z) del centro inferior, un vector de orientación 3D normalizado (componentes entre -1 y 1), el diámetro (flotante), la altura (flotante) y los componentes de color RGB (enteros entre 0 y 255).
*   Las líneas que comienzan con `#` se consideran comentarios y deben ser ignoradas.

### Proceso de Parseo Paso a Paso

El parser debe leer el archivo línea por línea y, para cada línea, identificar el tipo de elemento y extraer sus propiedades.
1.  **Lectura de Archivo:** Abrir el archivo `.rt` y leer su contenido, idealmente línea por línea.
2.  **Identificación de Elementos:** Para cada línea, se debe identificar el identificador del elemento (por ejemplo, `A`, `C`, `L`, `sp`, `pl`, `cy`). Esto generalmente se hace examinando el primer token de la línea.
3.  **Funciones de Parseo Específicas:** Una vez identificado el tipo de elemento, se invoca una función de parseo especializada (por ejemplo, `parse_camera`, `parse_light`, `parse_object`) para extraer y almacenar sus propiedades específicas. Estas funciones deben manejar la conversión de cadenas a tipos numéricos (flotantes, enteros) y la construcción de estructuras vectoriales.
4.  **Validación de Datos:** Durante el parseo, es crucial validar que los valores extraídos estén dentro de los rangos permitidos (por ejemplo, FOV entre 0 y 180, componentes RGB entre 0 y 255, relaciones de brillo entre 0.0 y 1.0).[7, 15] Si se encuentran valores fuera de rango o formatos incorrectos, el parser debe reportar un error y salir del programa.
5.  **Conversiones y Preprocesamiento:** Algunas propiedades pueden requerir conversión o preprocesamiento. Por ejemplo, el diámetro de esferas y cilindros debe convertirse a radio, ya que es más útil para los cálculos de intersección posteriores. Los vectores de orientación deben normalizarse si no lo están ya.
6.  **Población de la Estructura de Escena:** Los datos parseados se utilizan para poblar dinámicamente la estructura `t_scene`. Los objetos y las luces se añaden a sus respectivas listas enlazadas. Se deben utilizar banderas o contadores para asegurar que los elementos que solo pueden aparecer una vez (como la luz ambiental, la cámara y la resolución) no se dupliquen, generando un error si se detecta una duplicidad. También es esencial verificar que al menos una cámara esté presente en la escena.

### Manejo de Errores y Robustez

Un parser robusto debe ser capaz de manejar una variedad de errores, incluyendo:
*   **Archivos no encontrados o ilegibles.**
*   **Líneas con formato incorrecto:** Elementos faltantes, tipos de datos incorrectos, valores fuera de rango.
*   **Declaraciones duplicadas** para elementos únicos.
*   **Escenas incompletas:** Por ejemplo, la ausencia de una cámara.

Una función `error_exit`  puede ser útil para imprimir mensajes de error descriptivos y terminar el programa de manera controlada. La implementación de un parser que maneje estas validaciones es tan importante como la lógica de renderizado, ya que asegura que el motor solo procese datos válidos y bien formados.

## 4. Renderizado y Teoría Matemática

El corazón del proyecto `miniRT` reside en su motor de renderizado, que aplica principios matemáticos para simular la interacción de la luz con los objetos y construir la imagen final.

### Configuración de la Cámara y Generación de Rayos

La cámara virtual es el punto de vista desde el cual se observa la escena. La configuración de la cámara implica definir su posición (`C <position>`), su orientación (`C <orientation>`), y su campo de visión (`C <FOV>`). La orientación es un vector normalizado que apunta en la dirección en la que la cámara está mirando. El campo de visión (FOV) determina el ángulo visible desde la cámara, y por lo tanto, la "ampliación" de la escena proyectada en el plano de la imagen.

Para cada píxel en la imagen de salida, se debe construir un rayo primario que se origine en la posición de la cámara y pase a través del centro de ese píxel en el plano de visión (viewport).[4, 6, 8, 25] El plano de visión es un rectángulo virtual que actúa como la "ventana" al mundo 3D.

La ecuación paramétrica de un rayo es fundamental en el ray tracing:
`P(t) = O + t * D` [6, 24, 27]
Donde:
*   `P(t)` es un punto en el rayo a una distancia `t` del origen.
*   `O` es el origen del rayo (la posición de la cámara para los rayos primarios).[6, 15, 24, 27]
*   `D` es el vector de dirección del rayo, que debe ser un vector unitario normalizado.[6, 15, 24, 27]

Para generar el vector de dirección `D` para cada píxel, se utilizan las propiedades precalculadas de la cámara (como la esquina inferior izquierda del viewport, el vector horizontal y el vector vertical). Para un píxel en las coordenadas (u, v) del plano de la imagen (normalizadas entre 0 y 1), la dirección del rayo se calcula como:
`r.dir = cam.btm_left_cnr + (u * cam.horiz) + (v * cam.vert) - cam.orig` 
Donde `u` es la coordenada x del píxel relativa al ancho de la imagen, y `v` es la coordenada y del píxel relativa a la altura de la imagen.

### Intersección de Rayos con Primitivas Geométricas

El siguiente paso crucial es determinar qué objeto, si lo hay, intersecta primero el rayo y en qué punto. Esto implica resolver ecuaciones matemáticas para cada tipo de primitiva. La solución de estas ecuaciones produce uno o más valores para `t`. El valor `t` positivo más pequeño corresponde al punto de intersección visible más cercano al origen del rayo.[6, 24, 26] Las intersecciones con `t < 0` deben ignorarse, ya que representan puntos detrás del origen del rayo.[6, 26]

#### Intersección Rayo-Esfera

La ecuación implícita de una esfera centrada en `C = (xc, yc, zc)` con radio `R` es:
`(x - xc)^2 + (y - yc)^2 + (z - zc)^2 - R^2 = 0` Claro, aquí tienes toda la investigación en formato Markdown:

# Guía Detallada para el Desarrollo del Proyecto miniRT de la Escuela 42

## Introducción: El Mundo del Ray Tracing

El proyecto `miniRT` de la Escuela 42 introduce a los estudiantes en el fascinante campo de los gráficos por computadora, centrándose en la técnica de renderizado conocida como ray tracing. Esta metodología avanzada de generación de imágenes 3D simula de manera precisa el comportamiento de la luz, trazando rayos hipotéticos desde una cámara virtual y calculando sus interacciones con los objetos de una escena.[1] El objetivo primordial es producir imágenes de un realismo notable, capturando fenómenos visuales complejos como sombras realistas, reflejos especulares y la refracción de la luz a través de materiales transparentes.[1]

A diferencia de las técnicas de rasterización tradicionales, que proyectan modelos 3D directamente sobre una pantalla 2D, el ray tracing opera invirtiendo conceptualmente la trayectoria de la luz. En lugar de simular la luz que emana de las fuentes, traza los rayos "hacia atrás" desde el punto de vista de la cámara, a través de cada píxel del plano de la imagen, y hacia la escena 3D hasta que intersectan con fuentes de luz u objetos que reflejan o refractan la luz. Este enfoque de trazado inverso es considerablemente más eficiente computacionalmente, ya que evita simular la vasta mayoría de los rayos de luz emitidos por las fuentes que nunca alcanzarían la cámara en una escena típica.[3]

Aunque intrínsecamente más intensivo en cálculos debido a su detallada simulación de la luz, el ray tracing produce imágenes de una calidad y realismo inigualables.[2] Es fundamental comprender que existe una relación inherente entre el realismo visual y la carga computacional en los gráficos por computadora. Lograr una mayor fidelidad visual generalmente implica un costo computacional más elevado. Las aplicaciones gráficas contemporáneas, incluidos los videojuegos modernos, están adoptando cada vez más un enfoque de "renderizado híbrido". Esta estrategia combina inteligentemente las fortalezas del ray tracing con la velocidad de la rasterización tradicional, aprovechando los avances en el hardware para lograr tanto la fidelidad visual como el rendimiento en tiempo real.[2] Para los estudiantes, esta comprensión enmarca el proyecto `miniRT` no solo como un ejercicio de implementación algorítmica, sino como una incursión inicial en el dominio crítico de la optimización del rendimiento dentro de tareas gráficas computacionalmente exigentes.

El proyecto `miniRT` en la Escuela 42 es una introducción fundamental a los principios de los gráficos 3D por computadora. Sumerge a los estudiantes en los conceptos centrales de los algoritmos de ray tracing y los fundamentos matemáticos esenciales requeridos para el renderizado.[4] El proyecto exige la construcción de un renderizador 3D básico completamente en C. Esto implica varias etapas clave: parsear las descripciones de la escena desde un formato de archivo `.rt` personalizado, calcular con precisión las intersecciones de rayos con objetos y, finalmente, renderizar las imágenes resultantes para su visualización utilizando la biblioteca gráfica MiniLibX.[4]

Los objetivos de aprendizaje son amplios y buscan desarrollar un conjunto de habilidades robusto:
*   **Dominio de las matemáticas de vectores 3D:** Esto incluye operaciones fundamentales para cálculos geométricos, como la suma de vectores, la resta, el producto escalar, el producto vectorial y la normalización.[4]
*   **Comprensión del comportamiento de la luz y los modelos de sombreado:** Se espera que los estudiantes comprendan cómo la luz interactúa con las superficies e implementen modelos como el sombreado de Phong para simular una iluminación realista.[4]
*   **Renderizado de primitivas geométricas básicas:** El proyecto requiere específicamente la capacidad de renderizar esferas, planos y cilindros.[4]
*   **Interconexión con MiniLibX:** La competencia en el uso de la biblioteca MiniLibX para la creación de ventanas, la gestión de búferes de imagen, la manipulación de píxeles y el manejo de eventos es crucial.[4]
*   **Parseo y validación de archivos:** El desarrollo de parsers robustos para interpretar archivos de descripción de escenas y validar su contenido es un componente significativo.[4]
*   **Técnicas de optimización:** Aunque es un bonus, el proyecto fomenta la exploración de métodos para mejorar el rendimiento del renderizado, lo que sugiere los desafíos prácticos de la programación gráfica.[4]

La extensa lista de objetivos del proyecto revela que `miniRT` es mucho más que una simple tarea de codificación; está diseñado como una introducción holística y multidisciplinaria a los gráficos por computadora. Esto implica que el éxito requiere no solo competencia en codificación, sino una comprensión profunda e integrada en varios dominios: matemáticas, física, estructuras de datos y programación de bajo nivel. El proyecto actúa como un paso fundamental, vinculando conceptos teóricos abstractos con resultados visuales tangibles, lo que es un sello distintivo del aprendizaje basado en proyectos de 42.[5] Este enfoque integral prepara a los estudiantes para temas más avanzados en el campo al construir una base sólida e interconectada.

## 1. Arquitectura del Proyecto: Estableciendo los Cimientos

El diseño arquitectónico de `miniRT` es crucial para su desarrollo y mantenimiento. El proyecto se estructura en módulos bien definidos para gestionar la complejidad y promover la claridad del código.

### Diseño de Sistema de Alto Nivel (Parser, Motor de Ray Tracing, Interfaz MiniLibX)

Conceptualmente, el proyecto `miniRT` se puede dividir claramente en dos módulos principales e interdependientes: el **Parser** y el **Motor de Renderizado**.[6] Esta separación de responsabilidades es fundamental para gestionar la complejidad. El flujo de ejecución general sigue una tubería distinta:
1.  **Ingesta de Archivos de Escena:** El programa comienza leyendo e interpretando el archivo de descripción de escena `.rt`.
2.  **Estructuración de Datos:** La información parseada se organiza y almacena meticulosamente dentro de una estructura de datos coherente en memoria que representa toda la escena.
3.  **Configuración del Entorno Gráfico:** De forma concurrente o subsiguiente, se inicializa la biblioteca gráfica MiniLibX y se crea la ventana de visualización.
4.  **Generación y Trazado de Rayos:** El núcleo del proceso de renderizado implica iterar a través de cada píxel de la imagen de salida. Para cada píxel, se genera un rayo único, que se origina desde la cámara virtual y se extiende hacia la escena 3D.
5.  **Intersección y Sombreado:** Este rayo se "traza" para determinar si intersecta con algún objeto en la escena. Si ocurre una intersección, el color en ese punto se calcula basándose en modelos de iluminación complejos y cálculos de sombras.
6.  **Dibujo de Píxeles:** El color calculado se escribe en el búfer de imagen de MiniLibX, "pintando" efectivamente el píxel.
7.  **Visualización e Interacción:** Finalmente, la imagen completamente renderizada se muestra en la ventana de MiniLibX, y el programa entra en un bucle de eventos para manejar las entradas del usuario.

La descripción de la escena en sí es un archivo de texto estructurado que contiene definiciones para la luz ambiental, las propiedades de la cámara, varias fuentes de luz y primitivas geométricas como esferas, planos y cilindros.

La división explícita de la arquitectura en "parser" y "motor" es una manifestación directa del principio de "separación de responsabilidades". Esta no es simplemente una sugerencia, sino una práctica fundamental para desarrollar software robusto y mantenible, especialmente dentro de las limitaciones de la Norminette de 42. Al aislar las responsabilidades de la interpretación de la entrada (parser) de los cálculos gráficos centrales (motor), el sistema se vuelve significativamente más fácil de desarrollar, depurar y extender. Por ejemplo, si se agregara un nuevo tipo de objeto geométrico (como un cono), las modificaciones se limitarían principalmente al parser (para leer el nuevo tipo de objeto) y a las funciones de intersección dentro del motor, sin requerir una revisión completa de toda la tubería de renderizado. Esta modularidad contribuye directamente a la claridad del código y a la manejabilidad del proyecto.

### Principios de Diseño Modular para Proyectos en C

La adhesión a la Norminette de 42 es de suma importancia y promueve intrínsecamente un diseño modular. La estricta regla que limita las funciones a un máximo de 25 líneas (excluyendo las llaves) obliga a los desarrolladores a descomponer tareas complejas en unidades más pequeñas, altamente enfocadas y fácilmente verificables. Esta restricción, aunque desafiante, es un potente impulsor para la creación de una base de código altamente modular y componible.

La Norminette, a menudo percibida por los estudiantes como un conjunto de reglas de estilo de codificación arbitrarias y restrictivas, en realidad cumple un propósito pedagógico y arquitectónico mucho más profundo. Sus limitaciones estrictas, en particular el límite de 25 líneas por función y la restricción en las declaraciones de variables, obligan implícitamente a los estudiantes a adoptar buenos principios de diseño modular. Para cumplir, es imperativo descomponer algoritmos complejos en numerosas funciones más pequeñas y de propósito único. Esta restricción, en lugar de obstaculizar la creatividad, cultiva una disciplina que conduce a un código más legible, testeable y mantenible, cualidades esenciales para proyectos de software de mayor envergadura. Así, la Norminette actúa como un ejecutor arquitectónico, guiando a los estudiantes hacia patrones de diseño de software intrínsecamente mejores al hacer imposible que el código monolítico o fuertemente acoplado pase la validación.

Una organización de archivos típica para proyectos en C de 42, y de hecho para proyectos en C bien estructurados en general, sigue un patrón jerárquico:
*   `src/`: Contiene los archivos de implementación (`.c`) para varios módulos (por ejemplo, `parser/`, `render/`, `utils/`, `main.c`).
*   `include/`: Alberga los archivos de cabecera (`.h`), que definen estructuras de datos (structs, typedefs) y prototipos de funciones, haciéndolos accesibles a través de diferentes archivos fuente.
*   `utils/`: A menudo un subdirectorio dentro de `src/` o `include/` dedicado a funciones de utilidad comunes, como operaciones de matemáticas vectoriales (`vec3_add`, `vec3_dot`, etc.) que se utilizan en todo el proyecto.[18, 19]

Esta estructura asegura una clara distinción entre la interfaz (cabeceras) y la implementación (archivos fuente), facilitando un desarrollo organizado y una navegación más sencilla por la base de código.

### Visión General del Grafo de Escena y Flujo de Datos

La estructura de datos central, a menudo denominada `t_scene`, es responsable de contener todos los elementos parseados que definen el entorno 3D.[7, 20] Esto incluye:
*   Configuraciones de luz ambiental global (`t_ambient`).
*   Configuración de la cámara (`t_camera`).
*   Una colección de fuentes de luz.
*   Una colección de objetos geométricos (esferas, planos, cilindros).
*   Fundamentalmente, los parámetros de resolución (`xres`, `yres`) también se almacenan aquí.[7]
*   Para asegurar la integridad de la escena, se suelen utilizar banderas (por ejemplo, `res_init`, `al_init`, `cam_nb`) dentro de `t_scene` para rastrear si los elementos singulares (Ambient, Cámara, Luz) han sido inicializados, evitando múltiples declaraciones.[7]

Un enfoque común y flexible para gestionar colecciones de objetos y luces es utilizar **listas enlazadas**.[7] Esto se evidencia por la presencia de punteros `*next` dentro de las estructuras `t_light` y `t_cam` [7], y el parámetro `t_obj **list` que se pasa frecuentemente a las funciones de parseo.[7] Esta asignación dinámica permite que la escena contenga un número arbitrario de luces y objetos, en lugar de estar limitada por arrays de tamaño fijo.

Para manejar la heterogeneidad de las primitivas geométricas (esferas, planos, cilindros) dentro de una única lista, se suele emplear una estructura `t_object` genérica. Esta `t_object` contendría una `union` (`u_figures`) que puede albergar los datos específicos para cada tipo de primitiva (por ejemplo, `t_sphere`, `t_plane`, `t_cylinder`), junto con atributos comunes como el color y las propiedades del material.[7] Un campo `type` dentro de `t_object` indicaría entonces qué miembro de la unión está activo actualmente.

El diseño de `t_scene` con listas enlazadas para luces y objetos, combinado con la unión implícita dentro de un `t_object` genérico [7], es un patrón clásico y eficiente en C para gestionar datos heterogéneos. Este enfoque permite que el programa `miniRT` maneje escenas con un número y tipos variables de primitivas geométricas y fuentes de luz sin necesidad de preasignar arrays de tamaño fijo o implementar bucles de procesamiento separados para cada tipo de objeto. Promueve la reutilización y extensibilidad del código, haciendo que sea sencillo añadir nuevos tipos de objetos o fuentes de luz en el futuro simplemente extendiendo la unión y añadiendo las funciones de parseo e intersección correspondientes, en lugar de rediseñar todo el sistema de gestión de la escena. Esto demuestra una aplicación práctica de las capacidades de estructuración de datos de bajo nivel de C para aplicaciones gráficas flexibles y escalables.

## 2. Definición de Estructuras: Los Bloques Constructivos

La correcta definición de las estructuras de datos es la piedra angular de un proyecto de ray tracing eficiente y comprensible. Estas estructuras no solo almacenan la información de la escena, sino que también facilitan los complejos cálculos matemáticos necesarios para el renderizado.

### Tipos de Datos Fundamentales

#### Vectores (`t_vec3` o similar para puntos/direcciones/colores 3D)

Un bloque de construcción fundamental para cualquier aplicación de gráficos 3D es una estructura vectorial robusta. Para `miniRT`, un vector 3D (por ejemplo, `t_vec3` o `t_point3`) es esencial para representar:
*   **Puntos en el espacio 3D:** Como centros de objetos, posiciones de luces o coordenadas de la cámara.
*   **Direcciones:** Como direcciones de rayos, normales de superficie o vectores de luz.
*   **Colores RGB:** Donde cada componente (Rojo, Verde, Azul) puede tratarse como un escalar dentro del vector.[6] Aunque algunos fragmentos mencionan `t_vec4` [15], lo que indica un posible uso de coordenadas homogéneas o un cuarto componente para propósitos específicos (como el alfa en el color, o un componente 'w' para puntos versus vectores), un `t_vec3` suele ser suficiente para la representación central de la geometría 3D y el color en un ray tracer básico.

La necesidad explícita de una estructura de vector 3D y un conjunto completo de operaciones vectoriales [4, 6] subraya que el álgebra lineal, específicamente las matemáticas vectoriales, constituye el lenguaje y la columna vertebral computacional de los gráficos 3D por computadora. Cada interacción geométrica, cada cálculo de luz y cada transformación espacial dentro del ray tracer dependen en gran medida de estas operaciones fundamentales. Sin una biblioteca de matemáticas vectoriales bien implementada y comprendida a fondo, los pasos subsiguientes (como las pruebas de intersección rayo-objeto, los cálculos de normales de superficie y las aplicaciones de modelos de iluminación) se vuelven abrumadoramente complejos y propensos a errores. Esto resalta la profunda interdependencia entre la teoría matemática abstracta y la implementación de programación concreta, enfatizando que el dominio de las matemáticas vectoriales es sinónimo de dominar los gráficos 3D.

#### Operaciones Vectoriales Básicas (Suma, Resta, Producto Escalar, Producto Vectorial, Normalización, Escala)

Se debe implementar un conjunto completo de operaciones vectoriales para facilitar todos los cálculos geométricos. Estas suelen incluir:
*   `vec3_add(v1, v2)`: Suma de vectores.[4, 6]
*   `vec3_sub(v1, v2)`: Resta de vectores.[4, 6]
*   `vec3_scale(v, scalar)`: Escalar un vector por un escalar.[4, 6]
*   `vec3_dot(v1, v2)`: Calcula el producto escalar, crucial para cálculos de ángulos, proyecciones e iluminación.[4, 6]
*   `vec3_cross(v1, v2)`: Calcula el producto vectorial, esencial para encontrar vectores perpendiculares y normales de superficie.[4]
*   `vec3_normalize(v)`: Normaliza un vector a longitud unitaria, crítico para vectores de dirección y normales de superficie para asegurar cálculos consistentes, especialmente en modelos de iluminación.[6, 21]
*   `vec3_magnitude(v)`: Calcula la longitud de un vector.[6]

### Estructuras de Elementos de Escena

#### Cámara (`t_camera`: posición, orientación, FOV)

La estructura `t_camera` define el punto de vista virtual desde el cual se observa y renderiza la escena 3D.[7] Sus propiedades se parsean del elemento `C` en el archivo `.rt`.[15]
*   Campos clave incluyen:
    *   `t_vec3 position`: Las coordenadas (x,y,z) del punto ocular de la cámara.[15]
    *   `t_vec3 orientation`: Un vector 3D normalizado que indica hacia dónde mira la cámara, con componentes típicamente en el rango [-1,1].[15]
    *   `int fov`: El campo de visión horizontal en grados, típicamente en el rango .[15]
*   Para optimización, es muy beneficioso precalcular valores adicionales derivados de la cámara durante el parseo o la inicialización, como:
    *   `t_vec3 btm_left_cnr`: Las coordenadas 3D de la esquina inferior izquierda del viewport virtual (plano de proyección).[15]
    *   `t_vec3 horiz`: Un vector que representa la extensión horizontal completa del viewport.[15]
    *   `t_vec3 vert`: Un vector que representa la extensión vertical completa del viewport.[15] Estos tres vectores, combinados con el origen de la cámara, son cruciales para generar rayos a través de cada píxel.[15]

#### Fuente de Luz (`t_light`: posición, brillo, color)

La estructura `t_light` define una fuente de luz puntual que ilumina la escena.[7] Sus propiedades se parsean del elemento `L` en el archivo `.rt`.[15]
*   Campos clave incluyen:
    *   `t_vec3 position`: Las coordenadas (x,y,z) de la fuente de luz.[15]
    *   `double bright_ratio`: La relación de brillo de la luz, típicamente en el rango [0.0, 1.0].[15]
    *   `t_color3 light_color`: El color RGB de la fuente de luz, con componentes en el rango [0-255].[15]
*   Si se admiten múltiples luces, esta estructura también requerirá un puntero `t_light *next` para enlazar las fuentes de luz en una lista.[7]

#### Luz Ambiental (`t_ambient`: ratio, color)

La estructura `t_ambient` (o directamente dentro de `t_scene`) representa una fuente de luz global, no direccional, que proporciona un nivel base de iluminación a toda la escena, simulando la luz indirecta.[7, 22, 23] Sus propiedades se parsean del elemento `A`.[15]
*   Campos clave incluyen:
    *   `double ratio`: La relación de iluminación ambiental, típicamente en el rango [0.0, 1.0].[15]
    *   `t_color3 color`: El color RGB de la luz ambiental, con componentes en el rango [0-255].[15]
*   Dado que la luz ambiental suele ser una única configuración global, a menudo reside directamente como un miembro dentro de la estructura `t_scene` en lugar de en una lista enlazada.[7, 20]

### Estructuras de Primitivas Geométricas

#### Esfera (`t_sphere`: centro, radio)

Representa un objeto esférico en la escena. Parseada del elemento `sp`.[15]
*   Estructura: `typedef struct s_sphere { t_vec4 center; float radius; } t_sphere;`.[15]
*   El archivo `.rt` especifica un `diameter`, que debe convertirse a `radius` durante el parseo, ya que el radio es matemáticamente más conveniente para los cálculos de intersección.[15]

#### Plano (`t_plane`: punto, vector normal)

Representa una superficie plana infinita. Parseada del elemento `pl`.[15]
*   Estructura: `typedef struct s_plane { t_vec4 point; t_vec4 vector; } t_plane;`.[15]
*   `point` es cualquier punto que se encuentra en el plano, y `vector` es el vector normal 3D normalizado perpendicular al plano, que define su orientación.[15]

#### Cilindro (`t_cylinder`: centro, vector de orientación, radio, altura)

Representa un objeto cilíndrico. Parseada del elemento `cy`.[15]
*   Estructura: `typedef struct s_cylinder { t_vec4 center; t_vec4 n; float radius; float height; float angle; t_vec4 axis; } t_cylinder;`.[15]
*   `center` típicamente se refiere a las coordenadas del centro inferior del cilindro. `n` (o `axis`) es el vector de orientación 3D normalizado que define el eje del cilindro. `radius` se deriva del `diameter` parseado, y `height` define su extensión a lo largo del eje.[15] El campo `angle` mencionado en [15] podría ser un valor interno precalculado para transformaciones específicas o un artefacto de una implementación más compleja, pero la definición central se basa en el centro, la orientación, el radio y la altura.

### Estructuras de Rayos e Intersecciones

#### Rayo (`t_ray`: origen, dirección)

La entidad fundamental en el ray tracing, que representa una línea en el espacio 3D a lo largo de la cual viaja la luz.[6, 15, 24, 25]
*   Estructura: `typedef struct s_ray { t_vec4 orig; t_vec4 dir; } t_ray;`.[15]
*   `orig`: El punto 3D desde el cual se origina el rayo (por ejemplo, la posición de la cámara para los rayos primarios, o un punto de intersección para los rayos de sombra/reflexión).
*   `dir`: El vector de dirección 3D normalizado a lo largo del cual viaja el rayo.

#### Información de Intersección (`t_intersection`: distancia `t`, punto de intersección, normal de superficie, objeto golpeado)

Cuando un rayo intersecta exitosamente un objeto, la información detallada sobre esa intersección es crítica para los cálculos de sombreado subsiguientes.[4, 25] Esta información se encapsula típicamente en una estructura.
*   Campos clave para `t_intersection` (o `t_hit_record`):
    *   `float t`: La distancia paramétrica a lo largo del rayo desde su origen hasta el punto de intersección. Este es el valor más crucial, ya que el `t` positivo más pequeño identifica el objeto visible.[6, 24, 26]
    *   `t_vec3 point`: Las coordenadas 3D precisas del punto de intersección en el espacio del mundo.[25, 26]
    *   `t_vec3 normal`: El vector normal de superficie 3D normalizado en el punto de intersección. Este vector es perpendicular a la superficie y apunta hacia afuera, esencial para los cálculos de iluminación.[21, 25, 26]
    *   `t_color3 color`: El color RGB base del objeto en el punto de intersección, tal como se define en el archivo `.rt`.
    *   `int object_type`: Una enumeración o identificador que indica el tipo de primitiva geométrica que fue golpeada (por ejemplo, `SPHERE`, `PLANE`, `CYLINDER`).[20]
    *   `void *object_ptr`: Un puntero genérico a la estructura de objeto específica (`t_sphere *`, `t_plane *`, etc.) que fue golpeada. Esto permite el acceso a las propiedades únicas del objeto (por ejemplo, radio, altura, coeficientes de material) para el sombreado.

La recurrente relevancia del parámetro `t` (distancia a lo largo del rayo) en los cálculos de intersección [6, 24, 26] revela su papel fundamental más allá de ser un simple resultado matemático. En el ray tracing, el valor `t` es el principal árbitro de la visibilidad. Cuando un rayo intersecta múltiples objetos, el objeto correspondiente al valor `t` positivo más pequeño es el más cercano al origen del rayo y, por lo tanto, el visible. Esta comprensión es crítica para implementar correctamente la lógica de intersección, asegurando que los objetos en primer plano ocluyan correctamente a los que están detrás, y constituye la base para una percepción precisa de la profundidad en la escena renderizada.

### Estructura de Datos de Escena (`t_scene`)

Esta es la estructura general que encapsula toda la información parseada que define la escena 3D.[7, 20] Actúa como el repositorio central para todos los elementos de la escena, que luego se pasan al motor de renderizado.
*   Campos típicos dentro de `t_scene`:
    *   `t_ambient ambient_light`: Almacena las propiedades de la luz ambiental global.
    *   `t_camera camera`: Almacena las propiedades de la cámara principal.
    *   `t_light *lights`: Un puntero al inicio de una lista enlazada que contiene todas las fuentes de luz definidas en la escena.
    *   `t_object *objects`: Un puntero al inicio de una lista enlazada que contiene todos los objetos geométricos (esferas, planos, cilindros) en la escena.
    *   `int width`: La resolución horizontal de la imagen de salida.[7]
    *   `int height`: La resolución vertical de la imagen de salida.[7]
    *   `int res_init`, `int al_init`, `int cam_nb`: Banderas booleanas o contadores para asegurar que elementos singulares como la resolución, la luz ambiental y la cámara se definan exactamente una vez, según las reglas del archivo `.rt`.[7]

La tabla a continuación resume las estructuras de datos esenciales para el proyecto `miniRT`.

| Nombre de la Estructura | Propósito | Campos Clave |
|---|---|---|
| `t_vec3` | Representa un punto 3D, dirección o color RGB. | `float x, y, z` |
| `t_ray` | Define un rayo en el espacio 3D. | `t_vec3 orig`, `t_vec3 dir` |
| `t_intersection` | Almacena información detallada de una intersección rayo-objeto. | `float t`, `t_vec3 point`, `t_vec3 normal`, `t_color3 color`, `int object_type`, `void *object_ptr` |
| `t_camera` | Configuración de la cámara virtual. | `t_vec3 position`, `t_vec3 orientation`, `int fov`, `t_vec3 btm_left_cnr`, `t_vec3 horiz`, `t_vec3 vert` |
| `t_light` | Define una fuente de luz puntual. | `t_vec3 position`, `double bright_ratio`, `t_color3 light_color`, `t_light *next` |
| `t_ambient` | Configuración de la luz ambiental global. | `double ratio`, `t_color3 color` |
| `t_sphere` | Representa una esfera. | `t_vec3 center`, `float radius` |
| `t_plane` | Representa un plano infinito. | `t_vec3 point`, `t_vec3 normal` |
| `t_cylinder` | Representa un cilindro. | `t_vec3 center`, `t_vec3 axis`, `float radius`, `float height` |
| `t_object` | Estructura genérica para primitivas geométricas. | `int type`, `t_color3 color`, `union u_figures { t_sphere sp; t_plane pl; t_cylinder cy; /*... */ } fig`, `t_object *next` |
| `t_scene` | Contenedor principal de todos los elementos de la escena. | `t_ambient ambient_light`, `t_camera camera`, `t_light *lights`, `t_object *objects`, `int width`, `int height`, `int res_init`, `int al_init`, `int cam_nb` |

## 3. Parser del Archivo.rt

El parser es el componente inicial y crítico del proyecto `miniRT`, encargado de interpretar el archivo de descripción de escena `.rt` y transformar su contenido textual en las estructuras de datos en memoria que el motor de renderizado utilizará.

### Formato del Archivo.rt

El archivo `.rt` es un formato de texto simple que describe la escena 3D. Cada elemento de la escena se define en una línea separada, y la información del elemento puede estar separada por uno o más espacios.[15] Los elementos pueden definirse en cualquier orden, pero aquellos con una letra mayúscula (como `A` para luz ambiental, `C` para cámara, `L` para luz puntual) solo pueden declararse una vez en el archivo.[15]

Los tipos de elementos esperados en un archivo `.rt` incluyen:
*   **Luz Ambiental (`A`):** Identificador `A`, seguido de la relación de iluminación ambiental (flotante entre 0.0 y 1.0) y los componentes de color RGB (enteros entre 0 y 255).[15]
*   **Cámara (`C`):** Identificador `C`, seguido de las coordenadas (x,y,z) del punto de vista, un vector de orientación 3D normalizado (componentes entre -1 y 1), y el campo de visión horizontal en grados (entero entre 0 y 180).[15]
*   **Luz Puntual (`L`):** Identificador `L`, seguido de las coordenadas (x,y,z) del punto de luz, la relación de brillo (flotante entre 0.0 y 1.0) y los componentes de color RGB (enteros entre 0 y 255).[15]
*   **Esfera (`sp`):** Identificador `sp`, seguido de las coordenadas (x,y,z) del centro, el diámetro (flotante) y los componentes de color RGB (enteros entre 0 y 255).[15]
*   **Plano (`pl`):** Identificador `pl`, seguido de las coordenadas (x,y,z) de un punto en el plano, un vector de orientación 3D normalizado (componentes entre -1 y 1) y los componentes de color RGB (enteros entre 0 y 255).[15]
*   **Cilindro (`cy`):** Identificador `cy`, seguido de las coordenadas (x,y,z) del centro inferior, un vector de orientación 3D normalizado (componentes entre -1 y 1), el diámetro (flotante), la altura (flotante) y los componentes de color RGB (enteros entre 0 y 255).[15]
*   Las líneas que comienzan con `#` se consideran comentarios y deben ser ignoradas.[4]

### Proceso de Parseo Paso a Paso

El parser debe leer el archivo línea por línea y, para cada línea, identificar el tipo de elemento y extraer sus propiedades.
1.  **Lectura de Archivo:** Abrir el archivo `.rt` y leer su contenido, idealmente línea por línea.[7]
2.  **Identificación de Elementos:** Para cada línea, se debe identificar el identificador del elemento (por ejemplo, `A`, `C`, `L`, `sp`, `pl`, `cy`).[15] Esto generalmente se hace examinando el primer token de la línea.
3.  **Funciones de Parseo Específicas:** Una vez identificado el tipo de elemento, se invoca una función de parseo especializada (por ejemplo, `parse_camera`, `parse_light`, `parse_object`) para extraer y almacenar sus propiedades específicas. Estas funciones deben manejar la conversión de cadenas a tipos numéricos (flotantes, enteros) y la construcción de estructuras vectoriales.[15]
4.  **Validación de Datos:** Durante el parseo, es crucial validar que los valores extraídos estén dentro de los rangos permitidos (por ejemplo, FOV entre 0 y 180, componentes RGB entre 0 y 255, relaciones de brillo entre 0.0 y 1.0).[7, 15] Si se encuentran valores fuera de rango o formatos incorrectos, el parser debe reportar un error y salir del programa.
5.  **Conversiones y Preprocesamiento:** Algunas propiedades pueden requerir conversión o preprocesamiento. Por ejemplo, el diámetro de esferas y cilindros debe convertirse a radio, ya que es más útil para los cálculos de intersección posteriores.[15] Los vectores de orientación deben normalizarse si no lo están ya.
6.  **Población de la Estructura de Escena:** Los datos parseados se utilizan para poblar dinámicamente la estructura `t_scene`. Los objetos y las luces se añaden a sus respectivas listas enlazadas.[7] Se deben utilizar banderas o contadores para asegurar que los elementos que solo pueden aparecer una vez (como la luz ambiental, la cámara y la resolución) no se dupliquen, generando un error si se detecta una duplicidad.[7] También es esencial verificar que al menos una cámara esté presente en la escena.[15]

### Manejo de Errores y Robustez

Un parser robusto debe ser capaz de manejar una variedad de errores, incluyendo:
*   **Archivos no encontrados o ilegibles.**
*   **Líneas con formato incorrecto:** Elementos faltantes, tipos de datos incorrectos, valores fuera de rango.
*   **Declaraciones duplicadas** para elementos únicos.[7]
*   **Escenas incompletas:** Por ejemplo, la ausencia de una cámara.[15]

Una función `error_exit` [4] puede ser útil para imprimir mensajes de error descriptivos y terminar el programa de manera controlada. La implementación de un parser que maneje estas validaciones es tan importante como la lógica de renderizado, ya que asegura que el motor solo procese datos válidos y bien formados.

## 4. Renderizado y Teoría Matemática

El corazón del proyecto `miniRT` reside en su motor de renderizado, que aplica principios matemáticos para simular la interacción de la luz con los objetos y construir la imagen final.

### Configuración de la Cámara y Generación de Rayos

La cámara virtual es el punto de vista desde el cual se observa la escena. La configuración de la cámara implica definir su posición (`C <position>`), su orientación (`C <orientation>`), y su campo de visión (`C <FOV>`).[15] La orientación es un vector normalizado que apunta en la dirección en la que la cámara está mirando.[15] El campo de visión (FOV) determina el ángulo visible desde la cámara, y por lo tanto, la "ampliación" de la escena proyectada en el plano de la imagen.[6]

Para cada píxel en la imagen de salida, se debe construir un rayo primario que se origine en la posición de la cámara y pase a través del centro de ese píxel en el plano de visión (viewport).[4, 6, 8, 25] El plano de visión es un rectángulo virtual que actúa como la "ventana" al mundo 3D.[6]

La ecuación paramétrica de un rayo es fundamental en el ray tracing:
`P(t) = O + t * D` [6, 24, 27]
Donde:
*   `P(t)` es un punto en el rayo a una distancia `t` del origen.
*   `O` es el origen del rayo (la posición de la cámara para los rayos primarios).[6, 15, 24, 27]
*   `D` es el vector de dirección del rayo, que debe ser un vector unitario normalizado.[6, 15, 24, 27]

Para generar el vector de dirección `D` para cada píxel, se utilizan las propiedades precalculadas de la cámara (como la esquina inferior izquierda del viewport, el vector horizontal y el vector vertical).[15] Para un píxel en las coordenadas (u, v) del plano de la imagen (normalizadas entre 0 y 1), la dirección del rayo se calcula como:
`r.dir = cam.btm_left_cnr + (u * cam.horiz) + (v * cam.vert) - cam.orig` [15]
Donde `u` es la coordenada x del píxel relativa al ancho de la imagen, y `v` es la coordenada y del píxel relativa a la altura de la imagen.[15]

### Intersección de Rayos con Primitivas Geométricas

El siguiente paso crucial es determinar qué objeto, si lo hay, intersecta primero el rayo y en qué punto. Esto implica resolver ecuaciones matemáticas para cada tipo de primitiva. La solución de estas ecuaciones produce uno o más valores para `t`. El valor `t` positivo más pequeño corresponde al punto de intersección visible más cercano al origen del rayo.[6, 24, 26] Las intersecciones con `t < 0` deben ignorarse, ya que representan puntos detrás del origen del rayo.[6, 26]

#### Intersección Rayo-Esfera

La ecuación implícita de una esfera centrada en `C = (xc, yc, zc)` con radio `R` es:
`(x - xc)^2 + (y - yc)^2 + (z - zc)^2 - R^2 = 0` [24]
O en forma vectorial: `(P - C) ⋅ (P - C) - R^2 = 0`.[24, 28]

Sustituyendo la ecuación del rayo `P(t) = O + tD` en la ecuación de la esfera, se obtiene una ecuación cuadrática en términos de `t`:
`(D ⋅ D)t^2 + 2D ⋅ (O - C)t + (O - C) ⋅ (O - C) - R^2 = 0`.[24, 28, 29]
Esto se puede escribir como `At^2 + Bt + C = 0`, donde:
*   `A = D ⋅ D` [24, 28, 29]
*   `B = 2D ⋅ (O - C)` [24, 28, 29]
*   `C = (O - C) ⋅ (O - C) - R^2` [24, 28, 29]

Las soluciones para `t` se encuentran usando la fórmula cuadrática: `t = (-B ± sqrt(B^2 - 4AC)) / (2A)`.[24, 28]
El discriminante `(B^2 - 4AC)` determina el número de intersecciones:
*   Si es negativo, no hay intersección.[6, 24, 28]
*   Si es cero, el rayo es tangente a la esfera (una intersección).[6, 24, 28]
*   Si es positivo, hay dos intersecciones (el rayo entra y sale de la esfera).[6, 24, 28]

Se debe seleccionar el valor `t` positivo más pequeño. La normal de la superficie en el punto de intersección `P` de una esfera es simplemente el vector normalizado desde el centro de la esfera hasta `P`: `N = normalize(P - C)`.[15, 21]

#### Intersección Rayo-Plano

Un plano se define por un punto `S` en el plano y un vector normal `N` perpendicular a él La ecuación implícita de un plano es:
`N ⋅ (P - S) = 0` [27, 30, 31]
Sustituyendo la ecuación del rayo `P(t) = O + tD` en la ecuación del plano:
`N ⋅ (O + tD - S) = 0`
Expandiendo y resolviendo para `t`:
`t = (S - O) ⋅ N / (D ⋅ N)`

Casos especiales:
*   Si `D ⋅ N = 0`, el rayo es paralelo al plano. No hay intersección a menos que el rayo ya esté en el plano (`(S - O) ⋅ N = 0`).
*   Se debe verificar que `t` sea positivo.
La normal de la superficie para un plano es simplemente su vector normal `N` predefinido.

#### Intersección Rayo-Cilindro

La intersección con un cilindro es más compleja, ya que un cilindro finito consta de un cuerpo cilíndrico infinito y dos tapas (planos).[15, 32, 33]
1.  **Intersección con el Cilindro Infinito:** Un cilindro infinito a lo largo del eje Y con radio `r` tiene la ecuación `x^2 + z^2 - r^2 = 0`.[32, 33] Para un cilindro general con centro `pa` y vector de eje `va`, la ecuación es:
    `(q - pa - (va ⋅ (q - pa))va)^2 - r^2 = 0` [32, 33]
    Sustituyendo `q = O + tD` y resolviendo, se obtiene una ecuación cuadrática `At^2 + Bt + C = 0` similar a la de la esfera, pero con coeficientes más complejos que involucran `va`.[32, 33]
2.  **Verificación de Tapas:** Una vez que se encuentran las intersecciones con el cilindro infinito, se debe verificar si los puntos de intersección caen dentro de la altura definida del cilindro (entre las tapas superior e inferior).[15, 32, 33] Esto se hace comprobando la proyección del punto de intersección sobre el eje del cilindro.
3.  **Intersección con las Tapas:** Las tapas del cilindro son planos. Se calculan las intersecciones del rayo con los planos que definen las tapas superior e inferior del cilindro.[15, 32, 33] Para cada intersección de tapa, se debe verificar si el punto de intersección se encuentra dentro del círculo de la tapa.[32, 33]
Se elige el `t` positivo más pequeño entre todas las intersecciones válidas (cuerpo o tapas). La normal de la superficie para el cuerpo del cilindro se calcula como el vector desde el punto de intersección hasta el eje del cilindro, normalizado. Para las tapas, la normal es simplemente el vector del eje del cilindro (o su negativo, dependiendo de la tapa).

### Modelos de Iluminación y Sombreado

Una vez que se encuentra el punto de intersección más cercano, se calcula el color de ese píxel. Esto implica aplicar modelos de iluminación que simulan cómo la luz interactúa con la superficie del objeto. El modelo de iluminación de Phong es un modelo empírico comúnmente utilizado que combina componentes de luz ambiental, difusa y especular.[4, 22, 34]

#### Componentes del Modelo de Iluminación de Phong

*   **Luz Ambiental (Ambient):** Representa la luz indirecta que se dispersa por toda la escena, proporcionando una iluminación de base uniforme.[7, 22, 23] Se calcula multiplicando el color del objeto por la relación y el color de la luz ambiental global.[22]
    `Color_Ambient = Objeto.Color * Ambient.Ratio * Ambient.Color` 

*   **Luz Difusa (Diffuse):** Simula la luz que se dispersa uniformemente en todas las direcciones desde una superficie rugosa (superficie Lambertiana).[22, 23, 36] La intensidad de la luz difusa depende del ángulo entre el vector normal de la superficie (`N`) y el vector de dirección hacia la fuente de luz (`L`). Se calcula utilizando la ley del coseno de Lambert:
    `Color_Difuso = Objeto.Color * Luz.Color * max(0, N ⋅ L)` [22, 23, 34, 37]
    Donde `N` y `L` son vectores unitarios. El `max(0,...)` asegura que solo se considere la luz que incide en la superficie.

*   **Luz Especular (Specular):** Representa los reflejos brillantes que aparecen en superficies lisas y brillantes, como el metal o el plástico pulido.[22, 23] La intensidad de la luz especular depende del ángulo entre el vector de dirección del ojo (`E`, que es el inverso de la dirección del rayo) y el vector de dirección de la luz reflejada (`R`). `R` se calcula como la reflexión de `L` sobre `N`:
    `R = 2 * (N ⋅ L) * N - L` [34, 38]
    La contribución especular se calcula como:
    `Color_Especular = Especular.Color * Luz.Color * max(0, E ⋅ R)^p` [22, 34]
    Donde `p` es el exponente especular (potencia de Phong), que controla el tamaño y la intensidad del brillo (un `p` más grande produce un brillo más pequeño e intenso).[22, 34]

La contribución total de la iluminación local para un punto es la suma de estos componentes:
`Color_Total = Color_Ambient + ∑ (Color_Difuso + Color_Especular)` (sumado para cada fuente de luz).

#### Sombras (Shadows)

Para simular sombras, se lanza un "rayo de sombra" desde el punto de intersección `P` en la superficie hacia cada fuente de luz `Li`. Es crucial desplazar ligeramente el origen del rayo de sombra fuera de la superficie para evitar que intersecte el propio objeto del que proviene.
`Rayo_Sombra_Origen = P + ε * L_dir` (donde `ε` es un valor pequeño, como 0.001) 
`Rayo_Sombra_Direccion = normalize(Li.position - P)`

Si este rayo de sombra intersecta cualquier otro objeto en la escena *antes* de llegar a la fuente de luz, significa que la fuente de luz está ocluida y el punto `P` está en sombra con respecto a esa luz. En este caso, la contribución difusa y especular de esa luz en particular para el punto `P` se anula (o se reduce significativamente).

#### Reflexiones (Reflections) y Refracciones (Refractions) (Bonus)

Para un mayor realismo, `miniRT` puede extenderse para incluir reflexiones y refracciones. Esto implica un trazado de rayos recursivo.
*   **Reflexiones:** Si una superficie es reflectante, se lanza un rayo de reflexión desde el punto de intersección.[3, 10, 35, 39] La dirección del rayo de reflexión `Rr` se calcula de manera similar al vector `R` en el modelo de Phong, pero usando la dirección del rayo incidente `Ri` (que es el inverso del vector de dirección del rayo primario):
    `Rr = Ri - 2 * (N ⋅ Ri) * N` 
    El color devuelto por este rayo reflejado se mezcla con el color de la iluminación local del punto.[35, 39]
*   **Refracciones:** Para objetos transparentes, un rayo puede pasar a través de la superficie y doblarse (refractarse).[3, 10, 39] Esto requiere el uso de la Ley de Snell y el índice de refracción del material. Se lanza un rayo de refracción, y su color se mezcla con la iluminación local y los reflejos.[35, 39]

La capacidad del ray tracing para combinar múltiples trayectorias de luz, como las reflexiones y refracciones, es lo que permite un fotorrealismo significativo. Sin embargo, cada rebote o refracción implica lanzar un nuevo rayo y realizar más cálculos de intersección y sombreado, lo que aumenta drásticamente la complejidad computacional.

## 5. MiniLibX e Imagen

MiniLibX es una biblioteca gráfica ligera que proporciona una interfaz sencilla para la creación de ventanas, la gestión de imágenes y el manejo de eventos, lo que la hace ideal para proyectos de gráficos en 42 School.

### Inicialización de MiniLibX y Creación de Ventanas

El primer paso es inicializar la conexión con el entorno gráfico utilizando `mlx_init()`. Esta función devuelve un puntero `void *` que representa la conexión con MiniLibX; este puntero es crucial y debe almacenarse, ya que se requerirá para la mayoría de las funciones posteriores de MiniLibX.

Una vez inicializado MiniLibX, se puede crear una ventana utilizando `mlx_new_window(mlx_ptr, width, height, title)`. Esta función devuelve un puntero `void *` al identificador de la ventana, que también debe almacenarse. La ventana se abrirá con las dimensiones especificadas y un título.

### Gestión del Búfer de Imagen y Manipulación de Píxeles

Para dibujar en la ventana, se utiliza un búfer de imagen en memoria. Se crea una nueva imagen en memoria con `mlx_new_image(mlx_ptr, width, height)`.[12, 13, 40] Esta función devuelve un puntero `void *` al identificador de la imagen.

Para acceder y manipular los datos de los píxeles de esta imagen, se utiliza `mlx_get_data_addr(img_ptr, &bits_per_pixel, &size_line, &endian)`.[12, 13, 40]
*   `img_ptr`: El identificador de la imagen.
*   `bits_per_pixel`: Un puntero a un entero que se llenará con el número de bits utilizados para representar un píxel (normalmente 32 para ARGB).
*   `size_line`: Un puntero a un entero que se llenará con el número de bytes utilizados para almacenar una línea de la imagen en memoria.
*   `endian`: Un puntero a un entero que indica el orden de los bytes (0 para little-endian, 1 para big-endian).
*   La función devuelve un puntero `char *` al inicio del área de memoria donde se almacenan los datos de los píxeles.[12, 13, 40]

Los datos de los píxeles se almacenan como un array lineal de bytes. Para dibujar un píxel en las coordenadas (x, y) con un color determinado, se calcula el desplazamiento dentro de este búfer. Si `bits_per_pixel` es 32 (4 bytes por píxel), el desplazamiento se calcula como `(y * size_line) + (x * (bits_per_pixel / 8))`.[11, 12, 40] El color (un entero de 32 bits que representa ARGB) se escribe en esta posición de memoria. Es importante manejar la endianness si el entorno lo requiere, aunque a menudo se puede simplificar si `bits_per_pixel` es 32.[12, 40] Una función auxiliar `ft_put_pixel` es comúnmente implementada para encapsular esta lógica y realizar comprobaciones de límites para evitar "segmentation faults".[11]

### Visualización de la Imagen

Una vez que todos los píxeles han sido renderizados en el búfer de imagen, la imagen se muestra en la ventana utilizando `mlx_put_image_to_window(mlx_ptr, win_ptr, img_ptr, x, y)`.[4, 8, 12, 13] Los parámetros `x` e `y` definen la posición de la esquina superior izquierda de la imagen dentro de la ventana.

### Manejo de Eventos

MiniLibX proporciona un bucle de eventos principal, `mlx_loop(mlx_ptr)`, que espera eventos y llama a funciones definidas por el usuario asociadas con esos eventos.[8, 14] Esta función es un bucle infinito y no retorna.[14]

Se pueden asignar funciones (conocidas como "hooks") para diferentes tipos de eventos:
*   `mlx_key_hook(win_ptr, funct_ptr, param)`: Para eventos de pulsación de tecla.[14] La función `funct_ptr` recibirá el `keycode` de la tecla presionada y el `param`.[14] Es común usar esto para salir del programa con la tecla ESC.[8]
*   `mlx_mouse_hook(win_ptr, funct_ptr, param)`: Para eventos de botón del ratón.[14]
*   `mlx_expose_hook(win_ptr, funct_ptr, param)`: Para eventos de "expose", que se activan cuando una parte de la ventana necesita ser redibujada (por ejemplo, cuando la ventana se trae al foco).[14]
*   `mlx_loop_hook(mlx_ptr, funct_ptr, param)`: Una función que se llama continuamente cuando no hay otros eventos pendientes.[14] Esto puede usarse para animaciones o para redibujar la escena en cada fotograma si se implementa un renderizado en tiempo real.

Para cerrar la ventana y salir del programa de forma segura, es fundamental manejar el evento de cierre de ventana. Esto a menudo implica configurar un "hook" para el evento `DestroyNotify` (X event 17) o el protocolo `WM_DELETE_WINDOW` si se trabaja con X11, lo que permite una salida limpia.[14, 41]

## 6. Ejecución General del Proyecto

La ejecución del proyecto `miniRT` sigue un flujo de programa estructurado, desde la inicialización hasta el bucle de renderizado y el manejo de eventos.

### Flujo del Programa

1.  **Función `main`:**
    *   Verificar los argumentos de la línea de comandos para asegurar que se proporciona un archivo `.rt` válido.
    *   Inicializar MiniLibX (`mlx_init()`) y crear la ventana (`mlx_new_window()`).
    *   Inicializar la estructura de datos `t_scene`.
    *   Invocar el parser para leer el archivo `.rt` y poblar la estructura `t_scene` con todas las luces, la cámara y los objetos.
    *   Crear un búfer de imagen de MiniLibX (`mlx_new_image()`) y obtener su dirección de datos (`mlx_get_data_addr()`).[12, 40]

2.  **Bucle de Renderizado:**
    *   Iterar sobre cada píxel de la imagen (por ejemplo, con bucles anidados `for` para `y` y `x` desde 0 hasta `height-1` y `width-1` respectivamente).
    *   Para cada píxel:
        *   Construir un rayo primario desde la cámara a través del píxel actual.
        *   Trazar el rayo a través de la escena, probando intersecciones con todos los objetos (`t_sphere`, `t_plane`, `t_cylinder`).[4, 8]
        *   Identificar el objeto más cercano que intersecta el rayo (el que tiene el `t` positivo más pequeño).[6, 24, 26]
        *   Si se encuentra una intersección:
            *   Calcular el color del píxel utilizando el modelo de iluminación (Phong) y teniendo en cuenta las sombras (lanzando rayos de sombra a cada luz).[4, 6, 10, 22, 35]
            *   (Opcional, para bonus) Calcular contribuciones de reflexión y refracción recursivamente.[35, 39]
        *   Si no se encuentra intersección, asignar el color de fondo (por ejemplo, negro o un color predefinido).
        *   Escribir el color calculado en el búfer de imagen de MiniLibX en la posición del píxel actual (`ft_put_pixel` o similar).

3.  **Visualización y Bucle de Eventos:**
    *   Una vez que todos los píxeles han sido renderizados en el búfer de imagen, mostrar la imagen en la ventana (`mlx_put_image_to_window()`).[4, 8, 13]
    *   Configurar los "hooks" de eventos necesarios (por ejemplo, para cerrar la ventana con ESC o el botón de cierre).[8, 14]
    *   Iniciar el bucle principal de MiniLibX (`mlx_loop(mlx_ptr)`) para manejar las interacciones del usuario.[8, 14] Este bucle mantendrá la ventana abierta y responderá a los eventos hasta que el programa se cierre.

### Gestión de Recursos

Es esencial gestionar adecuadamente la memoria y otros recursos. Esto incluye:
*   Liberar la memoria asignada dinámicamente para las estructuras de la escena (listas enlazadas de objetos y luces) antes de que el programa finalice.
*   MiniLibX maneja gran parte de su propia memoria interna, pero es una buena práctica destruir la imagen (`mlx_destroy_image()`) y la ventana (`mlx_destroy_window()`) antes de salir, aunque en algunos entornos MiniLibX puede tener fugas de memoria conocidas si no se usan bibliotecas adicionales.[42, 43]

## 7. Norminette y Estilo

La Norminette es un estándar de codificación estricto impuesto por la Escuela 42 para todos los proyectos en C del Common Core Su propósito es fomentar la claridad, la legibilidad y la modularidad del código, aunque a menudo se percibe inicialmente como una restricción.

### Reglas Clave de la Norminette

*   **Nombres de Archivos y Directorios:** Solo minúsculas, dígitos y guiones bajos (snake_case)
*   **Nombres de Identificadores:**
    *   Variables y funciones: Solo minúsculas, dígitos y guiones bajos (snake_case)
    *   Nombres de estructuras: Deben comenzar con `s_` (por ejemplo, `s_sphere`)
    *   Nombres de `typedef`: Deben comenzar con `t_` (por ejemplo, `t_sphere`)
    *   Nombres de uniones: Deben comenzar con `u_`
    *   Nombres de enumeraciones: Deben comenzar con `e_`
    *   Nombres de globales: Deben comenzar con `g_`
    *   Todos los identificadores deben estar en inglés y ser lo más explícitos o mnemotécnicos posible
*   **Longitud de Función:** Cada función debe tener un máximo de 25 líneas, sin contar las llaves de la función Esto promueve la descomposición de tareas en funciones pequeñas y de propósito único.
*   **Longitud de Línea:** Cada línea debe tener un máximo de 80 columnas de ancho, incluyendo los comentarios Las tabulaciones cuentan como el número de espacios que representan, no como una sola columna
*   **Espacios y Saltos de Línea:**
    *   Una instrucción por línea
    *   Cada función debe estar separada por una línea vacía
    *   Las líneas vacías deben estar realmente vacías (sin espacios ni tabulaciones)
    *   Una línea nunca puede terminar con espacios o tabulaciones
    *   No puede haber dos espacios consecutivos
    *   Debe haber un salto de línea después de cada llave de apertura o final de estructura de control
    *   Cada coma o punto y coma debe ir seguido de un espacio, a menos que sea el final de una línea
    *   Cada operador u operando debe estar separado por un único espacio
    *   Cada palabra clave de C debe ir seguida de un espacio, excepto para las palabras clave de tipos (como `int`, `char`, `float`, etc.) y `sizeof`
*   **Declaraciones de Variables:**
    *   Deben estar al principio de una función
    *   Una sola declaración de variable por línea
    *   Las declaraciones y las inicializaciones no pueden estar en la misma línea, excepto para variables globales (cuando se permite), estáticas y constantes
    *   Debe haber una línea vacía entre las declaraciones de variables y el resto de la función
    *   No se permiten otras líneas vacías dentro de una función
    *   Las variables no pueden ser más de 5 por función.[16]
    *   Los asteriscos de los punteros deben ir pegados a los nombres de las variables (por ejemplo, `char *my_string;`)
*   **Estructuras de Control:** Las estructuras de control (`if`, `while`, etc.) deben usar llaves, a menos que contengan una sola instrucción en una sola línea
*   **Retorno de Función:** El valor de retorno de una función debe estar entre paréntesis (por ejemplo, `return (0);`).[16]
*   **Parámetros de Función:** Una función puede tomar un máximo de 4 parámetros con nombre.[17] Las funciones sin argumentos deben prototiparse explícitamente con `void`.[17]

La Norminette, aunque a menudo percibida como un conjunto de reglas de estilo de codificación arbitrarias y restrictivas, en realidad sirve a un propósito pedagógico y arquitectónico mucho más profundo. Sus limitaciones estrictas, en particular el límite de 25 líneas por función y la restricción en las declaraciones de variables, obligan implícitamente a los estudiantes a adoptar buenos principios de diseño modular. Para cumplir, es imperativo descomponer algoritmos complejos en numerosas funciones más pequeñas y de propósito único. Esta restricción, en lugar de obstaculizar la creatividad, cultiva una disciplina que conduce a un código más legible, testeable y mantenible, cualidades esenciales para proyectos de software de mayor envergadura. Así, la Norminette actúa como un ejecutor arquitectónico, guiando a los estudiantes hacia patrones de diseño de software intrínsecamente mejores al hacer imposible que el código monolítico o fuertemente acoplado pase la validación.

## 8. Uso de Git

Git es una herramienta fundamental para el control de versiones en el desarrollo de software, y su uso adecuado es una habilidad esencial en 42 School, donde el trabajo en grupo y la colaboración son pilares.[5]

### Mejores Prácticas de Git para Proyectos de 42

*   **Repositorio Centralizado:** Los proyectos de 42 suelen requerir un repositorio Git centralizado (por ejemplo, en GitHub o GitLab) para la colaboración y la evaluación.
*   **Ramas (Branches):**
    *   **`main` o `master`:** Mantener esta rama limpia y funcional en todo momento. Solo se deben fusionar en ella las características completas y probadas.
    *   **Ramas de Características (Feature Branches):** Para cada nueva característica o corrección de errores, crear una rama separada (por ejemplo, `git checkout -b my-feature`).[44] Esto aísla el trabajo y evita que los cambios no probados afecten la rama principal.
    *   **Ramas de Desarrollo/Integración:** En proyectos de grupo, puede ser útil tener una rama `dev` o `develop` donde se fusionen las ramas de características antes de integrarlas en `main`.
*   **Commits:**
    *   **Commits Atómicos:** Cada commit debe representar un cambio lógico y autocontenido. Evitar commits grandes que mezclen múltiples funcionalidades.
    *   **Mensajes de Commit Claros:** Escribir mensajes de commit descriptivos que expliquen *qué* se cambió y *por qué*. Un buen mensaje de commit facilita la revisión del código y la depuración futura.
    *   **Frecuencia:** Realizar commits con frecuencia. Esto crea un historial de versiones detallado y permite revertir fácilmente a estados anteriores si surge un problema.
*   **Flujo de Trabajo:**
    *   **`git pull`:** Antes de comenzar a trabajar o fusionar, siempre obtener los últimos cambios del repositorio remoto para evitar conflictos.
    *   **`git add <files>`:** Stage los cambios que se desean incluir en el próximo commit.
    *   **`git commit -m "Mensaje"`:** Crear un commit con los cambios staged.[44]
    *   **`git push`:** Subir los commits locales al repositorio remoto.[44]
    *   **`git merge` o `git rebase`:** Integrar cambios de otras ramas. `merge` crea un nuevo commit de fusión, mientras que `rebase` reescribe el historial de commits para que parezca una secuencia lineal. La elección depende de las preferencias del equipo y la complejidad del historial.
    *   **`git status` y `git diff`:** Utilizar estas herramientas con frecuencia para verificar el estado del repositorio y revisar los cambios antes de hacer commit.
*   **Manejo de Conflictos:** Los conflictos de fusión son inevitables en el trabajo en equipo. Aprender a resolverlos manualmente es una habilidad crucial.
*   **`.gitignore`:** Utilizar un archivo `.gitignore` para excluir archivos generados automáticamente (como los archivos objeto `.o`, ejecutables, etc.) del control de versiones. Esto mantiene el repositorio limpio y enfocado solo en el código fuente.

El uso de Git de manera efectiva no es solo una práctica técnica, sino una habilidad de colaboración fundamental. Un historial de Git bien mantenido es una forma de documentación viva que permite a los equipos entender la evolución del proyecto, identificar la causa de los errores y colaborar de manera eficiente. En el entorno de 42, donde el aprendizaje es autónomo y colaborativo, un dominio sólido de Git es tan importante como la capacidad de codificar.

## 9. Makefile

Un Makefile es un script que automatiza el proceso de compilación y enlace de un proyecto, especialmente en C. En la Escuela 42, se exige que los proyectos incluyan un Makefile que cumpla con reglas y objetivos específicos.

### Reglas y Objetivos Estándar de 42

El Makefile para `miniRT` debe incluir, como mínimo, los siguientes objetivos (targets) [45]:

*   **`$(NAME)`:** Este objetivo compila y enlaza el programa ejecutable final. `$(NAME)` es una variable que contendrá el nombre del ejecutable (por ejemplo, `miniRT`).[45]
    *   Debe definir el compilador (generalmente `gcc` o `cc`) y las banderas de compilación (`CFLAGS`) requeridas por 42, como `-Wall -Wextra -Werror` para habilitar todas las advertencias y tratarlas como errores.[45]
    *   Debe gestionar la compilación de archivos fuente (`.c`) a archivos objeto (`.o`) y luego el enlace de los archivos objeto para crear el ejecutable.
    *   Es crucial que el programa no se vuelva a enlazar innecesariamente si solo se han modificado los archivos objeto, lo que implica una gestión adecuada de las dependencias.[45]

*   **`all`:** Este es el objetivo predeterminado y el más común. Debe construir el programa completo, lo que generalmente significa que depende del objetivo `$(NAME)`.[45] Al ejecutar `make` sin argumentos, se ejecutará `all`.

*   **`clean`:** Este objetivo es responsable de eliminar todos los archivos objeto (`.o`) generados durante la compilación.[45] Su propósito es limpiar los archivos intermedios, dejando solo los archivos fuente y el ejecutable final. Se suele utilizar el comando `rm` para esto.[45, 46]

*   **`fclean`:** Este objetivo realiza una "limpieza total". Debe eliminar todos los archivos generados por el Makefile, incluyendo los archivos objeto (`.o`) y el ejecutable final (`$(NAME)`).[45] Típicamente, `fclean` depende de `clean` y luego elimina el ejecutable.[45]

*   **`re`:** Este objetivo significa "reconstruir". Debe limpiar completamente el proyecto (ejecutando `fclean`) y luego reconstruirlo desde cero (ejecutando `all`).[45] Es útil para asegurar una compilación limpia después de cambios significativos o para resolver problemas de compilación.

### Consideraciones Adicionales

*   **`PHONY` Targets:** Los objetivos `all`, `clean`, `fclean`, y `re` no corresponden a archivos reales que se crean. Para evitar que `make` intente crear archivos con estos nombres, deben declararse como objetivos `PHONY`.[45] Esto asegura que siempre se ejecuten los comandos asociados cuando se invocan.
*   **Variables:** El uso de variables (por ejemplo, `CC`, `CFLAGS`, `NAME`, `SOURCES`, `OBJECTS`) hace que el Makefile sea más modular y fácil de mantener.
*   **Reglas Implícitas y Explícitas:** `make` tiene reglas implícitas para compilar archivos `.c` a `.o`, pero es una buena práctica definir reglas explícitas o de patrón (`%.o: %.c`) para mayor control y claridad.[45, 46]
*   **MiniLibX Linkeo:** El Makefile debe incluir las banderas de enlace (`LDFLAGS`) necesarias para vincular con la biblioteca MiniLibX y otras bibliotecas del sistema (como `OpenGL` y `Appkit` en macOS, o `X11` y `Xext` en Linux).[9, 42] Por ejemplo: `-L/usr/local/lib -lmlx -framework OpenGL -framework Appkit` para macOS, o `-L/usr/X11/lib -lmlx -lXext -lX11` para Linux.[9, 42] También se debe especificar la inclusión de cabeceras (`-I`).

Un Makefile bien estructurado es más que un simple script de compilación; es una parte integral de la base de código que define cómo se construye y gestiona el proyecto. Su correcta implementación es un requisito fundamental en 42 y refleja la comprensión del estudiante sobre los procesos de construcción de software.

## 10. BONUS: Optimización con Hilos (Multithreading)

El ray tracing es una tarea computacionalmente intensiva, ya que el color de cada píxel se calcula de forma independiente. Esta característica lo convierte en un candidato ideal para la paralelización utilizando multithreading.

### Concepto de Multithreading en Ray Tracing

El multithreading permite dividir un proceso en múltiples unidades más pequeñas llamadas hilos (threads), que pueden ejecutarse simultáneamente. En el contexto del ray tracing, esto significa que diferentes partes de la imagen pueden ser renderizadas por diferentes hilos de forma concurrente.[48, 49] Dado que el cálculo del color de un píxel es en gran medida independiente del cálculo de otros píxeles (ignorando efectos como la iluminación global que podrían introducir dependencias), el ray tracing se considera un "problema vergonzosamente paralelo".

La paralelización puede mejorar significativamente el rendimiento del renderizado, especialmente en sistemas con múltiples núcleos de CPU.

### Implementación con Pthreads en C

En C, la biblioteca POSIX Threads (pthreads) se utiliza para implementar multithreading. Para usarla, se debe incluir el archivo de cabecera `<pthread.h>` y compilar el programa con la bandera `-lpthread`.

El proceso básico de multithreading con pthreads implica:
1.  **Creación de Hilos:** Utilizar `pthread_create()` para crear un nuevo hilo. Esta función toma:
    *   Un puntero a una variable `pthread_t` donde se almacenará el ID del nuevo hilo.
    *   Un puntero a un objeto de atributos de hilo (a menudo `NULL` para atributos predeterminados).
    *   Un puntero a la función que el hilo ejecutará (la "rutina del hilo"). Esta función debe tener la firma `void* (*start_routine)(void*)`.
    *   Un único argumento que se pasará a la función del hilo (a menudo `NULL` o un puntero a una estructura de datos que contenga la información necesaria para el hilo).

2.  **Unión de Hilos:** Utilizar `pthread_join()` para esperar a que un hilo termine su ejecución. Esto es crucial para asegurar que todos los cálculos de renderizado hayan finalizado antes de que el programa principal intente mostrar la imagen o salir.

### Estrategias de Paralelización

Existen varias formas de dividir el trabajo de renderizado entre los hilos:
*   **División por Filas/Columnas:** Asignar un conjunto de filas o columnas de píxeles a cada hilo. Por ejemplo, un hilo podría renderizar las filas pares y otro las impares, o dividir la imagen en franjas horizontales o verticales.
*   **División por Tiles (Baldosas):** Dividir la imagen en baldosas (por ejemplo, de 64x64 o 32x32 píxeles) y asignar dinámicamente estas baldosas a los hilos disponibles. Esta estrategia es a menudo más eficiente en términos de equilibrio de carga, ya que los hilos pueden tomar nuevas baldosas a medida que terminan las suyas, minimizando el tiempo de inactividad si algunas regiones son más complejas de renderizar que otras.

### Consideraciones de Rendimiento y Sincronización

Aunque el multithreading puede ofrecer mejoras significativas, también introduce desafíos:
*   **Sobrecarga de Hilos:** La creación y gestión de hilos conlleva una sobrecarga. Si el trabajo por hilo es demasiado pequeño, la sobrecarga puede superar las ganancias de rendimiento.
*   **Balanceo de Carga:** Es vital distribuir el trabajo de manera uniforme entre los hilos para evitar que algunos hilos terminen mucho antes que otros, dejando núcleos de CPU inactivos. La división por baldosas con asignación dinámica es una buena estrategia para esto.
*   **Condiciones de Carrera (Race Conditions):** Si múltiples hilos acceden y modifican datos compartidos sin la sincronización adecuada, pueden ocurrir condiciones de carrera, llevando a resultados incorrectos o comportamiento impredecible.[47, 52]
    *   **Mutexes:** Para proteger los datos compartidos (como un contador de píxeles procesados o una estructura de datos de escena que podría ser modificada por múltiples hilos, aunque en ray tracing puro esto es menos común ya que los píxeles son independientes), se pueden usar mutexes (locks). Un mutex asegura que solo un hilo pueda acceder a una sección crítica del código a la vez.
    *   `pthread_mutex_init()`: Inicializa un mutex.
    *   `pthread_mutex_lock()`: Bloquea el mutex; si ya está bloqueado, el hilo espera.
    *   `pthread_mutex_unlock()`: Libera el mutex.
    *   `pthread_mutex_destroy()`: Elimina un mutex.
*   **Falsos Compartidos (False Sharing):** Un problema de rendimiento en sistemas con caché que puede ocurrir si diferentes hilos modifican datos que residen en la misma línea de caché, incluso si los datos lógicamente no están relacionados. Esto puede llevar a invalidaciones de caché excesivas y reducir el rendimiento. La asignación de bloques contiguos de píxeles (como en la división por baldosas) tiende a mitigar esto en comparación con la asignación de píxeles intercalados.

La implementación de multithreading en `miniRT` no solo demuestra una comprensión de la optimización del rendimiento, sino que también expone a los estudiantes a los desafíos y soluciones de la programación concurrente, una habilidad valiosa en el desarrollo de software moderno.

## Conclusiones

El proyecto `miniRT` de la Escuela 42 representa un desafío integral que va más allá de la mera codificación, sirviendo como una profunda inmersión en los fundamentos de los gráficos 3D por computadora y la programación de sistemas. La guía detallada presentada subraya la importancia de una comprensión holística, que abarca desde la teoría matemática hasta las prácticas de implementación y gestión de proyectos.

La arquitectura del proyecto, con su clara separación entre el parser y el motor de renderizado, es un ejemplo fundamental de cómo la modularidad y la división de responsabilidades son esenciales para manejar la complejidad del software. Esta estructura, junto con la estricta adherencia a la Norminette, no solo impone un estilo de codificación, sino que también fuerza intrínsecamente la adopción de principios de diseño robustos, como la descomposición de funciones en unidades pequeñas y manejables. El cumplimiento de la Norminette, lejos de ser una simple formalidad, actúa como un catalizador para la creación de código legible, mantenible y testeable, habilidades críticas para cualquier desarrollador.

La definición de estructuras de datos es el esqueleto del ray tracer. La comprensión de que las matemáticas vectoriales son el lenguaje computacional de los gráficos 3D es primordial, ya que cada interacción geométrica y cálculo de iluminación se basa en operaciones vectoriales precisas. La utilización de listas enlazadas y uniones para gestionar la heterogeneidad de los elementos de la escena demuestra una aplicación práctica de las capacidades de bajo nivel de C para construir sistemas flexibles y extensibles. Además, la comprensión del parámetro `t` en las intersecciones como el árbitro de la visibilidad es fundamental para la precisión del renderizado.

El parser del archivo `.rt` es la puerta de entrada a la escena 3D, y su robustez, incluyendo la validación de datos y el manejo de errores, es tan crucial como el propio motor de renderizado. En cuanto al renderizado, la aplicación de la teoría matemática para la generación de rayos y las pruebas de intersección con esferas, planos y cilindros constituye el núcleo algorítmico. La implementación de modelos de iluminación como Phong, junto con la simulación de sombras, reflexiones y refracciones, es lo que confiere realismo a las imágenes, aunque con un costo computacional inherente.

La integración con MiniLibX es la interfaz visual del proyecto, permitiendo la creación de ventanas, la manipulación de píxeles y el manejo de eventos del usuario. Un flujo de ejecución general bien definido, desde la inicialización hasta el bucle de renderizado y eventos, asegura un programa funcional y reactivo. Finalmente, las prácticas de Git y la creación de un Makefile conforme a los estándares de 42 son habilidades de ingeniería de software indispensables que garantizan la colaboración efectiva y la gestión eficiente del ciclo de vida del proyecto.

El bonus de optimización con multithreading destaca la naturaleza computacionalmente intensiva del ray tracing y la necesidad de paralelización para mejorar el rendimiento. La aplicación de pthreads y el manejo de consideraciones como el balanceo de carga y las condiciones de carrera son un paso importante hacia la programación de alto rendimiento.

En síntesis, el proyecto `miniRT` es una experiencia de aprendizaje multifacética que desafía a los estudiantes a integrar conocimientos de matemáticas, física, algoritmos y diseño de software. Su correcta implementación no solo resulta en un renderizador funcional, sino que también cultiva una comprensión profunda de los principios fundamentales que sustentan los gráficos por computadora, preparando a los estudiantes para desafíos más avanzados en el campo.
O en forma vectorial: `(P - C) ⋅ (P - C) - R^2 = 0`.[24, 28]

Sustituyendo la ecuación del rayo `P(t) = O + tD` en la ecuación de la esfera, se obtiene una ecuación cuadrática en términos de `t`:
`(D ⋅ D)t^2 + 2D ⋅ (O - C)t + (O - C) ⋅ (O - C) - R^2 = 0`.[24, 28, 29]
Esto se puede escribir como `At^2 + Bt + C = 0`, donde:
*   `A = D ⋅ D` [24, 28, 29]
*   `B = 2D ⋅ (O - C)` [24, 28, 29]
*   `C = (O - C) ⋅ (O - C) - R^2` [24, 28, 29]

Las soluciones para `t` se encuentran usando la fórmula cuadrática: `t = (-B ± sqrt(B^2 - 4AC)) / (2A)`.[24, 28]
El discriminante `(B^2 - 4AC)` determina el número de intersecciones:
*   Si es negativo, no hay intersección.[6, 24, 28]
*   Si es cero, el rayo es tangente a la esfera (una intersección).[6, 24, 28]
*   Si es positivo, hay dos intersecciones (el rayo entra y sale de la esfera).[6, 24, 28]

Se debe seleccionar el valor `t` positivo más pequeño. La normal de la superficie en el punto de intersección `P` de una esfera es simplemente el vector normalizado desde el centro de la esfera hasta `P`: `N = normalize(P - C)`.[15, 21]

#### Intersección Rayo-Plano

Un plano se define por un punto `S` en el plano y un vector normal `N` perpendicular a él La ecuación implícita de un plano es:
`N ⋅ (P - S) = 0` [27, 30, 31]
Sustituyendo la ecuación del rayo `P(t) = O + tD` en la ecuación del plano:
`N ⋅ (O + tD - S) = 0`
Expandiendo y resolviendo para `t`:
`t = (S - O) ⋅ N / (D ⋅ N)`

Casos especiales:
*   Si `D ⋅ N = 0`, el rayo es paralelo al plano. No hay intersección a menos que el rayo ya esté en el plano (`(S - O) ⋅ N = 0`).
*   Se debe verificar que `t` sea positivo.
La normal de la superficie para un plano es simplemente su vector normal `N` predefinido.

#### Intersección Rayo-Cilindro

La intersección con un cilindro es más compleja, ya que un cilindro finito consta de un cuerpo cilíndrico infinito y dos tapas (planos).[15, 32, 33]
1.  **Intersección con el Cilindro Infinito:** Un cilindro infinito a lo largo del eje Y con radio `r` tiene la ecuación `x^2 + z^2 - r^2 = 0`.[32, 33] Para un cilindro general con centro `pa` y vector de eje `va`, la ecuación es:
    `(q - pa - (va ⋅ (q - pa))va)^2 - r^2 = 0` [32, 33]
    Sustituyendo `q = O + tD` y resolviendo, se obtiene una ecuación cuadrática `At^2 + Bt + C = 0` similar a la de la esfera, pero con coeficientes más complejos que involucran `va`.[32, 33]
2.  **Verificación de Tapas:** Una vez que se encuentran las intersecciones con el cilindro infinito, se debe verificar si los puntos de intersección caen dentro de la altura definida del cilindro (entre las tapas superior e inferior).[15, 32, 33] Esto se hace comprobando la proyección del punto de intersección sobre el eje del cilindro.
3.  **Intersección con las Tapas:** Las tapas del cilindro son planos. Se calculan las intersecciones del rayo con los planos que definen las tapas superior e inferior del cilindro.[15, 32, 33] Para cada intersección de tapa, se debe verificar si el punto de intersección se encuentra dentro del círculo de la tapa.[32, 33]
Se elige el `t` positivo más pequeño entre todas las intersecciones válidas (cuerpo o tapas). La normal de la superficie para el cuerpo del cilindro se calcula como el vector desde el punto de intersección hasta el eje del cilindro, normalizado. Para las tapas, la normal es simplemente el vector del eje del cilindro (o su negativo, dependiendo de la tapa).

### Modelos de Iluminación y Sombreado

Una vez que se encuentra el punto de intersección más cercano, se calcula el color de ese píxel. Esto implica aplicar modelos de iluminación que simulan cómo la luz interactúa con la superficie del objeto. El modelo de iluminación de Phong es un modelo empírico comúnmente utilizado que combina componentes de luz ambiental, difusa y especular.[4, 22, 34]

#### Componentes del Modelo de Iluminación de Phong

*   **Luz Ambiental (Ambient):** Representa la luz indirecta que se dispersa por toda la escena, proporcionando una iluminación de base uniforme.[7, 22, 23] Se calcula multiplicando el color del objeto por la relación y el color de la luz ambiental global.[22]
    `Color_Ambient = Objeto.Color * Ambient.Ratio * Ambient.Color` 

*   **Luz Difusa (Diffuse):** Simula la luz que se dispersa uniformemente en todas las direcciones desde una superficie rugosa (superficie Lambertiana).[22, 23, 36] La intensidad de la luz difusa depende del ángulo entre el vector normal de la superficie (`N`) y el vector de dirección hacia la fuente de luz (`L`). Se calcula utilizando la ley del coseno de Lambert:
    `Color_Difuso = Objeto.Color * Luz.Color * max(0, N ⋅ L)` [22, 23, 34, 37]
    Donde `N` y `L` son vectores unitarios. El `max(0,...)` asegura que solo se considere la luz que incide en la superficie.

*   **Luz Especular (Specular):** Representa los reflejos brillantes que aparecen en superficies lisas y brillantes, como el metal o el plástico pulido.[22, 23] La intensidad de la luz especular depende del ángulo entre el vector de dirección del ojo (`E`, que es el inverso de la dirección del rayo) y el vector de dirección de la luz reflejada (`R`). `R` se calcula como la reflexión de `L` sobre `N`:
    `R = 2 * (N ⋅ L) * N - L` [34, 38]
    La contribución especular se calcula como:
    `Color_Especular = Especular.Color * Luz.Color * max(0, E ⋅ R)^p` [22, 34]
    Donde `p` es el exponente especular (potencia de Phong), que controla el tamaño y la intensidad del brillo (un `p` más grande produce un brillo más pequeño e intenso).[22, 34]

La contribución total de la iluminación local para un punto es la suma de estos componentes:
`Color_Total = Color_Ambient + ∑ (Color_Difuso + Color_Especular)` (sumado para cada fuente de luz).

#### Sombras (Shadows)

Para simular sombras, se lanza un "rayo de sombra" desde el punto de intersección `P` en la superficie hacia cada fuente de luz `Li`. Es crucial desplazar ligeramente el origen del rayo de sombra fuera de la superficie para evitar que intersecte el propio objeto del que proviene.
`Rayo_Sombra_Origen = P + ε * L_dir` (donde `ε` es un valor pequeño, como 0.001) 
`Rayo_Sombra_Direccion = normalize(Li.position - P)`

Si este rayo de sombra intersecta cualquier otro objeto en la escena *antes* de llegar a la fuente de luz, significa que la fuente de luz está ocluida y el punto `P` está en sombra con respecto a esa luz. En este caso, la contribución difusa y especular de esa luz en particular para el punto `P` se anula (o se reduce significativamente).

#### Reflexiones (Reflections) y Refracciones (Refractions) (Bonus)

Para un mayor realismo, `miniRT` puede extenderse para incluir reflexiones y refracciones. Esto implica un trazado de rayos recursivo.
*   **Reflexiones:** Si una superficie es reflectante, se lanza un rayo de reflexión desde el punto de intersección.[3, 10, 35, 39] La dirección del rayo de reflexión `Rr` se calcula de manera similar al vector `R` en el modelo de Phong, pero usando la dirección del rayo incidente `Ri` (que es el inverso del vector de dirección del rayo primario):
    `Rr = Ri - 2 * (N ⋅ Ri) * N` 
    El color devuelto por este rayo reflejado se mezcla con el color de la iluminación local del punto.[35, 39]
*   **Refracciones:** Para objetos transparentes, un rayo puede pasar a través de la superficie y doblarse (refractarse).[3, 10, 39] Esto requiere el uso de la Ley de Snell y el índice de refracción del material. Se lanza un rayo de refracción, y su color se mezcla con la iluminación local y los reflejos.[35, 39]

La capacidad del ray tracing para combinar múltiples trayectorias de luz, como las reflexiones y refracciones, es lo que permite un fotorrealismo significativo. Sin embargo, cada rebote o refracción implica lanzar un nuevo rayo y realizar más cálculos de intersección y sombreado, lo que aumenta drásticamente la complejidad computacional.

## 5. MiniLibX e Imagen

MiniLibX es una biblioteca gráfica ligera que proporciona una interfaz sencilla para la creación de ventanas, la gestión de imágenes y el manejo de eventos, lo que la hace ideal para proyectos de gráficos en 42 School.

### Inicialización de MiniLibX y Creación de Ventanas

El primer paso es inicializar la conexión con el entorno gráfico utilizando `mlx_init()`. Esta función devuelve un puntero `void *` que representa la conexión con MiniLibX; este puntero es crucial y debe almacenarse, ya que se requerirá para la mayoría de las funciones posteriores de MiniLibX.

Una vez inicializado MiniLibX, se puede crear una ventana utilizando `mlx_new_window(mlx_ptr, width, height, title)`. Esta función devuelve un puntero `void *` al identificador de la ventana, que también debe almacenarse. La ventana se abrirá con las dimensiones especificadas y un título.

### Gestión del Búfer de Imagen y Manipulación de Píxeles

Para dibujar en la ventana, se utiliza un búfer de imagen en memoria. Se crea una nueva imagen en memoria con `mlx_new_image(mlx_ptr, width, height)`.[12, 13, 40] Esta función devuelve un puntero `void *` al identificador de la imagen.

Para acceder y manipular los datos de los píxeles de esta imagen, se utiliza `mlx_get_data_addr(img_ptr, &bits_per_pixel, &size_line, &endian)`.[12, 13, 40]
*   `img_ptr`: El identificador de la imagen.
*   `bits_per_pixel`: Un puntero a un entero que se llenará con el número de bits utilizados para representar un píxel (normalmente 32 para ARGB).
*   `size_line`: Un puntero a un entero que se llenará con el número de bytes utilizados para almacenar una línea de la imagen en memoria.
*   `endian`: Un puntero a un entero que indica el orden de los bytes (0 para little-endian, 1 para big-endian).
*   La función devuelve un puntero `char *` al inicio del área de memoria donde se almacenan los datos de los píxeles.[12, 13, 40]

Los datos de los píxeles se almacenan como un array lineal de bytes. Para dibujar un píxel en las coordenadas (x, y) con un color determinado, se calcula el desplazamiento dentro de este búfer. Si `bits_per_pixel` es 32 (4 bytes por píxel), el desplazamiento se calcula como `(y * size_line) + (x * (bits_per_pixel / 8))`.[11, 12, 40] El color (un entero de 32 bits que representa ARGB) se escribe en esta posición de memoria. Es importante manejar la endianness si el entorno lo requiere, aunque a menudo se puede simplificar si `bits_per_pixel` es 32.[12, 40] Una función auxiliar `ft_put_pixel` es comúnmente implementada para encapsular esta lógica y realizar comprobaciones de límites para evitar "segmentation faults".[11]

### Visualización de la Imagen

Una vez que todos los píxeles han sido renderizados en el búfer de imagen, la imagen se muestra en la ventana utilizando `mlx_put_image_to_window(mlx_ptr, win_ptr, img_ptr, x, y)`.[4, 8, 12, 13] Los parámetros `x` e `y` definen la posición de la esquina superior izquierda de la imagen dentro de la ventana.

### Manejo de Eventos

MiniLibX proporciona un bucle de eventos principal, `mlx_loop(mlx_ptr)`, que espera eventos y llama a funciones definidas por el usuario asociadas con esos eventos.[8, 14] Esta función es un bucle infinito y no retorna.[14]

Se pueden asignar funciones (conocidas como "hooks") para diferentes tipos de eventos:
*   `mlx_key_hook(win_ptr, funct_ptr, param)`: Para eventos de pulsación de tecla.[14] La función `funct_ptr` recibirá el `keycode` de la tecla presionada y el `param`.[14] Es común usar esto para salir del programa con la tecla ESC.[8]
*   `mlx_mouse_hook(win_ptr, funct_ptr, param)`: Para eventos de botón del ratón.[14]
*   `mlx_expose_hook(win_ptr, funct_ptr, param)`: Para eventos de "expose", que se activan cuando una parte de la ventana necesita ser redibujada (por ejemplo, cuando la ventana se trae al foco).[14]
*   `mlx_loop_hook(mlx_ptr, funct_ptr, param)`: Una función que se llama continuamente cuando no hay otros eventos pendientes.[14] Esto puede usarse para animaciones o para redibujar la escena en cada fotograma si se implementa un renderizado en tiempo real.

Para cerrar la ventana y salir del programa de forma segura, es fundamental manejar el evento de cierre de ventana. Esto a menudo implica configurar un "hook" para el evento `DestroyNotify` (X event 17) o el protocolo `WM_DELETE_WINDOW` si se trabaja con X11, lo que permite una salida limpia.[14, 41]

## 6. Ejecución General del Proyecto

La ejecución del proyecto `miniRT` sigue un flujo de programa estructurado, desde la inicialización hasta el bucle de renderizado y el manejo de eventos.

### Flujo del Programa

1.  **Función `main`:**
    *   Verificar los argumentos de la línea de comandos para asegurar que se proporciona un archivo `.rt` válido.
    *   Inicializar MiniLibX (`mlx_init()`) y crear la ventana (`mlx_new_window()`).
    *   Inicializar la estructura de datos `t_scene`.
    *   Invocar el parser para leer el archivo `.rt` y poblar la estructura `t_scene` con todas las luces, la cámara y los objetos.
    *   Crear un búfer de imagen de MiniLibX (`mlx_new_image()`) y obtener su dirección de datos (`mlx_get_data_addr()`).[12, 40]

2.  **Bucle de Renderizado:**
    *   Iterar sobre cada píxel de la imagen (por ejemplo, con bucles anidados `for` para `y` y `x` desde 0 hasta `height-1` y `width-1` respectivamente).
    *   Para cada píxel:
        *   Construir un rayo primario desde la cámara a través del píxel actual.
        *   Trazar el rayo a través de la escena, probando intersecciones con todos los objetos (`t_sphere`, `t_plane`, `t_cylinder`).[4, 8]
        *   Identificar el objeto más cercano que intersecta el rayo (el que tiene el `t` positivo más pequeño).[6, 24, 26]
        *   Si se encuentra una intersección:
            *   Calcular el color del píxel utilizando el modelo de iluminación (Phong) y teniendo en cuenta las sombras (lanzando rayos de sombra a cada luz).[4, 6, 10, 22, 35]
            *   (Opcional, para bonus) Calcular contribuciones de reflexión y refracción recursivamente.[35, 39]
        *   Si no se encuentra intersección, asignar el color de fondo (por ejemplo, negro o un color predefinido).
        *   Escribir el color calculado en el búfer de imagen de MiniLibX en la posición del píxel actual (`ft_put_pixel` o similar).

3.  **Visualización y Bucle de Eventos:**
    *   Una vez que todos los píxeles han sido renderizados en el búfer de imagen, mostrar la imagen en la ventana (`mlx_put_image_to_window()`).[4, 8, 13]
    *   Configurar los "hooks" de eventos necesarios (por ejemplo, para cerrar la ventana con ESC o el botón de cierre).[8, 14]
    *   Iniciar el bucle principal de MiniLibX (`mlx_loop(mlx_ptr)`) para manejar las interacciones del usuario.[8, 14] Este bucle mantendrá la ventana abierta y responderá a los eventos hasta que el programa se cierre.

### Gestión de Recursos

Es esencial gestionar adecuadamente la memoria y otros recursos. Esto incluye:
*   Liberar la memoria asignada dinámicamente para las estructuras de la escena (listas enlazadas de objetos y luces) antes de que el programa finalice.
*   MiniLibX maneja gran parte de su propia memoria interna, pero es una buena práctica destruir la imagen (`mlx_destroy_image()`) y la ventana (`mlx_destroy_window()`) antes de salir, aunque en algunos entornos MiniLibX puede tener fugas de memoria conocidas si no se usan bibliotecas adicionales.[42, 43]

## 7. Norminette y Estilo

La Norminette es un estándar de codificación estricto impuesto por la Escuela 42 para todos los proyectos en C del Common Core Su propósito es fomentar la claridad, la legibilidad y la modularidad del código, aunque a menudo se percibe inicialmente como una restricción.

### Reglas Clave de la Norminette

*   **Nombres de Archivos y Directorios:** Solo minúsculas, dígitos y guiones bajos (snake_case)
*   **Nombres de Identificadores:**
    *   Variables y funciones: Solo minúsculas, dígitos y guiones bajos (snake_case)
    *   Nombres de estructuras: Deben comenzar con `s_` (por ejemplo, `s_sphere`)
    *   Nombres de `typedef`: Deben comenzar con `t_` (por ejemplo, `t_sphere`)
    *   Nombres de uniones: Deben comenzar con `u_`
    *   Nombres de enumeraciones: Deben comenzar con `e_`
    *   Nombres de globales: Deben comenzar con `g_`
    *   Todos los identificadores deben estar en inglés y ser lo más explícitos o mnemotécnicos posible
*   **Longitud de Función:** Cada función debe tener un máximo de 25 líneas, sin contar las llaves de la función Esto promueve la descomposición de tareas en funciones pequeñas y de propósito único.
*   **Longitud de Línea:** Cada línea debe tener un máximo de 80 columnas de ancho, incluyendo los comentarios Las tabulaciones cuentan como el número de espacios que representan, no como una sola columna
*   **Espacios y Saltos de Línea:**
    *   Una instrucción por línea
    *   Cada función debe estar separada por una línea vacía
    *   Las líneas vacías deben estar realmente vacías (sin espacios ni tabulaciones)
    *   Una línea nunca puede terminar con espacios o tabulaciones
    *   No puede haber dos espacios consecutivos
    *   Debe haber un salto de línea después de cada llave de apertura o final de estructura de control
    *   Cada coma o punto y coma debe ir seguido de un espacio, a menos que sea el final de una línea
    *   Cada operador u operando debe estar separado por un único espacio
    *   Cada palabra clave de C debe ir seguida de un espacio, excepto para las palabras clave de tipos (como `int`, `char`, `float`, etc.) y `sizeof`
*   **Declaraciones de Variables:**
    *   Deben estar al principio de una función
    *   Una sola declaración de variable por línea
    *   Las declaraciones y las inicializaciones no pueden estar en la misma línea, excepto para variables globales (cuando se permite), estáticas y constantes
    *   Debe haber una línea vacía entre las declaraciones de variables y el resto de la función
    *   No se permiten otras líneas vacías dentro de una función
    *   Las variables no pueden ser más de 5 por función.[16]
    *   Los asteriscos de los punteros deben ir pegados a los nombres de las variables (por ejemplo, `char *my_string;`)
*   **Estructuras de Control:** Las estructuras de control (`if`, `while`, etc.) deben usar llaves, a menos que contengan una sola instrucción en una sola línea
*   **Retorno de Función:** El valor de retorno de una función debe estar entre paréntesis (por ejemplo, `return (0);`).[16]
*   **Parámetros de Función:** Una función puede tomar un máximo de 4 parámetros con nombre.[17] Las funciones sin argumentos deben prototiparse explícitamente con `void`.[17]

La Norminette, aunque a menudo percibida como un conjunto de reglas de estilo de codificación arbitrarias y restrictivas, en realidad sirve a un propósito pedagógico y arquitectónico mucho más profundo. Sus limitaciones estrictas, en particular el límite de 25 líneas por función y la restricción en las declaraciones de variables, obligan implícitamente a los estudiantes a adoptar buenos principios de diseño modular. Para cumplir, es imperativo descomponer algoritmos complejos en numerosas funciones más pequeñas y de propósito único. Esta restricción, en lugar de obstaculizar la creatividad, cultiva una disciplina que conduce a un código más legible, testeable y mantenible, cualidades esenciales para proyectos de software de mayor envergadura. Así, la Norminette actúa como un ejecutor arquitectónico, guiando a los estudiantes hacia patrones de diseño de software intrínsecamente mejores al hacer imposible que el código monolítico o fuertemente acoplado pase la validación.

## 8. Uso de Git

Git es una herramienta fundamental para el control de versiones en el desarrollo de software, y su uso adecuado es una habilidad esencial en 42 School, donde el trabajo en grupo y la colaboración son pilares.

### Mejores Prácticas de Git para Proyectos de 42

*   **Repositorio Centralizado:** Los proyectos de 42 suelen requerir un repositorio Git centralizado (por ejemplo, en GitHub o GitLab) para la colaboración y la evaluación.
*   **Ramas (Branches):**
    *   **`main` o `master`:** Mantener esta rama limpia y funcional en todo momento. Solo se deben fusionar en ella las características completas y probadas.
    *   **Ramas de Características (Feature Branches):** Para cada nueva característica o corrección de errores, crear una rama separada (por ejemplo, `git checkout -b my-feature`).[44] Esto aísla el trabajo y evita que los cambios no probados afecten la rama principal.
    *   **Ramas de Desarrollo/Integración:** En proyectos de grupo, puede ser útil tener una rama `dev` o `develop` donde se fusionen las ramas de características antes de integrarlas en `main`.
*   **Commits:**
    *   **Commits Atómicos:** Cada commit debe representar un cambio lógico y autocontenido. Evitar commits grandes que mezclen múltiples funcionalidades.
    *   **Mensajes de Commit Claros:** Escribir mensajes de commit descriptivos que expliquen *qué* se cambió y *por qué*. Un buen mensaje de commit facilita la revisión del código y la depuración futura.
    *   **Frecuencia:** Realizar commits con frecuencia. Esto crea un historial de versiones detallado y permite revertir fácilmente a estados anteriores si surge un problema.
*   **Flujo de Trabajo:**
    *   **`git pull`:** Antes de comenzar a trabajar o fusionar, siempre obtener los últimos cambios del repositorio remoto para evitar conflictos.
    *   **`git add <files>`:** Stage los cambios que se desean incluir en el próximo commit.
    *   **`git commit -m "Mensaje"`:** Crear un commit con los cambios staged.[44]
    *   **`git push`:** Subir los commits locales al repositorio remoto.[44]
    *   **`git merge` o `git rebase`:** Integrar cambios de otras ramas. `merge` crea un nuevo commit de fusión, mientras que `rebase` reescribe el historial de commits para que parezca una secuencia lineal. La elección depende de las preferencias del equipo y la complejidad del historial.
    *   **`git status` y `git diff`:** Utilizar estas herramientas con frecuencia para verificar el estado del repositorio y revisar los cambios antes de hacer commit.
*   **Manejo de Conflictos:** Los conflictos de fusión son inevitables en el trabajo en equipo. Aprender a resolverlos manualmente es una habilidad crucial.
*   **`.gitignore`:** Utilizar un archivo `.gitignore` para excluir archivos generados automáticamente (como los archivos objeto `.o`, ejecutables, etc.) del control de versiones. Esto mantiene el repositorio limpio y enfocado solo en el código fuente.

El uso de Git de manera efectiva no es solo una práctica técnica, sino una habilidad de colaboración fundamental. Un historial de Git bien mantenido es una forma de documentación viva que permite a los equipos entender la evolución del proyecto, identificar la causa de los errores y colaborar de manera eficiente. En el entorno de 42, donde el aprendizaje es autónomo y colaborativo, un dominio sólido de Git es tan importante como la capacidad de codificar.

## 9. Makefile

Un Makefile es un script que automatiza el proceso de compilación y enlace de un proyecto, especialmente en C. En la Escuela 42, se exige que los proyectos incluyan un Makefile que cumpla con reglas y objetivos específicos.

### Reglas y Objetivos Estándar de 42

El Makefile para `miniRT` debe incluir, como mínimo, los siguientes objetivos (targets) [45]:

*   **`$(NAME)`:** Este objetivo compila y enlaza el programa ejecutable final. `$(NAME)` es una variable que contendrá el nombre del ejecutable (por ejemplo, `miniRT`).[45]
    *   Debe definir el compilador (generalmente `gcc` o `cc`) y las banderas de compilación (`CFLAGS`) requeridas por 42, como `-Wall -Wextra -Werror` para habilitar todas las advertencias y tratarlas como errores.[45]
    *   Debe gestionar la compilación de archivos fuente (`.c`) a archivos objeto (`.o`) y luego el enlace de los archivos objeto para crear el ejecutable.
    *   Es crucial que el programa no se vuelva a enlazar innecesariamente si solo se han modificado los archivos objeto, lo que implica una gestión adecuada de las dependencias.[45]

*   **`all`:** Este es el objetivo predeterminado y el más común. Debe construir el programa completo, lo que generalmente significa que depende del objetivo `$(NAME)`.[45] Al ejecutar `make` sin argumentos, se ejecutará `all`.

*   **`clean`:** Este objetivo es responsable de eliminar todos los archivos objeto (`.o`) generados durante la compilación.[45] Su propósito es limpiar los archivos intermedios, dejando solo los archivos fuente y el ejecutable final. Se suele utilizar el comando `rm` para esto.[45, 46]

*   **`fclean`:** Este objetivo realiza una "limpieza total". Debe eliminar todos los archivos generados por el Makefile, incluyendo los archivos objeto (`.o`) y el ejecutable final (`$(NAME)`).[45] Típicamente, `fclean` depende de `clean` y luego elimina el ejecutable.[45]

*   **`re`:** Este objetivo significa "reconstruir". Debe limpiar completamente el proyecto (ejecutando `fclean`) y luego reconstruirlo desde cero (ejecutando `all`).[45] Es útil para asegurar una compilación limpia después de cambios significativos o para resolver problemas de compilación.

### Consideraciones Adicionales

*   **`PHONY` Targets:** Los objetivos `all`, `clean`, `fclean`, y `re` no corresponden a archivos reales que se crean. Para evitar que `make` intente crear archivos con estos nombres, deben declararse como objetivos `PHONY`.[45] Esto asegura que siempre se ejecuten los comandos asociados cuando se invocan.
*   **Variables:** El uso de variables (por ejemplo, `CC`, `CFLAGS`, `NAME`, `SOURCES`, `OBJECTS`) hace que el Makefile sea más modular y fácil de mantener.
*   **Reglas Implícitas y Explícitas:** `make` tiene reglas implícitas para compilar archivos `.c` a `.o`, pero es una buena práctica definir reglas explícitas o de patrón (`%.o: %.c`) para mayor control y claridad.[45, 46]
*   **MiniLibX Linkeo:** El Makefile debe incluir las banderas de enlace (`LDFLAGS`) necesarias para vincular con la biblioteca MiniLibX y otras bibliotecas del sistema (como `OpenGL` y `Appkit` en macOS, o `X11` y `Xext` en Linux).[9, 42] Por ejemplo: `-L/usr/local/lib -lmlx -framework OpenGL -framework Appkit` para macOS, o `-L/usr/X11/lib -lmlx -lXext -lX11` para Linux.[9, 42] También se debe especificar la inclusión de cabeceras (`-I`).

Un Makefile bien estructurado es más que un simple script de compilación; es una parte integral de la base de código que define cómo se construye y gestiona el proyecto. Su correcta implementación es un requisito fundamental en 42 y refleja la comprensión del estudiante sobre los procesos de construcción de software.

## 10. BONUS: Optimización con Hilos (Multithreading)

El ray tracing es una tarea computacionalmente intensiva, ya que el color de cada píxel se calcula de forma independiente. Esta característica lo convierte en un candidato ideal para la paralelización utilizando multithreading.

### Concepto de Multithreading en Ray Tracing

El multithreading permite dividir un proceso en múltiples unidades más pequeñas llamadas hilos (threads), que pueden ejecutarse simultáneamente. En el contexto del ray tracing, esto significa que diferentes partes de la imagen pueden ser renderizadas por diferentes hilos de forma concurrente.[48, 49] Dado que el cálculo del color de un píxel es en gran medida independiente del cálculo de otros píxeles (ignorando efectos como la iluminación global que podrían introducir dependencias), el ray tracing se considera un "problema vergonzosamente paralelo".

La paralelización puede mejorar significativamente el rendimiento del renderizado, especialmente en sistemas con múltiples núcleos de CPU.

### Implementación con Pthreads en C

En C, la biblioteca POSIX Threads (pthreads) se utiliza para implementar multithreading. Para usarla, se debe incluir el archivo de cabecera `<pthread.h>` y compilar el programa con la bandera `-lpthread`.

El proceso básico de multithreading con pthreads implica:
1.  **Creación de Hilos:** Utilizar `pthread_create()` para crear un nuevo hilo. Esta función toma:
    *   Un puntero a una variable `pthread_t` donde se almacenará el ID del nuevo hilo.
    *   Un puntero a un objeto de atributos de hilo (a menudo `NULL` para atributos predeterminados).
    *   Un puntero a la función que el hilo ejecutará (la "rutina del hilo"). Esta función debe tener la firma `void* (*start_routine)(void*)`.
    *   Un único argumento que se pasará a la función del hilo (a menudo `NULL` o un puntero a una estructura de datos que contenga la información necesaria para el hilo).

2.  **Unión de Hilos:** Utilizar `pthread_join()` para esperar a que un hilo termine su ejecución. Esto es crucial para asegurar que todos los cálculos de renderizado hayan finalizado antes de que el programa principal intente mostrar la imagen o salir.

### Estrategias de Paralelización

Existen varias formas de dividir el trabajo de renderizado entre los hilos:
*   **División por Filas/Columnas:** Asignar un conjunto de filas o columnas de píxeles a cada hilo. Por ejemplo, un hilo podría renderizar las filas pares y otro las impares, o dividir la imagen en franjas horizontales o verticales.
*   **División por Tiles (Baldosas):** Dividir la imagen en baldosas (por ejemplo, de 64x64 o 32x32 píxeles) y asignar dinámicamente estas baldosas a los hilos disponibles. Esta estrategia es a menudo más eficiente en términos de equilibrio de carga, ya que los hilos pueden tomar nuevas baldosas a medida que terminan las suyas, minimizando el tiempo de inactividad si algunas regiones son más complejas de renderizar que otras.

### Consideraciones de Rendimiento y Sincronización

Aunque el multithreading puede ofrecer mejoras significativas, también introduce desafíos:
*   **Sobrecarga de Hilos:** La creación y gestión de hilos conlleva una sobrecarga. Si el trabajo por hilo es demasiado pequeño, la sobrecarga puede superar las ganancias de rendimiento.
*   **Balanceo de Carga:** Es vital distribuir el trabajo de manera uniforme entre los hilos para evitar que algunos hilos terminen mucho antes que otros, dejando núcleos de CPU inactivos. La división por baldosas con asignación dinámica es una buena estrategia para esto.
*   **Condiciones de Carrera (Race Conditions):** Si múltiples hilos acceden y modifican datos compartidos sin la sincronización adecuada, pueden ocurrir condiciones de carrera, llevando a resultados incorrectos o comportamiento impredecible.[47, 52]
    *   **Mutexes:** Para proteger los datos compartidos (como un contador de píxeles procesados o una estructura de datos de escena que podría ser modificada por múltiples hilos, aunque en ray tracing puro esto es menos común ya que los píxeles son independientes), se pueden usar mutexes (locks). Un mutex asegura que solo un hilo pueda acceder a una sección crítica del código a la vez.
    *   `pthread_mutex_init()`: Inicializa un mutex.
    *   `pthread_mutex_lock()`: Bloquea el mutex; si ya está bloqueado, el hilo espera.
    *   `pthread_mutex_unlock()`: Libera el mutex.
    *   `pthread_mutex_destroy()`: Elimina un mutex.
*   **Falsos Compartidos (False Sharing):** Un problema de rendimiento en sistemas con caché que puede ocurrir si diferentes hilos modifican datos que residen en la misma línea de caché, incluso si los datos lógicamente no están relacionados. Esto puede llevar a invalidaciones de caché excesivas y reducir el rendimiento. La asignación de bloques contiguos de píxeles (como en la división por baldosas) tiende a mitigar esto en comparación con la asignación de píxeles intercalados.

La implementación de multithreading en `miniRT` no solo demuestra una comprensión de la optimización del rendimiento, sino que también expone a los estudiantes a los desafíos y soluciones de la programación concurrente, una habilidad valiosa en el desarrollo de software moderno.

## Conclusiones

El proyecto `miniRT` de la Escuela 42 representa un desafío integral que va más allá de la mera codificación, sirviendo como una profunda inmersión en los fundamentos de los gráficos 3D por computadora y la programación de sistemas. La guía detallada presentada subraya la importancia de una comprensión holística, que abarca desde la teoría matemática hasta las prácticas de implementación y gestión de proyectos.

La arquitectura del proyecto, con su clara separación entre el parser y el motor de renderizado, es un ejemplo fundamental de cómo la modularidad y la división de responsabilidades son esenciales para manejar la complejidad del software. Esta estructura, junto con la estricta adherencia a la Norminette, no solo impone un estilo de codificación, sino que también fuerza intrínsecamente la adopción de principios de diseño robustos, como la descomposición de funciones en unidades pequeñas y manejables. El cumplimiento de la Norminette, lejos de ser una simple formalidad, actúa como un catalizador para la creación de código legible, mantenible y testeable, habilidades críticas para cualquier desarrollador.

La definición de estructuras de datos es el esqueleto del ray tracer. La comprensión de que las matemáticas vectoriales son el lenguaje computacional de los gráficos 3D es primordial, ya que cada interacción geométrica y cálculo de iluminación se basa en operaciones vectoriales precisas. La utilización de listas enlazadas y uniones para gestionar la heterogeneidad de los elementos de la escena demuestra una aplicación práctica de las capacidades de bajo nivel de C para construir sistemas flexibles y extensibles. Además, la comprensión del parámetro `t` en las intersecciones como el árbitro de la visibilidad es fundamental para la precisión del renderizado.

El parser del archivo `.rt` es la puerta de entrada a la escena 3D, y su robustez, incluyendo la validación de datos y el manejo de errores, es tan crucial como el propio motor de renderizado. En cuanto al renderizado, la aplicación de la teoría matemática para la generación de rayos y las pruebas de intersección con esferas, planos y cilindros constituye el núcleo algorítmico. La implementación de modelos de iluminación como Phong, junto con la simulación de sombras, reflexiones y refracciones, es lo que confiere realismo a las imágenes, aunque con un costo computacional inherente.

La integración con MiniLibX es la interfaz visual del proyecto, permitiendo la creación de ventanas, la manipulación de píxeles y el manejo de eventos del usuario. Un flujo de ejecución general bien definido, desde la inicialización hasta el bucle de renderizado y eventos, asegura un programa funcional y reactivo. Finalmente, las prácticas de Git y la creación de un Makefile conforme a los estándares de 42 son habilidades de ingeniería de software indispensables que garantizan la colaboración efectiva y la gestión eficiente del ciclo de vida del proyecto.

El bonus de optimización con multithreading destaca la naturaleza computacionalmente intensiva del ray tracing y la necesidad de paralelización para mejorar el rendimiento. La aplicación de pthreads y el manejo de consideraciones como el balanceo de carga y las condiciones de carrera son un paso importante hacia la programación de alto rendimiento.

En síntesis, el proyecto `miniRT` es una experiencia de aprendizaje multifacética que desafía a los estudiantes a integrar conocimientos de matemáticas, física, algoritmos y diseño de software. Su correcta implementación no solo resulta en un renderizador funcional, sino que también cultiva una comprensión profunda de los principios fundamentales que sustentan los gráficos por computadora, preparando a los estudiantes para desafíos más avanzados en el campo.

Para un proyecto de ray tracing como MiniRT, necesitas estructuras de datos que representen los elementos clave de tu escena y el proceso de renderizado. Aquí te detallo las structs esenciales y una explicación de por qué son importantes:

-----

## 1\. Vectores y Puntos (Básicos pero Fundamentales)

Aunque a menudo no son "structs" en sí mismos en C (se pueden representar como arrays de `double` o `float`), es crucial tener funciones y una forma consistente de manejar las operaciones de vectores. Podrías definir algo así:

```c
typedef struct s_vec3
{
    double x;
    double y;
    double z;
} t_vec3;
```

**Por qué son importantes:**

  * **Posiciones:** Para cámaras, luces, centros de objetos, puntos de intersección.
  * **Direcciones:** Para los rayos, normales de superficie, direcciones de luz.
  * **Colores:** Los colores RGB también se pueden representar como vectores de 3 componentes (aunque normalmente de 0 a 1 o de 0 a 255).
  * **Operaciones:** Necesitarás funciones para suma, resta, producto escalar (dot product), producto vectorial (cross product), normalización, longitud, etc.

-----

## 2\. Rayo

El concepto central del ray tracing. Cada píxel en la pantalla se "dispara" con un rayo.

```c
typedef struct s_ray
{
    t_vec3 origin;    // Punto de origen del rayo (normalmente la cámara o el punto de rebote)
    t_vec3 direction; // Vector de dirección normalizado del rayo
} t_ray;
```

**Por qué es importante:**

  * Es la unidad fundamental para las intersecciones. Con su origen y dirección, puedes calcular cualquier punto a lo largo del rayo y verificar si intersecta con objetos en la escena.

-----

## 3\. Cámara

Define el punto de vista desde el que se renderiza la escena.

```c
typedef struct s_camera
{
    t_vec3 position;        // Posición de la cámara en el espacio 3D
    t_vec3 orientation;     // Vector de dirección normalizado hacia donde mira la cámara
    double fov;             // Campo de visión (Field of View) en grados
    // Puedes añadir más si quieres control avanzado, como 'up_vector'
} t_camera;
```

**Por qué es importante:**

  * A partir de la cámara, calculas los rayos primarios que pasan por cada píxel de la "pantalla virtual". El FOV es crucial para determinar la "perspectiva" de la imagen.

-----

## 4\. Luz

Representa las fuentes de luz en la escena.

```c
typedef struct s_light
{
    t_vec3 position;    // Posición de la fuente de luz
    double brightness;  // Intensidad de la luz (0.0 a 1.0)
    t_vec3 color;       // Color de la luz (RGB)
} t_light;

typedef struct s_ambient_light
{
    double ratio;       // Ratio de luz ambiental (0.0 a 1.0)
    t_vec3 color;       // Color de la luz ambiental
} t_ambient_light;
```

**Por qué son importantes:**

  * La luz es fundamental para el sombreado y el color final de los píxeles. La luz ambiental (ambient light) proporciona una iluminación base a toda la escena, mientras que las luces puntuales (point lights) simulan fuentes de luz directas.

-----

## 5\. Objetos Geométricos

Necesitas estructuras para cada tipo de objeto que tu MiniRT soportará (esferas, planos, cilindros, etc.). Todos deberían compartir propiedades comunes y tener sus propiedades específicas.

### 5.1 Propiedades Comunes de Objetos

Es útil tener una enumeración para los tipos de objetos y una struct genérica que contenga las propiedades que todos los objetos tienen en común.

```c
typedef enum e_object_type
{
    SPHERE,
    PLANE,
    CYLINDER,
    // ... otros objetos que implementes
} t_object_type;

typedef struct s_object
{
    t_object_type type; // Tipo de objeto
    t_vec3 color;       // Color base del objeto (RGB)
    // t_material material; // Si implementas materiales más complejos (difuso, especular, etc.)
    void *data;         // Puntero a la struct específica del objeto (ej. t_sphere, t_plane)
} t_object;
```

**Por qué es importante:**

  * Permite manejar diferentes tipos de objetos de forma polimórfica (a través de un array de `t_object`), facilitando las comprobaciones de intersección y el renderizado.

### 5.2 Estructuras Específicas de Objetos

#### Esfera

```c
typedef struct s_sphere
{
    t_vec3 center;  // Centro de la esfera
    double radius;  // Radio de la esfera
} t_sphere;
```

#### Plano

```c
typedef struct s_plane
{
    t_vec3 position;      // Un punto en el plano
    t_vec3 normal;        // Vector normal del plano (normalizado)
} t_plane;
```

#### Cilindro (Ejemplo más complejo)

```c
typedef struct s_cylinder
{
    t_vec3 position;      // Centro de la base del cilindro
    t_vec3 axis;          // Vector de dirección del eje (normalizado)
    double radius;        // Radio del cilindro
    double height;        // Altura del cilindro
} t_cylinder;
```

**Por qué son importantes:**

  * Contienen los parámetros geométricos específicos necesarios para calcular las intersecciones del rayo con ese tipo particular de objeto.

-----

## 6\. Información de Intersección (Hit Record)

Cuando un rayo intersecta un objeto, necesitas almacenar los detalles de esa intersección para calcular el color.

```c
typedef struct s_hit_record
{
    t_vec3 point;         // Punto de intersección en el espacio 3D
    t_vec3 normal;        // Vector normal de la superficie en el punto de intersección (normalizado)
    double t;             // Distancia a lo largo del rayo hasta el punto de intersección
    t_object *object;     // Puntero al objeto que fue intersectado
    // Puedes añadir más, como las coordenadas UV para texturas si las implementas
} t_hit_record;
```

**Por qué es importante:**

  * Esta información es esencial para el sombreado. Con el punto de intersección y la normal de la superficie, puedes calcular cómo la luz interactúa con el objeto, incluyendo sombras, reflejos y refracciones.

-----

## 7\. Escena

Contiene todos los elementos que componen la escena a renderizar.

```c
typedef struct s_scene
{
    t_ambient_light ambient;  // Luz ambiental
    t_camera camera;          // Cámara principal
    t_list *lights;           // Lista de fuentes de luz (puedes usar una lista enlazada o un array dinámico)
    t_list *objects;          // Lista de objetos geométricos en la escena
    int width;                // Ancho de la imagen de salida
    int height;               // Alto de la imagen de salida
} t_scene;
```

**Por qué es importante:**

  * Es la representación completa del mundo 3D que vas a renderizar. La lectura del archivo `.rt` del MiniRT poblará esta estructura.

-----

## 8\. Materiales (Opcional, para un renderizado más avanzado)

Si quieres ir más allá de un color sólido y tener propiedades como reflectividad, especularidad, o refracción.

```c
typedef struct s_material
{
    t_vec3  albedo;       // Color difuso / base
    double  fuzz;         // Parámetro para la reflexión difusa (para efectos de "glossy" o borroso)
    double  ref_idx;      // Índice de refracción (para transparencia/vidrio)
    double  reflectivity; // Coeficiente de reflexión
    // ... otros parámetros como emisividad, etc.
} t_material;
```

**Por qué es importante:**

  * Permite crear objetos con apariencias mucho más realistas, simulando cómo la luz interactúa con diferentes superficies (mate, brillante, transparente, metálico).

-----

## Ejemplo de cómo se podrían usar:

1.  **Parsing:** Leer el archivo `.rt` y poblar la `t_scene` con las `t_camera`, `t_ambient_light`, y listas de `t_light` y `t_object`. Cada `t_object` tendría su `type` y un puntero a su struct específica (`t_sphere`, `t_plane`, etc.) a través de `data`.
2.  **Ray Generation:** Para cada píxel de la imagen final, calcular un `t_ray` desde la `t_camera`.
3.  **Intersection:** Para cada rayo, iterar a través de la lista de `t_object` en la `t_scene` y calcular la intersección. Si hay una intersección, almacenar los detalles en una `t_hit_record`. Siempre te quedas con la intersección más cercana al origen del rayo.
4.  **Shading:** Usar la `t_hit_record` (punto de intersección, normal de superficie, objeto) y las `t_light` de la `t_scene` para calcular el color final del píxel, aplicando modelos de iluminación como Phong o simplemente difusión y ambiente. Aquí es donde considerarías sombras (disparando "rayos de sombra" hacia las luces) y posiblemente reflejos/refracciones (disparando rayos secundarios).

Estas structs forman la columna vertebral de cualquier ray tracer básico. A medida que tu proyecto crezca y quieras añadir más funcionalidades (texturas, materiales complejos, aceleración espacial como BVH), necesitarás añadir más estructuras o expandir las existentes.

Claro, aquí tienes un ejemplo paso a paso de cómo se calcula el color de un solo píxel en un proyecto como MiniRT. Vamos a asumir una escena simple con una cámara, un plano y una fuente de luz puntual.

**Escena:**

* **Cámara:**
    * Posición: $(0, 0, 0)$
    * Orientación: Mira a lo largo del eje Z positivo.
* **Objeto:**
    * Un plano con la ecuación $z = -10$.
    * Color: Verde (RGB: $0, 255, 0$)
* **Fuente de Luz:**
    * Posición: $(0, 10, -5)$
    * Color: Blanco (RGB: $255, 255, 255$)
* **Pixel que vamos a calcular:**
    * Coordenadas en la pantalla (por ejemplo, en una ventana de 800x600): $(x, y) = (400, 300)$, que es el centro de la pantalla.

**Paso 1: Generar el Rayo**

El primer paso es crear un rayo que salga de la cámara y pase a través del centro del píxel $(400, 300)$.

* **Origen del Rayo ($O$):** La posición de la cámara, que es $(0, 0, 0)$.
* **Dirección del Rayo ($D$):** La dirección del rayo hacia el píxel. Como el píxel está en el centro de la pantalla y la cámara mira a lo largo del eje Z, la dirección del rayo será simplemente a lo largo del eje Z. En este caso, $D = (0, 0, 1)$.

La ecuación del rayo es:
$P(t) = O + tD = (0, 0, 0) + t(0, 0, 1) = (0, 0, t)$

**Paso 2: Intersección del Rayo con Objetos**

Ahora, necesitamos ver si este rayo choca con algún objeto en la escena. Empezamos con nuestro plano, cuya ecuación es $z = -10$.

* Sustituimos la ecuación del rayo en la del plano:
    * La coordenada $z$ del rayo es $t$.
    * La coordenada $z$ del plano es $-10$.
    * Igualamos ambas: $t = -10$.

* El valor de $t$ representa la "distancia" a lo largo del rayo hasta el punto de intersección. Un valor negativo de $t$ significa que la intersección ocurre "detrás" del origen del rayo (la cámara), por lo que no es visible. En este caso, el rayo se lanza en la dirección Z positiva, pero el plano está en Z negativo. No hay intersección visible.

* **¡Oops, cambiemos el plano para que sea visible!**
    * Nuevo plano: $z = 10$.
    * Hacemos el mismo cálculo: $t = 10$.
    * Como $t > 0$, hay una intersección válida. El punto de intersección es $P(10) = (0, 0, 10)$.

* **Calculamos la normal de la superficie:**
    * La normal de un plano $z = 10$ es un vector perpendicular a la superficie. En este caso, es $(0, 0, 1)$. La normal del plano se denota como $N = (0, 0, 1)$.

**Paso 3: Calcular la Iluminación (Shading)**

Ya sabemos que el rayo chocó con el plano verde en el punto $(0, 0, 10)$ y la normal de la superficie es $N = (0, 0, 1)$. Ahora calculamos el color final del píxel.

* **Rayo de Sombra:**
    * Primero, necesitamos ver si este punto está a la sombra de la fuente de luz.
    * Creamos un nuevo rayo de sombra que va desde el punto de intersección $P_{hit} = (0, 0, 10)$ hasta la fuente de luz $L_{pos} = (0, 10, -5)$.
    * **Vector de la luz ($L_{vec}$):** $L_{pos} - P_{hit} = (0, 10, -5) - (0, 0, 10) = (0, 10, -15)$.
    * Normalizamos este vector para obtener la dirección de la luz ($L_{dir}$): $L_{dir} = (0, 10, -15) / \text{longitud de } (0, 10, -15)$.
    * La longitud es $\sqrt{0^2 + 10^2 + (-15)^2} = \sqrt{100 + 225} = \sqrt{325} \approx 18.03$.
    * $L_{dir} \approx (0, 0.55, -0.83)$.
    * Ahora, lanzamos un rayo de sombra desde $P_{hit}$ en la dirección $L_{dir}$. Verificamos si este rayo choca con algún objeto antes de llegar a la fuente de luz. Como no hay otros objetos en nuestra escena simple, el punto no está a la sombra.

* **Cálculo de la Luz Difusa:**
    * La luz difusa es la luz que se refleja por igual en todas las direcciones. Su intensidad depende del ángulo entre la normal de la superficie ($N$) y la dirección de la luz ($L_{dir}$). Usamos el producto escalar para esto.
    * **Producto escalar:** $N \cdot L_{dir} = (0, 0, 1) \cdot (0, 0.55, -0.83) = (0 \times 0) + (0 \times 0.55) + (1 \times -0.83) = -0.83$.
    * ¡Un producto escalar negativo! Esto significa que la fuente de luz está "detrás" de la superficie del plano (con respecto a la normal). Esto es un error común. La normal del plano debe apuntar hacia la cámara, por lo que debería ser $(0, 0, -1)$.
    * **¡Corregimos la normal!** $N = (0, 0, -1)$.
    * **Nuevo producto escalar:** $N \cdot L_{dir} = (0, 0, -1) \cdot (0, 0.55, -0.83) = (0 \times 0) + (0 \times 0.55) + (-1 \times -0.83) = 0.83$.
    * El valor $0.83$ es la intensidad de la luz difusa (el "factor de atenuación"). Lo multiplicamos por el color de la luz y el color del objeto para obtener el color final.

* **Color final:**
    * El color del plano es verde: $(0, 255, 0)$.
    * El color de la luz es blanco: $(255, 255, 255)$.
    * **Intensidad final de color:** Color del Objeto $\times$ Intensidad de la Luz $\times$ Factor de Atenuación.
    * **Por canal:**
        * Rojo: $(0 / 255) \times (255 / 255) \times 0.83 = 0$
        * Verde: $(255 / 255) \times (255 / 255) \times 0.83 = 0.83 \times 255 \approx 211$
        * Azul: $(0 / 255) \times (255 / 255) \times 0.83 = 0$
    * El color final del píxel es un verde oscuro: **(0, 211, 0)**.

**Resumen del Flujo de Trabajo para un Solo Píxel:**

1.  **Generar el rayo de la cámara:** Calcular el origen y la dirección del rayo para el píxel actual.
2.  **Buscar la intersección más cercana:** Iterar a través de todos los objetos de la escena. Para cada objeto, resolver la ecuación de intersección (e.g., cuadrática para una esfera, lineal para un plano). Guardar el punto de intersección más cercano a la cámara.
3.  **Calcular el sombreado:** Si se encontró una intersección:
    * Lanzar un rayo de sombra hacia cada fuente de luz para determinar si el punto está a la sombra.
    * Calcular la intensidad de la luz (con el producto escalar) para la luz difusa y especular.
    * Multiplicar los colores del objeto y la luz por la intensidad calculada.
4.  **Asignar el color al píxel:** El color final del píxel es el resultado de la iluminación. Si no se encontró ninguna intersección, el píxel se colorea con el color de fondo.

Este proceso, que parece largo para un solo píxel, se repite millones de veces para cada píxel de la pantalla, lo que genera la imagen completa.

bibligrafia:
[text](https://circuitoelectrico.com/calculo-iluminacion/graficos-diagramas-iluminacion/)
