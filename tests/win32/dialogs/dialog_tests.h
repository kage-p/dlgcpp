#pragma once

#include "mocks/mock_dialog.h"
#include "support/unit_test.h"

namespace dlgcpp
{
    namespace tests
    {
        class DialogImplTests : public UnitTestSupport::CUnitTest
        {
        protected:
            void SetUp() override;
            void TearDown() override;

            std::shared_ptr<testing::NiceMock<MockDialog>> _mockDialog;
        };
    }
}