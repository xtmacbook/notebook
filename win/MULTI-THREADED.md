# MULTI-THREADED PROGRAMMING  NATIVE THREAD FOR WIN32
https://www.bogotobogo.com/cplusplus/multithreaded2B.php
Win32 provides following types of objects that can be used for thread synchronization:

 * Mutex
 * Critical section
 * Semaphore
 * Condition
 * Event

## Mutex
## Critical section
Critical section (see here, too) is a method of ensuring only a single thread executes a region of code. It is declared within <font color=yellow> a process</font> and are not resources provided by the kernel. In other words, critical section is a lock object that can only be used to synchronize threads in the<font color=yellow> same process</font>, and they do not have handles. Because they are within the process, access to critical sections is quicker than it would be if access had to be brokered by the kernel. This is one of the defferences: critical sections do not go down to the kernel level, but mutex does go down to the kernel level.
## Mutex
Mutexes are ** <font color=yellow>kernel object </font> **, which enables them to be shared <font color=yellow>between processes</font>. This also means that mutex-protected sections are heavier weight than critical sections.

## Seamaphore
The semaphores are a way of keeping track of a count of numbers as well as a way of communicating resource availability between threads. They can be used as an alternative implementation of a mutex at the simplest level. For a more complex use would be to communicate readiness between multiple threads.

## Condition
They work with either a critical section or a slim reader/writer lock to allow threads to sleep until a condition becomes true
