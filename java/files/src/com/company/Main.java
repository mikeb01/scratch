package com.company;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Iterator;

public class Main
{

    public static void main(String[] args) throws IOException
    {

        File file = new File("/tmp/foo/bar/baz/ln");
//        file.createNewFile();

        System.out.println(file.delete());

//        File root = new File("/tmp/foo");
//        File child = new File(root, "bar/baz");
//
//        root.mkdirs();
//        child.mkdirs();
//
//        Files.createSymbolicLink(new File(child, "ln").toPath(), root.toPath());


//        File current = new File("/tmp/foo");
//        getChild(current);
    }

    static int count(Path p)
    {
        int i = 0;
        for (Path path : p)
        {
            i++;
        }
        return i;
    }

    private static void getChild(File current) throws IOException
    {
        if (count(current.toPath()) > 20)
        {
            return;
        }

        File[] files = current.listFiles();

        for (File file : files)
        {
            String currentPath = current.getCanonicalPath();
            String childPath = file.getCanonicalPath();

            System.out.printf("Is revisited: %s: Parent: %s, Child: %s%n", currentPath.startsWith(childPath), current, file);
//            System.out.printf("Is revisited: %s: Parent: %s, Child: %s%n", currentPath.startsWith(childPath), currentPath, childPath);

            getChild(file);
        }
    }
}
