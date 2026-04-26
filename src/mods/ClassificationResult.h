#pragma once

#include "ModType.h"

#include <QList>
#include <QString>

namespace woh::mods {

enum class ClassificationStatus {
    success,
    unknown,
    ambiguous
};

struct ClassificationResult {
    ClassificationStatus status = ClassificationStatus::unknown;
    ModType type = ModType::unknown;
    QList<ModType> matched_types;
    QString message;

    [[nodiscard]] bool is_success() const {
        return status == ClassificationStatus::success;
    }

    [[nodiscard]] bool is_unknown() const {
        return status == ClassificationStatus::unknown;
    }

    [[nodiscard]] bool is_ambiguous() const {
        return status == ClassificationStatus::ambiguous;
    }

    [[nodiscard]] bool has_errors() const {
        return !is_success();
    }
};

} // namespace woh::mods
