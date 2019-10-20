#include "tokenizer.h"

namespace Tokenizer {

Tokenizer::Tokenizer(std::string stream)
    : index(0), stream(stream), state(State::Data)
{
}

char Tokenizer::nextInputCharacter() { return stream[index]; }

void Tokenizer::nextToken()
{
  char cc = nextInputCharacter();
  std::cout << cc << std::endl;
  switch (state) {
    case State::Data: {
      if (cc == '<') {
        setState(State::TagOpenState);
      }
      else if (cc == '\0') {
        // TODO:
      }
      else {
        // TODO:
      }
    }
    case State::TagOpenState: {
      if (cc == '/') {
        setState(State::EndTagOpenState);
      }
      else if (('a' <= cc && cc <= 'z') || ('A' <= cc && cc <= 'Z')) {
        setState(State::TagNameState);
      }
    }
    case State::TagNameState: {
      if (cc == 'j') {
      }
    }
    default: {
    }
  }
}

void Tokenizer::setState(State state) { this->state = state; }

}  // namespace Tokenizer
