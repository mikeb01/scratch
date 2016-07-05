package org.sample;

import com.pholser.junit.quickcheck.ForAll;
import com.pholser.junit.quickcheck.From;
import com.pholser.junit.quickcheck.generator.Fields;
import org.junit.contrib.theories.Theories;
import org.junit.contrib.theories.Theory;
import org.junit.runner.RunWith;
import org.sample.basic.rounded.BasicRiskRepository;
import org.sample.column.rounded.ColumnStoreRiskRepository;

import static org.hamcrest.CoreMatchers.is;
import static org.junit.Assert.assertThat;

@RunWith(Theories.class)
public class MarginTheoryTest
{
    private static final long INSTRUMENT_ID = 231231;
    private final RiskRepository columnRiskRepository = new ColumnStoreRiskRepository(INSTRUMENT_ID, 16);
    private final RiskRepository basicRiskRepository = new BasicRiskRepository(INSTRUMENT_ID, 16);

    public static class Entry
    {
        public long accountId;
        public long quantity;
        public long cumulativeCost;
        public long realisedPnL;
        public double bidPrice;
        public double askPrice;
        public double marginRate;
    }

    @Theory
    public void valuation(@ForAll @From(Fields.class) Entry e)
    {
        addRiskPosition(e.accountId);
        applyDelta(e.accountId, e.quantity, e.cumulativeCost, e.realisedPnL);
        applyValuation(e.bidPrice, e.askPrice, e.marginRate);
        compare(e.accountId);
    }

    private void compare(long accountId)
    {
        long columnValuation = columnRiskRepository.getRiskPosition(accountId).getValuation();
        long basicValuation = basicRiskRepository.getRiskPosition(accountId).getValuation();

        assertThat(basicValuation, is(columnValuation));
    }

    private void applyValuation(double bidPrice, double askPrice, double marginRate)
    {
        basicRiskRepository.valuationNotional(bidPrice, askPrice, marginRate);
        columnRiskRepository.valuationNotional(bidPrice, askPrice, marginRate);
    }

    private void applyDelta(long accountId, long quantity, long cumulativeCost, long realisedPnL)
    {
        columnRiskRepository.getRiskPosition(accountId).updateRisk(quantity, cumulativeCost, realisedPnL);
        basicRiskRepository.getRiskPosition(accountId).updateRisk(quantity, cumulativeCost, realisedPnL);
    }

    private void addRiskPosition(long accountId)
    {
        columnRiskRepository.addRiskPosition(accountId);
        basicRiskRepository.addRiskPosition(accountId);
    }
}
