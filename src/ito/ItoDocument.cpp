#include "ItoDocument.h"

namespace woh::ito {

ItoField* ItoSection::find_field(QStringView key) noexcept {
    for (auto& field : fields) {
        if (field.key == key) {
            return &field;
        }
    }
    return nullptr;
}

const ItoField* ItoSection::find_field(QStringView key) const noexcept {
    for (const auto& field : fields) {
        if (field.key == key) {
            return &field;
        }
    }
    return nullptr;
}

ItoSection* ItoDocument::find_section(QStringView name) noexcept {
    for (auto& section : sections) {
        if (section.name == name) {
            return &section;
        }
    }
    return nullptr;
}

const ItoSection* ItoDocument::find_section(QStringView name) const noexcept {
    for (const auto& section : sections) {
        if (section.name == name) {
            return &section;
        }
    }
    return nullptr;
}

} // namespace woh::ito