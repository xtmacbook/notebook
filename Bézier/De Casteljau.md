[Bézier Curves ](https://www.scratchapixel.com/lessons/advanced-rendering/bezier-curve-rendering-utah-teapot/bezier-curve) :+1:
***
# Bezier Curves

&emsp;&emsp;In short, the algorithm to evaluate a Bezier curve of any order N is to just linearly interpolate between two curves of degree N-1. Below are some examples to help show some details.<br>
The simplest version of a Bezier curve is a linear curve, which has a degree of 1. It is just a linear interpolation between two points A and B at time t, where t is a value from 0 to 1. When t has a value of 0, you will get point A. When t has a value of 1, you will get point B. For values of t between 0 and 1, you will get points along the line between A and B.<br>

The equation for this is super simple and something you’ve probably seen before: P(t) = A*(1-t) + B*t.<br>

## quadratic curve
&emsp;&emsp;The next simplest version of a Bezier curve is a quadratic curve, which has a degree of 2 and control points A,B,C. A quadratic curve is just a linear interpolation between two curves of degree 1 (aka linear curves). Specifically, you take a linear interpolation between A,B, and a linear interpolation between B,C, and then take a linear interpolation between those two results. That will give you your quadratic curve<br>
![Image text](https://demofox2.files.wordpress.com/2015/07/bezier2.gif?w=800)<br>

## cube curve
&emsp;&emsp;The next version is a cubic curve which has a degree of 3 and control points A,B,C,D. A cubic curve is just a linear interpolation between two quadratic curves. Specifically, the first quadratic curve is defined by control points A,B,C and the second quadratic curve is defined by control points B,C,D <br>
![Image text](https://demofox2.files.wordpress.com/2015/07/bezier3.gif?w=800)

## quartic  curve
&emsp;&emsp;The next version is a quartic curve, which has a degree of 4 and control points A,B,C,D,E. A quartic curve is just a linear interpolation between two cubic curves. The first cubic curve is defined by control points A,B,C,D and the second cubic curve is defined by control points B,C,D,E.<br>
![Image text](https://demofox2.files.wordpress.com/2015/07/bezier4.gif?w=800)<br>

![Image text](beziercurve.png)<br>
&emsp;&emsp;Like this pitchure a Bézier curve we only need 4 points. These point are control points defined in 3D space. As with surfaces, the curve itself doesn't exist until we compute it by combining these 4 points weighted by some ***coefficients***. **Parametric curves** are curves which are defined by an equation.<br>
 
![Image text](untitled.png)<br>
&emsp;&emsp;We mentioned before that shape of the curve was the result of the combining the control points weighted by some value (equation 1):<br>
![Image text](equation1.png)<br>
&emsp;&emsp;where P1, P2, P3, P4 are the Bézier control points and **k1,k2, k3, k4**are coefficients (scalar) weighting the contribution of the control points. Looking at figure 3, intuitively you can see that when t= 0, the first point on the curve coincides with the control point P1 (for t= 1, the end point coincides with P4). In other words you can write that:<br>
![Image text](equation2.png)<br>
Now the parameter **t** appears nowhere in these equations. The value of t is actually used to compute the coefficients k1
k1, k2, k3, k4 themselves with the following set of equations:<br>
&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;**k1(t)=(1−t)∗(1−t)∗(1−t)**  
&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;**k2(t)=3(1−t)2∗t**  
&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;**k3(t)=3(1−t)∗t2k4(t)=t3** <br>
We can re-write the equation we use above to compute P (equation 1) in a more formal way. You can express it as a sum of control points multiplied by some coefficients:<br>
curve(t)=∑i=0nbi,n(t)Pi,t∈[0,1]<br>
The coefficients Bi,n are polynomials ("a polynomial is an expression of finite length constructed from variables and constants, using only the operations of addition,
 subtraction, multiplication, and non-negative integer exponents") known as the ** Bernstein polynomials**. 
As you can see from this equation, there are n + 1 coefficients and control points involved in the sum (the sum starts with i = 0 and finishes with i = n, <br>
therefore if n = 3, we have i = 0, 1, 2, 3, that is 4 coefficients).<br>
It is actually possible to change the value of n (remember that the degree of a polynomial is the largest degree of any one term in this polynomial).<br>
 Using the Bernstein polynomials, when n = 0, computing P is equivalent to a linear interpolation:<br>
 P=(1−t)∗P0+t∗P1<br>
 When n = 2, we say that the Bézier curve is **quadratic** (t or (1−t) is raised to the power of 2) and P can be computed with the following equation:<br>
 P=(1−t)2∗P0+2(1−t)t∗P1+t2∗P2<br>
 When n = 3 (equation 2), we say that the Bézier curve is **cubic** (t or (1−t) is raised to the power of 3).<br>

 ## Bézier Basis Matrix
 &emsp;&emsp;It is possible to develop the four Bézier basis functions used to compute the coefficients:<br>
 k1(t)=−1t3+3t2−3t+1&emsp;&emsp;k2(t)=3t3−6t2+3t+0&emsp;&emsp;k3(t)=−3t3+3t2+0t+0&emsp;&emsp;k4(t)=1t3+0t2+0t+0 <br>
 Finally we can extract the constants a, b, c, d from these equations and write them as a 4x4 matrix:<br>
 which we call the Bézier basis matrix. The main idea behind this notation is that the parametric equations can be expressed in the following compact form:<br>
 
# The De Casteljau Algorithm for Evaluating Bezier Curves <br>
The principle of the algorithm relies on computing intermediate positions on each one of the three segments defined by the four control points by linearly interpolating the control points for a given t. The result of this process are three points which can themselves be connected to each other to form two line segments. We repeat the linear interpolation on these two line segments from which we get two new positions which again we can interpolate. The final point we get from this process lies on the curve at t (this process is illustrated in figure 7). Here is the pseudocode implementing this algorithm:<br>
![Image Text](feature7.png)
```c
point decasteljau(point P1, point P2, point P3, point P4, float t) 
{ 
  // compute first tree points along main segments P1-P2, P2-P3 and P3-P4
  point P12 = (1 - t) * P1 + t * P2; 
  point P23 = (1 - t) * P2 + t * P3; 
  point P34 = (1 - t) * P3 + t * P4; 
  // compute two points along segments P1P2-P2P3 and P2P3-P3P4
  point P1223 = (1 - t) * P12 + t * P23; 
  point P2334 = (1 - t) * P23 + t * P34; 

  // finally compute P
  return (1 - t) * P1223 + t * P2334; // P 
} 
```
## Properties of Bézier Curves
Bézier curves have some interesting properties. We already know that the first and end point of the curve coincide with the first and fourth control point. The line P1-P2 and P3-P4 are also tangent to the first and end point of the curve. It results from this property that the transition between two Bézier curves is invisible if the line P3-P4 from the first curve is aligned (they are collinear) with the line P1-P2 from the second curve (see figure 8). This property can be used to either extend an existing Bezier curve (by joining several curves together) or splitting an existing curve in two (see further down). Finally the **convex hull** (defined by the control points) of the Bézier polygon contains the Bézier curve (see figure 8).<br>
![Image text](Figure8.png)
## Connecting and Splitting Bezier Curves
Several Bézier curves can be connected to each other. A **0th order continuity** means that the curves are only joined (the last and first point of the first and second curve do join) but are not tangent. **1st order continuity** means that not only the points join but that the curves are also tangent at the intersection point. **2nd order continuity** is possible assuming more constraints on the alignments of the control points (but are more complex to implement particularly as the degree of the curve (the value of n) increases).

If two Bézier curves can be connected to each other with 1st order continuity (if the last two and first two control points of the first and second point are collinear) then it is also possible to divide a Bézier curve in two sub-curves (which are also Bézier curves). The technique to split the curve relies on the De Casteljau algorithm presented above. If you look at the final image of the animation in figure 7 (which we have reproduced in figure 10), it is intuitively possible to see that two sub-curves can be built from the points P1, P12, P1223 and P for the first sub-curve and P, P2334, P34 and P4 for the second sub-curve. Adapting the pseudocode implementing the De Casteljau method to generate the control points for these two sub-curves is straightforward (the function takes four point for the original input curve and two series of four points for the two generated curves):  <br>
```c
point splitBezier(point P[4], float t, point P1[4], point P2[4]) 
{ 
    // compute first tree points along main segments P1-P2, P2-P3 and P3-P4
    point P12 = (1 - t) * P[0] + t * P[1]; 
    point P23 = (1 - t) * P[1] + t * P[2]; 
    point P34 = (1 - t) * P[2] + t * P[3]; 
    // compute two points along segments P1P2-P2P3 and P2P3-P3P4
    point P1223 = (1 - t) * P12 + t * P23; 
    point P2334 = (1 - t) * P23 + t * P34; 
    // finally compute P
    point P = (1 - t) * P1223 + t * P2334; 
    P1[0] = P[0], P1[1] = P12, P1[2] = P1223, P1[3] = P; 
    P2[0] = P, P2[1] = P2334, P2[2] = P34, P3[3] = P4; 
} 
```
It can be particularly useful to recursively split a curve until the resulting sub-curves are considered to be small enough. If we chose to split the curve at t = 0.5 then we can optimise the De Casteljau algorithm slightly and write (substituting 0.5 for t in the previous code and arranging the terms):<br>
```c
point splitBezierOptimize(point P[4], point P1[4], point P2[4]) 
{ 
    P1[0] = P[0], 
    P1[1] = (P[0] + P[1]) / 2; 
    P1[2] = (P[0] + 2P[1] + P[2]) / 4; 
    P1[3] = P2[0] = (P[0] + 3(P[1] + P[2]) + P[3]) / 8; 
    P2[1] = (P[3] + 2P[2] + P[1]) / 4; 
    P2[2] = (P[2] + P[3]) / 2; 
    P2[3] = P[3]; 
} 
```
# Bézier Surfaces
Once we understand the principle of Bézier curve extending the same technique to Bézier surface is really straightforward. Rather than having 4 points we will define the surface with 16 points which you can see as a grid of 4x4 control points. In the case of curves we had 1 parameter to move along the curve (t). In the case of surface we will need two: one to move in the u direction and one to move in the v direction (see figure 1). Both u and v are contained within the range [0,1]. You can see the process of going from (u, v) to 3D space as a remapping of the unit square (defined by u, v) into a smooth continuous surface within the tree dimensional space. intuitively, you can see that a Bézier surface is made of many curves going along either the u or v direction. For instance, if we move in the v direction we need to interpolate the curves defined by the points 1, 2, 3, 4 and 5, 6, 7, 8 (and so on for the rest of the surface). If we move along the u direction, the curves defined by the points 1, 5, 9, 13 and 2, 6, 10, 14 need to be interpolated instead. More formally, a point on the Bézier surface depends of the parametric values u and v, and can be defined (equation 1) as a double sum of control points and coefficients ("a sum of Bézier curves"):

P(u,v)=∑i=0n∑j=0mBni(u)Bmj(v)Pij
P(u,v)=∑i=0n∑j=0mBin(u)Bjm(v)Pij
 
where as for curves,


Bni(u)=(ni)ui(1−u)n−i
Bin(u)=(ni)ui(1−u)n−i


is a Bernstein polynomial (equation 2), and

(ni)=n!i!(n−i)!
(ni)=n!i!(n−i)!
 
is a binomial coefficient (equation 3). We can see easily see the similarities with curves. If you are interested in the terminology, we say that a Bézier surface (or patch) is constructed as the tensor product of two Bézier curves. Note that we will only consider bicubic Bézier surface in this lesson, that is, surfaces for which n = 3 and m = 3. Therefore, the Bernstein polynomials (equation 4) are the same as with bicubic curves (we have four of them and they are the same for n and m. We just show the coefficients for u but the same equation are used for v):

K1(u)=(1−t)3K2(u)=3(1−t)2∗tK3(u)=3(1−t)∗t2K4(u)=t


