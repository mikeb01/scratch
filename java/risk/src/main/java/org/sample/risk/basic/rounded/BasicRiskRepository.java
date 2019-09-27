package org.sample.risk.basic.rounded;

import org.sample.RiskPosition;
import org.sample.RiskRepository;

import java.util.Collection;
import java.util.HashMap;
import java.util.Map;
import java.util.Objects;

public class BasicRiskRepository implements RiskRepository
{
    private final Map<Long, BasicRiskPosition> accountId2Position;
    private final long instrumentId;

    public BasicRiskRepository(long instrumentId, int numPositions)
    {
        this.instrumentId = instrumentId;
        accountId2Position = new HashMap<>(numPositions);
    }

    @Override
    public void valuationNotional(double bidPrice, double askPrice, double marginRate)
    {
        Collection<BasicRiskPosition> positions = accountId2Position.values();
        for (BasicRiskPosition position : positions)
        {
            position.valueNotional(bidPrice, askPrice, marginRate);
        }
    }

    @Override
    public RiskPosition getRiskPosition(long accountId)
    {
        BasicRiskPosition position = accountId2Position.get(accountId);
        Objects.requireNonNull(position);

        return position;
    }

    @Override
    public RiskPosition addRiskPosition(long accountId)
    {
        BasicRiskPosition position = accountId2Position.get(accountId);
        if (null == position)
        {
            position = new BasicRiskPosition(accountId);
            accountId2Position.put(accountId, position);
        }

        return position;
    }
}
