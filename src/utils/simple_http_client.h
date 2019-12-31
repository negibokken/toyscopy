#ifndef SimpleHttpClient_h
#define SimpleHttpClient_h

#include <curl/curl.h>
#include <iostream>
#include <string>

namespace ToyScopyUtil {

class SimpleHttpClient {
 public:
  std::string fetch(std::string url);
};

}  // namespace ToyScopyUtil

#endif