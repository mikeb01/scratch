package org.sample;

import org.openjdk.jmh.annotations.Benchmark;
import org.openjdk.jmh.annotations.Scope;
import org.openjdk.jmh.annotations.State;

@State(Scope.Benchmark)
public class Maps
{
    private Map map = new Map(4096, 45);

    @Benchmark
    public byte getByte()
    {
        return map.getByte(27);
    }

    @Benchmark
    public byte getByteWithMasking()
    {
        return map.getByte(27);
    }

    private static class Map
    {
        private long address;
        private byte[] data;
        private static final long MASK = 1L << 63;


        public Map(int size, long initialAddress)
        {
            if (2 * initialAddress > size)
            {
                throw new IllegalArgumentException();
            }

            data = new byte[size];
        }

        public byte getByte(int offset)
        {
            return data[(int) (address + offset)];
        }

        public byte getByteWithBitMasking(int offset)
        {
            return data[(int) (address() + offset(offset))];
        }

        private long address()
        {
            return ~MASK & address;
        }

        private int offset(int offset)
        {
            return (int) ((MASK & (~address)) >>> 63) * offset;
        }
    }
}
