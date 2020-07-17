Although a number of techniques have been proposed for rendering terrains, we classify them into **three categories**: **hierarchical** algorithms, that recursively subdivide the
heightmap using a common data structure; **triangular irregular meshes**, where the triangles
can be of any shape and size to give the most faithful representation of the
terrain; and **GPU-based approaches**, that cache vertices or triangles on the graphics
card, so they can be rendered efficiently.

&emsp;&emsp;Hierarchical terrain-rendering algorithms recursively subdivide the heightmap into a
primitive shape, resulting in a hierarchy.

&emsp;&emsp;Triangular Irregular Networks (TINs) represent the terrain as a number of triangles
of different shapes and sizes.

&emsp;&emsp;In the late nineties, as more graphics cards with more powerful processors were released,previous algorithms were adapted to render more triangles.
