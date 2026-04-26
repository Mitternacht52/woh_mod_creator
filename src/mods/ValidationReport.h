#pragma once

#include "ValidationIssue.h"

#include <QList>
#include <QString>

namespace woh::mods {

class ValidationReport {

public:
    void add(const ValidationIssue& issue);
    void add(ValidationSeverity severity, const QString& message, const QString& section = {},
             const QString& key = {}, const QString& code = {});

    void add_error(const QString& message, const QString& section = {}, const QString& key = {},
                   const QString& code = {});

    [[nodiscard]] bool has_errors() const;
    [[nodiscard]] bool is_valid() const;

    [[nodiscard]] QList<ValidationIssue> errors() const;
    [[nodiscard]] const QList<ValidationIssue>& issues() const;

private:
    [[nodiscard]] QList<ValidationIssue> filter_by_severity(ValidationSeverity severity) const;

    QList<ValidationIssue> m_issues;
};

} // namespace woh::mods
