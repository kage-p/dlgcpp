#include "dlgcpp/controls/listview_col.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

class dlgcpp::controls::ListViewColumnImpl
{
public:
    std::string text;
    int width = 0;
};

ListViewColumn::ListViewColumn() :
    _impl(new ListViewColumnImpl())
{
}

ListViewColumn::ListViewColumn(
    const std::string& text,
    int width) :
    ListViewColumn()
{
    _impl->text = text;
    _impl->width = width;
}

ListViewColumn::ListViewColumn(
    const ListViewColumn& other) :
    ListViewColumn()
{
    _impl->text = other.text();
    _impl->width = other.width();
}

ListViewColumn& ListViewColumn::operator=(const ListViewColumn& other)
{
    if (this != &other)
    {
        _impl->text = other.text();
        _impl->width = other.width();
    }
    return *this;
}

const std::string& ListViewColumn::text() const
{
    return _impl->text;
}

void ListViewColumn::text(const std::string& value)
{
    _impl->text = value;
}

int ListViewColumn::width() const
{
    return _impl->width;
}

void ListViewColumn::width(int value)
{
    _impl->width = value;
}
