package org.sample;

import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Enumeration;

/**
 * Created by mikeb01 on 31/08/15.
 */
public class Networks
{
    public static void main(String[] args) throws SocketException, InterruptedException
    {
        Thread.sleep(3000);
        System.out.println("\n\n\n");
        System.out.println("Interfaces:");

        Enumeration<NetworkInterface> networkInterfaces = NetworkInterface.getNetworkInterfaces();
        while (networkInterfaces.hasMoreElements())
        {
            NetworkInterface networkInterface = networkInterfaces.nextElement();
            System.out.println(networkInterface);

        }
        System.out.println("\n\n\n");
    }
}
