#include "EventEmitter.h"

namespace CanvasMVP
{

    std::atomic<ListenerId> EventEmitter::nextListenerId{0};
    std::unordered_map<EventType, std::vector<EventListener>> EventEmitter::listeners_;
    ListenerId EventEmitter::on(EventType eventType, std::function<void()> callback)
    {
        auto &listeners = listeners_[eventType];
        auto listenerId = nextListenerId.fetch_add(1);

        listeners.push_back(EventListener{
            .id = listenerId,
            .callback = callback});
        return listenerId;
    }

    void EventEmitter::off(EventType eventType, ListenerId id)
    {
        auto entry = listeners_.find(eventType);

        if (entry == listeners_.end())
            return;

        auto &listeners = entry->second;
        listeners.erase(
            std::remove_if(listeners.begin(), listeners.end(), [id](const EventListener &e)
                           { return e.id == id; }),
            listeners.end());
    }
    void EventEmitter::emit(EventType eventType)
    {
        auto entry = listeners_.find(eventType);

        if (entry == listeners_.end())
        {
            return;
        }

        auto &listeners = entry->second;

        for (EventListener listener : listeners)
        {
            listener.callback();
        };
    }
}