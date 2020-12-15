#ifndef CSSTokenizer_h
#define CSSTokenizer_h

#include <deque>
#include <string>

#include "../../utils/log_util.h"

namespace Flash {

class CSSToken {
 public:
  enum CSSTokenType {
    IdentToken,
    FunctionToken,
    AtKeywordToken,
    HashToken,
    StringToken,
    BadstringToken,
    UrlToken,
    BadurlToken,
    DelimToken,
    NumberToken,
    PercentageToken,
    DimensionToken,
    WhitespaceToken,
    CDOToken,
    CDCToken,
    ColonToken,
    SemicolonToken,
    CommaToken,
    LeftBlockBracketToken,
    RightBlockBracketToken,
    LeftBracketToken,
    RightBracketToken,
    LeftCurlyBracketToken,
    RightCurlyBracketToken,
    EOFToken,
  };

 private:
  CSSTokenType tokenType;
  std::string value;
  std::string flag;

 public:
  CSSToken(CSSTokenType tokenType) : tokenType(tokenType){};
  inline CSSTokenType getTokenType() { return tokenType; }
  inline void appendValue(char c) { value += c; }
  inline void appendValue(std::string str) { value += str; }
  inline std::string getValue() { return value; }
  inline void setFlag(std::string str) { flag = str; }
  inline std::string getFlag() { return flag; }
};

class CSSTokenizer {
  // https://drafts.csswg.org/css-syntax-3/#tokenization
  // The output of tokenization step is a stream of zero or more of the
  // following tokens: <ident-token>, <function-token>, <at-keyword-token>,
  // <hash-token>, <string-token>, <bad-string-token>, <url-token>,
  // <bad-url-token>, <delim-token>, <number-token>, <percentage-token>,
  // <dimension-token>, <whitespace-token>, <CDO-token>, <CDC-token>,
  // <colon-token>, <semicolon-token>, <comma-token>, <[-token>, <]-token>,
  // <(-token>, <)-token>, <{-token>, and <}-token>.
 private:
  int idx;
  bool isEOF;
  bool isNext(char c);
  char nextInputCharacter();
  void ignoreToken();
  CSSToken* createCSSToken(CSSToken::CSSTokenType tokenName);
  std::deque<CSSToken*> tokenQueue;
  bool isNextThreeWouldStartIdentifier();
  void reconsumeInputCharacter() { idx--; };
  CSSToken* currentInputToken;

  std::string src;

 public:
  CSSTokenizer();
  CSSTokenizer(std::string src);
  ~CSSTokenizer();
  bool pumpToken();
  bool canTakeNextToken();
  void consumeToken();
  void reconsumeToken();
  CSSToken* nextToken();
  bool isEmpty();
  void emitToken(CSSToken* token);
  bool hasNextCharacter();
};
}  // namespace Flash
#endif