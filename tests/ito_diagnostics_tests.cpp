#include <gtest/gtest.h>

#include <QStringList>

#include "ItoDiagnostics.h"

using namespace woh::ito;

namespace {

ItoDiagnostic make_diagnostic(ItoDiagnosticSeverity severity, QString message,
                              ItoDiagnosticLocation location = {}) {
    return ItoDiagnostic{
        .severity = severity,
        .source = ItoDiagnosticSource::parser,
        .code = ItoParserDiagnosticCode::expected_key_value,
        .message = std::move(message),
        .location = std::move(location),
    };
}

} // namespace

TEST(ItoDiagnosticsTests, LocationHelpersReportAvailableCoordinates) {
    ItoDiagnosticLocation location;
    EXPECT_FALSE(location.has_text_position());
    EXPECT_FALSE(location.has_section());
    EXPECT_FALSE(location.has_field());

    location.line = 3;
    EXPECT_TRUE(location.has_text_position());

    location = {};
    location.section_index = 0;
    EXPECT_TRUE(location.has_section());

    location = {};
    location.section_name = QStringLiteral("Main");
    EXPECT_TRUE(location.has_section());

    location = {};
    location.field_index = 1;
    EXPECT_TRUE(location.has_field());

    location = {};
    location.field_key = QStringLiteral("name");
    EXPECT_TRUE(location.has_field());
}

TEST(ItoDiagnosticsTests, DiagnosticValidityDependsOnMessage) {
    const ItoDiagnostic invalid = make_diagnostic(ItoDiagnosticSeverity::error, {});
    const ItoDiagnostic valid =
        make_diagnostic(ItoDiagnosticSeverity::warning, QStringLiteral("Warning message"));

    EXPECT_FALSE(invalid.is_valid());
    EXPECT_TRUE(valid.is_valid());
}

TEST(ItoDiagnosticsTests, AddIgnoresInvalidDiagnostic) {
    ItoDiagnostics diagnostics;

    diagnostics.add(make_diagnostic(ItoDiagnosticSeverity::error, {}));

    EXPECT_TRUE(diagnostics.empty());
    EXPECT_EQ(diagnostics.size(), 0U);
}

TEST(ItoDiagnosticsTests, AddOverloadStoresAllFields) {
    ItoDiagnostics diagnostics;
    const ItoDiagnosticLocation location{
        .line = 7,
        .column = 4,
        .section_index = 1,
        .field_index = 2,
        .section_name = QStringLiteral("Main"),
        .field_key = QStringLiteral("name"),
    };

    diagnostics.add(ItoDiagnosticSeverity::warning, ItoDiagnosticSource::writer,
                    ItoWriterDiagnosticCode::file_write_failed,
                    QStringLiteral("Failed to write file"), location);

    ASSERT_EQ(diagnostics.size(), 1U);

    const ItoDiagnostic& diagnostic = *diagnostics.begin();
    EXPECT_EQ(diagnostic.severity, ItoDiagnosticSeverity::warning);
    EXPECT_EQ(diagnostic.source, ItoDiagnosticSource::writer);
    EXPECT_TRUE(std::holds_alternative<ItoWriterDiagnosticCode>(diagnostic.code));
    EXPECT_EQ(std::get<ItoWriterDiagnosticCode>(diagnostic.code),
              ItoWriterDiagnosticCode::file_write_failed);
    EXPECT_EQ(diagnostic.message, QStringLiteral("Failed to write file"));
    EXPECT_EQ(diagnostic.location.line, 7);
    EXPECT_EQ(diagnostic.location.column, 4);
    EXPECT_EQ(diagnostic.location.section_index, 1);
    EXPECT_EQ(diagnostic.location.field_index, 2);
    EXPECT_EQ(diagnostic.location.section_name, QStringLiteral("Main"));
    EXPECT_EQ(diagnostic.location.field_key, QStringLiteral("name"));
}

