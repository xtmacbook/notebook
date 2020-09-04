#  c#  Monitor.Pulse
  pluse: 通知 等待队列中线程，locked object状态发生了变化，只有当前拥有lock的线程可以触发pluse函数，当前拥有该lock的线程释放lock对象。
  当等待队列线程接受到这个信号，就移入到就绪队列。

  # Monitor.Wait
    首先释放lock，然后阻止当前线程直到再次获得lock。有个时间设置，防止当前线程永远的等下去，比如另一个线程再释放lock前没有调用pluse函数。释放lock后，进入等待队列,如果在设定时间内，没有接受到pluse 或者pluse all，则进入就绪队列。如果设定时间为0，则线程释放lock后直接进入就绪队列。


注意: Note that if a thread calls Pulse when no other threads are waiting, the Pulse is lost. 所以这就要求 all these methods must be called from within a lock statement.

例如:
```
readonly object key = new object();
// thread A
lock ( key ) Monitor.Wait( key );
// thread B
lock ( key ) Monitor.Pulse( key );
```
You may have noticed a little problem with the code above. If thread A holds the lock on the key object, why does thread B not block when it tries to acquire the lock? This is, of course, handled properly. The call to Wait in thread A releases the lock before it waits. This allows thread B to acquire the lock and call Pulse. Thread A then resumes, but it has to wait until thread B releases the lock, so it can reacquire it and complete the Wait call. Note that Pulse never block.
