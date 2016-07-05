package org.sample;

/**
 * Created by mikeb01 on 05/06/15.
 */
public interface RiskRepository
{
    void valuationNotional(double bidPrice, double askPrice, double marginRate);
    RiskPosition getRiskPosition(long accountId);
    RiskPosition addRiskPosition(long accountId);
}
