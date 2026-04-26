#include "ModLoadResult.h"

namespace woh::mods {

ModLoadResult ModLoadResult::success(QString file_path, ClassificationResult classification,
                                     ValidationReport validation, std::unique_ptr<IModModel> model) {
    ModLoadResult result;
    result.file_path = std::move(file_path);
    result.status = ModLoadStatus::success;
    result.classification = std::move(classification);
    result.validation = std::move(validation);
    result.model = std::move(model);
    return result;
}

ModLoadResult ModLoadResult::file_error(QString file_path, QString message) {
    ModLoadResult result;
    result.file_path = std::move(file_path);
    result.status = ModLoadStatus::file_error;
    result.message = std::move(message);
    return result;
}

ModLoadResult ModLoadResult::parse_error(QString file_path, QString message) {
    ModLoadResult result;
    result.file_path = std::move(file_path);
    result.status = ModLoadStatus::parse_error;
    result.message = std::move(message);
    return result;
}

ModLoadResult ModLoadResult::classification_error(QString file_path,
                                                  ClassificationResult classification,
                                                  QString message) {
    ModLoadResult result;
    result.file_path = std::move(file_path);
    result.status = ModLoadStatus::classification_error;
    result.classification = std::move(classification);
    result.message = std::move(message);
    return result;
}

ModLoadResult ModLoadResult::validation_error(QString file_path,
                                              ClassificationResult classification,
                                              ValidationReport validation, QString message) {
    ModLoadResult result;
    result.file_path = std::move(file_path);
    result.status = ModLoadStatus::validation_error;
    result.classification = std::move(classification);
    result.validation = std::move(validation);
    result.message = std::move(message);
    return result;
}

bool ModLoadResult::is_success() const {
    return status == ModLoadStatus::success && model != nullptr && classification.is_success() &&
           validation.is_valid();
}

bool ModLoadResult::has_model() const {
    return model != nullptr;
}

} // namespace woh::mods
