## OpenGl Image Format
An Image Format describes the way that the images in Textures and renderbuffers store their data.There are three basic kinds of image formats: **color**, **depth**, and **depth/stencil**.
### Color  
Colors in OpenGL are stored in **RGBA** format.Color formats can be stored in one of 3 ways: **normalized integers, floating-point, or integral**.  

OpenGL has a particular syntax for writing its color format enumerants. It looks like this:
 **GL_[components​][size​][type​]**  
 components​ :the list of components that the format stores  
 size​ :  bitdepth for each component  
 type​ :   indicates which of the 5 types  

 
*  "": No type suffix means unsigned normalized integer format.
* "_SNORM": Signed normalized integer format.
* "F": Floating-point. Thus, GL_RGBA32F is a floating-point format where each component is a 32-bit IEEE floating-point value.
* "I": Signed integral format. Thus GL_RGBA8I gives a signed integer format where each of the four components is an integer on the range [-128, 127].
* "UI": Unsigned integral format. The values go from [0, MAX_INT] for the integer size.

## glTexStorage**D or glTexImage*D
&emsp;&emsp;**glTexStorage2D** — simultaneously specify storage for all levels of a two-dimensional or one-dimensional array texture.Once a texture is specified with this command, the format and dimensions of all levels become immutable unless it is a proxy texture. The contents of the image may still be modified, however, its storage requirements may not change. Such a texture is referred to as an immutable-format texture.  
## glTex{,Sub}Image*D  
&emsp;&emsp;specify a two-dimensional texture subimage.
**glTexSubImage2D**( GLenum target,  GLint level,  GLint xoffset,  GLint yoffset,  GLsizei width,  GLsizei height,  GLenum format,  GLenum type,  const void * pixels);Texturing maps a portion of a specified texture image onto each graphical primitive for which texturing is enabled.  

## glFormat && glInternalFormat  
format :Specifies the format of the **pixel data**.   glFormat specifies the format parameter passed to glTex{,Sub}Image*D.  

internalformat: Specifies the number of **color components** in the texture.For uncompressed textures,Specifies the sized internal format to be** used to store texture image** data

glInternalFormat specifies the internalformat parameter passed to glTexStorage*D or glTexImage*D  

type : Specifies the data type of the pixel data
