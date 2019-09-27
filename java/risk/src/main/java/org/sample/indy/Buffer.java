package org.sample.indy;

import java.lang.invoke.*;

public class Buffer
{
    private final long UNMAPPED = 0;
    private static final MethodHandle FALLBACK;
    private static final MethodHandle NORMAL;

    static
    {
        MethodHandles.Lookup lookup = MethodHandles.lookup();
        try
        {
            final VarHandle address1 = lookup.findVarHandle(lookup.lookupClass(), "address", Long.TYPE);
            final MethodHandle address2 = lookup.findVirtual(
                lookup.lookupClass(), "addressUnmapped", MethodType.methodType(Long.TYPE));
            NORMAL = address1.toMethodHandle(VarHandle.AccessMode.GET);
            FALLBACK = address2;//.toMethodHandle(VarHandle.AccessMode.GET);
        }
        catch (NoSuchFieldException | IllegalAccessException | NoSuchMethodException e)
        {
            throw new RuntimeException(e);
        }
    }

    private final SwitchPoint switchPoint = new SwitchPoint();
    private final MethodHandle getter = switchPoint.guardWithTest(NORMAL, FALLBACK);

    private final long address;

    public void unmap()
    {
        SwitchPoint.invalidateAll(new SwitchPoint[] { switchPoint });
    }

    public long getAddress()
    {
        try
        {
            return (long) (long) getter.invoke(this);
        }
        catch (Throwable throwable)
        {
            throw new RuntimeException(throwable);
        }
    }

    public long getAddressDirect()
    {
        return address;
    }

    public long addressUnmapped()
    {
        throw new IllegalStateException();
    }

    public Buffer(long address)
    {
        this.address = address;
    }

    public static void main(String[] args)
    {
        Buffer b = new Buffer(987234);

        System.out.println(b.getAddress());

        b.unmap();

        System.out.println(b.getAddress());
    }
}
