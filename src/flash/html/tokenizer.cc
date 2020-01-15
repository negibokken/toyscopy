#include "tokenizer.h"

namespace Tokenizer {

Tokenizer::Tokenizer(std::string stream)
    : index(0), stream(stream), state(State::DataState) {}

char Tokenizer::nextInputCharacter() { return stream[index++]; }

void Tokenizer::ignoreToken(char c) {
  char cc = c;
  while (cc == '\t' || cc == 0x0A || cc == 0x0C || cc == ' ') {
    cc = nextInputCharacter();
  }
}

bool Tokenizer::pumpToken() {
  char cc = nextInputCharacter();
  ToyScopyUtil::logUtil(">%c<", cc);
  switch (state) {
    case State::DataState: {
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
        createNewToken(Tag::Token::Character);
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
      } else if (ToyScopyUtil::isASCIIAlphabet(cc)) {
        createNewToken(Tag::Token::Type::StartTag);
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
        setState(State::SelfClosingStartTagState);
        return true;
      } else if (cc == '>') {
        emitToken();
        setState(State::DataState);
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
        reconsumeToken();
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
      } else if (cc == '/') {
        setState(State::SelfClosingStartTagState);
        return true;
      } else if (cc == '=') {
        setState(State::BeforeAttributeValueState);
        return true;
      } else if (ToyScopyUtil::isASCIIUpper(cc)) {
        appendAttributeName(ToyScopyUtil::asciiUpper2lower(cc));
        return true;
      } else {
        appendAttributeName(cc);
        return true;
      }
      break;
    }
    case State::AfterAttributeNameState: {
      ToyScopyUtil::logUtil("AfterAttributeNameState: %c", cc);
      if (ToyScopyUtil::isKindOfSpace(cc)) {
        ignoreToken(cc);
        return true;
      } else if (cc == '/') {
        setState(State::SelfClosingStartTagState);
        return true;
      } else if (cc == '=') {
        setState(State::BeforeAttributeValueState);
        return true;
      } else if (cc == '>') {
        setState(State::DataState);
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
      ToyScopyUtil::logUtil("BeforeAttributeValueState: %c", cc);
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
      ToyScopyUtil::logUtil("AttributeDoubleQuotedState: %c", cc);
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
      ToyScopyUtil::logUtil("AttributeSingleQuotedState is not implemented");
      break;
    }
    case State::AttributeValueUnQuotedState: {
      ToyScopyUtil::logUtil("AttributeUnQuotedState is not implemented");
      break;
    }
    case State::AfterAttributeValueQuotedState: {
      ToyScopyUtil::logUtil("AttributeValueQuotedState: %c", cc);
      if (cc == '\t' || cc == 0x0A || cc == 0x0C || cc == ' ') {
        setState(State::BeforeAttributeNameState);
        return true;
      } else if (cc == '/') {
        setState(State::SelfClosingStartTagState);
        return true;
      } else if (cc == '>') {
        setState(State::DataState);
        emitToken();
        return true;
      }
      break;
    }
    case State::SelfClosingStartTagState: {
      if (cc == '>') {
        setState(State::DataState);
        emitToken();
        return true;
      }
      break;
    }
    case State::EndTagOpenState: {
      if (ToyScopyUtil::isASCIIAlphabet(cc)) {
        createNewToken(Tag::Token::Type::EndTag);
        appendTagName(cc);
        setState(State::TagNameState);
        return true;
      }
      break;
    }
    case State::MarkdownDeclarationOpenState: {
      const std::string DOCTYPE = "DOCTYPE";
      if (ToyScopyUtil::asciiUpper2lower(cc) ==
          ToyScopyUtil::asciiUpper2lower(DOCTYPE[0])) {
        int idx = 1;
        while (idx < DOCTYPE.length()) {
          cc = nextInputCharacter();
          if (ToyScopyUtil::asciiUpper2lower(DOCTYPE[idx++]) !=
              ToyScopyUtil::asciiUpper2lower(cc))
            break;
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
      } else if (ToyScopyUtil::isASCIIUpper(cc)) {
        createNewToken(Tag::Token::Type::DOCTYPE);
        appendTagName(ToyScopyUtil::asciiUpper2lower(cc));
        setState(State::DoctypeNameState);
        return true;
      } else if (ToyScopyUtil::isASCIIAlphabet(cc)) {
        createNewToken(Tag::Token::Type::DOCTYPE);
        appendTagName(cc);
        setState(State::DoctypeNameState);
        return true;
      }
      break;
    }
    case State::DoctypeNameState: {
      if (cc == '>') {
        setState(State::DataState);
        emitToken();
        return true;
      } else if (ToyScopyUtil::isASCIIUpper(cc)) {
        appendTagName(ToyScopyUtil::asciiUpper2lower(cc));
        return true;
      } else if (ToyScopyUtil::isASCIIAlphabet(cc)) {
        appendTagName(cc);
        return true;
      }
      break;
    }
    case State::RAWTEXTState: {
      if (cc == '<') {
        setState(State::RAWTEXTLessThanSignState);
        return true;
      } else if (cc == '\0') {
        // FIXME: Error
        return true;
      } else if (cc == EOF) {
        // FIXME: Emit EOF
        return false;
      } else {
        emitToken(cc);
        return true;
      }
      break;
    }
    case State::RAWTEXTLessThanSignState: {
      if (cc == '/') {
        clearBuffer();
        setState(State::RAWTEXTEndTagOpenState);
        return true;
      } else {
        emitToken('<');
        setState(State::RAWTEXTState);
        reconsumeToken();
        return true;
      }
      break;
    }
    case State::RAWTEXTEndTagOpenState: {
      if (ToyScopyUtil::isASCIIAlphabet(cc)) {
        createNewToken(Tag::Token::EndTag);
        setState(State::RAWTEXTEndTagNameState);
        reconsumeToken();
        return true;
      } else {
        emitToken('<');
        emitToken('/');
        reconsumeToken();
        setState(State::RAWTEXTState);
        return true;
      }
      break;
    }
    case State::RAWTEXTEndTagNameState: {
      if (ToyScopyUtil::isKindOfSpace(cc)) {
        // TODO
      } else if (cc == '/') {
        // TODO
      } else if (cc == '>') {
        if (isAppropriateEndTag()) {
          setState(State::DataState);
          emitToken();
          return true;
        }
        // Anything Else
      } else if (ToyScopyUtil::isASCIIUpper(cc)) {
        ToyScopyUtil::asciiUpper2lower(cc);
        appendTagName(cc);
        appendBuffer(cc);
        return true;
      } else if (ToyScopyUtil::isASCIILower(cc)) {
        appendTagName(cc);
        appendBuffer(cc);
        return true;
      } else {
        // TODO
      }
      break;
    }
    default: { return true; }
  }
  return true;
}

bool Tokenizer::isAppropriateEndTag() {
  return lastStartToken->getTagName() == token->getTagName();
}

void Tokenizer::setState(State state) { this->state = state; }

}  // namespace Tokenizer
