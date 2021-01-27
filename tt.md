It can be seen in Fig. 1 that the projection of the pixel into texture space may not be equal in both the s- and t-axis. When this is the case the mapping is said to be anisotropic. In the example shown, there is greater texel compression along the s-axis than along the t-axis.

It is the function of the texture filtering algorithm to resample the texture image to the projected scene as accurately as possible, accounting for this spatially varying minification. The simplest filter function, point sampling, makes no consideration of minification and simply uses the calculated texture address to find the nearest-neighbouring texel. For a minification ratio less than one (magnification) this implies a pixel replication method and results in a blocky image. For a minification ratio greater than one this results in aliasing artefacts and loss of information. Most real-time graphics hardware systems today utilise an isotropic filter kernel. These filters reduce aliasing effects at the expense of blurring the resulting image. In this paper we describe the hardware realisation of a filtering algorithm that adapts the filter kernel to the shape of a pixel's texture space projection — an anisotropic texture filter.

Section 2 of this paper provides a discussion on alternative texture filtering algorithms. This begins with a look at high-quality off-line space variant rendering filters, followed in Section 2.1 with the introduction of the notion of pre-filtering. Section 2.2 looks at possible practical real-time filters that provide a compromise between these two techniques. Section 3 provides a detailed description of the hardware implementation of such an anisotropic filter, designed as a low-cost solution for the adaptation of an existing rasterisation ASIC design. 4 Results, 5 Conclusions discuss the results achieved and the conclusions drawn from the work. Finally, in Section 6 we look at some possible extensions and future work.

2. Texture filtering
An accurate filter algorithm will include the contribution of all texels covered by the inverse mapped screen pixel, normally using a weighting function of some kind. In Fig. 1, this means the colours of each texel intersected by the shaded pixel pre-image contribute to the final colour of that pixel. The weighting will depend on the warped filter function used. The pre-image should be considered as this mapped filter function. Filters have been developed that either approximate this pre-image or directly model it. An example of the latter approach was suggested by Fournier and Fiume [9]. Theirs is perhaps the most accurate filter design to date. Parametric patches are used to model the warped filter surface function. Pre-convolution of the basis segments of each patch using a patch resolution pyramid (NIL-map) allows a constant cost solution. Even so the computational and memory cost to performance ratio of this filter is still too high for real-time hardware use, at least for the time being. A number of filters have been developed that approximate the filter kernel [10], [11], [12] and Heckbert [1] provides a good summary of their operation. He classified these filters as direct convolution filters in that they directly compute a weighted contribution of each texel mapped by a pixel. The key element to filter design and operation is in the understanding and control of the filter shape. The texels contributing to a pixel and the weighting applied to them depend completely upon this. While Fig. 1 shows the screen space pixel represented as a square, this is only an approximation that is convenient for practical implementation [13]. However, by weighting the individual elements of a square block of texture data, any filter shape from point, box, cone or Gaussian, can be approximated.

The filter suggested by Feibush et al. [11] operates by fitting a rectangular bounding rectangle to the pixel filter shape, which is then mapped to a quadrilateral in texture space. A further bounding rectangle is then applied to this with all the encapsulated texel samples mapped back into screen space where they are clipped to the pixel filter region and their contribution determined by a weighting function. Therefore, this filter uses uniform supersampling in texture space. The filter design by Gangnet et al. [12] uses a circular pixel representation that maps to an ellipse in texture space. The magnitude of the greater axis of this ellipse is used to determine a supersampling rate for the pixel in screen space. This is normally a square grid. Each of the samples in this grid are then mapped to texture space where a texture value is acquired for each by bilinear interpolation. These sample values can then be weighted by the desired filter function and combined to give the final texture value. Greene and Heckbert's [14] elliptical weighted average (EWA) filter also uses a circle representation. However, in this case the texture space ellipsoid parabolic calculation is used for both the texel inclusion testing and for indexing a look up table-based filter function. This filter therefore, in effect performs texture space scan conversion of an elliptic function.

Although the computation of the elliptical parabolic can be performed incrementally, generally the computational cost of elliptical operations are high [15]. These operations include finding the major and minor axes [12], [14] and determining a bounding rectangle for inclusion testing [14] or mapping [11]. Due to this high cost the ellipse is often approximated with a parallelogram [12], [16]. Fig. 2 shows this representation using a square pixel mapped into texture space.


Download : Download full-size image
Fig. 2. Parallelogram approximation of screen space to texture space projection.

