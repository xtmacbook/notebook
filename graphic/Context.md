

# OpenGL Context  
&emsp;&emsp;An OpenGL context represents many things. A context stores all of the state associated with this instance of OpenGL. It represents the (potentially visible) default framebuffer that rendering commands will draw to when not drawing to a framebuffer object. Think of a context as an object that holds all of OpenGL; when a context is destroyed, OpenGL is destroyed.

Contexts are localized within a particular process of execution (an application, more or less) on an operating system. A process can create multiple OpenGL contexts. Each context can represent a **separate viewable surface**, like a window in an application.

Each context has its own set of OpenGL Objects, which are independent of those from other contexts. A context's objects can be **shared** with other contexts. Most OpenGL objects are sharable, including Sync Objects and GLSL Objects. Container Objects are not sharable, nor are Query Objects.

Any object sharing must be made **explicitly**, either as the context is created or before a newly created context creates any objects. However, contexts do not have to share objects; they can remain completely separate from one another.

In order for any OpenGL commands to work, a context must be current; all OpenGL commands affect the state of whichever context is current. The current context is a thread-local variable, so a single process can have several threads, each of which has its **own current context**. However, a single context cannot be current in multiple threads at the same time.  

# Device context && Rendering contexts  
The **Device context** are used for **resource creation / manipulation**, while all **state** are handled through the Rendering contexts. When you create a Device context you automatically create one Rendering context called the immidiate context which generally are all you need for non-multithreaded rendering.  

I don't know how accurate this description of the decide context actually is in reality, but think of the **device context** as the object holding the **window-side resources** such as the frame buffers, describing the pixel formats of the window and frame buffer, and such. From OpenGL's perspective, the DC is more or less the window itself. Tasks such as allocating a frame buffer, depth buffer and stencil buffer, swapping double buffers at the end of a rendering pass and deciding the pixel format of the window, is not done via OpenGL, but with the help of a platform dependent layer such as WGL for Windows, glx on X-based systems and so on.



The rendering context is the **OpenGL-side resources** such as texutre objects, vertex buffer objects and shader programs, and it also manages states such as which texture is attached to which texture unit, which shader programs are active, which vertex attribute buffers are bound to the shader attributes, and so on.






Basically, any call to function beginning with gl is made on the rendering context. A rendering context is attached to a device context with for example the wglMakeCurrent function on Windows. You can have for example two windows (each window is practically equal to a device context) and one rendering context, and you then direct the rendering to each window by attaching the rendering context to each of the window and draw whatever you want.
 
Note that the handle to the DC can only be used by a single thread at any one time.  

**The GDI does not support multiple threads. You must use a distinct device context and a distinct rendering context for each thread**  

To enhance performance, access to graphics device interface (gdi) objects (such as palettes, device contexts, regions, and the like) is not serialized. this creates a potential danger for processes that have multiple threads sharing these objects. for example, if one thread deletes a gdi object while another thread is using it, the results are unpredictable. this danger can be avoided simply by not sharing gdi objects. if sharing is unavoidable (or desirable), the application must provide its own mechanisms for **synchronizing access**. for more information about synchronizing access, see synchronizing execution of multiple threads.

## Object Sharing  
&emsp;&emsp;You can create multiple OpenGL contexts. This is useful, as the current GL context is thread-specific. Normally, each context is entirely separate from the others; nothing done in one can affect the others.

At context creation time however, you are able to create a context that shares objects with another existing context. This means you can use objects created in one context in another context.

Not all object types can be shared across contexts. Objects that contain references to other objects cannot be shared, nor can Query Objects be shared. All other object types can be shared. This includes GLSL Objects and Sync Objects, which do not follow the OpenGL Object model.

Note that state changes to objects made in one context are not necessarily immediately visible in another. There are specific rules that govern the visibility of object state data. If you are using threading, you need to do some synchronization on your own to ensure that changes made in one context have been made, before trying to use those changes in another context.



# Multithreading and Shared Contexts
When use multithreding ,the data from one thread to other thread.
Each additional thread that needs to call OpenGL functions must create its *own context* and explicitly connect that context to the first context in order to share OpenGL objects. (translate data from one thread to other thread)
```
  in main thread:
main–hrc = wglCreateContextAttribsARB(hdc, NULL, attribs);
worker1–hrc =wglCreateContextAttribsARB(hdc, main–hrc, NULL);
worker2–hrc =wglCreateContextAttribsARB(hdc, main–hrc, NULL);

```
When using shared contexts, however, the driver will create one command queue for each thread, and no such implicit synchronization will take place. A thread can thus start a DMA transfer in a memory block that is currently used in a draw call by the device. This usually results in partially updated meshes or instance data.  


## pinned memory  
&emsp;&emsp;Pinned memory is a portion of the main RAM that can be directly used by the device through the PCI express bus (PCI-e). This is also known as page-locked memory.Pinned memory is standard CPU memory and there is no actual transfer to devicememory:
in this case, the device will use data directly fromthismemory location.
The PCI-e bus can access data faster than the device is able to render it, so there is no
performance penalty for doing this, but the driver can change that at any time and
transfer the data to device memory.
## Memory Transfers  
&emsp;&emsp;There are two ways to upload and download data to the device. The first way is to use the** glBufferData** and **glBufferSubData** functions.these functions take user data and copy them to **pinned memory** directly accessible by the device. This process is similar to a standard memcpy. Once this is done, the drivers start the **DMA transfer**, which is **asynchronous**,and return from glBufferData.                                
&emsp;&emsp;&emsp;**vertex data** -----> **app memory** ----(glbufferData)----->[ **Memory accessible directly by GPU**  ---(DMA transfer)--->**GPU memory** ]   
