# The Clipmap

参考:The Clipmap : A Virtual MipMap (Christopher C. Tanner, Christopher J. Migdal, and Michael T. Jones)

## Abstract
We describe the clipmap, a <font color=yellow>dynamic</font> texture representation that
efficiently <font color=red>caches</font> textures of arbitrarily <font color=blue>large</font> size in a finit
amount of physical memory for rendering at real-time rates.

## Observations About MipMaps
A mipmap as defined by Williams [6] is
a collection of correlated images of increasingly reduced resolution arranged, in spirit, as a resolution pyramid. Starting with level 0, the largest and finest level, each lower level represents the image using half as many texels in each dimension: 1D = 1/2, 2D = 1/4,
and 3D = 1/8 the texels.
在使用mipmap渲染的时候，像素通过几何位置变换、纹理矩阵变换投影到mipmap 空间。要渲染的像素点可能是从一个或者多个level层级中的一个或者多个texel samples获取到。samples的选择也考虑到level的选择，最好是pixel-to-texel接近1:1的层级。层级的选择部分是由display resolution决定的。
![texel access with mipmap](d:/imgs/awm.png)
***majority*** of the mipmap pyramid
will ***not be used*** in the rendering of a single image no matter what
geometry is rendered.The basis of our system is this realization
that <font color=yellow>eyepoint</font> and <font color=yellow>display resolution</font> control access into the mip-map
and that in the case of the very large textures we are con cerned with, only a minute fraction of the texels in the mipmap are accessible. We can build hardware to exploit this fact by rendering from the minimal subset of the mipmap needed for each frame-astructure we term a <font color=yellow>clipmap</font>.

## Clipmap
clipmap是mipmap部分内容动态实时更新的表示.
### mipmap的每个level被clip成特定的大小

![Clipmap Region within a Mipmap](d:/imgs/crwm.png)
<font color=yellow>ClipSize</font>限制mipmap每个level纹理大小，大于这个值的会被clip。这样定义了一个<font color=yellow>Clipmap Stack</font>用来存储level集合。并不是所有的level集合被存储，而是大小为ClipSize的level被存储。下面小于ClipSize的为<font color=yellow>Clipmap Pyramid</font> 被全部存储在texture memory中。
![Clipmap Region within a Mipmap](d:/imgs/csapl.png)  
###使用<font color=yellow>ClipCenter</font>定义clipmap的内容
为每个clipmap stack的层级定义一个clipcenter,这样每个stack通过Clip Center和Clip Size就可以定义texture region ,这个texture region 被缓存起来。

我们的实现是为stack level 0级定义一个clipCenter，然后根据深度依次平移定义每个级别的clipCenter.This forces each level to be centered along a line from the level 0 center to the clipmap apex。
In normal use, however, the ClipSize is set to equal or exceed the
***display size***.
### Invalid Border for Parallel Update
上面定义的stack level只是mipmap level静态一部分。为了未来渲染真所需数据和paged data数据的加载。我们引入了<font color=yellow>Invalid_Border</font>.定义一个texels宽度的带，这个带在每个stack level层级里。在这个带里的texels是不激活状态，not accessed by the texel-sample indexing logic.The size of the active portion of a
clip map is simply ClipSize minus twice the InvalidBorder, known
as the <font color=yellow>EffectiveSize</font> of each stack level。InvalidBorder区域的数据区域就作为加载数据的目的地。
![Clipmap with InvalidBorder and EffectiveSize](d:/imgs/cwi.png)
### TextureOffset for Efficient Update

## Clipmap Storage Efficienc(有效性)

Consider a 16 level (32768x32768) clipmap to be rendered on a (1024x1024)
display. We begin our analysis with pixel-exact rendering. Given
the display size, we know that the upper bound on texture usage
2048x2048,from a single level is so we set the ClipSize to 2048. There
will be four clipped levels forming our Clipmap Stack and 12 levels in the Clipmap Pyramid. The storage required for this is 2048x2048 texels * 4 levels + 4/3 * texels for the pyramid = 42.7 MB at
2 bytes per texel. This perfect clipmap configuration requires only
42.7 MB of the full 2.8 GB present in the complete mipmap. A
more typical configuration using a 1024 ClipSize will achieve attractive results using 1024x1024 * 5 stack texels +* 4/3 pyramidtexels = 12.7 MB. Finally, a 512 texel ClipSize yields reasonable
results with 512x512 * 6 stack texels + * 4/3 pyramid texels = 3.7 MB of storage.

# clipmaps的更新

