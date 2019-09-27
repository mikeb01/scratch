/*
 * JOCL - Java bindings for OpenCL
 * 
 * Copyright 2009 Marco Hutter - http://www.jocl.org/
 */

package org.jocl.samples;

import org.openjdk.jmh.annotations.Benchmark;
import org.openjdk.jmh.annotations.Scope;
import org.openjdk.jmh.annotations.State;
import org.openjdk.jmh.annotations.TearDown;

@State(Scope.Benchmark)
public class JOCLBenchmark
{
    private JOCLSample sample1 = new JOCLSample(10000);
    private JOCLSample sample2 = new JOCLSample(10000);

    @TearDown
    public void tearDown()
    {
        sample1.cleanup();
        sample2.cleanup();
    }

    @Benchmark
    public void benchJocl()
    {
        sample1.main();
    }

    @Benchmark
    public void benchJava()
    {
        sample2.doPureJava();
    }

    public static void main(String[] args)
    {
        final JOCLBenchmark joclBenchmark = new JOCLBenchmark();
        joclBenchmark.benchJocl();
        joclBenchmark.tearDown();
    }
}