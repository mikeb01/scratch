package org.sample.risk.column.doubles;

import org.sample.RiskPosition;

public class ColumnStoreRiskPosition implements RiskPosition
{
    private final ColumnStoreRiskRepository repository;
    private long accountId;

    public ColumnStoreRiskPosition(ColumnStoreRiskRepository repository, long accountId)
    {
        this.repository = repository;
        this.accountId = accountId;
    }

    @Override
    public long getAccountId()
    {
        return accountId;
    }

    @Override
    public long getOpenQuantity()
    {
        return repository.getOpenQuantity(accountId);
    }

    @Override
    public long getCumulativeCost()
    {
        return repository.getCumulativeCost(accountId);
    }

    @Override
    public long getProfit()
    {
        return repository.getProfit(accountId);
    }

    @Override
    public long getValuation()
    {
        return repository.getValuation(accountId);
    }

    @Override
    public void updateRisk(long quantityDelta, long cumulativeCostDelta, long realisedPnLDelta)
    {
        repository.updateRisk(accountId, quantityDelta, cumulativeCostDelta, realisedPnLDelta);
    }
}
