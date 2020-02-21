# Bézier triangle

![Bézier triangle](https://blog.demofox.org/2019/12/07/bezier-triangles/)

## Calculating Barycentric Coordinates
That explanation might have been a little thick. Lets look at lines and triangles as an example.<br>
First is lines:<br>
![Image text](https://demofox2.files.wordpress.com/2019/12/barycentriccoordinateslineb.png)
To calculate the barycentric coordinates (s,t) we first start with s.

The simplex opposite P10 is the A to P01 line segment, which has a length of 4. Dividing that by 5, we get a value of 0.8. So, the value of s is 0.8.

The value of t can be found the same way. The opposite line segment of P01 is P10 to A and has a length of 1, which we can divide by 5 to get 0.2. We could have also just calculated 1 – 0.8 since they have to add up to 1.0!

The Barycentric coordinates of point A on that line is (0.8, 0.2).

Calculating the Barycentric coordinates of a triangle is very similar. In the image below, point A has Barycentric coordinates of (0.5, 0.333, 0.166)<br>
![Image Text](https://demofox2.files.wordpress.com/2019/12/barycentriccoordinatestriangleb.png)

## Bézier triangle
A general nth-order Bézier triangle has (n + 1)(n + 2)/2 control points.
With linear order (n = 1), the resulting Bézier triangle is actually a regular flat triangle, with the triangle vertices equaling the three control points. A **quadratic** (n = 2) Bézier triangle features 6 control points which are all located on the edges. The **cubic** (n = 3) Bézier triangle is defined by 10 control points and is the lowest order Bézier triangle that has an internal control point, not located on the edges. In all cases, the edges of the triangle will be Bézier curves of the same degree.  
&emsp;&emsp;Much like how a **linear Bezier curve** is a linear interpolation (lerp) between two values, **a linear Bezier triangle** is a linear interpolation between three values. Each edge of the triangle is also a linear Bezier curve.  
&emsp;&emsp;A quadratic Bezier curve is made by linearly interpolating between two linear Bezier curves. A **quadratic Bezier triangle** is made by linearly interpolating between three linear Bezier triangles.  
### quadratic Bezier triangle  (二次)
&emsp;&emsp;A quadratic Bezier triangle is made by linearly interpolating between three linear Bezier triangles.How does this work on a Bezier triangle? Each edge of the triangle in a quadratic Bezier curve which gives us control points like the below:<br>
![Image text](https://demofox2.files.wordpress.com/2019/12/quadraticbeziertriangle2dc.png)  
> 
> - What we do first is use point P’s Barycentric coordinates to interpolate the values of the red triangle: 002, 011, 101 to get point A. You treat those values as if they are at the corners of the larger triangle (if you think about it, this is how quadratic Bezier curves work too with the time parameter. Each line segment has t=0 at the start and t=1 at the end).  
> - We next interpolate the values of the green triangle: 011, 020, 110 to get point B.  
> - We then interpolate the values of the blue triangle: 101, 110, 200 to get the point C.  
> - Now we can do a regular linear barycentric interpolation between those three values, using the same barycentric coordinates, and get a quadratic Bezier triangle. Each edge of the triangle is also a quadratic Bezier Curve.  
> 

## cubic Bezier triangle
&emsp;&emsp;For a cubic Bezier triangle, it’s much the same. You evaluate a quadratic Bezier for each corner, and then linearly interpolate between them to get a cubic. Below is an image showing the control points used to create the three quadratic Bezier triangles.  
&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;![Image text](https://demofox2.files.wordpress.com/2019/12/cubicbeziertriangle2dr.png)<br>
&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;![Image text](https://demofox2.files.wordpress.com/2019/12/cubicbeziertriangle2dg.png)<br>
&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;![Image text](https://demofox2.files.wordpress.com/2019/12/cubicbeziertriangle2db.png)



