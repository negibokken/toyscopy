#include "tokenizer.h"

namespace Tokenizer {

Tokenizer::Tokenizer(std::string stream)
    : index(0), stream(stream), state(State::Data) {}

char Tokenizer::nextInputCharacter() { return stream[index++]; }

void Tokenizer::ignoreToken(char c) {
  char cc = c;
  while (cc == '\t' || cc == 0x0A || cc == 0x0C || cc == 0x0A) {
    cc = nextInputCharacter();
  }
}

inline bool isKindOfSpace(char cc) {
  // Tab, LF, LL, Space
  return (cc == '\t' || cc == 0x0A || cc == 0x0C || cc == ' ');
}

inline bool isASCIIAlphabet(char cc) {
  return ('a' <= cc && cc <= 'z') || ('A' <= cc && cc <= 'Z');
}

inline bool isASCIIUpper(char cc) { return ('A' <= cc && cc <= 'Z'); }
inline bool asciiUpper2lower(char cc) {
  return ('A' <= cc && cc <= 'Z') ? cc + 32 : cc;
}

bool Tokenizer::nextToken() {
  char cc = nextInputCharacter();
  switch (state) {
    case State::Data: {
      if (cc == '<') {
        setState(State::TagOpenState);
        return true;
      } else if (cc == '\0') {
        // FIXME: null is not EOF
        return false;
      } else if (cc == EOF) {
        // TODO: emit an end-of-file token
        return false;
      } else {
        // TODO:
        createNewToken(Tag::Character);
        appendCharacter(cc);
        emitToken();
        return true;
      }
    } break;
    case State::TagOpenState: {
      if (cc == '/') {
        setState(State::EndTagOpenState);
        return true;
      } else if (cc == '!') {
        setState(State::MarkdownDeclarationOpenState);
        return true;
      } else if (isASCIIAlphabet(cc)) {
        createNewToken(Tag::Type::StartTag);
        setState(State::TagNameState);
        appendTagName(cc);
        return true;
      }
    } break;
    case State::TagNameState: {
      // 0x0A: LF, 0xFF: FORM FEED, 0x20: SPACE
      if (cc == '\t' || cc == 0x0A || cc == 0x0C || cc == ' ') {
        setState(State::BeforeAttributeNameState);
        return true;
      } else if (cc == '/') {
        // TODO:
        return true;
      } else if (cc == '>') {
        emitToken();
        setState(State::Data);
        return true;
      } else {
        appendTagName(cc);
        return true;
      }
      break;
    }
    case State::BeforeAttributeNameState: {
      if (cc == '\t' || cc == 0x0A || cc == 0x0C || cc == ' ') {
        ignoreToken(cc);
        return true;
      } else if (cc == '/' || cc == '>' || cc == EOF) {
        setState(State::AfterAttributeNameState);
        return true;
      } else {
        createAttribute();
        appendAttributeName(cc);
        setState(State::AttributeNameState);
        return true;
      }
      break;
    }
    case State::AttributeNameState: {
      if (cc == '\t' || cc == 0x0A || cc == 0x0C || cc == ' ' || cc == '/' ||
          cc == '>' || cc == EOF) {
        setState(State::AfterAttributeNameState);
        return true;
      } else if (cc == '=') {
        setState(State::BeforeAttributeValueState);
        return true;
      } else if (isASCIIUpper(cc)) {
        appendAttributeName(asciiUpper2lower(cc));
        return true;
      } else {
        appendAttributeName(cc);
        return true;
      }
      break;
    }
    case State::AfterAttributeNameState: {
      if (isKindOfSpace(cc)) {
        ignoreToken(cc);
        return true;
      } else if (cc == '/') {
        setState(State::SelfClosingStartTagState);
        return true;
      } else if (cc == '=') {
        setState(State::BeforeAttributeValueState);
        return true;
      } else {
        createAttribute();
        appendAttributeName(cc);
        setState(State::AttributeNameState);
        return true;
      }
      break;
    }
    case State::BeforeAttributeValueState: {
      if (cc == '\t' || cc == 0x0A || cc == 0x0C || cc == ' ') {
        ignoreToken(cc);
        return true;
      } else if (cc == '"') {
        setState(State::AttributeValueDoubleQuotedState);
        return true;
      } else if (cc == '\'') {
        setState(State::AttributeValueSingleQuotedState);
        return true;
      } else {
        setState(State::AttributeValueUnQuotedState);
        return true;
      }
      break;
    }
    case State::AttributeValueDoubleQuotedState: {
      if (cc == '"') {
        setState(State::AfterAttributeValueQuotedState);
        return true;
      } else {
        appendAttributeValue(cc);
        return true;
      }
      break;
    }
    case State::AttributeValueSingleQuotedState: {
      std::cout << "AttributeValueSingleQUotedState is not implemented"
                << std::endl;
      break;
    }
    case State::AttributeValueUnQuotedState: {
      std::cout << "AttributeValueUnQUotedState is not implemented"
                << std::endl;
      break;
    }
    case State::AfterAttributeValueQuotedState: {
      if (cc == '\t' || cc == 0x0A || cc == 0x0C || cc == ' ') {
        setState(State::BeforeAttributeNameState);
        return true;
      } else if (cc == '>') {
        setState(State::Data);
        emitToken();
        return true;
      }
      break;
    }
    case State::EndTagOpenState: {
      if (isASCIIAlphabet(cc)) {
        createNewToken(Tag::Type::EndTag);
        appendTagName(cc);
        setState(State::TagNameState);
        return true;
      }
      break;
    }
    case State::MarkdownDeclarationOpenState: {
      const std::string DOCTYPE = "DOCTYPE";
      if (cc == DOCTYPE[0]) {
        int idx = 1;
        while (idx < DOCTYPE.length()) {
          cc = nextInputCharacter();
          if (DOCTYPE[idx++] != cc) break;
        }
        if (idx == DOCTYPE.length()) {
          setState(State::DoctypeState);
          return true;
        }
        return false;
      }
      break;
    }
    case State::DoctypeState: {
      if (cc == '\t' || cc == 0x0A || cc == 0x0C || cc == ' ') {
        setState(State::BeforeDoctypeNameState);
        return true;
      }
      break;
    }
    case State::BeforeDoctypeNameState: {
      if (cc == '\t' || cc == 0x0A || cc == 0x0C || cc == 0x0A || cc == ' ') {
        ignoreToken(cc);
        return true;
      } else if (isASCIIUpper(cc)) {
        createNewToken(Tag::Type::DOCTYPE);
        appendTagName(asciiUpper2lower(cc));
        setState(State::DoctypeNameState);
        return true;
      } else if (isASCIIAlphabet(cc)) {
        createNewToken(Tag::Type::DOCTYPE);
        appendTagName(cc);
        setState(State::DoctypeNameState);
        return true;
      }
      break;
    }
    case State::DoctypeNameState: {
      if (cc == '>') {
        setState(State::Data);
        emitToken();
        return true;
      } else if (isASCIIUpper(cc)) {
        appendTagName(asciiUpper2lower(cc));
        return true;
      } else if (isASCIIAlphabet(cc)) {
        appendTagName(cc);
        return true;
      }
    }
    default: { return true; }
  }
  return true;
}

void Tokenizer::setState(State state) {
  // printf("state: %d\n", state);
  this->state = state;
}

}  // namespace Tokenizer
