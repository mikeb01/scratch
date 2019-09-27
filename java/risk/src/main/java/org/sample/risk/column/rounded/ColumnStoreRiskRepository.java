package org.sample.risk.column.rounded;

import org.sample.RiskPosition;
import org.sample.RiskRepository;

import java.util.HashMap;
import java.util.Map;
import java.util.Objects;

import static java.lang.Math.round;
import static java.util.Arrays.copyOf;

public class ColumnStoreRiskRepository implements RiskRepository
{
    private final long instrumentId;
    private final Map<Long, Integer> accountId2Index = new HashMap<Long, Integer>();
    private long[] openQuantity;
    private long[] cumulativeCost;
    private long[] realisedPnL;
    private long[] margin;
    private long[] unrealisedPnL;
    private long[] valuation;
    private int limit = 0;
    private double[] tempD;

    public ColumnStoreRiskRepository(long instrumentId, int initialSize)
    {
        this.instrumentId = instrumentId;
        resizeTo(initialSize);
    }

    public ColumnStoreRiskRepository(long instrumentId)
    {
        this(instrumentId, 16);
    }

    private void resizeTo(int length)
    {
        System.err.println("Resize to: " + length);

        if (openQuantity == null)
        {
            openQuantity = new long[length];
            cumulativeCost = new long[length];
            realisedPnL = new long[length];
            margin = new long[length];
            unrealisedPnL = new long[length];
            valuation = new long[length];
            tempD = new double[length];
        }
        else
        {
            openQuantity = copyOf(openQuantity, length);
            cumulativeCost = copyOf(cumulativeCost, length);
            realisedPnL = copyOf(realisedPnL, length);
            margin = copyOf(margin, length);
            unrealisedPnL = copyOf(unrealisedPnL, length);
            valuation = copyOf(valuation, length);
            tempD = new double[length];
        }
    }

    @Override
    public RiskPosition addRiskPosition(long accountId)
    {
        Integer index = accountId2Index.get(accountId);
        if (null == index)
        {
            addRow(accountId);
        }
        return new ColumnStoreRiskPosition(this, accountId);
    }

    private void addRow(long accountId)
    {
        if (limit >= openQuantity.length)
        {
            resizeTo(openQuantity.length * 2);
        }

        int index = limit;
        accountId2Index.put(accountId, index);
        limit++;
    }

    @Override
    public RiskPosition getRiskPosition(long accountId)
    {
        if (!accountId2Index.containsKey(accountId))
        {
            throw new IllegalArgumentException("AccountId not found: " + accountId);
        }

        return new ColumnStoreRiskPosition(this, accountId);
    }

    public long getOpenQuantity(long accountId)
    {
        return getCell(openQuantity, accountId);
    }

    public void updateRisk(long accountId, long quantityDelta, long cumulativeCostDelta, long realisedPnLDelta)
    {
        Integer index = accountId2Index.get(accountId);
        Objects.requireNonNull(index);

        openQuantity[index] += quantityDelta;
        cumulativeCost[index] += cumulativeCostDelta;
        realisedPnL[index] += realisedPnLDelta;
    }

    public long getCumulativeCost(long accountId)
    {
        return getCell(cumulativeCost, accountId);
    }

    public long getProfit(long accountId)
    {
        return getCell(realisedPnL, accountId);
    }

    public long getValuation(long accountId)
    {
        return getCell(valuation, accountId);
    }

    private long getCell(long[] column, long accountId)
    {
        Integer index = accountId2Index.get(accountId);
        if (null == index)
        {
            throw new IllegalStateException("AccountId '" + accountId + "' unknown");
        }

        return column[index];
    }

    @Override
    public void valuationNotional(double bidPrice, double askPrice, double marginRate)
    {
        double[] notionalCost = tempD;

        notionalCost(openQuantity, bidPrice, askPrice, notionalCost);
        marginNotional(notionalCost, marginRate, margin);
        unrealisedPnL(notionalCost, cumulativeCost, realisedPnL, unrealisedPnL);
        calculateValuation(margin, unrealisedPnL, valuation);
    }

    private static void calculateValuation(long[] margin, long[] unrealisedPnL, long[] valuation)
    {
        for (int i = 0; i < margin.length && i < unrealisedPnL.length; i++)
        {
            valuation[i] = unrealisedPnL[i] - margin[i];
        }
    }

    private static void marginNotional(double[] notionalCost, double marginRate, long[] out)
    {
        for (int i = 0; i < out.length && i < notionalCost.length; i++)
        {
            out[i] = round(notionalCost[i] * marginRate);
        }
    }

    private static void unrealisedPnL(double[] notionalCost, long[] cumulativeCost, long[] realisedPnL, long[] out)
    {
        for (int i = 0; i < notionalCost.length && i < out.length; i++)
        {
            out[i] = round(notionalCost[i]) - (cumulativeCost[i] + realisedPnL[i]);
        }
    }

    private static void notionalCost(long[] openQuantity, double bidPrice, double askPrice, double[] out)
    {
        final double quantityToDoubleScalingFactor = 0.01;
        final double cashToStandardQuantityScalingFactor = 10000.0;
        final double scalingFactor = quantityToDoubleScalingFactor * cashToStandardQuantityScalingFactor;

        for (int i = 0; i < openQuantity.length && i < out.length; i++)
        {
            long quantity = openQuantity[i];
            out[i] = quantity * scalingFactor * ((quantity < 0) ? askPrice : bidPrice);
        }
    }
}
