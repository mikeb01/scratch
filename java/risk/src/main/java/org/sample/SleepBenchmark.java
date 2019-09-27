package org.sample;

import org.openjdk.jmh.annotations.Benchmark;
import org.openjdk.jmh.annotations.BenchmarkMode;

import java.util.concurrent.locks.LockSupport;


public class SleepBenchmark
{
    @Benchmark
    public void parkNanos1()
    {
        LockSupport.parkNanos(1);
    }

    @Benchmark
    public void parkNanos10()
    {
        LockSupport.parkNanos(10);
    }

    @Benchmark
    public void parkNanos100()
    {
        LockSupport.parkNanos(100);
    }

    @Benchmark
    public void parkMillis() throws InterruptedException
    {
        Thread.sleep(1);
    }
}
