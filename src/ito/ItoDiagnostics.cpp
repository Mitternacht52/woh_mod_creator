#include "ItoDiagnostics.h"

#include <utility>
#include <QStringList>

namespace woh::ito {

namespace {

QString severity_to_string(ItoDiagnosticSeverity severity) {
    switch (severity) {
    case ItoDiagnosticSeverity::info:
        return QStringLiteral("info");
    case ItoDiagnosticSeverity::warning:
        return QStringLiteral("warning");
    case ItoDiagnosticSeverity::error:
        return QStringLiteral("error");
    }

    return QStringLiteral("unknown");
}

QString format_location(const ItoDiagnosticLocation& location) {
    QStringList parts;

    if (location.line >= 0) {
        if (location.column >= 0) {
            parts.push_back(QStringLiteral("line %1, column %2")
                .arg(location.line)
                .arg(location.column));
        }
        else {
            parts.push_back(QStringLiteral("line %1").arg(location.line));
        }
    }

    if (!location.section_name.isEmpty()) {
        parts.push_back(QStringLiteral("section [%1]").arg(location.section_name));
    }

    if (!location.field_key.isEmpty()) {
        parts.push_back(QStringLiteral("field \"%1\"").arg(location.field_key));
    }

    return parts.join(QStringLiteral(", "));
}

QString format_diagnostic(const ItoDiagnostic& diagnostic) {
    const QString location = format_location(diagnostic.location);

    if (location.isEmpty()) {
        return QStringLiteral("%1: %2")
            .arg(severity_to_string(diagnostic.severity), diagnostic.message);
    }

    return QStringLiteral("%1: %2 (%3)")
        .arg(severity_to_string(diagnostic.severity), diagnostic.message, location);
}

} // namespace

bool ItoDiagnosticLocation::has_text_position() const noexcept {
    return line >= 0;
}

bool ItoDiagnosticLocation::has_section() const noexcept {
    return section_index >= 0 || !section_name.isEmpty();
}

bool ItoDiagnosticLocation::has_field() const noexcept {
    return field_index >= 0 || !field_key.isEmpty();
}

bool ItoDiagnostic::is_valid() const noexcept {
    return !message.isEmpty();
}

QString ItoDiagnostics::first_error_message() const {
    for (const auto& diagnostic : items) {
        if (diagnostic.severity == ItoDiagnosticSeverity::error) {
            return format_diagnostic(diagnostic);
        }
    }

    return {};
}

QString ItoDiagnostics::summary(std::size_t max_items) const {
    if (items.empty()) {
        return QStringLiteral("No diagnostics result");
    }

    if (max_items < 0) {
        max_items = 0;
    }

    QStringList lines;
    lines.push_back(QStringLiteral("%1 error(s), %2 warning(s), %3 info message(s)")
        .arg(error_count())
        .arg(warning_count())
        .arg(info_count()));

    const std::size_t limit = std::min(max_items, items.size());
    for (std::size_t i = 0; i < limit; ++i) {
        lines.push_back(format_diagnostic(items[i]));
    }

    if (items.size() > limit) {
        lines.push_back(QStringLiteral("... and %1 more").arg(items.size() - limit));
    }

    return lines.join(u'\n');
}

void ItoDiagnostics::add(ItoDiagnostic diagnostic) {
    if (!diagnostic.is_valid()) {
        return;
    }

    items.push_back(std::move(diagnostic));
}

void ItoDiagnostics::add(ItoDiagnosticSeverity severity, ItoDiagnosticSource source,
                         ItoDiagnosticCode code, QString message, ItoDiagnosticLocation location) {
    add(ItoDiagnostic{
        .severity = severity,
        .source = source,
        .code = std::move(code),
        .message = std::move(message),
        .location = std::move(location),
    });
}

void ItoDiagnostics::clear() noexcept {
    items.clear();
}

void ItoDiagnostics::reserve(size_type count) {
    items.reserve(count);
}

bool ItoDiagnostics::empty() const noexcept {
    return items.empty();
}

ItoDiagnostics::size_type ItoDiagnostics::size() const noexcept {
    return items.size();
}

bool ItoDiagnostics::has_errors() const noexcept {
    return error_count() > 0;
}

bool ItoDiagnostics::has_warnings() const noexcept {
    return warning_count() > 0;
}

bool ItoDiagnostics::has_info() const noexcept {
    return info_count() > 0;
}

ItoDiagnostics::size_type ItoDiagnostics::error_count() const noexcept {
    return count_by_severity(ItoDiagnosticSeverity::error);
}

ItoDiagnostics::size_type ItoDiagnostics::warning_count() const noexcept {
    return count_by_severity(ItoDiagnosticSeverity::warning);
}

ItoDiagnostics::size_type ItoDiagnostics::info_count() const noexcept {
    return count_by_severity(ItoDiagnosticSeverity::info);
}

ItoDiagnostics::iterator ItoDiagnostics::begin() noexcept {
    return items.begin();
}

ItoDiagnostics::const_iterator ItoDiagnostics::begin() const noexcept {
    return items.begin();
}

ItoDiagnostics::const_iterator ItoDiagnostics::cbegin() const noexcept {
    return items.cbegin();
}

ItoDiagnostics::iterator ItoDiagnostics::end() noexcept {
    return items.end();
}

ItoDiagnostics::const_iterator ItoDiagnostics::end() const noexcept {
    return items.end();
}

ItoDiagnostics::const_iterator ItoDiagnostics::cend() const noexcept {
    return items.cend();
}

ItoDiagnostics::size_type
ItoDiagnostics::count_by_severity(ItoDiagnosticSeverity severity) const noexcept {
    size_type count = 0;

    for (const auto& diagnostic : items) {
        if (diagnostic.severity == severity) {
            ++count;
        }
    }

    return count;
}

} // namespace woh::ito
