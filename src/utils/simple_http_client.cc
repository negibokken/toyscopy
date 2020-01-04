#include "simple_http_client.h"

namespace ToyScopyUtil {

inline size_t write(void *contents, size_t size, size_t nmemb, std::string *s) {
  size_t newLength = size * nmemb;
  try {
    s->append((char *)contents, newLength);
  } catch (std::bad_alloc &e) {
    // handle memory problem
    return 0;
  }
  return newLength;
}

inline std::string getProtocolfromUrl(std::string url) {
  int cnt = 0;
  char protocol[4096];
  for (int i = 0; i < url.size(); i++) {
    if (url[i] == ':') {
      if (i + 2 > url.size()) {
        return "invalid";
      }
      if (url[i + 1] == '/' && url[i + 2] == '/') {
        protocol[cnt] = '\0';
        return protocol;
      }
    }
    protocol[cnt] = url[i];
  }
  return protocol;
}

std::string ToyScopyUtil::SimpleHttpClient::fetch(std::string url) {
  if (getProtocolfromUrl(url) == "invalid") {
    return "";
  }
  CURL *curl = curl_easy_init();
  std::string s;
  if (curl) {
    CURLcode res;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
  }
  return s;
}

}  // namespace ToyScopyUtil