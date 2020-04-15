# VAO VBO  
A VAO holds all of the state associated with each attribute array (enabled, buffer, offset, stride, format, divisor), and also the element (index) buffer:  
The state contained in a VAO consists of:  
1. For each attributes:  
	+ Whether it is enabled (glEnableVertexAttribArray/glDisableVertexAttribArray)
	+ The size, type, stride, normalised flag, integer flag, long flag, and offset (“pointer”) (glVertexAttribPointer)
	+ The buffer object containing the data (i.e. the buffer which was bound to GL_ARRAY_BUFFER at the time of the glVertexAttribPointer call).
	+ The attribute divisor (glVertexAttribDivisor)
	
2. The buffer currently bound to GL_ELEMENT_ARRAY_BUFFER  

**Note that the current GL_ARRAY_BUFFER binding isn’t stored in a VAO. You don’t need to bind a VAO before binding a buffer to that target or modifying the buffer’s data**

If you have multiple “objects” which all have the same shaders and thus use the same attributes, the sensible approach is to store all of the attributes and indices in a single set of buffers (one element buffer and at most one buffer per attribute), so you can draw all of the objects with a single glDrawElements() call, or arbitrary subsets using multiple glDrawElements() calls each using a subrange of the element array or a single glMultiDrawElements() call. All of that state can go in a single VAO.

If each object has its own set of buffers, then it also needs its own VAO, and will need a separate draw call.
