#pragma once

#include "dlgcpp/controls/control.h"

namespace dlgcpp
{
    namespace controls
    {
        class IProgress : public virtual IControl
        {
        public:
            virtual bool vertical() const = 0;
            virtual void vertical(bool value) = 0;
            virtual int value() const = 0;
            virtual void value(int value) = 0;
            virtual std::pair<int, int> range() const = 0;
            virtual void range(int from, int to) = 0;

            virtual IEvent<ISharedControl>& ChangedEvent() = 0;
        };

        typedef std::shared_ptr<IProgress> ISharedProgress;

        class ProgressImpl;

        class Progress :
            public Control,
            public IProgress
        {
        public:
            explicit Progress(const Position& p = Position());
            ~Progress() override;

            // IProgress impl.
            bool vertical() const;
            void vertical(bool value);
            int value() const;
            void value(int value);
            std::pair<int, int> range() const;
            void range(int from, int to);

            IEvent<ISharedControl>& ChangedEvent();

        private:
            Progress(std::shared_ptr<ProgressImpl> impl);

            std::shared_ptr<ProgressImpl> _impl;
        };
    }
}
