# The De Casteljau Algorithm for Evaluating Bezier Curves
In short, the algorithm to evaluate a Bezier curve of any order N is to just linearly interpolate between two curves of degree N-1. Below are some examples to help show some details.<br>
The simplest version of a Bezier curve is a linear curve, which has a degree of 1. It is just a linear interpolation between two points A and B at time t, where t is a value from 0 to 1. When t has a value of 0, you will get point A. When t has a value of 1, you will get point B. For values of t between 0 and 1, you will get points along the line between A and B.<br>

The equation for this is super simple and something you’ve probably seen before: P(t) = A*(1-t) + B*t.<br>

# quadratic curve
The next simplest version of a Bezier curve is a quadratic curve, which has a degree of 2 and control points A,B,C. A quadratic curve is just a linear interpolation between two curves of degree 1 (aka linear curves). Specifically, you take a linear interpolation between A,B, and a linear interpolation between B,C, and then take a linear interpolation between those two results. That will give you your quadratic curve.
![Image text](https://demofox2.files.wordpress.com/2015/07/bezier2.gif?w=800)

# cube curve
The next version is a cubic curve which has a degree of 3 and control points A,B,C,D. A cubic curve is just a linear interpolation between two quadratic curves. Specifically, the first quadratic curve is defined by control points A,B,C and the second quadratic curve is defined by control points B,C,D <br>
![Image text](https://demofox2.files.wordpress.com/2015/07/bezier3.gif?w=800)

# quartic  curve
The next version is a quartic curve, which has a degree of 4 and control points A,B,C,D,E. A quartic curve is just a linear interpolation between two cubic curves. The first cubic curve is defined by control points A,B,C,D and the second cubic curve is defined by control points B,C,D,E.
![Image text](https://demofox2.files.wordpress.com/2015/07/bezier4.gif?w=800)
