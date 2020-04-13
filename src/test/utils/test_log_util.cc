#include <gtest/gtest.h>
#include <gtest/internal/gtest-port.h>

#include <string>

#include "../../utils/log_util.h"

TEST(LogUtil, NoOutputTest) {
  testing::internal::CaptureStdout();
  const char* env_p = std::getenv("DEBUG");
  ToyScopyUtil::logUtil("no output because env is not set");
  if (env_p == NULL || strcmp(env_p, "true") != 0) {
    ASSERT_STREQ("", testing::internal::GetCapturedStdout().c_str());
  } else {
    ASSERT_STREQ("no output because env is not set\n",
                 testing::internal::GetCapturedStdout().c_str());
  }
}
