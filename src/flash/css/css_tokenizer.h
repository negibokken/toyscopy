#ifndef CSSTokenizer_h
#define CSSTokenizer_h

#include <queue>

namespace Flash {

class CSSToken;

class CSSTokenizer {
  // https://drafts.csswg.org/css-syntax-3/#tokenization
  // The output of tokenization step is a stream of zero or more of the
  // following tokens: <ident-token>, <function-token>, <at-keyword-token>,
  // <hash-token>, <string-token>, <bad-string-token>, <url-token>,
  // <bad-url-token>, <delim-token>, <number-token>, <percentage-token>,
  // <dimension-token>, <whitespace-token>, <CDO-token>, <CDC-token>,
  // <colon-token>, <semicolon-token>, <comma-token>, <[-token>, <]-token>,
  // <(-token>, <)-token>, <{-token>, and <}-token>.
 public:
  enum {
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

  std::queue<CSSToken*> tokenQueue;

 private:
  char nextInputCharacter();
  void ignoreToken();
  bool hasNextCharacter();

 public:
  CSSTokenizer();
  void pumpToken();
  bool canTakeNextToken();
  void consumeToken();
  CSSToken* nextToken();
  void emitToken();
};
}  // namespace Flash
#endif