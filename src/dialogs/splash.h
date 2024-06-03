#pragma once

#include "dlgcpp.h"

#include <string>

namespace dlgcpp
{
    namespace dialogs
    {
        class ISplashDialog
        {
        public:
            virtual const std::string& logoBitmapId() const = 0;
            virtual void logoBitmapId(const std::string& value) = 0;
            virtual const std::string& message() const = 0;
            virtual void message(const std::string& value) = 0;
            virtual int timeout() const = 0;
            virtual void timeout(int value) = 0;
            virtual void show() = 0;
            virtual void close() = 0;
        };

        class SplashDialog : public ISplashDialog
        {
        public:
            explicit SplashDialog(std::shared_ptr<IDialog> parent = nullptr);

            // ISplashDialog impl.
            const std::string& logoBitmapId() const override;
            void logoBitmapId(const std::string& value) override;
            const std::string& message() const override;
            void message(const std::string& value) override;
            int timeout() const override;
            void timeout(int value) override;
            void show() override;
            void close() override;

        private:
            struct
            {
                std::string logoBitmapId;
                std::string message;
                int timeout = 800;
            } _props;

            std::shared_ptr<IDialog> _parent;
            std::shared_ptr<Dialog> _splashDialog;
        };
    }
}
