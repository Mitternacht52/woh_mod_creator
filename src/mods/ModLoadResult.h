#pragma once

#include "ClassificationResult.h"
#include "IModModel.h"
#include "ValidationReport.h"

#include <memory>

#include <QString>

namespace woh::mods {

	enum class ModLoadStatus {
		success,
		file_error,
		parse_error,
		classification_error,
		validation_error
	};

	struct ModLoadResult {
		QString file_path;
		ModLoadStatus status = ModLoadStatus::parse_error;
		QString message;
		ClassificationResult classification;
		ValidationReport validation;
		std::unique_ptr<IModModel> model;

		[[nodiscard]] static ModLoadResult success(QString file_path, ClassificationResult classification,
		                                           ValidationReport validation,
		                                           std::unique_ptr<IModModel> model);
		[[nodiscard]] static ModLoadResult file_error(QString file_path, QString message);
		[[nodiscard]] static ModLoadResult parse_error(QString file_path, QString message);
		[[nodiscard]] static ModLoadResult classification_error(QString file_path,
		                                                        ClassificationResult classification,
		                                                        QString message = {});
		[[nodiscard]] static ModLoadResult validation_error(QString file_path,
		                                                    ClassificationResult classification,
		                                                    ValidationReport validation,
		                                                    QString message = {});

		[[nodiscard]] bool is_success() const;
		[[nodiscard]] bool has_model() const;
	};

} // namespace woh::mods
