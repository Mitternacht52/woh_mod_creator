#pragma once

#include <QString>

#include <vector>
#include <variant>

namespace woh::ito {

enum class ItoDiagnosticSeverity {
	info, 
	warning,
	error,
};

enum class ItoDiagnosticSource {
	parser,
	writer,
	validator,
};

enum class ItoParserDiagnosticCode {
	file_open_failed,
	field_outside_section,
	empty_section_name,
	expected_key_value,
	empty_field_key,
	value_must_be_quoted,
	unescaped_quote,
	unterminated_escape,
};

enum class ItoWriterDiagnosticCode {
	empty_section_name,
	empty_section,
	empty_field_key,
	file_open_failed,
	file_write_failed,
};

enum class ItoValidatorDiagnosticCode {
	duplicate_section,
	duplicate_field,
};

struct ItoDiagnosticLocation {
    qsizetype line = -1;
    qsizetype column = -1;

	qsizetype section_index = -1;
    qsizetype field_index = -1;

	QString section_name;
    QString field_key;

	[[nodiscard]] bool has_text_position() const noexcept;
    [[nodiscard]] bool has_section() const noexcept;
    [[nodiscard]] bool has_field() const noexcept;
};

using ItoDiagnosticCode =
    std::variant<ItoParserDiagnosticCode, ItoWriterDiagnosticCode, ItoValidatorDiagnosticCode>;

struct ItoDiagnostic {
    ItoDiagnosticSeverity severity;
    ItoDiagnosticSource source;
    ItoDiagnosticCode code;
    QString message;
    ItoDiagnosticLocation location;

	[[nodiscard]] bool is_valid() const noexcept;
};

struct ItoDiagnostics {
	using container_type = std::vector<ItoDiagnostic>;
	using value_type = container_type::value_type;
	using size_type = container_type::size_type;
	using iterator = container_type::iterator;
	using const_iterator = container_type::const_iterator;    

	void add(ItoDiagnostic diagnostic);
    void add(ItoDiagnosticSeverity severity, ItoDiagnosticSource source, ItoDiagnosticCode code,
             QString message, ItoDiagnosticLocation location = {});

    void clear() noexcept;
    void reserve(size_type count);

	[[nodiscard]] bool empty() const noexcept;
    [[nodiscard]] size_type size() const noexcept;

    [[nodiscard]] bool has_errors() const noexcept;
    [[nodiscard]] bool has_warnings() const noexcept;
    [[nodiscard]] bool has_info() const noexcept;

	[[nodiscard]] size_type error_count() const noexcept;
    [[nodiscard]] size_type warning_count() const noexcept;
    [[nodiscard]] size_type info_count() const noexcept;

	[[nodiscard]] iterator begin() noexcept;
    [[nodiscard]] const_iterator begin() const noexcept;
    [[nodiscard]] const_iterator cbegin() const noexcept;

	[[nodiscard]] iterator end() noexcept;
    [[nodiscard]] const_iterator end() const noexcept;
    [[nodiscard]] const_iterator cend() const noexcept;

private:
    [[nodiscard]] size_type count_by_severity(ItoDiagnosticSeverity severity) const noexcept;

    std::vector<ItoDiagnostic> items;
};

} // namespace woh::ito