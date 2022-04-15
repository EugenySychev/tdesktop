/*
    This file is a part of iMe code client
*/
#include <vector>

namespace Data {
class ChatFilters;
} // namespace Data

namespace IME {

class ImeSettings {

public:
    ImeSettings();

    static std::vector<Data::ChatFilter> getDefaultFilters();

};
} // namespace IME