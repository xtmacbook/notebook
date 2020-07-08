# CUDA Programming Guide
At its core are three key abstractions - a ** hierarchy of thread groups **, ** shared memories **, and ** barrier synchronization ** - that are simply exposed to the programmer as a minimal set of language extensions.

## Programming Model
### &emsp;&emsp;Kernels
&emsp;&emsp;&emsp;&emsp;A kernel is defined using the __global__ declaration specifier and the number of CUDA threads that execute that kernel for a given kernel call is specified using a new <<<...>>>execution.
### &emsp;&emsp;Thread Hierarchy:
&emsp;&emsp;&emsp;&emsp; ** thread block ** 可以是一二三维度的， 每个block里的线程数量是有限制的，all threads of a block are expected to reside on the same processor core and must share the limited memory resources of that core。Blocks又被划到一二三维的**Grid**中。Thread blocks are required to ** execute independently ** : It must be possible to execute them in any order, in parallel or in series。
### &emsp;&emsp;Memory Hierarchy:

Each thread has private **local memory**. Each thread block has **shared memory** visible to all threads of the block and with the same lifetime as the block. All threads have access to the same **global memory**.Two additional **read-only** memory spaces accessible by **all threads**: the **constant** and **texture memory** spaces.
