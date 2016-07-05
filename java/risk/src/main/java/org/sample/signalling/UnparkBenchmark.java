package org.sample.signalling;

import org.openjdk.jmh.annotations.Benchmark;
import org.openjdk.jmh.annotations.Scope;
import org.openjdk.jmh.annotations.Setup;
import org.openjdk.jmh.annotations.State;

import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.LockSupport;
import java.util.concurrent.locks.ReentrantLock;

@State(Scope.Benchmark)
public class UnparkBenchmark
{
    private volatile long value = 0;
    private Thread t;

    @Setup
    public void setup()
    {
        t = new Thread(() ->
        {
            while (!Thread.currentThread().isInterrupted())
            {
                LockSupport.park();
                long l = value;
            }
        });
        t.setDaemon(true);
        t.start();
    }

    @Benchmark
    public void run()
    {
        value++;
        LockSupport.unpark(t);
    }
}
