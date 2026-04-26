#include "ValidationReport.h"

namespace woh::mods {

void ValidationReport::add(const ValidationIssue& issue) {
    m_issues.push_back(issue);
}

void ValidationReport::add(ValidationSeverity severity, const QString& message,
                           const QString& section, const QString& key, const QString& code) {
    add({severity, message, section, key, code});
}

void ValidationReport::add_error(const QString& message, const QString& section,
                                 const QString& key, const QString& code) {
    add(ValidationSeverity::error, message, section, key, code);
}

bool ValidationReport::has_errors() const {
    for (const auto& issue : m_issues) {
        if (issue.severity == ValidationSeverity::error) {
            return true;
        }
    }

    return false;
}

bool ValidationReport::is_valid() const {
    return !has_errors();
}

QList<ValidationIssue> ValidationReport::errors() const {
    return filter_by_severity(ValidationSeverity::error);
}

const QList<ValidationIssue>& ValidationReport::issues() const {
    return m_issues;
}

QList<ValidationIssue> ValidationReport::filter_by_severity(ValidationSeverity severity) const {
    QList<ValidationIssue> result;

    for (const auto& issue : m_issues) {
        if (issue.severity == severity) {
            result.push_back(issue);
        }
    }

    return result;
}

} // namespace woh::mods
