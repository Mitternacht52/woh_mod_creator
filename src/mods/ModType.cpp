#include "ModType.h"

namespace woh::mods {

QString mod_type_to_string(ModType type) {
	switch (type) {
	case ModType::character:
		return QStringLiteral("character");
	case ModType::enemy:
		return QStringLiteral("enemy");
	case ModType::event:
		return QStringLiteral("event");
	case ModType::mystery:
		return QStringLiteral("mystery");
	case ModType::unknown:
	default:
		return QStringLiteral("unknown");
	}
}

} // namespace woh::mods