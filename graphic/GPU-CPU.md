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
