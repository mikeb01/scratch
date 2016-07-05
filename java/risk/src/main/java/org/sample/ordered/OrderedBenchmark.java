package org.sample.ordered;

import org.openjdk.jmh.annotations.*;

import java.util.Arrays;
import java.util.Random;
import java.util.TreeSet;

@State(Scope.Benchmark)
public class OrderedBenchmark
{
    @Param("1000")
    public int arraySize;

    private String[] data;
    private String key;

    @Setup(Level.Invocation)
    public void setUp()
    {
        data = new String[arraySize];
        final Random r = new Random();
        char[] bytes = new char[5];
        for (int i = 0; i < arraySize; i++)
        {
            data[i] = randomString(r, bytes);
        }

        key = data[5];
    }

    @Benchmark
    public String lookBySort()
    {
        Arrays.sort(data);

        int i = Arrays.binarySearch(data, key);
        return data[i];
    }

    @Benchmark
    public boolean lookByTree()
    {
        TreeSet<String> set = new TreeSet<>();
        for (String s : data)
        {
            set.add(s);
        }

        return set.contains(key);
    }

    public static String randomString(Random r, char[] array)
    {
        int starNum = 42;
        int endNum = 172;

        for (int i = 0; i < array.length; i++)
        {
            array[i] = (char) (r.nextInt(endNum - starNum) + starNum);
        }

        return new String(array);
    }
}
