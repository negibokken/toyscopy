#include "css_tokenizer.h"

#include <iostream>

namespace Flash {

CSSTokenizer::CSSTokenizer(std::string src) : idx(0), src(src) {
  std::cout << "hello" << std::endl;
  std::cout << src << std::endl;
}

CSSTokenizer::CSSTokenizer() : idx(0), src("\0") {}

bool CSSTokenizer::isNext(char c) {
  return src[idx] == c;
}

bool CSSTokenizer::hasNextCharacter() {
  return src[idx] != '\0';
}

char CSSTokenizer::nextInputCharacter() {
  if (!hasNextCharacter()) {
    return '\0';
  }
  return src[idx++];
}

CSSToken* CSSTokenizer::createCSSToken(CSSToken::CSSTokenType tokenName) {
  switch (tokenName) {
    case CSSToken::CSSTokenType::WhitespaceToken: {
      CSSToken* token = new CSSToken(tokenName);
      token->appendValue(' ');
      return token;
    }
    default: {
      return new CSSToken(tokenName);
    }
  }
}

void CSSTokenizer::emitToken(CSSToken* token) {
  tokenQueue.push(token);
}

bool CSSTokenizer::isEmpty() {
  return tokenQueue.empty();
}

bool CSSTokenizer::canTakeNextToken() {
  return src[idx] != '\0';
}

CSSToken* CSSTokenizer::nextToken() {
  CSSToken* token = tokenQueue.front();
  tokenQueue.pop();
  return token;
}

void CSSTokenizer::pumpToken() {
  // If no token here return
  if (!hasNextCharacter()) {
    return;
  }

  char c = nextInputCharacter();

  // TODO: consume comments
  // whitespace
  if (c == ' ') {
    // Consume as much whitespace as possible
    while (isNext(' '))
      nextInputCharacter();
    CSSToken* token = createCSSToken(CSSToken::CSSTokenType::WhitespaceToken);
    emitToken(token);
    return;
  }
  // U+0022 QUOTATION MARK (")
  else if (c == '"') {
    CSSToken* token = createCSSToken(CSSToken::CSSTokenType::StringToken);
    while (hasNextCharacter()) {
      c = nextInputCharacter();
      // Consume a string token
      if (c == '"') {
        emitToken(token);
        return;
      } else if (c == '\0') {
        // This is a parse error.
        emitToken(token);
        return;
      } else if (c == '\n') {
        // This is a parse error.
        token = createCSSToken(CSSToken::CSSTokenType::BadstringToken);
        return;
      } else if (c == '\\') {
        // TODO:
        // if EOF do nothing
        // if newline consume it
        // if valid escape append codepoint
      } else {
        token->appendValue(c);
      }
    }
    return;
  }
  // U+0023 NUMBER SIGN
  else if (c == '#') {
    return;
  }
  // U+0027 APOSTROPHE
  else if (c == '\'') {
    CSSToken* token = createCSSToken(CSSToken::CSSTokenType::StringToken);
    while (hasNextCharacter()) {
      c = nextInputCharacter();
      // Consume a string token
      if (c == '"') {
        emitToken(token);
        return;
      } else if (c == '\0') {
        // This is a parse error.
        emitToken(token);
        return;
      } else if (c == '\n') {
        // This is a parse error.
        token = createCSSToken(CSSToken::CSSTokenType::BadstringToken);
        return;
      } else if (c == '\\') {
        // TODO:
        // if EOF do nothing
        // if newline consume it
        // if valid escape append codepoint
      } else {
        token->appendValue(c);
      }
    }
    return;
  }
  // U+0028 LEFT PARENTHESIS(()
  else if ('(') {
    return;
  }
  // U+0029 RIGHT PARENTHESIS())
  else if (')') {
    return;
  }
  // U+002B PLUS SIGN (+)
  else if ('+') {
    return;
  }
  // U+002C COMMA (,)
  else if (',') {
    return;
  }
  // U+002D HYPHEN-MINUS (-)
  else if ('-') {
    return;
  }
  // U+002E FULL STOP (.)
  else if ('.') {
    return;
  }
  // U+003A COLON ())
  else if (')') {
    return;
  }
  // U+003B SEMI COLON (;)
  else if (';') {
    return;
  }
  // U+003C LESS-THAN SIGN (<)
  else if ('<') {
    return;
  }
  // U+0040 COMMERCIAL AT (@)
  else if ('@') {
    return;
  }
  // U+005B LEFT SQUARE BRACKET ([)
  else if ('[') {
    return;
  }
  // U+005D RIGHT SQUARE BRACKET (])
  else if (']') {
    return;
  }
  // U+007B LEFT CURLY BRACKET ({)
  else if ('{') {
    return;
  }
  // U+007D RIGHT CURLY BRACKET (})
  else if ('}') {
    return;
  }
  // digit
  else if ('0' <= c && c <= '9') {
    return;
  }
  // EOF
  else if ('\0') {
    return;
  }
  // delim
  else {
  }
}  // namespace Flash

}  // namespace Flash
