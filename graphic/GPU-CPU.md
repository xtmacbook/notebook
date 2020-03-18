# GPU CPU data transfer
Host (CPU) data allocations are **pageable** by default. The GPU cannot access data directly from pageable host memory, so when a data transfer from pageable host memory to device memory is invoked, the CUDA driver must first allocate a temporary **page-locked, or “pinned”,** host array, copy the host data to the pinned array, and then transfer the data from the pinned array to device memory, as illustrated below. https://devblogs.nvidia.com/how-optimize-data-transfers-cuda-cc/  
## Implicit Synchronization
When an OpenGL call is done, it usually is not executed immediately. Instead,
most commands are placed in the device command queue. Actual rendering may
take place **two frames later** and sometimes more depending on the device’s performance
and on driver settings (triple buffering, max prerendered frames, multi-GPU
configurations, etc.). This lag between the application and the drivers can be measured
by the timing functions glGetInteger64v(GL TIMESTAMP,&time) and
glQueryCounter(query,GL TIMESTAMP).  
&emsp;&emsp;However, when using glBufferSubData or glMapBuffer[Range], nothing
in the API itself prevents us from modifying data that are currently used by the device
for rendering the previous frame, as shown in Figure 28.3. **Drivers have to avoid
this problem by blocking** the function until the desired data are not used anymore:
this is called an implicit synchronization. This can seriously damage performance or
cause annoying jerks. A synchronization might block until all previous frames in
the device command queue are finished, which could add several milliseconds to the
performance time.


# Synchronization 
## Sync objects  
&emsp;&emsp;The purpose of sync objects is to synchronize the **CPU with the GPU's** actions. To do this, sync objects have the concept of a current status. The status of a sync object can be signaled or unsignaled; this state represents some condition of the GPU, depending on the particular type of sync object and how it was used.  
To block all CPU operations until a sync object is signaled,you call:  
```
 enum glClientWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout)  
```
There is another function for waiting on sync objects: 
```
void glWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout)
```  
What glWaitSync does is prevent the driver from adding any commands to the GPU's command queue until this sync object is signaled. This function **does not halt the CPU**. The driver will still put commands in its internal buffer. **But none of them will be seen by the GPU until this sync object is signaled. **  
A fence is a sync object that is added to the OpenGL command stream. It **starts unsignaled**, and becomes signaled when the GPU executes and completes the fence. Because OpenGL must execute and complete commands in order, when a fence becomes signaled, you can be certain that the GPU has **completed all OpenGL commands issued before the fence was created**. 
## Asynchronous action (注意：是渲染函数异步)
&emsp;&emsp;OpenGL **Rendering Commands** are assumed to be asynchronous. If you call any of the glDraw* functions to initiate rendering, it is not at all guaranteed that the rendering has finished by the time the call returns. Indeed, it is perfectly legitimate for rendering to not even have started when this function returns.   
An OpenGL Rendering Command can be in one of three conceptual states: **unissued**, **issued** but **not complete**, and ** complete**.  
A command is unissued if the command has been given to the OpenGL driver, but the driver has not yet given the command to the hardware to actually execute. When the rendering hardware starts running out of actually issued commands to process, the OpenGL driver can take some of the unissued commands and issue them. 
An issued but not complete command is one that has been given to the hardware, but the full results of the command are not yet ready. The hardware has a queue of these commands; unless there is a hardware fault of some kind, the hardware will execute all of the commands in that queue. 
A command is complete when it is out of the pipeline entirely. For rendering commands, this means that its effects have been written to the Framebuffer, Transform Feedback buffers, or other outputs, as appropriate to the current state. For pixel transfers to buffer objects, this means that the pixel data is now stored in the buffer object as requested. For pixel transfers from buffer objects, this means that the pixel data is now stored in the texture object that was uploaded to. And so forth.     

## Synchronization options  
&emsp;&emsp;OpenGL provides two simple mechanisms for explicit synchronization: glFinish and glFlush. 
The simplest to understand is glFinish. It will not return, **stopping the current CPU thread**, until all rendering commands that have been sent have completed. 
Conceptually, the GPU has something called a "command queue". This is a list of commands written by the OpenGL driver at the behest of the user. Just about every OpenGL function will map to one or more commands that will be added to the command queue. Any command that is placed in the command queue will be read by the GPU and executed. However, the command queue has a **finite length**. If you add too many commands in a short space of time, the driver cannot write them all to the GPU's command queue. What the driver can do is write them to **internal memory**. These commands are in the "unissued" state. Sometime later, the unissued commands are added to the GPU's queue. When the driver does this is the question.[**from internal memory to gpu memoery**] 
The driver may set up some kind of asynchronous message that tells it when the GPU's queue is nearly empty so that it can add more if possible. However, this is generally not the case. OpenGL allows the driver to have the freedom to not make this check until you actually execute an OpenGL call. And even then, not all calls will make this check. 
What this means is that it is theoretically possible for OpenGL to be sitting there, with lots of unissued commands in the driver's buffer, but with the GPU command queue being totally empty. The driver knows that there is work, but if you don't execute another OpenGL command (any command) for a while, it can never verify this check. 
Normally this isn't a problem. But imagine this circumstance. You render a lot of stuff, all in a short space of time. You sort your data to achieve maximum efficiency, so submitting the data takes less time than rendering it. 
Because you add a lot of commands in a short space of time, the driver has to buffer many of these commands. But if you don't make any OpenGL calls after you have submitted all of the data (because you're off doing non-OpenGL stuff), then the driver never has the chance to push these buffered commands into the command queue. Obviously, you will be issuing commands next frame, but you'd probably rather not wait that long for the GPU to start rendering. 
The purpose of glFlush is to tell OpenGL to sit there and wait, halting the current CPU thread, until all commands have been added to the GPU's command queue. This won't take as long as glFinish, but it can still be time consuming. 



