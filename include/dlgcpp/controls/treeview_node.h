#pragma once

#include <memory>
#include <string>

namespace dlgcpp
{
    namespace controls
    {
        class ITreeViewNode;
        typedef std::shared_ptr<ITreeViewNode> ISharedTreeViewNode;

        class ITreeViewNode
        {
        public:
            virtual int id() const = 0;
            virtual const std::string& tag() const = 0;
            virtual const std::string& text() const = 0;
            virtual void text(const std::string& value) = 0;
            virtual bool checked() const = 0;
            virtual void checked(bool value) = 0;
        };

        class TreeViewNodeImpl;

        class TreeViewNode : public ITreeViewNode
        {
        public:
            // construct with tag
            explicit TreeViewNode(
                const std::string& tag = {},
                const std::string& text = {},
                bool checked = false);

            // construct with id
            TreeViewNode(
                int id,
                const std::string& text = {},
                bool checked = false,
                const std::string& tag = {});

            ~TreeViewNode() = default;

            // ITreeViewNode impl.
            int id() const override;
            const std::string& tag() const override;
            const std::string& text() const override;
            void text(const std::string& value) override;
            bool checked() const override;
            void checked(bool value) override;

        private:
            std::shared_ptr<TreeViewNodeImpl> _impl;
        };
    }
}