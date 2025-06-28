#include "dlgcpp/controls/listview_col.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

struct dlgcpp::controls::listview_col_priv
{
    std::string text;
    int role = 0;
    int width = 0;
};

ListViewColumn::ListViewColumn(const std::string& text, int role, int width) :
    _priv(new listview_col_priv())
{
    _priv->text = text;
    _priv->role = role;
    _priv->width = width;
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

int ListViewColumn::role() const
{
    return _priv->role;
}

void ListViewColumn::role(int value)
{
    _priv->role = value;
}

int ListViewColumn::width() const
{
    return _priv->width;
}

void ListViewColumn::width(int value)
{
    _priv->width = value;
}