The parallelogram is formed from the directional vectors r1 and r2 as shown in Fig. 2, where sx, tx, sy and ty are the partial derivatives of the perspectively corrected s and t address with respect to x and y. The calculation of these derivatives can be performed directly or incrementally [8], [17]. These derivatives provide information on the change in s and t for a unit change in x and y and hence give an indication of the amount of texture minification within a pixel. These derivatives are used in determining the supersampling rate in multisampling filters such as that by Gangnet et al. and can be used to provide information on the ratio and direction of anisotropy of the pixel projection in texture space.

2.1. Texture pre-filtering
By the nature of their operation, the costs of the approximating filters described so far, in terms of computational cost and memory access requirements, are arbitrary, dependent on the degree of texture space minification calculated. For this reason they have tended to be confined to off-line software rendering roles. Practical real-time hardware implementations require a low and, where possible, constant cost solution. To this end several pre-filtering techniques have been developed. Pre-filtering is used to band limit the texture image to the available resampling rate. Bilinear filtering and trilinear filtering using MIP-map image pyramids are perhaps the most commonly used of these methods today [4], [17], [18]. More information on MIP-map filtering is provided in Section 3.1.

The advantage of such MIP-map filtering techniques is that they provide a constant cost filter that effectively reduces aliasing. A disadvantage is that they require more texture storage, up to one and a third that of the original texture map. This is an important consideration especially in light of the increasing use of multiple texture maps in applications. In fact, due to this cost, many systems seldom store or use all the MIP-map levels. A further disadvantage is that these filters are isotropic in nature. With the filter by Gangnet et al. [12] the amount of texture minification was determined from the major axis of the ellipse (or in practise, that of the approximating parallelogram) to determine the supersampling rate. The filter shape was then implied through the mapping of each sample to texture space and the use of weighting. With trilinear filtering the minification is also found from the major axis, but this time it is used to determine the chosen pre-filtered level of detail. Filtering is then performed using either two or three linear interpolation passes of the eight neighbouring texels from the adjacent MIP-map levels. Therefore, the filter shape is isotropic in texture space. Although choosing the major axis ensures aliasing artefacts are effectively reduced, this can cause over filtering along the minor axis and hence blurring in the resulting image. Further details on methods for determining the texture minification ratio and MIP-map level selection can be found in [17].

Attempts have been made to develop pre-filtering schemes with some degree of anisotropy. Crow's summed area tables [19] and extension to 4D MIP-maps [15], allow mapping to axis aligned arbitrary rectangles. OpenGL models this approach with separate texture scaling in the s- and t-axis to reduce the aspect ratio of the pixel pre-image [20]. Improved results are returned, but the storage requirements are higher than Williams’ MIP-map pyramid [18]. Also textures skewed to 45° will still yield an inaccurate bounding box and a blurred result.

2.2. Hybrid filters
The seemingly ideal filter is one that accounts for the true anisotropy of the texture space mapping while utilising pre-filtering to enable a constant cost implementation. Therefore, the obvious progression is to use a hybrid of the above approaches. Heckbert suggested the implementation of the EWA filter with a MIP-map image pyramid [15]. Here the minor rather than the major axis of the projected ellipse (or parallelogram approximation), determines the MIP-map level on which the EWA filter is then applied. To further limit the arbitrary nature of the cost of this filter the eccentricity of the ellipse (the ratio of the major to minor axes) is clamped to a predetermined maximum limit. Where the MIP-map level is determined from the minor axis MinAxis:
(1)
An evaluation of the effectiveness of different clamping values can be found in [2]. Another combination is the use of MIP-maps with multisampling. The filter by Gangnet et al. uses supersampling for two purposes. First, the individual mapping of multiple samples to texture space allows the filter to adapt to the shape of the projected pixel. Secondly, the number of pixel supersamples are determined by the level of texture minification so that the filter gets a good coverage of the contributing texels. If MIP-maps are introduced then this latter role can be replaced by level selection and the number of samples can be a constant figure. This is now a constant cost implementation. For accurate results the bilinear filtering should now be replaced with trilinear. So now we have an N×N sub-pixel sample grid (with or without extended support and Gaussian weighting) mapped into texture space.

If we assume the minification ratio is constant within the pixel the mapping may look something like that shown in Fig. 3. In fact, the sub-pixel minification ratio is not constant, but continuously changing. However, the parallelogram representation is based on this assumption and it is convenient for simplified hardware implementation.


