#ifndef CSSTokenizer_h
#define CSSTokenizer_h

#include <queue>
#include <string>

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
  };

 private:
  CSSTokenType tokenType;
  std::string value;

 public:
  CSSToken(CSSTokenType tokenType) : tokenType(tokenType){};
  inline CSSTokenType getTokenType() { return tokenType; }
  inline void appendValue(char c) { value += c; }
  inline void appendValue(std::string str) { value += str; }
  inline std::string getValue() { return value; }
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
  bool isNext(char c);
  char nextInputCharacter();
  void ignoreToken();
  bool hasNextCharacter();
  CSSToken* createCSSToken(CSSToken::CSSTokenType tokenName);
  std::queue<CSSToken*> tokenQueue;

  std::string src;

 public:
  CSSTokenizer();
  CSSTokenizer(std::string src);
  void pumpToken();
  bool canTakeNextToken();
  void consumeToken();
  CSSToken* nextToken();
  bool isEmpty();
  void emitToken(CSSToken* token);
};
}  // namespace Flash
#endif