
#pragma once

#include <string_view>

inline constexpr std::string_view FORBIDDEN_FULLNAME_CHARS =
    " \t\n\r\v\f"
    "0123456789"
    "!\"#$%&()*+,./:;<=>?@[\\]^_`{|}~";