Download : Download full-size image
Fig. 3. Multisample projection into texture space.

The use of a square grid clearly limits the efficiency of the filter for mappings with a high anisotropic ratio or eccentricity. We would like the sample points to be more evenly spread in texture space along the projected x- and y-axis and to optimise the efficient use of the sample points that are currently being individually and hence expensively, mapped into texture space. The obvious option is to use a N×M grid whose dimensions are determined by the eccentricity of the mapping. Taken to the extreme, in conjunction with trilinear filtering we can use a N×1 grid, where the MIP-map level selection is based on the minification in the minor axis of the parallelogram and N is selected based on the eccentricity. We refer to such a design as a multiple-isotropic filter as it performs anisotropic filtering through multiple trilinear operations.

Schilling et al. [16] developed an algorithm of this form called footprint assembly. In their algorithm only the original pixel centre requires projection into texture space rather than all N points. The other sample points are calculated cheaply through linear traversal along the major axis of the parallelogram. This allows the filtering operation to be independent of the existing address generation. In their proposal this permitted the efficient use of a separate texturing ASIC. Finally, they also suggest that the maximum permissible value of N should be clamped to control the performance impact of the technique. It is clear from the previous discussion that this can be performed in the same way as the eccentricity clamping described for the EWA filter, demonstrated in Eq. (1). This method and its implications for the implementation of the filter are described in more detail in Section 3.2.5.

Fig. 4 illustrates the principle of this filtering operation. In the example shown the texture space mapping demonstrates an anisotropic ratio of approximately four to one.


Download : Download full-size image
Fig. 4. Multiple isotropic filtering.

A similar filter design has recently been adopted for the Talisman reference architecture [21], [22]. In that implementation they report the use of a trapezoidal reconstruction filter for anisotropic ratios greater than two to one. This means that the two end points are weighted differently to those points inside. However, this operation adds complexity to the averaging operation and we chose to omit it from our implementation.

The main advantage of such MIP-map based filters is that they are very similar to hardware already existing in many graphics accelerators. In the following section we describe how such a filter can be implemented as an extension to existing rasterisation hardware with little extra cost.

3. Anisotropic filter implementation
This section describes the detailed implementation of a practical multiple-isotropic anisotropic texture filter based on the discussion in the previous sections. The description provides further algorithmic and implementational details as well as silicon estimates for the filter's implementation as an extension to an existing rasterisation ASIC design.

3.1. Algorithm issues
As illustrated in Fig. 4, the footprint assembly [16] approach calculates N local filtering addresses along the line of anisotropy. Texture coordinates are subject to tiling modifications. These determine how texture coordinates are modified should it leave the bounds of the texture size. Typical tiling modes include WRAP, CLAMP and WRAP_INVERT, and their operation is self-explanatory. In the same way that the extra texel addresses generated for bilinear filtering are subject to these modifiers, so too are the local anisotropic sample points.

Fig. 5 illustrates the sampling on two adjacent MIP-map levels. In this diagram the lower detail level l+1 is shown out of scale with enlarged texels to illustrate clearly the relationship between the texels on adjacent levels. It can be seen that fewer unique texels are required on the lower detail level. In total 18 different texels must be accessed from texture memory. Due to the nature of this filtering operation there is a natural coherence in the texel requirement that will ensure the good performance of any texture caching scheme.


Download : Download full-size image
Fig. 5. Multiple MIP-map sampling on two levels.

At each of the tile mode adjusted sample positions the texture must be filtered. Using the parallelogram approximation for the mapping of a screen pixel in texture space, as shown in Fig. 2, the texture space minification is considered to be uniform across the area of the pixel. This means that the texture minification and hence the MIP-map level and inter-level interpolation fraction for each isotropic trilinear filtering operation are the same. This is shown in Fig. 6, which presents a three-dimensional view of the filtering process. Using this assumption, the cost of the operation can be further reduced, by using only a single inter-level interpolation. In this situation the colours for the sample points on each level are accumulated separately.


Download : Download full-size image
Fig. 6. 3D view of MIP-map operation.

If the anisotropic filter is an extension to an existing pipeline, then the choice between using N trilinear operations or 2N bilinear and a single inter-level stage will depend upon the existing trilinear hardware. Systems supporting single-cycle trilinear filtering generally have the texture memory organised into eight interleaved banks [16] to allow this operation. In such cases, individual trilinear operations will be more efficient. However, trilinear filtering is also often performed using two separate bilinear interpolations, using the same hardware in two cycles. With this latter arrangement the single inter-level stage method should be adopted.

