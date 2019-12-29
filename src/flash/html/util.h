#ifndef Util_h
#define Util_h

#include <string>

namespace ToyScopyUtil {

inline std::string toASCIIlower(std::string str) {
  for (int i = 0; i < str.size(); i++) {
    if ('A' <= str[i] && str[i] <= 'Z') {
      str[i] -= 32;
    }
  }
  return str;
}

}  // namespace ToyScopyUtil

#endif