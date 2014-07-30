#pragma once

namespace courier
{

template <typename Message>
class _Subscriber
{
public:
    virtual void onEvent (const Message*) = 0;
};

template <typename X, typename... XS>
class Subscriber : public Subscriber<X>, public Subscriber<XS...> {};

template <typename X>
class Subscriber<X> : public _Subscriber<X>{};

} // namespace courier
