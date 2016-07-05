import java.net.*;

public final class MulticastReceiver implements Runnable
{
    private volatile long messageCounter = 0;

    public static void main(final String[] args)
        throws Exception
    {
        //System.setProperty("java.net.preferIPv4Stack", "true");

        if (1 != args.length && 2 != args.length)
        {
            System.out.println("Usage: java MulticastReceiver <multicast address> [interface name]");
            return;
        }

        final int port = 4445;
        final String address = args[0];

        MulticastSocket socket;

        if (2 == args.length)
        {
            socket = new MulticastSocket(port);

            final SocketAddress socketAddress = new InetSocketAddress(address, port);
            final NetworkInterface networkInterface = NetworkInterface.getByName(args[1]);

            socket.joinGroup(socketAddress, networkInterface);
        }
        else
        {
            socket = new MulticastSocket(port);
            socket.joinGroup(InetAddress.getByName(address));
        }

        final byte[] buf = new byte[1024];
        final DatagramPacket packet = new DatagramPacket(buf, buf.length);

        final MulticastReceiver receiver = new MulticastReceiver();
        new Thread(receiver).start();
        while (true)
        {
            socket.receive(packet);
            receiver.messageCounter++;
        }
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

            System.out.format("Received %d messages in %dms\n", Long.valueOf(numberOfMessages), Long.valueOf(duration));

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