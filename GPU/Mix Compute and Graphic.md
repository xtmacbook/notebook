# Mixing CUDA with OpenGL  (Opengl Insights charpter:9)
Each OpenGL object must first be **registered** with the CUDA context as a **CUDA graphics resource**.There are two registering CUDA API calls:one for **buffer objects** and another for **texture and renderbuffer objects**. Both OpenGL and CUDA logically partition device memory into two kinds of memory:**texture memeory**and **linear memory**.   
&emsp;&emsp;Every time CUDA interacts with an OpenGL object, the object must be** mapped**and subsequently **unmapped** from the **CUDA context**.Once the graphics resource is** mapped into CUDA**, the
application can obtain a pointer to an object in the **CUDA address space** using one
of the cudaGraphicsResourceGetMapped* calls to start interacting with the object.An OpenGL object should not be accessed by OpenGL within the application while it is mapped in CUDA, as it can cause data corruption.   
&emsp;&emsp;There are many situations when the application requires distributing graphics and
compute portions across **two or more GPUs**.Developers must keep in mind that cross-GPU interoperability will **inevitably** involve data transfers as part of the content synchronization between the contexts,
which, depending on the data size, can impact the application performance.When implementing multi-GPU compute-graphics interoperability, developers are faced with multiple design options:
1. API interoperability
2. CUDA memory copy + API interoperability.
3. Na¨ıve interoperability  
&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;(Figure see OpenGL Insights)

[Navida Developer PDF](https://developer.download.nvidia.cn/GTC/PDF/GTC2012/PresentationPDF/S0267A-GTC2012-Mixing-Graphics-Compute.pdf)

Simple OpenGL-CUDA interop sample:

```
unsigned char *memPtr;
cudaArray *arrayPtr;
while (!done) {
  cudaGraphicsMapResources(1, &cudaResourceTex, cudaStream);
  cudaGraphicsMapResources(1, &cudaResourceBuf, cudaStream);
  cudaGraphicsSubResourceGetMappedArray(&cudaArray, cudaResourceTex, 0, 0);
  cudaGraphicsResourceGetMappedPointer((void **)&memPtr, &size, cudaResourceBuf);
  doWorkInCUDA(cudaArray, memPtr, cudaStream);//asynchronous
  cudaGraphicsUnmapResources(1, & cudaResourceTex, cudaStream);
  cudaGraphicsUnmapResources(1, & cudaResourceBuf, cudaStream);
  doWorkInGL(imagePBO, imageTex); //asynch
```

> Interoperability behavior:single GPU:
> 1. The resource is shared
> 2. Tasks are serialized

> Interoperability behavior: multiple GPUs:
> 1. Each context owns a copy of the resource
> 2. Tasks are serialized
> 3. map/unmap might do a host side synchronization

> 当在多个线程中 ，CUDA is producter,and the opengl is customer then:
> 1. Use mapping hint with cudaGraphicsResourceSetMapFlags() cudaGraphicsMapFlagsReadOnly/cudaGraphicsMapFlagsWriteDiscard
> 2. ```
    unsigned char *memPtr;
    cudaGraphicsResourceSetMapFlags(cudaResourceBuf, cudaGraphicsMapFlagsWriteDiscard)
    while (!done) {
      cudaGraphicsMapResources(1, &cudaResourceBuf, cudaStream);
      cudaGraphicsResourceGetMappedPointer((void **)&memPtr, &size, cudaResourceBuf);
      doWorkInCUDA(memPtr, cudaStream);//asynchronous
      cudaGraphicsUnmapResources(1, &cudaResourceBuf, cudaStream);
      doWorkInGL(imagePBO); //asynchronous
    }
    ```

> Interoperability behavior: multiple GPUs Improvements:
> 1. If the graphics and compute are interdependant use **pingpong buffers** for task overlap
> 2.  ```
    unsigned char *memPtr;
    int count = 0;
    while (!done) {
     cudaResourceBuf = (count%2) ? cudaResourceBuf1 : cudaResourceBuf2;
     imagePBO = (count%2) ? imagePBO1 : imagePBO2;
    cudaGraphicsMapResources(1, &cudaResourceBuf, cudaStream);
    cudaGraphicsResourceGetMappedPointer((void **)&memPtr, &size, cudaResourceBuf);
    doWorkInCUDA(memPtr, cudaStream);//asynchronous
    cudaGraphicsUnmapResources(1, & cudaResourceBuf, cudaStream);
    doWorkInGL(imagePBO); //asynchronous
    count++;
    }
 ```

> Use streams
> 1. Utilize the copy engines to hide the data transfer latency cost.
 ```
     unsigned char *memPtr;
    int count = 0;
    while (!done) {
     cudaResourceBuf = (count%2) ? cudaResourceBuf1 : cudaResourceBuf2;
     imagePBO = (count%2) ? imagePBO1 : imagePBO2;
     cudaStream= (count%2) ? cudaStream1 : cudaStream2;
    cudaGraphicsMapResources(1, &cudaResourceBuf, cudaStream);
    cudaGraphicsResourceGetMappedPointer((void **)&memPtr, &size, cudaResourceBuf);
    doWorkInCUDA(memPtr, cudaStream);//asynchronous
    cudaGraphicsUnmapResources(1, & cudaResourceBuf, cudaStream);
    doWorkInGL(imagePBO); //asynchronous
    count++;
    }
  ```
