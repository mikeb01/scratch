package org.sample.enums;

import org.openjdk.jmh.annotations.Benchmark;
import org.openjdk.jmh.annotations.Param;
import org.openjdk.jmh.annotations.Scope;
import org.openjdk.jmh.annotations.Setup;
import org.openjdk.jmh.annotations.State;
import org.openjdk.jmh.infra.Blackhole;

import java.nio.file.StandardOpenOption;
import java.util.Objects;

@State(Scope.Benchmark)
public class EnumBenchmark
{
    @Param({"2"})
    private int index;

    @Setup
    public void setup(Blackhole blackhole)
    {
        Objects.requireNonNull(blackhole);
    }


    @Benchmark
    public Object loadEnumValueOrdinal1()
    {
        TestEnum e = load((byte) index, TestEnum.class);
        return e;
    }

    @Benchmark
    public Object loadEnumValueOrdinal2()
    {
        TestEnum e = load(storeEnumValueOrdinal(TestEnum.B), TestEnum.enums);
        return e;
    }

//    @Benchmark
//    public Object storeEnumValueOrdinal()
//    {
//        StandardOpenOption e = load(index, StandardOpenOption.class);
//        return e;
//    }
//
//
    public byte storeEnumValueOrdinal(Enum e)
    {
        return (byte) e.ordinal();
    }


    private <T extends Enum> T load(byte ordinal, Class<T> clazz)
    {
        return clazz.getEnumConstants()[ordinal];
    }

    private <T extends Enum> T load(byte ordinal, Enum[] values)
    {
        return (T) values[ordinal];
    }
}
