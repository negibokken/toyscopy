#include "log_util.h"

namespace ToyScopyUtil {

void logUtil(const char* fmt, ...) {
  const char* env_p = std::getenv("DEBUG");
  if (env_p == NULL || strcmp(env_p, "true") != 0) {
    return;
  }

  va_list args;
  va_start(args, fmt);
  bool isPrevPercent = false;

  while (*fmt != '\0') {
    if (*fmt == '%') {
      isPrevPercent = true;
      fmt++;
    }

    if (!isPrevPercent) {
      printf("%c", *fmt);
      fmt++;
      continue;
    }

    if (*fmt == 'd') {
      int i = va_arg(args, int);
      std::cout << i;
    } else if (*fmt == 'c') {
      // note automatic conversion to integral type
      int c = va_arg(args, int);
      std::cout << static_cast<char>(c);
    } else if (*fmt == 's') {
      // note automatic conversion to integral type
      const char* str = va_arg(args, const char*);
      std::cout << str;
    } else if (*fmt == 'f') {
      double d = va_arg(args, double);
      std::cout << d;
    }
    isPrevPercent = false;
    fmt++;
  }
  std::cout << std::endl;

  va_end(args);
}
}  // namespace ToyScopyUtil
