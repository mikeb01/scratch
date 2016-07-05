package org.sample;


import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.Parameterized;
import org.sample.basic.rounded.BasicRiskRepository;
import org.sample.column.rounded.ColumnStoreRiskRepository;

import java.util.Arrays;
import java.util.Collection;

import static org.hamcrest.CoreMatchers.is;
import static org.junit.Assert.assertThat;

@RunWith(Parameterized.class)
public class RiskRepositoryTest
{
    private static final long INSTRUMENT_ID = 12341;
    private RiskRepository riskRepsoitory;

    public RiskRepositoryTest(String name, RiskRepository riskRepsoitory)
    {
        this.riskRepsoitory = riskRepsoitory;
    }

    @Parameterized.Parameters(name = "{0}")
    public static Collection<Object[]> params()
    {
        return Arrays.asList(new Object[][] {
                { "Column", new ColumnStoreRiskRepository(INSTRUMENT_ID, 16) },
                { "Basic", new BasicRiskRepository(INSTRUMENT_ID, 16) }
        });
    }

    @Test
    public void shouldAddAndGetRiskPosition() throws Exception
    {
        long accountId = 1231321;
        riskRepsoitory.addRiskPosition(accountId);
        RiskPosition riskPosition = riskRepsoitory.getRiskPosition(accountId);
        assertThat(riskPosition.getAccountId(), is(accountId));
    }

    @Test
    public void shouldAddAndGetMoreThanInitialSize() throws Exception
    {
        for (int i = 0; i < 64; i++)
        {
            riskRepsoitory.addRiskPosition(i + 5);
        }

        for (int i = 0; i < 64; i++)
        {
            long accountId = i + 5;
            assertThat(riskRepsoitory.getRiskPosition(accountId).getAccountId(), is(accountId));
        }
    }
}
