#pragma once

#include "dialogs/dialog_impl.h"
#include "mocks/fake_dialog.h"
#include "unit_test.h"

namespace dlgcpp
{
    namespace tests
    {
        class DialogImplTests : public UnitTestSupport::CUnitTest
        {
        protected:
            void SetUp() override;
            void TearDown() override;

            std::shared_ptr<dlgcpp::dialogs::DialogImpl> _target;
            std::shared_ptr<testing::NiceMock<FakeDialog>> _fakeDialog;
        };
    }
}