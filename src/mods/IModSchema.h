#pragma once

#include "IModModel.h"
#include "ModType.h"
#include "ValidationReport.h"

#include "ito/ItoDocument.h"

#include <memory>

#include <QString>

namespace woh::mods {

	class IModSchema {
	public:
		virtual ~IModSchema() = default;

		[[nodiscard]] virtual ModType type() const = 0;
		[[nodiscard]] virtual QString display_name() const = 0;

		[[nodiscard]] virtual bool matches_signature(const woh::ito::ItoDocument& document) const = 0;

		[[nodiscard]] virtual ValidationReport validate(const woh::ito::ItoDocument& document) const = 0;

		[[nodiscard]] virtual std::unique_ptr<IModModel> build_model(const woh::ito::ItoDocument& document) const = 0;
	};

} // namespace woh::mods
