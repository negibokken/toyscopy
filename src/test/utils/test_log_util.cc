#include <gtest/gtest.h>
#include <gtest/internal/gtest-port.h>
#include <string>

#include "../../utils/log_util.h"

TEST(LogUtil, NoOutputTest) {
  testing::internal::CaptureStdout();
  ToyScopyUtil::logUtil("no output because env is not set");
  ASSERT_STREQ("", testing::internal::GetCapturedStdout().c_str());
}
