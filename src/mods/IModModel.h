#pragma once

#include "ModType.h"

namespace woh::mods {

class IModModel {

public:
	virtual ~IModModel() = default;

	[[nodiscard]] virtual ModType type() const = 0;
};

} // namespace woh::mods
