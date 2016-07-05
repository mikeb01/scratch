package org.sample.basic.doubles;

import org.sample.RiskPosition;

import static java.lang.Math.round;

public class BasicRiskPosition implements RiskPosition
{
    private long accountId;
    private long openQuantity = 0;
    private long cumulativeCost = 0;
    private long realisedPnL = 0;
    private double margin;
    private double unrealisedPnL;
    private double valuation;

    public BasicRiskPosition(long accountId)
    {
        this.accountId = accountId;
    }

    @Override
    public void updateRisk(long quantityDelta, long cumulativeCostDelta, long realisedPnLDelta)
    {
        openQuantity += quantityDelta;
        cumulativeCost += cumulativeCostDelta;
        realisedPnL += realisedPnLDelta;
    }

    public void valueNotional(double bidPrice, double askPrice, double marginRate)
    {
        double notionalCost = notionalCost(openQuantity, bidPrice, askPrice);
        margin = marginNotional(notionalCost, marginRate);
        unrealisedPnL = unrealisedPnL(notionalCost, cumulativeCost, realisedPnL);
        valuation = unrealisedPnL - margin;
    }

    private double unrealisedPnL(double notionalCost, long cumulativeCost, long realisedPnL)
    {
        return notionalCost - (cumulativeCost + realisedPnL);
    }

    private double marginNotional(double notionalCost, double marginRate)
    {
        return notionalCost * marginRate;
    }

    private static double notionalCost(long quantity, double bidPrice, double askPrice)
    {
        final double quantityToDoubleScalingFactor = 0.01;
        final double cashToStandardQuantityScalingFactor = 10000.0;
        final double scalingFactor = quantityToDoubleScalingFactor * cashToStandardQuantityScalingFactor;

        return quantity * scalingFactor * ((quantity < 0) ? askPrice : bidPrice);
    }

    @Override
    public long getOpenQuantity()
    {
        return openQuantity;
    }

    @Override
    public long getCumulativeCost()
    {
        return cumulativeCost;
    }

    @Override
    public long getProfit()
    {
        return realisedPnL;
    }

    @Override
    public long getValuation()
    {
        return round(valuation);
    }

    @Override
    public long getAccountId()
    {
        return accountId;
    }
}
