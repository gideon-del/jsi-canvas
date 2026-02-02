#pragma once
#include <atomic>
#include <unordered_map>
#include <functional>
#include <variant>
#include <jsi/jsi.h>

using namespace facebook;
namespace CanvasMVP
{
    using ListenerId = size_t;
    using EventType = std::string;

    struct EventData
    {
        EventType type;
        using EventValue = std::variant<std::string,
                                        int,
                                        float,
                                        bool,
                                        EventData,
                                        std::vector<EventData>>;
        std::unordered_map<std::string, EventValue> data;

        void set(std::string key, EventValue value)
        {
            data[key] = value;
        }

        jsi::Value convertValue(jsi::Runtime &runtime, const EventValue &value) const
        {
            return std::visit([&](auto &&arg) -> jsi::Value
                              {
                                  using T = std::decay_t<decltype(arg)>;

                                  if constexpr (std::is_same_v<T, std::string>)
                                  {
                                      return jsi::String::createFromUtf8(runtime, arg);
                                  }
                                  else if constexpr (std::is_same_v<T, int>)
                                  {
                                      return jsi::Value(arg);
                                  }
                                  else if constexpr (std::is_same_v<T, float>)
                                  {
                                      return jsi::Value(static_cast<double>(arg));
                                  }
                                  else if constexpr (std::is_same_v<T, bool>)
                                  {
                                      return jsi::Value(arg);
                                  }
                                  else if constexpr (std::is_same_v<T, EventData>)
                                  {
                                      return arg.toJSI(runtime);
                                  }
                                  else if constexpr (std::is_same_v<T, std::vector<EventData>>)
                                  {
                                      jsi::Array arr(runtime, arg.size());
                                      for (size_t i = 0; i < arg.size(); ++i)
                                      {
                                          arr.setValueAtIndex(runtime, i, arg[i].toJSI(runtime));
                                      }
                                      return arr;
                                  }
                                  else
                                  {
                                      return jsi::Value::undefined();
                                  } },
                              value);
        }

        jsi::Object toJSI(jsi::Runtime &runtime) const
        {
            jsi::Object obj(runtime);

            obj.setProperty(runtime, "type", jsi::String::createFromUtf8(runtime, type));
            for (const auto &[key, val] : data)
            {
                obj.setProperty(
                    runtime,
                    key.c_str(),
                    convertValue(runtime, val));
            }

            return obj;
        };
    };

    struct EventListener
    {
        ListenerId id;
        std::function<void(EventData event)> callback;
    };

    class EventEmitter
    {
    private:
        static std::atomic<ListenerId> nextListenerId;
        static std::unordered_map<EventType, std::vector<EventListener>> listeners_;

    public:
        static ListenerId on(EventType eventType, std::function<void(EventData event)> callback);
        static void off(EventType eventType, ListenerId id);
        static void emit(EventData event);
    };

}