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

bool isASCIIAlphabet(char cc) {
  return ('a' <= cc && cc <= 'z') || ('A' <= cc && cc <= 'Z');
}

bool Tokenizer::nextToken() {
  char cc = nextInputCharacter();
  printf("%d %c\n", state, cc);
  // std::cout << state << std::endl;
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
      }
    } break;
    case State::TagOpenState: {
      // std::cout << "Tag:OpenState:hey" << std::endl;
      // printf("%c\n", cc);
      if (cc == '/') {
        setState(State::EndTagOpenState);
        return true;
      } else if (isASCIIAlphabet(cc)) {
        setState(State::TagNameState);
        return true;
      }
    } break;
    case State::TagNameState: {
      // 0x0A: LF, 0xFF: FORM FEED, 0x20: SPACE
      if (cc == '\t' || cc == 0x0A || cc == 0x0C || cc == 0x0A) {
        setState(State::BeforeAttributeNameState);
        return true;
      } else if (cc == '/') {
        // TODO:
        return true;
      } else if (cc == '>') {
        setState(State::Data);
        return true;
      }

      break;
    }
    case State::EndTagOpenState: {
      if (isASCIIAlphabet(cc)) {
        // TODO: create new end tag token
        setState(State::TagNameState);
        return true;
      }
      break;
    }
    default: {
      return false;
    }
  }
  return true;
}

void Tokenizer::setState(State state) {
  printf("%d\n", state);
  this->state = state;
}

}  // namespace Tokenizer
