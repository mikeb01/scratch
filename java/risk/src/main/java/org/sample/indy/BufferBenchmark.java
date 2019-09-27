package org.sample.indy;

import org.openjdk.jmh.annotations.Benchmark;
import org.openjdk.jmh.annotations.Scope;
import org.openjdk.jmh.annotations.State;

@State(Scope.Benchmark)
public class BufferBenchmark
{
    private final Buffer b = new Buffer(234234);

    @Benchmark
    public long get()
    {
        return b.getAddress();
    }

    @Benchmark
    public long getDirect()
    {
        return b.getAddressDirect();
    }
}
