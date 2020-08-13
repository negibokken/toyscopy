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

  switch (c) {
    // TODO: consume comments
    // whitespace
    case ' ': {
      // Consume as much whitespace as possible
      while (isNext(' '))
        nextInputCharacter();
      CSSToken* token = createCSSToken(CSSToken::CSSTokenType::WhitespaceToken);
      emitToken(token);
      return;
    }
    // U+0022 QUOTATION MARK (")
    case '"': {
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
    case '#': {
      break;
    }
    // U+0027 APOSTROPHE
    case '\'': {
      break;
    }
    // U+0028 LEFT PARENTHESIS(()
    case '(': {
      break;
    }
    // U+0029 RIGHT PARENTHESIS())
    case ')': {
      break;
    }
    // U+002B PLUS SIGN (+)
    case '+': {
      break;
    }
    // U+002C COMMA (,)
    case ',': {
      break;
    }
    // U+002D HYPHEN-MINUS (-)
    case '-': {
      break;
    }
    // U+002E FULL STOP (.)
    case '.': {
      break;
    }
    // U+003A COLON (:)
    case ':': {
      break;
    }
    // U+003B SEMI COLON (;)
    case ';': {
      break;
    }
    // U+003C LESS-THAN SIGN (<)
    case '<': {
      break;
    }
    // U+0040 COMMERCIAL AT (@)
    case '@': {
      break;
    }
    // U+005B LEFT SQUARE BRACKET ([)
    case '[': {
      break;
    }
    // U+005D RIGHT SQUARE BRACKET (])
    case ']': {
      break;
    }
    // U+007B LEFT CURLY BRACKET ({)
    case '{': {
      break;
    }
    // U+007D RIGHT CURLY BRACKET (})
    case '}': {
      break;
    }
    // digit
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9': {
      break;
    }
    // EOF
    case '\0': {
      break;
    }
    // delim
    default: {
    }
  }
}

}  // namespace Flash
