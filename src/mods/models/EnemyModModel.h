#pragma once

#include "mods/IModModel.h"

#include <QString>

namespace woh::mods {

	struct EnemyModModel final : public IModModel {
		[[nodiscard]] ModType type() const override {
			return ModType::enemy;
		}

		QString name;
		QString subtitle;
		QString type_value;
		QString location;
		QString author;
		QString intro;
		QString can_run;
		QString health;
		QString power;
		QString damagevalue;
		QString damagetype;
		QString exp;
		QString prize_type;
		QString prize_name;
		QString hit01;
		QString hit02;
		QString hit03;
		QString art01;
		QString art02;
		QString artfreq;
	};

} // namespace woh::mods
