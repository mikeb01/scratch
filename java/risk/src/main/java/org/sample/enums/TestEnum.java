package org.sample.enums;

public enum TestEnum
{
    A, B, C, D, E;

    public static final Enum<TestEnum>[] enums = TestEnum.class.getEnumConstants();


}
