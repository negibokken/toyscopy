#include "../../../flash/html/util.h"

#include <utility>
#include "gtest/gtest.h"

TEST(HTMLUtil, isKindOfSpace) {
  ASSERT_EQ(true, ToyScopyUtil::isKindOfSpace(' '));
  ASSERT_EQ(true, ToyScopyUtil::isKindOfSpace('\t'));
  ASSERT_EQ(true, ToyScopyUtil::isKindOfSpace(0x0A));
  ASSERT_EQ(true, ToyScopyUtil::isKindOfSpace(0x0C));
  ASSERT_EQ(false, ToyScopyUtil::isKindOfSpace('a'));
}

TEST(HTMLUtil, asciiUpper2lower) {
  std::vector<std::pair<char, char>> testcases = {
      {' ', ' '}, {'a', 'a'}, {'a', 'A'}, {'z', 'z'}, {'z', 'Z'}, {'#', '#'}};
  for (auto testcase : testcases) {
    ASSERT_EQ(testcase.first, ToyScopyUtil::asciiUpper2lower(testcase.second));
  }
}

TEST(HTMLUtil, toASCIIlower) {
  std::vector<std::pair<std::string, std::string>> testcases = {
      {" ", " "},   {"a", "a"},   {"a", "A"}, {"aa", "AA"},
      {"aa", "Aa"}, {"az", "AZ"}, {"!", "!"},
  };
  for (auto testcase : testcases) {
    ASSERT_STREQ(testcase.first.c_str(),
                 ToyScopyUtil::toASCIIlower(testcase.second).c_str());
  }
}

TEST(HTMLUtil, isASCIIAlphabet) {
  std::vector<std::pair<bool, char>> testcases = {{false, ' '}, {true, 'a'},
                                                  {true, 'A'},  {true, 'z'},
                                                  {true, 'Z'},  {false, '#'}};
  for (auto testcase : testcases) {
    ASSERT_EQ(testcase.first, ToyScopyUtil::isASCIIAlphabet(testcase.second));
  }
}

TEST(HTMLUtil, isASCIIUpper) {
  std::vector<std::pair<bool, char>> testcases = {{false, ' '}, {false, 'a'},
                                                  {true, 'A'},  {false, 'z'},
                                                  {true, 'Z'},  {false, '#'}};
  for (auto testcase : testcases) {
    ASSERT_EQ(testcase.first, ToyScopyUtil::isASCIIUpper(testcase.second));
  }
}

TEST(HTMLUtil, isASCIILower) {
  std::vector<std::pair<bool, char>> testcases = {{false, ' '}, {true, 'a'},
                                                  {false, 'A'}, {true, 'z'},
                                                  {false, 'Z'}, {false, '#'}};
  for (auto testcase : testcases) {
    ASSERT_EQ(testcase.first, ToyScopyUtil::isASCIILower(testcase.second));
  }
}