#pragma once
#include <string>

namespace dlgcpp
{
    namespace controls
    {
        class TreeViewNode
        {
        public:
            TreeViewNode();

            // construct with tag
            TreeViewNode(
                const std::string& tag = {},
                const std::string& text = {},
                bool checked = false);

            // construct with id
            TreeViewNode(
                int id,
                const std::string& text = {},
                bool checked = false,
                const std::string& tag = {});

            virtual int id() const;
            virtual const std::string& tag() const;
            virtual const std::string& text() const;
            virtual void text(const std::string& value);
            virtual bool checked() const;
            virtual void checked(bool value);

        private:
            int _id = 0;
            std::string _text;
            std::string _tag;
            bool _checked = false;
        };
    }
}