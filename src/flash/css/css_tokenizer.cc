#include "css_tokenizer.h"

#include <iostream>

namespace Flash {

// A code point with a value
// equal to or greater than U+0080 <control>.
bool isNonASCII(char c) {
  // return c >= 0x80;
  return false;
}

bool isDigitChar(char c) {
  return '0' <= c && c <= '9';
}

bool isIdentifierStart(char c) {
  return (isupper(c) || islower(c) || isNonASCII(c) || c == '_');
}

bool isIdentifier(char c) {
  return (isupper(c) || islower(c) || isdigit(c) || c == '-');
}

bool CSSTokenizer::isNextThreeWouldStartIdentifier() {
  if (idx + 2 >= src.size()) {
    return false;
  }
  return isIdentifierStart(src[idx]) && isIdentifierStart(src[idx + 1]) &&
         isIdentifierStart(src[idx + 2]);
}

CSSTokenizer::CSSTokenizer(std::string src) : idx(0), src(src), isEOF(false) {}
CSSTokenizer::CSSTokenizer() : idx(0), src("\0"), isEOF(false) {}

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
  return !tokenQueue.empty();
}

CSSToken* CSSTokenizer::nextToken() {
  CSSToken* token = tokenQueue.front();
  tokenQueue.pop();
  return token;
}

