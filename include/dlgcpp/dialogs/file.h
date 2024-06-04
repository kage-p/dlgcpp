#pragma once

#include "dlgcpp/dlgcpp.h"
#pragma once

#include <memory>
#include <string>

namespace dlgcpp
{
    namespace dialogs
    {
        class IFileDialog
        {
        public:
            virtual const std::string& fileName() const = 0;
            virtual void fileName(const std::string&) = 0;
            virtual const std::string& filters() const = 0;
            virtual void filters(const std::string&) = 0;
            virtual const std::string& title() const = 0;
            virtual void title(const std::string&) = 0;
            virtual bool create() = 0;
            virtual bool open() = 0;
            virtual bool save() = 0;
        };

        class FileDialog : public IFileDialog
        {
        public:
            explicit FileDialog(std::shared_ptr<IDialog> parent = nullptr);
            virtual ~FileDialog();

            // IFileDialog impl.
            const std::string& fileName() const override;
            void fileName(const std::string&) override;
            const std::string& filters() const override;
            void filters(const std::string&) override;
            const std::string& title() const override;
            void title(const std::string&) override;
            bool create() override;
            bool open() override;
            bool save() override;

        private:
            struct file_props* _props;

            bool show(bool isSaveFile, unsigned int flags, const std::string& defaultTitle);
        };
    }
}
