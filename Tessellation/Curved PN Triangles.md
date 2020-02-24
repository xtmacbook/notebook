## gl_TessCoord :  
gl_TessCoord represents the same concept with all primitive topologies.

This vector is the position within the current primitive, and it is probably best ** not to think of it as x, y and z.** Since it is relative to a surface, u, v and w (optional) are more intuitive. You can call the coordinates whatever you want (e.g. xyz, rgb, stp or uvw) , mind you, but uvw is the canonical notation.  
> 
>> ## OpenGL 4.5 (Core Profile) - 11.2.2 Tessellation Primitive Generation - p. 392<br> 
>>Each vertex produced by the primitive generator has an associated (u, v, w) or (u, v) position in a normalized parameter space, with parameter values in the ** range [0, 1] **, as illustrated in figure 11.1.** For triangles **, the vertex position is a ** barycentric coordinate (u, v, w) **, where u + v + w = 1, and indicates the relative influence of the three vertices of the triangle on the position of the vertex.** For quads ** and isolines, the position is a (u, v) coordinate indicating the relative horizontal and vertical position of the vertex ** relative to the subdivided rectangle **
>
In the case of quads, each component measures the distance from two edges of a unit square. In the case of triangles, it is the distance from the three edges of a triangle (barycentric coordinates). This is why your example tessellation shader for quads uses only u and v, while the triangle example uses u, v and w.
