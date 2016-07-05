/*
 * Copyright (c) 2014, Oracle America, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 *  * Neither the name of Oracle nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

package org.sample;

import org.openjdk.jmh.annotations.*;
import org.sample.basic.rounded.BasicRiskRepository;
import org.sample.column.rounded.ColumnStoreRiskRepository;

import java.util.Random;

@State(Scope.Benchmark)
public class MyBenchmark
{
    @Param
    RepositoryType type;

    @Param({"1000", "10000", "100000", "1000000", "10000000"}) int numPositions;

    RiskRepository riskRepository;

    @Setup
    public void setUp()
    {
        riskRepository = RepositoryType.newInstance(type, 12345L, numPositions);
        final Random r = new Random(5);

        for (int i = 0; i < numPositions; i++)
        {
            RiskPosition riskPosition = riskRepository.addRiskPosition(i);
            riskPosition.updateRisk(r.nextLong(), r.nextLong(), r.nextLong());
        }

        System.err.println("Completed data fill");
    }

    @Benchmark
    public void testMethod()
    {
        riskRepository.valuationNotional(0.6778, 0.6781, 0.02);
    }

    public enum RepositoryType
    {
        COLUMN, BASIC, COLUMN_2, BASIC_2;

        public static RiskRepository newInstance(RepositoryType type, long instrumentId, int numPositions)
        {
            switch (type)
            {
                case COLUMN:
                    return new ColumnStoreRiskRepository(instrumentId, numPositions);
                case COLUMN_2:
                    return new org.sample.column.doubles.ColumnStoreRiskRepository(instrumentId, numPositions);
                case BASIC:
                    return new BasicRiskRepository(instrumentId, numPositions);
                case BASIC_2:
                    return new org.sample.basic.doubles.BasicRiskRepository(instrumentId, numPositions);
                default:
                    throw new IllegalStateException();
            }
        }
    }

}
