<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>MiniRT Project README</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            line-height: 1.6;
            margin: 2em;
            background-color: #f4f4f4;
            color: #333;
        }
        .container {
            max-width: 900px;
            margin: auto;
            background: #fff;
            padding: 2em;
            border-radius: 8px;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
        }
        h1, h2, h3 {
            color: #2c3e50;
        }
        code, pre {
            background: #ecf0f1;
            padding: 2px 5px;
            border-radius: 4px;
            color: #c0392b;
            font-family: 'Courier New', Courier, monospace;
        }
        pre {
            padding: 1em;
            overflow-x: auto;
        }
        .section-separator {
            border-top: 1px solid #ddd;
            margin: 2em 0;
        }
        ul {
            list-style-type: none;
            padding: 0;
        }
        ul li:before {
            content: "•";
            color: #3498db;
            font-weight: bold;
            display: inline-block;
            width: 1em;
            margin-left: -1em;
        }
        .project-image {
            width: 100%;
            max-width: 600px;
            height: auto;
            margin: 1em 0;
            border: 1px solid #ddd;
            border-radius: 4px;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>MiniRT Project: A Ray-Tracing Engine</h1>
        <p>
            Welcome to the <strong>mini rt</strong> project. This is a compact but powerful ray-tracing engine built from scratch in C, designed to render realistic 3D scenes by simulating the path of light. The project handles geometric shapes, lighting, and advanced rendering techniques.
        </p>
        <div class="section-separator"></div>

        <h2>Project Structure and Core Concepts</h2>
        <p>
            The engine is built on a modular design, with different components handling specific aspects of the rendering process. Key concepts include:
        </p>
        <ul>
            <li><strong>Vectors and Geometry:</strong> The project relies on a robust set of vector math functions (<code>vec3_init</code>, <code>vec3_add</code>, <code>vec3_mul</code>, etc.) for all geometric calculations. Objects like spheres, planes, and cylinders are defined by their position, orientation, and other properties.</li>
            <li><strong>Ray Tracing:</strong> The core principle is to cast rays from the camera's position through each pixel of the virtual screen and determine the color of the closest object hit.</li>
            [cite_start]<li><strong>Scene Management:</strong> The code includes functions to manage the scene dynamically, allowing for the addition and freeing of objects and lights[cite: 248, 251, 252].</li>
        </ul>
        <div class="section-separator"></div>

        <h2>Mathematical Functions and Formulas (From My Perspective)</h2>
        <p>
            From a developer's standpoint, a project like mini rt is a fascinating exercise in applied mathematics. Here’s a breakdown of how key mathematical concepts are applied in the code:
        </p>
        <ul>
            <li>
                <strong>Using the <code>math</code> library (<code>atan2</code>, <code>M_PI</code>, etc.):</strong> The code uses functions like <code>atan2</code> and constants like <code>M_PI</code> (Pi) to perform complex calculations, specifically in texture mapping. For example, in the <code>get_uv_sphere</code> function, these are used to map a 3D point on a sphere's surface to a 2D coordinate for texture application. [cite_start]This involves converting a point in Cartesian coordinates to spherical coordinates to get the correct texture UVs. [cite: 329]
            </li>
            <li>
                <strong>Intersection Formulas:</strong> While the main intersection logic is not fully visible in the provided snippets, the structure implies that functions like <code>intersect</code> would contain fundamental formulas from linear algebra and geometry. These formulas solve for the intersection point between a ray and a geometric primitive (e.g., sphere, plane, etc.). This involves solving quadratic equations for spheres or plane equations for planes.
            </li>
            <li>
                [cite_start]<strong>Vector Normalization:</strong> A critical concept seen throughout the code (e.g., in <code>ray_init</code>, <code>camera_init</code>, <code>plane_init</code>, etc.) is vector normalization[cite: 503, 504, 513, 514, 516, 518]. A normalized vector (a vector with a length of 1) simplifies calculations for direction and lighting, as it's easier to compute dot products and other operations without being affected by the magnitude of the vector.
            </li>
            <li>
                <strong>Dot Product (<code>vec3_dot</code>):</strong> The dot product is an essential tool for lighting and shadows. It determines the angle between two vectors. A positive dot product means the vectors point in a similar direction (e.g., a surface facing a light source), while a negative one means they are facing away.
            </li>
        </ul>
        <div class="section-separator"></div>

        <h2>Quality Improvements and Optimizations</h2>
        <p>
            For a high-quality rendering, performance and visual fidelity are key. The code hints at several techniques:
        </p>
        <ul>
            <li>
                [cite_start]<strong>Parallel Processing:</strong> The <code>get_num_processors</code> function [cite: 361] suggests the use of multiple threads (Pthreads) to parallelize the ray-tracing process. By distributing the work of rendering the image across multiple CPU cores, the engine can achieve faster render times, especially for complex scenes.
            </li>
            <li>
                <strong>Subpixel Anti-Aliasing:</strong> The user requested an explanation of this, but the provided code does not explicitly show subpixel sampling functions. This technique, also known as supersampling, involves casting multiple rays per pixel and averaging the results to smooth out jagged edges. While not directly visible in the code snippets, this would be a crucial feature for improving the visual quality of the final image.
            </li>
        </ul>
        <div class="section-separator"></div>

        <h2>Scene Preview</h2>
        <p>
            Here is a preview of a scene rendered with the <strong>mini rt</strong> engine.
        </p>
        <img src="https://view.example.com/your_image.png" alt="MiniRT Rendered Scene" class="project-image">
        <p>
            <em>Please replace the image link above with the actual URL of your rendered scene.</em>
        </p>
    </div>
</body>
</html>