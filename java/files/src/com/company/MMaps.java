package com.company;

import java.io.File;
import java.io.IOException;
import java.lang.reflect.Field;
import java.nio.Buffer;
import java.nio.MappedByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.file.OpenOption;
import java.nio.file.Path;
import java.nio.file.StandardOpenOption;

public class MMaps
{

    public static void main(String[] args) throws IOException, NoSuchFieldException, IllegalAccessException
    {
        Field addressField = Buffer.class.getDeclaredField("address");
        addressField.setAccessible(true);

        for (int i = 0; i < 10; i++)
        {
            File file = File.createTempFile("aaa", ".b");
            file.deleteOnExit();

            FileChannel open = FileChannel.open(file.toPath(), StandardOpenOption.READ, StandardOpenOption.WRITE);
            MappedByteBuffer map = open.map(FileChannel.MapMode.READ_WRITE, 0, 4096);

            map.get(1);

            Long address = (Long) addressField.get(map);

//            System.out.println("Address: " + (address & 4095));
            System.out.println("Address: " + ((~address) >>> 63L));
        }
    }
}
