#pragma once

#include <QString>

namespace woh::mods {

enum class ValidationSeverity {
    error
};

struct ValidationIssue {
    ValidationSeverity severity = ValidationSeverity::error;

    QString message;
    QString section;
    QString key;
    QString code;

    [[nodiscard]] bool has_location() const {
        return !section.isEmpty() || !key.isEmpty();
    }
};

} // namespace woh::mods
