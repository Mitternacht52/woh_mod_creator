#include "ItoDiagnostics.h"

#include <utility>

namespace woh::ito {

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

void ItoDiagnostics::add(ItoDiagnostic diagnostic) {
    if (!diagnostic.is_valid()) {
        return;
    }

    items.push_back(std::move(diagnostic));
}

void ItoDiagnostics::add(ItoDiagnosticSeverity severity, ItoDiagnosticSource source,
                         ItoDiagnosticCode code, QString message,
                         ItoDiagnosticLocation location) {
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
