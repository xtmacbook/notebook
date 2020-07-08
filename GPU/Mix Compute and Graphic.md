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
