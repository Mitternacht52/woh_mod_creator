#include "ItoDocument.h"

namespace woh::ito {

ItoField* ItoSection::findField(QStringView key) noexcept {
    for (auto& field : fields) {
        if (field.key == key) {
            return &field;
        }
    }
    return nullptr;
}

const ItoField* ItoSection::findField(QStringView key) const noexcept {
    for (const auto& field : fields) {
        if (field.key == key) {
            return &field;
        }
    }
    return nullptr;
}

ItoSection* ItoDocument::findSection(QStringView name) noexcept {
    for (auto& section : sections) {
        if (section.name == name) {
            return &section;
        }
    }
    return nullptr;
}

const ItoSection* ItoDocument::findSection(QStringView name) const noexcept {
    for (const auto& section : sections) {
        if (section.name == name) {
            return &section;
        }
    }
    return nullptr;
}

}