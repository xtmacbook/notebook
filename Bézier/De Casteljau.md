[Bézier Curves ](https://www.scratchapixel.com/lessons/advanced-rendering/bezier-curve-rendering-utah-teapot/bezier-curve) :+1:
***
# Bezier Curves
![Image text](beziercurve.png)<br>
&emsp;&emsp;Like this pitchure a Bézier curve we only need 4 points. These point are control points defined in 3D space. As with surfaces, the curve itself doesn't exist until we compute it by combining these 4 points weighted by some ***coefficients***. **Parametric curves** are curves which are defined by an equation.<br>
![Image text](untitled.png)<br>
&emsp;&emsp;We mentioned before that shape of the curve was the result of the combining the control points weighted by some value (equation 1):<br>
![Image text](equation1.png)<br>
&emsp;&emsp;where P1, P2, P3, P4 are the Bézier control points and **k1,k2, k3, k4**are coefficients (scalar) weighting the contribution of the control points. Looking at figure 3, intuitively you can see that when t= 0, the first point on the curve coincides with the control point P1 (for t= 1, the end point coincides with P4). In other words you can write that:<br>
![Image text](equation2.png)<br>

# The De Casteljau Algorithm for Evaluating Bezier Curves <br>
&emsp;&emsp;In short, the algorithm to evaluate a Bezier curve of any order N is to just linearly interpolate between two curves of degree N-1. Below are some examples to help show some details.<br>
The simplest version of a Bezier curve is a linear curve, which has a degree of 1. It is just a linear interpolation between two points A and B at time t, where t is a value from 0 to 1. When t has a value of 0, you will get point A. When t has a value of 1, you will get point B. For values of t between 0 and 1, you will get points along the line between A and B.<br>

The equation for this is super simple and something you’ve probably seen before: P(t) = A*(1-t) + B*t.<br>

# quadratic curve
&emsp;&emsp;The next simplest version of a Bezier curve is a quadratic curve, which has a degree of 2 and control points A,B,C. A quadratic curve is just a linear interpolation between two curves of degree 1 (aka linear curves). Specifically, you take a linear interpolation between A,B, and a linear interpolation between B,C, and then take a linear interpolation between those two results. That will give you your quadratic curve<br>
![Image text](https://demofox2.files.wordpress.com/2015/07/bezier2.gif?w=800)<br>

# cube curve
&emsp;&emsp;The next version is a cubic curve which has a degree of 3 and control points A,B,C,D. A cubic curve is just a linear interpolation between two quadratic curves. Specifically, the first quadratic curve is defined by control points A,B,C and the second quadratic curve is defined by control points B,C,D <br>
![Image text](https://demofox2.files.wordpress.com/2015/07/bezier3.gif?w=800)<br>

# quartic  curve
&emsp;&emsp;The next version is a quartic curve, which has a degree of 4 and control points A,B,C,D,E. A quartic curve is just a linear interpolation between two cubic curves. The first cubic curve is defined by control points A,B,C,D and the second cubic curve is defined by control points B,C,D,E.<br>
![Image text](https://demofox2.files.wordpress.com/2015/07/bezier4.gif?w=800)<br>
