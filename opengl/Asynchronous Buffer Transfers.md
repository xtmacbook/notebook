# Asynchronous Buffer Transfers

When Calling OpenGL functions,the drivers translate calls into **commands** and add them into an **internal queue on the CPU side**.

## Buffer Object
Buffer objects are **linear memory** regions allocated in **device memory** or in **CPU
memory**.

## Pinned memory:
Pinned memory is standard CPU memory and there is no actual transfer to device memory
## Memeory Tranfers
The direct memory access **(DMA)** controller allows the OpenGL drivers to **asynchronously**
transfer memory blocks from user memory to device memory **without wasting CPU cycles**.但是这个只是从CPU角度来看是异步的。从GPU角度来看，GPU的传输数据和渲染是**不能同时进行**的。所有有了CUDA，这样在GPU在渲染的同时可以上传和下载纹理。

glBufferData & glBufferSubData：

这两个函数会把用户的数据copy到**pinned memeory**,pinned memory可以被GPU直接使用。复制的目的内存是根据用户设置的hints和系统的实现不同的。有时会在pinned CPU memeory.

glMapBuffer
是更有效的操作。

## Implicit Synchronization:
When an OpenGL call is done, it usually is not **executed immediately**. Instead,
most commands are placed in the device command queue. Actual rendering may
take place two frames later and sometimes more depending on the device’s performance
and on driver settings.However, when using glBufferSubData or glMapBuffer[Range],Drivers have to avoid this problem by blocking the function until the desired data are not used anymore:
this is called an **implicit synchronization**.This can seriously damage performance or
cause annoying jerks.
## UPloading

***Avaoid implicit synchronization***
+ Round-Robin Fashion (Multiple Buffer Objects)
+ Buffer Respecification (Orphaning)
+ Unsynchronized Buffers

## Downloading