3.2. Hardware implementation
Fig. 7 shows a schematic of the filtering unit for implementation within a projective renderer. This diagram assumes direct calculation of the homogeneous texture coordinates S, T and Q and the partial derivatives, sx, sy, tx and ty. For incremental interpolation the x and y inputs would be replaced by traversal control words.


Download : Download full-size image
Fig. 7. Anisotropic filter hardware units.

3.2.1. Generation of S, T and Q
As explained in Section 1 the texture address coordinates s and t are calculated by hyperbolic interpolation [3], [7], [8] of the projected address values at each vertex.
(2)
where S, T and Q are the linear expressions:
(3)
These expressions are calculated as functions of the plane of the triangle being rendered. Q is a function of the reciprocal of the depth of the object in the scene. These expressions can be solved by either direct algebraic solution for each pixel, or by incremental interpolation, in synchronisation with the primitive traversal of the scan conversion unit.

These calculations must be performed regardless of the filtering operation and are therefore a sunk cost in the choice of filtering algorithm.

3.2.2. Calculating sx sy tx and ty
These calculations are required in a trilinear filtering unit and as such are not considered additional costs of implementation for the anisotropic filtering unit.

Assuming texture coordinates are provided in the range 0–1, then these partial derivative terms must also be scaled to the physical dimensions of the texture to be used. sx and sy are multiplied by the texture width and tx and ty are multiplied by the texture height. Both texture width and height are of power of two magnitudes. Further details on the calculation of these partial derivatives can be found in [8], [17].

3.2.3. Level of detail determination
Again, this is an operation that must be performed in a trilinear filtering unit anyway. However, now the level selection is slightly different. The MIP-map level is selected based on the texture minification j of the minor axis of the approximating projection parallelogram, rather than the major axis. The minification ratio can therefore be estimated with
(4)
where r1, r2, d1 and d2 are the vectors that describe the projection parallelogram as shown in Fig. 8. Alternatively, the required level of detail can be determined from the major axis, as in trilinear filtering, only now scaled by the calculated eccentricity or anisotropic ratio. This is discussed further in Section 3.2.5.


Download : Download full-size image
Fig. 8. Projection parallelogram vector representation.

In our implementation the magnitudes of the vectors from Fig. 8 are approximated with
(5)
This approximation is based on the lower tolerance bound considered by the OpenGL specification [23]. While this provides an acceptable estimate to what is already an approximation, if required, the hypotenuse solution to these vectors can be approximated more closely using two part linear approximations of the kind suggested by McCormack et al. [24]. Fig. 14, Fig. 16 provide a comparison between the two methods and further comparisons can be found in [17].

Following the calculation of the texture minification j, the level of detail l and trilinear inter-level interpolation fraction f are now calculated with
(6)
For a fixed point system, because the levels refer to power of two increments, the level l can be found directly from the position of the most significant set bit (MSSB) in the binary word. The interpolation fraction, f, is then found from the remaining bits shifted right by l.

For a floating point system l can be found directly from the exponent of the floating point word. The significand then provides the trilinear fractional interpolant, f.

3.2.4. Computing the anisotropic ratio
The major axis of the parallelogram is determined as
(7)
The reason for this choice was based upon experimentation [25]. Fig. 9 illustrates different vector orientations.


Download : Download full-size image
Fig. 9. Choosing the major axis of anisotropy.

Fig. 9 shows that in cases (a) and (b) it is sensible to use the diagonals as the axes. This is emphasised in the eventuality of r1 and r2 being of equal magnitude, when there is a conflict as to which of the two should be used. However, in case (c) it can be argued that r2 is the correct choice and that the same conflict of choice now exists for d1 and d2. In the end both methods were implemented and no significant differences noted. It was therefore decided to choose the maximum of vectors r1 and r2 for three reasons. First, it is these vectors that are traditionally used in calculating the texture minification for trilinear filtering [8]. Therefore, this choice allows a smooth crossover from trilinear to anisotropic filtering. Secondly, because the larger of d1 and d2 will always be larger that the greater of r1 and r2, their use leads to, on average, a higher anisotropic ratio and higher filtering requirement. Thirdly, ratios of the form
(8)
as shown in Fig. 9(a) and (b), produce extra sample points at the extremities of the parallelogram where the actual area contribution is small. Filtering in this way would require a sample weighting of the kind suggested by Barkans [21]. Note that the result of Eq. (7) can be found as a by-product of the test in Eq. (4) and no separate comparison is needed.

