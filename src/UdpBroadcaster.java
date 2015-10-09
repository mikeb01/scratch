import java.net.*;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

public class UdpBroadcaster
{
    private static final int PORT = 9876;
    // "224.0.1.1", "ff02::3%en0"

    public static void main(String[] args)
    {
        try
        {
            InetAddress group = InetAddress.getByName(args[0]);
            NetworkInterface networkInterface = NetworkInterface.getByName(args[1]);
            CountDownLatch startLatch = new CountDownLatch(1);
            Thread client = new Thread(client(startLatch, group, networkInterface));
            client.start();

            if (!startLatch.await(5, TimeUnit.SECONDS))
            {
                throw new RuntimeException("Didn't start");
            }

            Thread server = new Thread(server(group));
            server.start();

            client.join();
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }
    }

    private static Runnable client(CountDownLatch startLatch, InetAddress group, NetworkInterface networkInterface)
    {
        return () ->
        {
            try (MulticastSocket multicastSocket = new MulticastSocket(PORT))
            {
                multicastSocket.joinGroup(new InetSocketAddress(group, PORT), networkInterface);
                startLatch.countDown();

                while (true)
                {
                    try
                    {
                        byte[] receiveData = new byte[256];
                        DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
                        multicastSocket.receive(receivePacket);
                        System.out.println(
                            "Client received from : " + receivePacket.getAddress() + ", " + new String(
                                receivePacket.getData()));
                    }
                    catch (Exception e)
                    {
                        e.printStackTrace();
                    }
                }
            }
            catch (Exception e)
            {
                e.printStackTrace();
            }
        };
    }

    private static Runnable server(InetAddress group)
    {
        return () ->
        {
            try (DatagramSocket serverSocket = new DatagramSocket())
            {
                try
                {
                    while (true)
                    {
                        Thread.sleep(1000);
                        byte[] sendData = new byte[256];
                        DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, group, PORT);
                        serverSocket.send(sendPacket);
                    }
                }
                catch (Exception e)
                {
                    e.printStackTrace();
                }
            }
            catch (Exception e)
            {
                e.printStackTrace();
            }
        };
    }
}