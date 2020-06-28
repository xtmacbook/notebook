## What's Noise
&emsp;&emsp;Noise is a mapping from R n to R - you input an n-dimensional point with real coordinates, and it returns
a real value. Currently the most common uses are for n=1, n=2, and n=3. The first is used for animation,
the second for cheap texture hacks, and the third for less-cheap texture hacks. Noise over R
4 is also very useful for time-varying solid textures, as I'll show later.  
&emsp;&emsp;Noise is band-limited - almost all of its energy (when looked at as a signal) is concentrated in a small part
of the frequency spectrum. High frequencies (visually small details) and low frequencies (large shapes)
contribute very little energy. Its appearance is similar to what you'd get if you took a big block of random
values and blurred it (ie: convolved with a gaussian kernel). Although that would be quite expensive to
compute.

##Simplex Noise  

![Simplex noise](https://github.com/xtmacbook/notebook/tree/master/images/simplx_noise.png)  
Given a point (x,y) (expressed in normal rectangular coordinates), we first transform the coordinates into (u,v) expressed in the simplex grid. Then we take the integer parts of u and v to find the corners of the containing equilateral triangle, and take the random values assigned to these corners. The "noise value" of the original point (x,y) is then some suitable interpolation of these values with respect to the distance from (x,y) to each corner.  
The implementation of this algorithm is explained in detail in several places. The code to transform into and back out of the simplex grid might look like this:  

```
final double F2 = 0.5*(Math.sqrt(3.0)-1.0);
double s = (xin+yin)*F2;
int u = fastfloor(xin+s);
int v = fastfloor(yin+s);
final double G2 = -(3.0-Math.sqrt(3.0))/6.0;
double t = (u+v)*G2;
double X0 = u+t;
double Y0 = v+t;

```  
So the question is, where do these funny factors F2 and G2 come from?
(https://kristiannielsen.livejournal.com/18962.html)
