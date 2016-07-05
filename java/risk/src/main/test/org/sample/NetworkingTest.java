package org.sample;

import org.junit.Test;
import sun.nio.ch.Net;

import java.io.FileDescriptor;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.net.ProtocolFamily;
import java.net.StandardProtocolFamily;
import java.nio.channels.SocketChannel;

/**
 * Created by mikeb01 on 02/07/15.
 */
public class NetworkingTest
{
    @Test
    public void testNet() throws Exception
    {
        SocketChannel socket = SocketChannel.open();

        Class<?> socketClass = Class.forName("sun.nio.ch.SocketChannelImpl");
        Field fdField = socketClass.getDeclaredField("fd");
        fdField.setAccessible(true);
        FileDescriptor fd = (FileDescriptor) fdField.get(socket);

        Class<Net> netClass = Net.class;
        Method setOptionMethod = netClass.getDeclaredMethod(
            "setIntOption0", FileDescriptor.class, boolean.class, int.class, int.class, int.class);
        setOptionMethod.setAccessible(true);

        int level = 6;
        int name = 1; // TCP_NODELAY

        setOptionMethod.invoke(null, fd, true, level, name, 1);
    }
}
