package org.sample;

import java.util.Arrays;
import java.util.Collections;
import java.util.Random;

/**
 * Created by mikeb01 on 04/07/15.
 */
public class SortLots
{
    public static void main(String[] args)
    {
        Random r = new Random();



        int iterations = 1000;
        int arraySize = 100000;
        Object[] array = arrayOfStrings(r, arraySize);

        while (true)
        {
//            shuffleArray(r, array);
            Arrays.sort(array, (a, b) -> ((String) a).compareTo((String) b));
        }

    }

    private static Object[] arrayOfStrings(Random r, int arraySize)
    {
        byte[] bs = new byte[5];
        Object[] array = new Object[arraySize];
        for (int i = 0; i < array.length; i++)
        {
            r.nextBytes(bs);
            array[i] = new String(bs);
        }

        return array;
    }


    private static void shuffleArray(Random rnd, Object[] ar)
    {
        for (int i = ar.length - 1; i > 0; i--)
        {
            final int index = rnd.nextInt(i + 1);
            final Object a = ar[index];
            ar[index] = ar[i];
            ar[i] = a;
        }
    }
}
