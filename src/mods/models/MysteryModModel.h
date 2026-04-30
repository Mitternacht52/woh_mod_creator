#pragma once

#include "mods/IModModel.h"

#include <QString>

namespace woh::mods {

	struct MysteryIntroBlock {
		QString art;
		QString text_one;
		QString text_two;
		QString text_thr;
		QString text_fou;
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
	};

	struct MysteryCardBlock {
		QString name;
		QString trigger;
		QString art;
		QString flavor;
		QString tags;
		QString a_effect;
		QString a_value;
		QString b_effect;
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
		QString end_txta;
		QString end_txtb;
		QString end_txtc;
	};

	struct MysteryModModel final : public IModModel {
		[[nodiscard]] ModType type() const override {
			return ModType::mystery;
		}

		// [mystery]
		QString name;
		QString author;
		QString art;
		QString description;
		QString custom_ui;
		QString mystery_sound;
		QString combat_sound;

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
