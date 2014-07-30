#pragma once

#include "Courier.h"
#include "Subscriber.h"

#include <typeindex>
#include <functional>
#include <unordered_map>
#include <list>

namespace courier
{

class BufferedCourier
{
public:

    BufferedCourier () = default;

    template <typename Message>
    void subscribe (Subscriber<Message>* subscriber)
    {
        Delegate f = [=] (const void* msg)
        {
            subscriber->onEvent((Message*)msg);
        };
        messageDelegates[std::type_index{ typeid(Message) }].push_back(f);
    }

    template <typename Message>
    void unsubscribe (Subscriber<Message>* subscriber)
    {
        Delegate f = [=] (const void* msg)
        {
            subscriber->onEvent((Message*)msg);
        };
        messageDelegates[std::type_index{ typeid(Message) }].remove(f);
    }

    template <typename Message>
    void send (const Message* msg)
    {
        messageEvents[std::type_index{ typeid(Message) }].push_back(msg);
        eventDeleters.push_back([=] () { delete msg; });
    }

    void dispatch ()
    {
        for (auto keyval : messageEvents)
        {
            const auto& messageType = keyval.first;
            auto& events = keyval.second;
            std::list<Delegate>& delegates = messageDelegates[messageType];
            for (const void* event : events)
            {
                for (Delegate& d : delegates) d(event);
            }
            events.clear();

            for (EventDeleter& ed : eventDeleters) ed();
            eventDeleters.clear();
        }
    }

private:

    using Delegate = std::function<void (const void*)>;

    std::unordered_map<std::type_index, std::list<Delegate>> messageDelegates;

    std::unordered_map<std::type_index, std::list<const void*>> messageEvents;

    using EventDeleter = std::function<void()>;
    std::list<EventDeleter> eventDeleters;

private:

    BufferedCourier (const BufferedCourier&)  = delete;
    BufferedCourier (const BufferedCourier&&) = delete;
    BufferedCourier& operator= (const BufferedCourier&)  = delete;
    BufferedCourier& operator= (const BufferedCourier&&) = delete;
};

} // namespace courier