The anisotropic ratio or eccentricity is therefore calculated with:
(9)
The anisotropic ratio determines the number of trilinear operations that are to be performed. The results of these are then averaged to find the final colour contribution. For ease of hardware implementation, Schilling et al. [16] suggested restricting this value to a power of two so that the final contribution can be found with a simple summation and shift operation. As explained in the following sections, this approximation also allows for other simplifications to the implementation and was therefore adopted in this implementation. However, it does introduce a limit to the extent to which N can be increased as explained in Section 3.2.5. Therefore, the number of trilinear operations N is found by rounding the anisotropic ratio to the nearest power of two.
(10)
The mechanism of this rounding will depend on the implementation and the number representation used. As with level extraction, for floating point the power of two is found direct from the exponent. Rounding is performed with the significand. Fixed point requires simple decoding logic to identify the bit position of the MSSB. All of the anisotropic example images provided, with the exception of Fig. 17, were generated using this approximation.

3.2.5. Anisotropic ratio clamping
To limit the number of trilinear operations the filter performs, a clamp value Nc is applied as shown in Eq. (1). If the anisotropic ratio Ar is greater than Nc then the calculation of N and minification j are performed with
(11)
This has the effect shown in Fig. 10. In this example an anisotropic ratio of approximately 9 is rounded to 8. Assuming the unit is clamped to Nc=4, then the minor axis, or j, is then effectively widened to adjust to this maximum limit.


Download : Download full-size image
Fig. 10. Anisotropic clamping operation.

In effect the major axis is used to determine texture minification in the same way as in a trilinear filter, but now scaled by the calculated anisotropic ratio N. As the algorithm performs a rounding to the nearest power of two to determine N, it might seem sensible to always use a scaled major axis value rather than the estimated minor axis from Eq. (4). In this way we can consider the filter as attempting to provide a good coverage of the pixel pre-image. However, scaling the minification figure in this way effectively quantises the minification range and with it the smooth variation of the trilinear inter-level interpolation fraction f. In fact both approaches are flawed as shown in Fig. 11.


Download : Download full-size image
Fig. 11. Alternative texture minification selection schemes: (a) using minor axis, (b) using major axis scaled to N.

Using the minor axis as shown in (a) will lead to aliasing when rounding down while a similar effect is seen in (b) for rounding up when using the scaled major axis. Using a combination of both, selecting the method based on the direction of rounding, might seem a reasonable solution to help reduce the introduction of aliasing. However, considering the case of an example where anisotropy increases and crosses the boundary from rounding down to rounding up, then this results in the transition from Fig. 11(1b) to (2a) and hence a fluctuating level selection. This will also have implications for the texture addressing coherency and it is therefore preferable to make a consistent choice between these two approaches. While neither method is perfect, they do provide substantial visual improvements over basic trilinear filtering at little extra cost. Fig. 18 provides an image example using Nc=4 without minification scaling being performed.

The requirement for rounding to a power of two value places a limit on the extent to which N can be increased before quality rather than performance becomes the deciding issue. This is because the error and aliasing effects shown in Fig. 11 increase with Nc. However, for low values of Nc, this approximation is tolerable. Fig. 17 provides a comparison for Nc=64 using N as any integer rather than limited to a power of two. In animation, for larger values of eccentricity then an integer based system is preferable.

3.2.6. Removing the divide
As explained, the number of MIP-map filter operations, N, is found as the power of two nearest to the calculated anisotropic ratio. We have further reduced the hardware cost by taking advantage of this approximation to remove the requirement for a full divider in calculating N. Eq. (10) can be approximated to a measured accuracy of 80% using the relationship:
(12)
Here an inner rounding operation is first performed on the denominator. By rounding the numerator and denominator separately, the divide is replaced with a cheaper operation. For a fixed point system this is a right shift and for a floating point system this is a small subtraction of the exponent. Using custom floating point logic the operation can be expressed as
(13)
where E is the exponent value and B is the bias. R is equal to 0 or 1 depending on whether the value requires rounding up or not. This is dependent upon the most significant bit of the significand. The subscripts n and d refer to the numerator and denominator, respectively.

