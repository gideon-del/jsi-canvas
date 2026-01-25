#pragma once
#include <atomic>
#include <unordered_map>
#include <functional>
namespace CanvasMVP
{
    using ListenerId = size_t;
    using EventType = std::string;
    struct EventListener
    {
        ListenerId id;
        std::function<void()> callback;
    };

    class EventEmitter
    {
    private:
        static std::atomic<ListenerId> nextListenerId;
        static std::unordered_map<EventType, std::vector<EventListener>> listeners_;

    public:
        static ListenerId on(EventType eventType, std::function<void()> callback);
        static void off(EventType eventType, ListenerId id);
        static void emit(EventType eventType);
    };

}