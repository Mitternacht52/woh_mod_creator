#pragma once

#include "mods/IModModel.h"

#include <QString>

namespace woh::mods {

	struct EventOptionBlock {
		QString option;
		QString test;
		QString success;
		QString winprize;
		QString winnumber;
		QString failure;
		QString failprize;
		QString failnumber;

		QString wineffect;
		QString faileffect;
		QString extra_winprize;
		QString extra_winnumber;
		QString extra_failprize;
		QString extra_failnumber;
	};

	struct EventModModel final : public IModModel {
		[[nodiscard]] ModType type() const override {
			return ModType::event;
		}

		QString name;
		QString location;
		QString author;
		QString contact;
		QString flavor;
		QString options;

		QString image;
		QString about;

		EventOptionBlock option_a;
		EventOptionBlock option_b;
		EventOptionBlock option_c;

		QString wavy_art;
		QString wavy_speed;
	};

} // namespace woh::mods
