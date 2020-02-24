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
&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;![Image text](https://demofox2.files.wordpress.com/2019/12/cubicbeziertriangle2db.png)<br>

On each triangle of the given mesh, we construct a cubic Bézier triangle **b(u,v,w)**of the following form:  

 
<math display="block" xmlns="http://www.w3.org/1998/Math/MathML">
  <mtable displaystyle="true">
    <mtr>
      <mtd columnalign="right">
        <mrow>
          <mspace width="-14.22636pt" />
          <mi mathvariant="bold">b</mi>
          <mo>(</mo>
          <mi>u</mi>
          <mo>,</mo>
          <mi>v</mi>
          <mo>,</mo>
          <mi>w</mi>
          <mo>)</mo>
          <mspace width="-0.166667em" />
          <mspace width="-0.166667em" />
        </mrow>
      </mtd>
      <mtd>
        <mrow>
          <mspace width="-0.166667em" />
          <mspace width="-0.166667em" />
          <mo>=</mo>
          <mspace width="-0.166667em" />
          <mspace width="-0.166667em" />
        </mrow>
      </mtd>
      <mtd columnalign="left">
        <mrow>
          <mspace width="-0.166667em" />
          <mspace width="-0.166667em" />
          <munder>
            <mo>&#x2211;</mo>
            <mrow>
              <mi>i</mi>
              <mo>+</mo>
              <mi>j</mi>
              <mo>+</mo>
              <mi>k</mi>
              <mo>=</mo>
              <mn>3</mn>
            </mrow>
          </munder>
          <mspace width="-0.166667em" />
          <mspace width="-0.166667em" />
          <mspace width="-0.166667em" />
          <msub>
            <mi mathvariant="bold">b</mi>
            <mrow>
              <mi>i</mi>
              <mi>j</mi>
              <mi>k</mi>
            </mrow>
          </msub>
          <mfrac>
            <mrow>
              <mn>3</mn>
              <mo>!</mo>
            </mrow>
            <mrow>
              <mi>i</mi>
              <mo>!</mo>
              <mi>j</mi>
              <mo>!</mo>
              <mi>k</mi>
              <mo>!</mo>
            </mrow>
          </mfrac>
          <msup>
            <mi>u</mi>
            <mi>i</mi>
          </msup>
          <msup>
            <mi>v</mi>
            <mi>j</mi>
          </msup>
          <msup>
            <mi>w</mi>
            <mi>k</mi>
          </msup>
          <mo>,</mo>
          <mspace width="3.33333pt" />
          <mi>u</mi>
          <mo>+</mo>
          <mi>v</mi>
          <mo>+</mo>
          <mi>w</mi>
          <mo>=</mo>
          <mn>1</mn>
          <mo>,</mo>
        </mrow>
      </mtd>
    </mtr>
  </mtable>
</math>  

where (u,v,w)are barycentric coordinates in a triangular domain and bijk are control-points  
![Image text](https://www.mdpi.com/symmetry/symmetry-08-00013/article_deploy/html/images/symmetry-08-00013-g002.png)  

&emsp;&emsp;&emsp;&emsp;Figure 2. (a) A cubic Bézier triangle; (b) Constructed on a triangle. <br>
The control-points of a cubic Bézier triangle are determined from its vertices **p1,p2**and **p3**, and the corresponding normals **n1,n2** and **n3** as follows:  
>>
>- b 300 = p 1 ,
>- b 030 = p 2 ,
>- b 003 = p 3 
>- b 210 = ( 2 p 1 + p 2 − w 12 n 1 ) / 3 
>- b 120 = ( 2 p 2 + p 1 − w 21 n 2 ) / 3 
>- b 021 = ( 2 p 2 + p 3 − w 23 n 2 ) / 3
>- b 012 = ( 2 p 3 + p 2 − w 32 n 3 ) / 3 
>- b 102 = ( 2 p 3 + p 1 − w 31 n 3 ) / 3 
>- b 201 = ( 2 p 1 + p 3 − w 13 n 1 ) / 3 
>- b 111 = E + ( E − V ) / 2 
>>  
![Image text](https://www.mdpi.com/symmetry/symmetry-08-00013/article_deploy/html/images/symmetry-08-00013-g003.png)  

where wij=(pj−pi)·ni,E=(b210+b120+b021+b012+b102+b201)/6 and V=(p1+p2+p3)/3. Figure 3b shows cubic Bézier triangles constructed from a triangular mesh in Figure 3a. We have already explained that this surface has **G1**continuity at triangle vertices, but only **G0** continuity across triangle edges.  

控制点并不是在三角形上，只是在控制新三角形的生成
-------------------------------------------
-------------------------------------------
## The Formula For Bezier Curves
Just like Bezier curves, you can come up with a multivariable polynomial to plug values into for Bezier Triangles, instead of using the De Casteljau algorithm. The De casteljau algorithm is more numerically stable, but the polynomial form is a lot more direct to calculate.

Let’s consider Bezier curves again first.

Lines (a 1-simplex) have two points, so we’ll describe control points as P_{xy}. If we want to make a degree N curve, our control points will be all the x,y pairs that add up to N. Here are the control points for the first few degrees of Bezier curves.  
1. Linear: P10, P01
2. Quadratic: P20, P11, P02
3. Cubic: P30, P21, P12, P03  
You might notice that a degree N curve has N+1 control points. Linear curves have 2 control points, quadratic curves have 3 control points, cubic curves have 4 control points and so on.

The index values on these control points tell you where they are on the line, and they also tell you the power of each barycentric coordinate they have, which we will get to in a minute.

The last part of making the formula is that the control point terms are also multiplied by the Nth row of Pascal’s triangle where row 0 is at the top row. 
![Image text](https://demofox2.files.wordpress.com/2019/12/pascalstriangle.png)<br>
Doing that, you have everything you need to create the familiar Bernstein basis polynomial form of Bezier curves.

The image below puts it all together. Orange is the name of the control point, which you can see also describes where it is on a line. The index values of the control point also describe the power of the Barycentric coordinates s and t which are in green. Lastly, the row of pascal’s triangle is in blue. To get the formula, you multiply each of those three things for each control point, and sum them up.

![Image text](https://demofox2.files.wordpress.com/2019/12/curvecontrolpoints.png)  

## The Formula For Bezier Triangles<br>
You can follow the same steps as the above for making the formula for Bezier triangles, but you need to use Pascal’s pyramid (aka trinomial coefficients) instead of Pascal’s triangle for the control points.<br>
This time, we have 3 index numbers on control points instead of the 2 that curves had, but we still find all permutations which add up to N.
![Image text](https://demofox2.files.wordpress.com/2019/12/pascalspyramid.png)<br>
Here are the control points for the first few degrees of Bezier triangles.
1. Linear: P100, P010, P001
2. Quadratic: P200, P110, P101, P020, P011, P002
3. Cubic: P300, P201, P210, P120, P111, P102, P030, P021, P012, P003 

Below are diagrams in the same style as the last section, which show the equation for a Bezier triangle and how to come up with it.

![Image text](https://demofox2.files.wordpress.com/2019/12/trianglecontrolpointslinear.png)  
![Image text](https://demofox2.files.wordpress.com/2019/12/trianglecontrolpointsquadratic.png)  
![Image text](https://demofox2.files.wordpress.com/2019/12/trianglecontrolpointscubic.png)  

 ## N-Patches ((shaded according to surface normal variation)
Given an input triangle mesh with normals at each vertex, the goal of the N-patches scheme by Vlachos et al. [2] is to construct a better looking surface on a triangle basis. The term "N-patches" is short for **Normal-Patches**, and these patches are also called PN triangles.
