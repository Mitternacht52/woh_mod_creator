#pragma once

#include <QString>

namespace woh::mods {

enum class ModType {
    character,
    enemy,
    event,
    mystery,
    unknown
};

[[nodiscard]] QString mod_type_to_string(ModType type);

} // namespace woh::mods
