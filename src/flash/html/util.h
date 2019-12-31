#ifndef Util_h
#define Util_h

#include <string>

namespace ToyScopyUtil {

inline bool isKindOfSpace(char cc) {
  // Tab, LF, LL, Space
  return (cc == '\t' || cc == 0x0A || cc == 0x0C || cc == ' ');
}

inline std::string toASCIIlower(std::string str) {
  for (int i = 0; i < str.size(); i++) {
    if ('A' <= str[i] && str[i] <= 'Z') {
      str[i] -= 32;
    }
  }
  return str;
}

inline bool isASCIIAlphabet(char cc) {
  return ('a' <= cc && cc <= 'z') || ('A' <= cc && cc <= 'Z');
}

inline bool isASCIIUpper(char cc) { return ('A' <= cc && cc <= 'Z'); }

inline bool isASCIILower(char cc) { return ('a' <= cc && cc <= 'z'); }

inline char asciiUpper2lower(char cc) {
  return ('A' <= cc && cc <= 'Z') ? cc + 32 : cc;
}

}  // namespace ToyScopyUtil

#endif