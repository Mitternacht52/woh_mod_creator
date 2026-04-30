#pragma once

#include "mods/IModModel.h"

#include <QString>

namespace woh::mods{

struct CharacterModModel final : public IModModel {
	[[nodiscard]] ModType type() const override {
		return ModType::character;
	}

	QString name;
	QString author;
	QString contact;

	QString strength;
	QString dexterity;
	QString perception;
	QString charisma;
	QString knowledge;
	QString luck;

	QString sprite_icon;
	QString sprite_back;
	QString sprite_house;

	QString portrait_a;
	QString name_a;
	QString portrait_b;
	QString name_b;

	QString menu_tag;
	QString menu_desc;

	QString perkpack_a;
	QString perkpack_b;
};

} //namespace woh::mods