Due to the fact that the rounding down of the denominator has a greater effect on the divide than rounding up, the accuracy can be increased to 85% by making the outer, shifted numerator rounding operation non-linear. This is achieved by detecting when the denominator is rounded down rather than up and in that situation, using two rounding bits instead of one.

3.2.7. Local texture address generation
The local  addresses of the N separate trilinear filter operations are now found. These are located at equal spacing along the major axis or “line of anisotropy” that passes through the projected pixel centre . Their location is based on the expression:
(14)
where  .

ds and dt are the orthogonal components of the major axis vector indicated by Eq. (7), scaled to the selected level of detail and by the eccentricity, N.
(15)
Where texscale=2l and l is the selected MIP-map level calculated in Eq. (6). For fixed point it is a right shift by l bits. Such scaling is required with trilinear filtering. Both the scale factor and N are always power of two magnitudes.

Fig. 12 shows the approach taken in the hardware to perform this line walking operation. The initial sp and tp coordinates are first subjected to an increment or decrement by ds/2 and dt/2, dependent upon the slope of the line of anisotropy. The remaining sn and tn sample positions are then found incrementally from this starting location.


Download : Download full-size image
Fig. 12. Local texture address generation.

This therefore requires a simple state machine and two interpolators. As explained in Section 3.1, the local addresses must be subject to the tiling mode in operation. This requires inexpensive wrap and clamp logic.

3.2.8. Colour accumulation
Finally, we consider the colour accumulation unit shown in Fig. 7. The function of this block will depend upon the method of MIP-map filtering used. If multiple trilinear operations are performed, then this requires just the number of samples N to be sent to it. The RGBA samples are summed together as they arrive at the accumulator, followed by a shift by N, when N values are received. Allowing for accumulation overflow, for Nc=4 this requires four 10-bit additions per sample. If the texture filter unit performs separate bilinear operations as explained in Section 3.1, then the colour accumulation block must also perform the inter-level interpolation. It is therefore sent the f component of the level of detail solution.

4. Results
The algorithm described in the preceding section was initially implemented in C++ using our algorithm/architectural prototyping environment [25]. This environment permits different algorithms to be implemented interchangeably in the context of a complete rasterisation pipeline. Data flow organisation, number representation and precision can be fine tuned within the environment and results generated rapidly in the form of statistics, images and animation. Both fixed point and floating point implementations were considered. Finally, the fixed point version was implemented in RTL VHDL and synthesised as part of a full rasterisation ASIC.

4.1. Image quality
The images in Fig. 13, Fig. 14, Fig. 15, Fig. 16, Fig. 17, Fig. 18, Fig. 19, Fig. 20, Fig. 21 compare the results of both trilinear filtering and anisotropic filtering. In all cases, the MIP-map level selection for the trilinear filter was based on the method recommended by Heckbert [8] and unless stated otherwise, the anisotropic filter was clamped to a maximum of four (Nc=4). Although, as explained in Section 2.2, this filter incorporates several simplifying assumptions, even so, these images show clear improvements in visual quality. The images in Fig. 13, Fig. 14, Fig. 15, Fig. 16, Fig. 17, Fig. 18 show the ubiquitous chequered plane. This was chosen to allow further comparison of this filter with other work. For example, these chequered planes may be compared with those images shown by Foley et al. [3], Fig. 17.12, which show the results obtained with the use of summed area tables [19] and EWA filtering [14], [15].


Download : Download full-size image
Fig. 13. Chequered plane using iostropic trilinear filtering, Nc=1.


Download : Download full-size image
Fig. 14. Chequered plane using anisotropic filtering limited to a maximum of Nc=4. Averaged 3.1 trilinear/pixel. Peaked 4 trilinear/pixel.


Download : Download full-size image
Fig. 15. Chequered plane using anisotropic filtering limited to a maximum of Nc=64. Averaged 5.6 trilinear/pixel. Peaked 64 trilinear/pixel.


Download : Download full-size image
Fig. 16. Chequered plane using anisotropic filtering limited to a maximum of Nc=4. Using hypotenuse vector estimation.


Download : Download full-size image
Fig. 17. Chequered plane using anisotropic filtering limited to a maximum of Nc=64, selecting N as nearest integer rather than approximated power of two. Averaged 4.4 trilinear/pixel. Peaked 45 trilinear/pixel.


Download : Download full-size image
Fig. 18. Chequered plane using anisotropic filtering limited to a maximum of Nc=4, selecting texture minification from minor axis without scaling to Nc.


