// Copyright (c) 2015 Baidu.com, Inc. All Rights Reserved
// Author: Ge,Jun (gejun@baidu.com)
// Date: Sun Aug  9 12:26:03 CST 2015

#include <stdlib.h>
#include <gflags/gflags.h>
#include "bvar/gflag.h"

namespace bvar {

GFlag::GFlag(const base::StringPiece& gflag_name) {
    expose(gflag_name);
}

GFlag::GFlag(const base::StringPiece& prefix,
      const base::StringPiece& gflag_name)
    : _gflag_name(gflag_name.data(), gflag_name.size()) {
    expose_as(prefix, gflag_name);
}

void GFlag::describe(std::ostream& os, bool quote_string) const {
    google::CommandLineFlagInfo info;
    if (!google::GetCommandLineFlagInfo(gflag_name().c_str(), &info)) {
        if (quote_string) {
            os << '"';
        }
        os << "Unknown gflag=" << gflag_name();
        if (quote_string) {
            os << '"';
        }
    } else {
        if (quote_string && info.type == "string") {
            os << '"' << info.current_value << '"';
        } else {
            os << info.current_value;
        }
    }
}

#ifdef BAIDU_INTERNAL
void GFlag::get_value(boost::any* value) const {
    google::CommandLineFlagInfo info;
    if (!google::GetCommandLineFlagInfo(gflag_name().c_str(), &info)) {
        *value = "Unknown gflag=" + gflag_name();
    } else if (info.type == "string") {
        *value = info.current_value;
    } else if (info.type == "int32") {
        *value = static_cast<int>(atoi(info.current_value.c_str()));
    } else if (info.type == "int64") {
        *value = static_cast<int64_t>(atoll(info.current_value.c_str()));
    } else if (info.type == "uint64") {
        *value = static_cast<uint64_t>(
            strtoull(info.current_value.c_str(), NULL, 10));
    } else if (info.type == "bool") {
        *value = (info.current_value == "true");
    } else if (info.type == "double") {
        *value = strtod(info.current_value.c_str(), NULL);
    } else {
        *value = "Unknown type=" + info.type + " of gflag=" + gflag_name();
    }
}
#endif

std::string GFlag::get_value() const {
    std::string str;
    if (!google::GetCommandLineOption(gflag_name().c_str(), &str)) {
        return "Unknown gflag=" + gflag_name();
    }
    return str;
}

bool GFlag::set_value(const char* value) {
    return !google::SetCommandLineOption(gflag_name().c_str(), value).empty();
}

}  // namespace bvar