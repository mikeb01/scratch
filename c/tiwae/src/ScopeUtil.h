//
// Created by Michael Barker on 08/08/15.
//

#ifndef TIWAE_SCOPEUTIL_H
#define TIWAE_SCOPEUTIL_H

#include <memory>

class OnScopeExit
{
public:
    template<typename func_t>
    inline OnScopeExit(const func_t &func) : m_holder(new FuncHolder<func_t>(func))
    {
    }

private:
    struct FuncHolderBase
    {
        virtual ~FuncHolderBase()
        {
        };
    };

    template<typename func_t>
    struct FuncHolder : public FuncHolderBase
    {
        func_t f;

        inline FuncHolder(const func_t &func) : f(func)
        {
        }

        virtual ~FuncHolder()
        {
            f();
        };
    };

    std::unique_ptr<FuncHolderBase> m_holder;
};

#endif //TIWAE_SCOPEUTIL_H
