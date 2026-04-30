#pragma once

#include "dlgcpp/defs.h"
#include "utfw/unit_test.h"

namespace dlgcpp
{
    namespace tests
    {
        class DialogTests : public UnitTestSupport::CUnitTest
        {
        protected:
            void SetUp() override;
            void TearDown() override;

            std::shared_ptr<dlgcpp::dialogs::Dialog> _target;
        };
    }
}
