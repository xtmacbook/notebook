
对domain是三角形的：
//                         v1
//                         /\
//                        /  \
//                  edge0/    \edge2
//                      /      \
//                     /        \
//                    /          \
//                  v2------------ v0
//                          edge1                              

gl_TessLevelOuter[] roughly determines the number of segments on each edge 
gl_TessLevelInner[0] roughly determines how many rings the triangle will contain(just for the domain  is triangle)
![Image text](http://in2gpu.com/wp-content/uploads/2014/06/tess12.png)

对与quads
in0   = horizontal tessellation
in1   = vertical tessellation
out0 = edge 0-3 
out1 = edge 2-3
out2 = edge 1-2 
out3 = edge 0-1):

![Image text](http://in2gpu.com/wp-content/uploads/2014/07/tess.png)
