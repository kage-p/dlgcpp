#include "controls/custom_p.h"
#include "dlgcpp/controls/custom.h"

using namespace dlgcpp::controls;

Custom::Custom(const std::string& className, const Position& p)
    : Custom(std::make_shared<CustomImpl>(className, p))
{
}

Custom::Custom(std::shared_ptr<CustomImpl> impl)
    : Control(impl), _impl(std::move(impl))
{
}

Custom::~Custom()
{
}
