#include <gtest/gtest.h>
#include <gtest/internal/gtest-port.h>
#include <string>

#include "../../utils/simple_http_client.h"

TEST(SimpleHttpClient, NormalFetchTest) {
  std::string expected = "<!doctype html>";
  ToyScopyUtil::SimpleHttpClient *cl = new ToyScopyUtil::SimpleHttpClient();

  std::string actual = cl->fetch("https://example.com");
  for (int i = 0; i < expected.length(); i++) {
    ASSERT_EQ(actual[i], expected[i]);
  }
}

TEST(SimpleHttpClient, InvalidUrlFetchTest) {
  ToyScopyUtil::SimpleHttpClient *cl = new ToyScopyUtil::SimpleHttpClient();

  std::string actual = cl->fetch("invalid:/url");
  ASSERT_EQ(actual, "");
}