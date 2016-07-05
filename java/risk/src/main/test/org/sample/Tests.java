package org.sample;

import org.hamcrest.Description;
import org.hamcrest.Matcher;
import org.hamcrest.TypeSafeMatcher;

import java.util.function.Predicate;

public class Tests
{
    public static <T> Matcher<T> match(Predicate<T> predicate, String desc)
    {
        return new TypeSafeMatcher<T>()
        {
            @Override
            protected boolean matchesSafely(T t)
            {
                return predicate.test(t);
            }

            @Override
            public void describeTo(Description description)
            {
                description.appendText(desc);
            }
        };
    }
}
