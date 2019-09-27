package org.sample.math;

import org.openjdk.jmh.annotations.Benchmark;
import org.openjdk.jmh.annotations.Param;
import org.openjdk.jmh.annotations.Scope;
import org.openjdk.jmh.annotations.State;

@State(Scope.Benchmark)
public class StrictMathBenchmark
{
    @Param({"10000000", "10000000000000"}) long x;
    @Param({"10000000", "10000000000000"}) long y;

    @Benchmark
    public long normalMath()
    {
        return x * y;
    }

    @Benchmark
    public long strictMath()
    {
        try
        {
            return StrictMath.multiplyExact(x, y);
        }
        catch (ArithmeticException e)
        {
            return 0;
        }
    }
}
