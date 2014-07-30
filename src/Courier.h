#pragma once

#include "Subscriber.h"

#include <typeindex>
#include <unordered_map>
#include <map>
#include <list>
#include <string>

namespace courier
{

class Courier
{
    using Listener = Subscriber<void*>;
    using MessageSubscribers = std::unordered_map<std::type_index, std::list<Listener*>>;

public:

    Courier () = default;

    template <typename Message>
    void subscribe (Subscriber<Message>* subscriber)
    {
        auto* l = reinterpret_cast<Listener*>(subscriber);
        messageSubscribers[std::type_index{ typeid(Message) }].push_back(l);
    }

    template <typename Message>
    void unsubscribe (Subscriber<Message>* subscriber)
    {
        auto* l = reinterpret_cast<Listener*>(subscriber);
        messageSubscribers[std::type_index{ typeid(Message) }].remove(l);
    }

    template <typename Message>
    void send (const Message& msg) const
    {
        MessageSubscribers::const_iterator map_it = messageSubscribers.find(std::type_index{ typeid(Message) });
        if (map_it != messageSubscribers.end())
        {
            for (Listener* l : map_it->second)
            {
                auto subscriber = reinterpret_cast<Subscriber<Message>*>(l);
                subscriber->onEvent(&msg);
            }
        }
    }

private:

    MessageSubscribers messageSubscribers;

    Courier (const Courier&)  = delete;
    Courier (const Courier&&) = delete;
    Courier& operator= (const Courier&)  = delete;
    Courier& operator= (const Courier&&) = delete;
};

} // namespace courier
