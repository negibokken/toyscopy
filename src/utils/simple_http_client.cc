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

std::string ToyScopyUtil::SimpleHttpClient::fetch(std::string url) {
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