bool CSSTokenizer::pumpToken() {
  // If no token here return
  if (!hasNextCharacter() && isEOF) {
    return false;
  }

  if (isNext('\0')) {
    CSSToken* token = createCSSToken(CSSToken::CSSTokenType::EOFToken);
    token->appendValue('\0');
    emitToken(token);
    isEOF = true;
    return false;
  }

  char c = nextInputCharacter();

  // std::cout << "C: " << c << std::endl;

  // TODO: consume comments
  // whitespace
  if (c == ' ') {
    // std::cout << "whitespace" << std::endl;
    // Consume as much whitespace as possible
    while (isNext(' '))
      nextInputCharacter();
    CSSToken* token = createCSSToken(CSSToken::CSSTokenType::WhitespaceToken);
    emitToken(token);
    return true;
  }
  // U+0022 QUOTATION MARK (")
  else if (c == '"') {
    CSSToken* token = createCSSToken(CSSToken::CSSTokenType::StringToken);
    while (hasNextCharacter()) {
      c = nextInputCharacter();
      // Consume a string token
      if (c == '"') {
        emitToken(token);
        return true;
      } else if (c == '\0') {
        // This is a parse error.
        emitToken(token);
        return true;
      } else if (c == '\n') {
        // This is a parse error.
        token = createCSSToken(CSSToken::CSSTokenType::BadstringToken);
        return true;
      } else if (c == '\\') {
        // TODO:
        // if EOF do nothing
        // if newline consume it
        // if valid escape append codepoint
        return true;
      } else {
        token->appendValue(c);
      }
    }
    return true;
  }
  // U+0023 NUMBER SIGN
  else if (c == '#') {
    CSSToken* token = createCSSToken(CSSToken::CSSTokenType::HashToken);
    token->appendValue(c);
    if (isNextThreeWouldStartIdentifier()) {
      token->setFlag("id");
    }
    // Consume identifier
    c = nextInputCharacter();
    while (isIdentifier(c)) {
      token->appendValue(c);
      if (!hasNextCharacter())
        break;
      c = nextInputCharacter();
    }
    reconsumeToken();
    emitToken(token);
    return true;
  }
  // U+0027 APOSTROPHE
  else if (c == '\'') {
    CSSToken* token = createCSSToken(CSSToken::CSSTokenType::StringToken);
    while (hasNextCharacter()) {
      c = nextInputCharacter();
      // Consume a string token
      if (c == '\'') {
        emitToken(token);
        return true;
      } else if (c == '\0') {
        // This is a parse error.
        emitToken(token);
        return true;
      } else if (c == '\n') {
        // This is a parse error.
        token = createCSSToken(CSSToken::CSSTokenType::BadstringToken);
        return true;
      } else if (c == '\\') {
        // TODO:
        // if EOF do nothing
        // if newline consume it
        // if valid escape append codepoint
        return true;
      } else {
        token->appendValue(c);
      }
    }
    return true;
  }
  // U+0028 LEFT PARENTHESIS(()
  else if (c == '(') {
    // std::cout << "(" << c << std::endl;
    CSSToken* token = createCSSToken(CSSToken::CSSTokenType::LeftBracketToken);
    token->appendValue(c);
    emitToken(token);
    return true;
  }
  // U+0029 RIGHT PARENTHESIS())
  else if (c == ')') {
    // std::cout << ")" << std::endl;
    CSSToken* token = createCSSToken(CSSToken::CSSTokenType::RightBracketToken);
    token->appendValue(c);
    emitToken(token);
    return true;
  }
  // U+002B PLUS SIGN (+)
  else if (c == '+') {
    // std::cout << "+" << std::endl;
    return true;
  }
  // U+002C COMMA (,)
  else if (c == ',') {
    // std::cout << "," << std::endl;
    CSSToken* token = createCSSToken(CSSToken::CSSTokenType::CommaToken);
    token->appendValue(c);
    emitToken(token);
    return true;
  }
  // U+002D HYPHEN-MINUS (-)
  else if (c == '-') {
    // std::cout << "-" << std::endl;
    return true;
  }
  // U+002E FULL STOP (.)
  else if (c == '.') {
    // std::cout << "." << std::endl;
    return true;
  }
  // U+003A COLON ())
  else if (c == ':') {
    // std::cout << ":" << std::endl;
    CSSToken* token = createCSSToken(CSSToken::CSSTokenType::ColonToken);
    token->appendValue(c);
    emitToken(token);
    return true;
  }
  // U+003B SEMI COLON (;)
  else if (c == ';') {
    // std::cout << ";" << std::endl;
    CSSToken* token = createCSSToken(CSSToken::CSSTokenType::SemicolonToken);
    token->appendValue(c);
    emitToken(token);
    return true;
  }
  // U+003C LESS-THAN SIGN (<)
  else if (c == '<') {
    // std::cout << "<" << std::endl;
    return true;
  }
  // U+0040 COMMERCIAL AT (@)
  else if (c == '@') {
    // std::cout << "@" << std::endl;
    return true;
  }
  // U+005B LEFT SQUARE BRACKET ([)
  else if (c == '[') {
    CSSToken* token =
        createCSSToken(CSSToken::CSSTokenType::LeftBlockBracketToken);
    token->appendValue(c);
    emitToken(token);
    return true;
  }
  // U+005D RIGHT SQUARE BRACKET (])
  else if (c == ']') {
    // std::cout << "]" << std::endl;
    CSSToken* token =
        createCSSToken(CSSToken::CSSTokenType::RightBlockBracketToken);
    token->appendValue(c);
    emitToken(token);
    return true;
  }
  // U+007B LEFT CURLY BRACKET ({)
  else if (c == '{') {
    // std::cout << "{" << std::endl;
    CSSToken* token =
        createCSSToken(CSSToken::CSSTokenType::LeftCurlyBracketToken);
    token->appendValue(c);
    emitToken(token);
    return true;
  }
  // U+007D RIGHT CURLY BRACKET (})
  else if (c == '}') {
    // std::cout << "}" << std::endl;
    CSSToken* token =
        createCSSToken(CSSToken::CSSTokenType::RightCurlyBracketToken);
    token->appendValue(c);
    emitToken(token);
    return true;
  }
  // digit
  else if (c == '0' <= c && c <= '9') {
    // std::cout << "num: " << c << std::endl;
    return true;
  }
  // EOF
  else if (c == '\0') {
    // std::cout << "null" << std::endl;
    CSSToken* token = createCSSToken(CSSToken::CSSTokenType::EOFToken);
    token->appendValue(c);
    emitToken(token);
    isEOF = true;
    return false;
  }
  // identifier-start code point
  else if (isIdentifierStart(c)) {
    CSSToken* token = createCSSToken(CSSToken::CSSTokenType::IdentToken);
    while (isIdentifierStart(c) || isDigitChar(c) || c == '-') {
      token->appendValue(c);
      if (!hasNextCharacter())
        break;
      c = nextInputCharacter();
    }
    reconsumeToken();
    emitToken(token);
    return true;
  }
  // delim
  else {
    std::cout << "else" << std::endl;
  }
  return true;
}  // namespace Flash

}  // namespace Flash
