#pragma once

#include "dlgcpp/defs.h"
#include <functional>
#include <optional>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>

namespace dlgcpp
{
    using EventBinding = size_t;

    template <class TOwner, class... Args>
    class IEvent
    {
    public:
        virtual EventBinding bind(std::function<void(TOwner, Args...args)> fn) = 0;
        virtual size_t count() = 0;
        virtual void clear() = 0;
        virtual const std::string& name() const = 0;
        virtual void invoke(Args... args) = 0;
        virtual EventBinding operator+=(std::function<void(TOwner, Args...args)> fn) = 0;
        virtual void operator-=(EventBinding ref) = 0;
    };

    template <class TOwner, class... Args>
    class Event : public IEvent<TOwner, Args...>
    {
    public:
        using ListenerFunc = std::function<void(TOwner, Args...)>;
        using OwnerFunc = std::function<TOwner()>;

        virtual ~Event() = default;

        void reset(OwnerFunc ownerFn, const std::string& name)
        {
            _ownerFn = std::move(ownerFn);
            _name = name;
        }

        // IEvent impl.
        inline EventBinding operator+=(ListenerFunc listener) override
        {
            return bind(listener);
        }

        inline void operator-=(EventBinding ref) override
        {
            _listenerFns.erase(ref);
        }

        inline EventBinding bind(ListenerFunc listener) override
        {
            EventBinding ref = _nextBindingRef++;
            _listenerFns[ref] = std::move(listener);
            return ref;
        }

        inline size_t count() override
        {
            return _listenerFns.size();
        }

        inline void clear() override
        {
            _listenerFns.clear();
        }

        inline const std::string& name() const override
        {
            return _name;
        }

        void invoke(Args... args) override
        {
            if (!_ownerFn || _ownerFn == nullptr)
            {
                DLGCPP_CERR("Event " << _name << " has no owner, cannot invoke");
                return;
            }

#ifdef DLGCPP_DEBUG
            std::ostringstream s_args; ((s_args << args << " "), ...);
            DLGCPP_CMSG("Event " << _name << " firing to " << _listenerFns.size() << " listeners. Args:" << s_args.str());
#endif

            if (_listenerFns.empty())
                return;

            TOwner owner = (*_ownerFn)();

            auto listeners = _listenerFns;
            for (auto& [id, fn] : listeners)
            {
                try
                {
                    fn(owner, args...);
                }
                catch (const std::exception& ex)
                {
                    DLGCPP_CMSG("Event " << _name << ":" << id << " listener threw : " << ex.what());
                }
                catch (...)
                {
                    DLGCPP_CMSG("Event " << _name << ":" << id << " listener threw an unknown exception");
                }
            }
        }

    private:
        std::string _name;
        std::unordered_map<EventBinding, ListenerFunc> _listenerFns;
        std::optional<OwnerFunc> _ownerFn;
        EventBinding _nextBindingRef = 1;
    };
}