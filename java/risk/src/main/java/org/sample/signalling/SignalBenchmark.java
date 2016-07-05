package org.sample.signalling;

import org.openjdk.jmh.annotations.Benchmark;
import org.openjdk.jmh.annotations.Scope;
import org.openjdk.jmh.annotations.Setup;
import org.openjdk.jmh.annotations.State;

import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

@State(Scope.Benchmark)
public class SignalBenchmark
{
    private volatile long value = 0;
    private Thread t;
    private Lock lock;
    private Condition condition;

    @Setup
    public void setup()
    {
        lock = new ReentrantLock();
        condition = lock.newCondition();

        t = new Thread(() -> {
            try
            {
                while (true)
                {
                    lock.lock();
                    try
                    {
                        condition.await();
                        long l = value;
                    }
                    finally
                    {
                        lock.unlock();
                    }
                }
            }
            catch (InterruptedException e)
            {
                // exit
            }
        });
        t.setDaemon(true);
        t.start();
    }

    @Benchmark
    public void run()
    {
        lock.lock();
        try
        {
            condition.signal();
        }
        finally
        {
            lock.unlock();
        }
    }
}
