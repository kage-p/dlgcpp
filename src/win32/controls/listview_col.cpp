#include "dlgcpp/controls/listview_col.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

struct dlgcpp::controls::listview_col_priv
{
    std::string text;
    int width = 0;
};

ListViewColumn::ListViewColumn() :
    _priv(new listview_col_priv())
{
}

ListViewColumn::ListViewColumn(
    const std::string& text,
    int width) :
    ListViewColumn()
{
    _priv->text = text;
    _priv->width = width;
}

ListViewColumn::ListViewColumn(
    const ListViewColumn& other) :
    ListViewColumn()
{
    _priv->text = other.text();
    _priv->width = other.width();
}

ListViewColumn& ListViewColumn::operator=(const ListViewColumn& other)
{
    if (this != &other)
    {
        _priv->text = other.text();
        _priv->width = other.width();
    }
    return *this;
}

ListViewColumn::~ListViewColumn()
{
    delete _priv;
}

const std::string& ListViewColumn::text() const
{
    return _priv->text;
}

void ListViewColumn::text(const std::string& value)
{
    _priv->text = value;
}

int ListViewColumn::width() const
{
    return _priv->width;
}

void ListViewColumn::width(int value)
{
    _priv->width = value;
}
