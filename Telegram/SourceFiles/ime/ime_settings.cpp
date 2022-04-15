#include "data/data_chat_filters.h"
#include "ime_settings.h"
namespace IME {

ImeSettings::ImeSettings() {}

std::vector<Data::ChatFilter> ImeSettings::getDefaultFilters() {
  Data::ChatFilter filter(2, "Private", "", Data::ChatFilter::Flag::IME_Private,
                          {}, {}, {});

  return {filter};
}
} // namespace IME
