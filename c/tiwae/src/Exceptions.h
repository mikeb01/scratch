/*
 * Copyright 2014 - 2015 Real Logic Ltd.
 * Copyright 2015 Michael Barker
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <exception>
#include <cstdint>
#include <string>
#include <stdexcept>

#ifndef TIWAE_EXCEPTIONS_H
#define TIWAE_EXCEPTIONS_H

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define SOURCEINFO  __PRETTY_FUNCTION__,  " : "  __FILE__  " : " TOSTRING(__LINE__)

class SourcedException : public std::exception
{
private:
    std::string m_where;
    std::string m_what;

public:
    SourcedException(const std::string &what, const std::string& function, const std::string& where)
        : m_where(function + where), m_what(what)
    {
    }

    virtual const char *what() const noexcept
    {
        return m_what.c_str();
    }

    const char *where() const noexcept
    {
        return m_where.c_str();
    }
};

#define DECLARE_SOURCED_EXCEPTION(exceptionName)                                            \
            class exceptionName : public SourcedException                                   \
            {                                                                               \
                public:                                                                     \
                    exceptionName (const std::string &what, const std::string& function, const std::string& where)   \
                            : SourcedException (what, function, where)                      \
                        {}                                                                  \
            }                                                                               \

DECLARE_SOURCED_EXCEPTION (IOException);

#endif //TIWAE_EXCEPTIONS_H
