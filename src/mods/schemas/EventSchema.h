#pragma once

#include "mods/IModSchema.h"

#include <memory>

namespace woh::mods {

	class EventSchema final : public IModSchema {
	public:
		[[nodiscard]] ModType type() const override;
		[[nodiscard]] QString display_name() const override;
		[[nodiscard]] bool matches_signature(const woh::ito::ItoDocument& document) const override;
		[[nodiscard]] ValidationReport validate(const woh::ito::ItoDocument& document) const override;
		[[nodiscard]] std::unique_ptr<IModModel> build_model(const woh::ito::ItoDocument& document) const override;
	};

} // namespace woh::mods
