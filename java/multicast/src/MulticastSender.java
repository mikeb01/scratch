import java.net.*;

public final class MulticastSender implements Runnable
{
    private volatile long messageCounter = 0;

    public static void main(final String[] args)
        throws Exception
    {
//        System.setProperty("java.net.preferIPv4Stack", "true");

        if (2 != args.length && 3 != args.length)
        {
            System.out.println("Usage: java MulticastSender <multicast address> <number of messages> [interface]");
            return;
        }

        final String address = args[0];

        final long count = Long.parseLong(args[1]);

        final int port = 4445;
        final int serverPort = 4447;

        final byte[] buffer = "This is a test string with sufficient data to send".getBytes("ASCII");

        final InetAddress group = InetAddress.getByName(address);
        MulticastSocket socket;

        if (3 == args.length)
        {
            final String interfaceAddress = args[2];
            System.out.println("Binding to interface: " + interfaceAddress);
            socket = new MulticastSocket(new InetSocketAddress(interfaceAddress, serverPort));
        }
        else
        {
            socket = new MulticastSocket(serverPort);
        }

        final DatagramPacket packet = new DatagramPacket(buffer, buffer.length, group, port);

        final MulticastSender sender = new MulticastSender();
        final Thread t = new Thread(sender);
        t.setDaemon(true);
        t.start();

        for (long i = count; --i != 0;)
        {
            packet.setData(buffer);
            socket.send(packet);
            sender.messageCounter++;
        }

        Thread.sleep(1000);

        socket.close();
    }

    @Override
    public void run()
    {
        long lastTimestamp = System.currentTimeMillis();
        long lastMessageCounter = 0L;

        while (true)
        {
            final long newTimestamp = System.currentTimeMillis();
            final long duration = newTimestamp - lastTimestamp;

            final long newMessageCounter = messageCounter;
            final long numberOfMessages = newMessageCounter - lastMessageCounter;

            System.out.format("Sent %d messages in %dms\n", Long.valueOf(numberOfMessages), Long.valueOf(duration));

            lastTimestamp = newTimestamp;
            lastMessageCounter = newMessageCounter;

            try
            {
                Thread.sleep(1000);
            }
            catch (final InterruptedException ex)
            {
                break;
            }
        }
    }
}