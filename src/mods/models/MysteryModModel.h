#pragma once

#include "mods/IModModel.h"

#include <QString>

namespace woh::mods {

	struct MysteryIntroBlock {
		QString mystery;
		QString stage;
		QString text_one;
		QString text_two;
		QString text_thr;
	};

	struct MysteryProgressStep {
		QString loc;
		QString txt;
		QString frc;
	};

	struct MysteryProgressBlock {
		QString location;
		QString background;

		MysteryProgressStep one;
		MysteryProgressStep two;
		MysteryProgressStep thr;
		MysteryProgressStep fou;
		MysteryProgressStep fiv;
		MysteryProgressStep six;
		MysteryProgressStep sev;
		MysteryProgressStep eig;
		MysteryProgressStep nin;
		MysteryProgressStep ten;
	};

	struct MysteryRestrictedLocationsBlock {
		QString school;
		QString hospital;
		QString downtown;
		QString seaside;
		QString mansion;
		QString forest;
		QString village;
		QString apartment;
		// если в гайде/версии игры есть extra locations, их можно добавить позже
	};

	struct MysteryCardBlock {
		QString title;
		QString desc;
		QString mystery;
		QString art;

		QString effect_a;
		QString a_value;

		QString effect_b;
		QString b_value;
	};

	struct MysteryEndingBlock {
		QString end_title;
		QString end_img;
		QString end_txta;
		QString end_txtb;
		QString end_txtc;
		QString end_txtd;
	};

	struct MysteryBigEndingBlock {
		QString end_txt;
	};

	struct MysteryModModel final : public IModModel {
		[[nodiscard]] ModType type() const override {
			return ModType::mystery;
		}

		// [mystery]
		QString name;
		QString author;
		QString contact;
		QString desc;
		QString intro;
		QString mystery;
		QString can_restart;
		QString oldgod;
		QString background;
		QString ending;
		QString combat;
		QString trigger;
		QString trigger_exp;
		QString trigger_effect;
		QString music;

		// [intro]
		MysteryIntroBlock intro_block;

		// [progress]
		MysteryProgressBlock progress;

		// optional [restricted_locations]
		MysteryRestrictedLocationsBlock restricted_locations;

		// optional [card_a], [card_b]
		MysteryCardBlock card_a;
		MysteryCardBlock card_b;

		// endings
		MysteryEndingBlock ending_a;
		MysteryEndingBlock ending_b;
		MysteryEndingBlock ending_c;
		MysteryBigEndingBlock big_ending;
	};

} // namespace woh::mods