Download : Download full-size image
Fig. 19. Selected frames from animation showing textured plane morphing from one axis of anisotropy to another, here shown using trilinear filtering.


Download : Download full-size image
Fig. 20. The same selected animation frames as shown in Fig. 19, produced using anisotropic texture filtering.


Download : Download full-size image
Fig. 21. Comparison of results of (a) trilinear and (b) anisotropic filtering for different textures.

4.2. Performance cost
As said, in the images shown in Fig. 14, Fig. 16, Fig. 18, Fig. 20 Nc was set to four. In our experience this is a sensible ceiling level, providing significant improvements over two, but not significantly different to choosing eight. The performance effect of using and Nc equal to four is that the cost per pixel will vary dependent on the minification ratio of each pixel up to a maximum of four trilinear operations. The important figure here, however, is the overall average cost per pixel for an entire frame. In the examples shown with a single texture using a single value of Nc for the entire scene, the average cost per pixel is generally less than Nc as some pixels will have less minification than others. For the image in Fig. 14, the average was 3.1 trilinear accesses per pixel compared to a clamped peak of 4. For Fig. 15, the average was 5.6 compared to a peak of 64 trilinear operations per pixel. Interestingly, Fig. 17 shows that setting N as the nearest integer requires a lower average number of samples. In fact, this is very scene dependent depending on whether the majority of rounding operations are up or down. In this case we see that the peak value was N=45. The peak of 64 seen for Fig. 15 only occurs due to the approximation described in Section 3.2.6. Using a true divide, the peak would have been 32. However, the Nc value of 64 was only used to allow any differences between using N as a power of two or integer to become apparent. In practise, a far lower value of Nc is recommended and these differences become less significant.

In busier scenes the average score will generally be much lower. As Nc can be set on a per primitive basis, the clamping figure can therefore be set to trade off between quality and performance as required. Fig. 21 shows that different textures benefit from anisotropic filtering by varying amounts, dependent on the spatial frequency and uniformity of the pattern. Text is a classic example of a texture that will benefit noticeably, while marble is seen to gain little.

The use of multi-texturing is becoming more widespread, both increasing the overall number of texture accesses made and also the type of textures used. Light maps are an example where expensive anisotropic filtering would be inappropriate. Therefore, the prudent use of Nc will see the overall performance impact of using anisotropic filtering, where it is required, diminished. Images in which fogging is applied are another example where Nc should be used adaptively.

4.3. Implementation cost
This section details the computational cost estimate of the anisotropic filtering unit above that required for a trilinear filter. The costs given here assume the use of the reduced divide logic.
1.
Calculation of S,T,Q,sx,tx,sy,ty — No extra cost

2.
Calculation of d1, d2 — 4 additions/subtractions and 2 comparisons. r1 and r2 are already known

3.
Calculation of j, MajAxis, MinAxis — 2 extra comparison tests

4.
Level of detail extraction — No extra cost

5.
Calculation of anisotropy
◦
Ratio calculation — 2 rounding operations, 1 exponent subtraction/1 right variable shift

◦
Clamp to Nc-Bit comparison and fixed shift


6.
Local texture address generation
◦
Calculation of ds and dt — orthogonal components of max(r1, r2) received via multiplexer from the existing test

◦
Scaling to N and texscale — 4 exponent subtractions/variable shifts (the size depends upon the texture dimensions and the maximum supported Nc)

◦
ds/2 and dt/2 — 2 exponent subtractions/fixed shift

◦
Address generation — small state machine, 2 interpolators (storage and increment/decrement)

◦
Wrap and clamp logic


7.
Colour accumulation — 4 10bit additions and 4 exponent subtractions/variable shifts (size dependent upon Nc)


4.3.1. Precision requirements
Precision modelling was performed within the C++ simulation environment [25]. This allowed custom floating point and fixed point precision to be applied to each individual part of the filtering procedure. For a floating point-based approach a maximum precision of 8 exponent and 11 fractional bits was required. For fixed point, to support textures of 1024×1024 with 15-bit fractional precision, a maximum of 25-bits was selected.

