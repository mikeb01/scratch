package org.sample;

/**
 * Created by mikeb01 on 05/06/15.
 */
public interface RiskPosition
{
    void updateRisk(long quantityDelta, long cumulativeCostDelta, long realisedPnLDelta);

    long getOpenQuantity();

    long getCumulativeCost();

    long getProfit();

    long getValuation();

    long getAccountId();
}
