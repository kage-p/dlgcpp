#include "controls/custom_impl.h"
#include "dlgcpp/controls/custom.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

Custom::Custom(const std::string& className, const Position& p)
    : Custom(std::make_shared<CustomImpl>(), p)
{
    auto ownerFn = [this]() -> ISharedControl { return std::static_pointer_cast<dlgcpp::controls::IControl>(shared_from_this()); };

    _className.reset(className, nullptr, ownerFn, "className");

    // pass a reference to the implementation class
    _impl->owner(this);
}

Custom::Custom(std::shared_ptr<CustomImpl> impl, const Position& p)
    : Control(impl, p), _impl(std::move(impl))
{
}

Custom::~Custom()
{
    _impl.reset();
}

IProperty<std::string, ISharedControl>& Custom::className()
{
    return _className;
}

void Custom::className(const std::string& value)
{
    _className = value;
}