TEST(ItoDiagnosticsTests, SeverityQueriesReflectStoredDiagnostics) {
    ItoDiagnostics diagnostics;

    EXPECT_FALSE(diagnostics.has_errors());
    EXPECT_FALSE(diagnostics.has_warnings());
    EXPECT_FALSE(diagnostics.has_info());

    diagnostics.add(make_diagnostic(ItoDiagnosticSeverity::warning, QStringLiteral("warn")));
    EXPECT_FALSE(diagnostics.has_errors());
    EXPECT_TRUE(diagnostics.has_warnings());
    EXPECT_FALSE(diagnostics.has_info());

    diagnostics.add(make_diagnostic(ItoDiagnosticSeverity::info, QStringLiteral("info")));
    diagnostics.add(make_diagnostic(ItoDiagnosticSeverity::error, QStringLiteral("error")));
    EXPECT_TRUE(diagnostics.has_errors());
    EXPECT_TRUE(diagnostics.has_warnings());
    EXPECT_TRUE(diagnostics.has_info());
}

TEST(ItoDiagnosticsTests, CountMethodsReturnExpectedValues) {
    ItoDiagnostics diagnostics;
    diagnostics.reserve(4);

    diagnostics.add(make_diagnostic(ItoDiagnosticSeverity::error, QStringLiteral("error 1")));
    diagnostics.add(make_diagnostic(ItoDiagnosticSeverity::warning, QStringLiteral("warning")));
    diagnostics.add(make_diagnostic(ItoDiagnosticSeverity::info, QStringLiteral("info")));
    diagnostics.add(make_diagnostic(ItoDiagnosticSeverity::error, QStringLiteral("error 2")));

    EXPECT_EQ(diagnostics.error_count(), 2U);
    EXPECT_EQ(diagnostics.warning_count(), 1U);
    EXPECT_EQ(diagnostics.info_count(), 1U);
}

TEST(ItoDiagnosticsTests, ClearRemovesAllDiagnostics) {
    ItoDiagnostics diagnostics;
    diagnostics.add(make_diagnostic(ItoDiagnosticSeverity::error, QStringLiteral("error")));
    diagnostics.add(make_diagnostic(ItoDiagnosticSeverity::warning, QStringLiteral("warning")));

    ASSERT_FALSE(diagnostics.empty());
    ASSERT_EQ(diagnostics.size(), 2U);

    diagnostics.clear();

    EXPECT_TRUE(diagnostics.empty());
    EXPECT_EQ(diagnostics.size(), 0U);
    EXPECT_EQ(diagnostics.error_count(), 0U);
    EXPECT_EQ(diagnostics.warning_count(), 0U);
    EXPECT_EQ(diagnostics.info_count(), 0U);
}

TEST(ItoDiagnosticsTests, IterationPreservesInsertionOrder) {
    ItoDiagnostics diagnostics;
    diagnostics.add(make_diagnostic(ItoDiagnosticSeverity::info, QStringLiteral("first")));
    diagnostics.add(make_diagnostic(ItoDiagnosticSeverity::warning, QStringLiteral("second")));
    diagnostics.add(make_diagnostic(ItoDiagnosticSeverity::error, QStringLiteral("third")));

    ASSERT_NE(diagnostics.begin(), diagnostics.end());
    EXPECT_EQ(diagnostics.cbegin()->message, QStringLiteral("first"));

    QStringList messages;
    for (const ItoDiagnostic& diagnostic : diagnostics) {
        messages.push_back(diagnostic.message);
    }

    ASSERT_EQ(messages.size(), 3);
    EXPECT_EQ(messages[0], QStringLiteral("first"));
    EXPECT_EQ(messages[1], QStringLiteral("second"));
    EXPECT_EQ(messages[2], QStringLiteral("third"));

    const ItoDiagnostics& const_diagnostics = diagnostics;
    auto it = const_diagnostics.begin();
    ASSERT_NE(it, const_diagnostics.end());
    EXPECT_EQ(it->message, QStringLiteral("first"));
    EXPECT_EQ(const_diagnostics.cend() - const_diagnostics.cbegin(), 3);
}
