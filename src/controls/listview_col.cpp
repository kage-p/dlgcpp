#include "dlgcpp/controls/listview_col.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;


ListViewColumn::ListViewColumn(
    const std::string& text,
    int width)
{
    auto ownerFn = [this]() -> ISharedListViewColumn
        {
            return std::static_pointer_cast<dlgcpp::controls::IListViewColumn>(shared_from_this());
        };

    _text.reset(text, nullptr, ownerFn, "text");
    _width.reset(width, nullptr, ownerFn, "width");
}

ListViewColumn::ListViewColumn(
    const ListViewColumn& other) :
    ListViewColumn(other._text, other._width)
{
}

ListViewColumn& ListViewColumn::operator=(const ListViewColumn& other)
{
    if (this != &other)
    {
        _text = other._text;
        _width = other._width;
    }
    return *this;
}

bool ListViewColumn::operator==(const ListViewColumn& other) const
{
    return (
        _text == other._text &&
        _width == other._width);
}

bool ListViewColumn::operator!=(const ListViewColumn& other) const
{
    return !(*this == other);
}

IProperty<std::string, ISharedListViewColumn>& ListViewColumn::text()
{
    return _text;
}

IProperty<int, ISharedListViewColumn>& ListViewColumn::width()
{
    return _width;
}