Table 1 provides a count of the functional units required in the implementation of the anisotropic filter on top of existing hardware for trilinear texture filtering. The gate count estimates are based on the European Silicon (ES2) ECDM 05 (0.5 μm) ASIC process. These units use the precision figures given above. Including extra control logic the cost of implementing fixed point anisotropic filtering was estimated at approximately 15 K gates and a fully floating point based system at between 45 and 50 K gates. The exact size of the units will depend on the precise process used, the constraints set and any optimisation achieved when combined into a complete system. Synthesis of the VHDL for the fixed point implementation resulted in the gate counts given in Table 2. A significant proportion of the gate count (29%) resulted from the synthesis of registers. These are found mainly in the Front End and Filter units, in almost equal quantities. The actual resulting cost of the additional hardware is therefore about 13 000 gates.

Table 1. Implementation cost gate counts

Functional unit	Gate count estimates
#	Fixed point	Floating point
Add/subtract	6	∼1000	∼4000
Comparison	4	∼1000	∼4000
Variable shift	1	25-bit∼1000	—
2	10-bit∼500	
6	4-bit∼200	
Power of two rounding logic	2	∼100	0
10-bit fixed add	Nc	∼350
8-bit exponent add/subtract	9	—	∼300
Table 2. Synthesis figures

Gate count
Front end (ratio calculation, LOD and sample increment calculation)	10 130
Address gen. (local filter addressing)	2479
Filter (trilinear filter and accumulation)	14 923
Total	27 532
5. Conclusions
In this paper we have presented an overview of texture filtering algorithms ranging from high-quality off-line methods to practical real-time implementations. This discussion culminated in the description of a low-cost implementation of an anisotropic texture filter.

When designing a texture filter implementation or selecting an existing one, there are two main considerations: The quality of the filtering results and the practicality of implementation. The issues relating to the quality of the filter include the degree to which the filter reduces or removes aliasing, at the expense of blurring the detail included in the texturing image. When investigating this it is important to consider the effects during animation as well as in still images. Two main artefacts can occur during animation. The image can appear to scintillate as higher frequency details emerge inconsistently on alternate frames. This is a significant problem. A filtering method may appear to over filter and produce excessive blurring when viewed for particular frames, but may prove necessary for animation. A second artefact, that can occur as a feature of MIP-map pre-filtering, is the presence of level waves. These occur where changes in the selected MIP-map level are abrupt and result in obvious changes in detail. During animation these can be seen as advancing wave fronts.

The key factors determining the quality of a texture filter are the size and shape of the filter kernel. In a high-quality filter the shape should adapt to the distortion generated by the mapping operation. The size of the filter should vary gradually to ensure no abrupt changes in filtering severity.

The practicality of the filter design depends upon the cost of implementation in terms of silicon and performance. Other important considerations include the storage requirements of the system in terms of texture memory and the ease with which the system can be integrated within existing graphics architectures.

In this paper we have described a filter that provides substantial visual improvements over trilinear filtering at a modest extra cost. To limit the performance reduction of the technique, a clamping figure Nc can be set on a per primitive basis. Although our investigation showed that Nc=4 was a good compromise between quality and cost, it is likely that most systems will initially support a maximum two to one ratio to minimise the performance hit. However, by allowing the clamp to be set on a per primitive basis, the workload can be set selectively. This is important because not all textures benefit to the same extent. For textures with low detail content, light maps and transparent textures, and in situations such as where fogging is applied, it would be wasteful to perform expensive high quality filtering. Selectively varying the workload in this way allows a higher average pixel rate.

6. Further work
The texture minification ratio and therefore, the selected MIP-map level data depend upon the minor axis of the texture space pixel projection. In some situations, for certain textures with precise high detail components such as text, aliasing artefacts can still occur. Therefore, the filter might be improved by adding a method of biasing the calculated texture minification to allow fine-tuning of the filter operation.

As explained in Section 3.1, trilinear filtering calls for many accesses to texture memory and texture caching schemes can be of great benefit and previous work [26], based on trilinear filtering, is applicable here. However, this is especially true of the filter implementation presented in this paper, where great texel spatial coherence was illustrated. We are currently working on caching schemes to target the access patterns of such filters.

As also discussed in Section 3.1, in this implementation the texture minification is assumed constant for the area of a pixel and it is this simplification that makes this filter a low-cost practical option. However, for a more accurate solution we would like to compensate for the sub-pixel variation in the minification ratio. This would allow the modelling of irregular pixel mapping as shown in Fig. 1. In this way the inter-level line shown in Fig. 6 would be seen to curve. To reduce complexity, this variation might be approximated using linear interpolation of the fraction f. However, such operations could cause the requirement for three or more MIP-map levels per pixel, which would then have further implications for any texture caching scheme.
