#pragma once

#include "dlgcpp/defs.h"
#include "dlgcpp/event.h"

#include <functional>
#include <memory>
#include <sstream>  
#include <string>
#include <type_traits>

namespace dlgcpp
{
    /// <summary>
    /// Interface for read-only property
    /// </summary>
    /// <typeparam name="TValue">Value type</typeparam>
    /// <typeparam name="TOwner">Owner type</typeparam>
    template <class TValue, class TOwner>
    class IReadOnlyProperty
    {
    public:
        virtual bool operator==(const TValue& other) const = 0;
        virtual bool operator!=(const TValue& other) const = 0;
        virtual const TValue* operator->() const = 0;
        virtual operator TValue() const = 0;
        virtual const std::string& name() const = 0;
        virtual const TValue& value() const = 0;
        virtual bool empty() const = 0;
        virtual IEvent<TOwner>& event() = 0;
    };

    /// <summary>
    /// Interface for read-write property 
    /// </summary>
    /// <typeparam name="TValue">Value type</typeparam>
    /// <typeparam name="TOwner">Owner type</typeparam>
    template <class TValue, class TOwner>
    class IProperty :
        public IReadOnlyProperty<TValue, TOwner>
    {
    public:
        virtual IProperty& operator=(const IProperty<TValue, TOwner>& other) = 0;
        virtual IProperty& operator=(const TValue& newValue) = 0;
        virtual void clear() = 0;
    };

    // helper: check if TValue is streamable
    template<typename T>
    concept Streamable = requires(std::ostream & os, T v) {
        { os << v } -> std::same_as<std::ostream&>;
    };

    /// <summary>
    /// Property implementation with change event and optional validation 
    /// </summary>
    /// <typeparam name="TValue">Value type</typeparam>
    /// <typeparam name="TOwner">Owner type</typeparam>
    template <class TValue, class TOwner>
    class Property :
        public IProperty<TValue, TOwner>
    {
    public:
        using ValidatorFunc = std::function<bool(const TValue&)>;
        using OwnerFunc = std::function<TOwner()>;

        Property(
            const TValue& initial = TValue()) :
            _value(initial)
        {
        }

        template<typename UValue>
        Property(
            UValue&& initial,
            ValidatorFunc validatorFn,
            OwnerFunc ownerFn,
            const std::string& name) :
            _value(std::forward<UValue>(initial)),
            _validatorFn(std::move(validatorFn)),
            _ownerFn(std::move(ownerFn)),
            _name(name)
        {
            _event.reset(_ownerFn, _name + "ChangedEvent");
        }

        Property(const Property<TValue, TOwner>& other) :
            _value(other._value),  // copy, not move
            _validatorFn(other._validatorFn),
            _ownerFn(other._ownerFn),
            _name(other._name)
        {
            _event.reset(_ownerFn, _name + "ChangedEvent");
        }

        ~Property() = default;

        template<typename UValue>
        void reset(
            UValue&& initial,
            ValidatorFunc validatorFn,
            OwnerFunc ownerFn,
            const std::string& name)
        {
            _value = std::forward<UValue>(initial);
            _validatorFn = std::move(validatorFn);
            _ownerFn = std::move(ownerFn);
            _name = name;

            _event.reset(_ownerFn, _name + "ChangedEvent");
        }

        void set(const TValue& newValue, bool force = false)
        {
            if (newValue == _value)
            {
                // no change
                return;
            }

            if (!force && _validatorFn && !_validatorFn(newValue))
            {
                DLGCPP_CERR("Property '" << _name << "' validation failed and was not updated.");
                return;
            }

            _value = newValue;

            if constexpr (Streamable<TValue>)
            {
                auto stringTruncated = [](auto&& v, std::size_t maxLen = 100) {
                    std::ostringstream oss;
                    oss << v;
                    std::string s = oss.str();
                    if (s.size() > maxLen) {
                        s.resize(maxLen);
                        s += "...";
                    }
                    return s;
                    };
                DLGCPP_CMSG("Property '" << _name << "' updated to '" << stringTruncated(_value) << "'");
            }
            else
            {
                DLGCPP_CMSG("Property '" << _name << "' updated.");
            }

            if (_event.count() > 0)
            {
                // only call event if listeners attached
                // this is important as we want to set values before owner construction.
                _event.invoke();
            }
        }

        // IProperty impl.

        inline bool operator==(const TValue& other) const override
        {
            return _value == other;
        }

        inline bool operator!=(const TValue& other) const override
        {
            return _value != other;
        }

        inline IProperty<TValue, TOwner>& operator=(const IProperty<TValue, TOwner>& other) override
        {
            if (this != &other)
            {
                set(other.value());
            }
            return *this;
        }

        inline Property& operator=(const Property& other)
        {
            if (this != &other)
            {
                this->operator=(static_cast<const IProperty<TValue, TOwner>&>(other));
            }
            return *this;
        }

        inline IProperty<TValue, TOwner>& operator=(const TValue& newValue) override
        {
            set(newValue);
            return *this;
        }

        inline const TValue* operator->() const  override
        {
            return &_value;
        }

        inline operator TValue() const override
        {
            return _value;
        }

        inline const TValue& value() const override
        {
            return _value;
        }

        virtual bool empty() const
        {
            if constexpr (std::is_pointer<TValue>::value)
            {
                return _value == nullptr;
            }
            else
            {
                return _value == TValue{};
            }
        }

        inline const std::string& name() const override
        {
            return _name;
        }

        inline void clear() override
        {
            set(TValue());
        }

        // TODO: by exposing the entire event, callers can unsubscribe internal handlers. wrap +/- only
        inline IEvent<TOwner>& event() override
        {
            return _event;
        }

    private:
        TValue _value;
        std::string _name;
        ValidatorFunc _validatorFn;
        OwnerFunc _ownerFn;
        Event<TOwner> _event;
    };
}