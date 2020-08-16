#include "css.h"

namespace Flash {
namespace CSS {

MediaList::MediaList() {}
MediaList::~MediaList() {}
std::string MediaList::item(unsigned long index) {
  if (index > items.size() - 1) {
    return "";
  }
  return items[index];
}
void MediaList::appendMedium(std::string medium) {
  items.push_back(medium);
  length++;
}
void MediaList::deleteMedium(std::string medium) {
  for (auto it = items.begin(); it != items.end(); ++it) {
    if (*it == medium) {
      items.erase(it);
      return;
    }
  }
}

StyleSheet::StyleSheet() {}
StyleSheet::~StyleSheet() {}

}  // namespace CSS
}  // namespace Flash