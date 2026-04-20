#include <gtest/gtest.h>

#include <QDir>
#include <QFile>
#include <QTemporaryDir>

#include "ItoParser.h"

using namespace woh::ito;

namespace {

const ItoDiagnostic& diagnostic_at(const ItoDiagnostics& diagnostics, qsizetype index) {
    auto it = diagnostics.cbegin();

    for (qsizetype current = 0; current < index; ++current) {
        ++it;
    }

    return *it;
}

} // namespace

TEST(ItoParserTests, ParseTextParsesSectionsAndFields) {
    const ItoParser parser;
    const QString input = QStringLiteral("[Main]\n"
                                         "name = \"Alice\"\n"
                                         "path=\"C:\\\\mods\\\\example\"\n"
                                         "[Other]\n"
                                         "flag = \"A \\\"quoted\\\" value\"");

    const ItoParseResult result = parser.parse_text(input);

    ASSERT_TRUE(result.is_ok());
    ASSERT_EQ(result.document.sections.size(), 2U);

    const ItoSection& main_section = result.document.sections[0];
    EXPECT_EQ(main_section.name, QStringLiteral("Main"));
    ASSERT_EQ(main_section.fields.size(), 2U);
    EXPECT_EQ(main_section.fields[0].key, QStringLiteral("name"));
    EXPECT_EQ(main_section.fields[0].value, QStringLiteral("Alice"));
    EXPECT_EQ(main_section.fields[1].key, QStringLiteral("path"));
    EXPECT_EQ(main_section.fields[1].value, QStringLiteral("C:\\mods\\example"));

    const ItoSection& other_section = result.document.sections[1];
    EXPECT_EQ(other_section.name, QStringLiteral("Other"));
    ASSERT_EQ(other_section.fields.size(), 1U);
    EXPECT_EQ(other_section.fields[0].value, QStringLiteral("A \"quoted\" value"));
}

TEST(ItoParserTests, ParseTextNormalizesBomAndDifferentLineEndings) {
    const ItoParser parser;
    const QString input = QStringLiteral("\uFEFF[Alpha]\r\n"
                                         "x = \"1\"\r"
                                         "[Beta]\n"
                                         "y = \"2\"\r\n");

    const ItoParseResult result = parser.parse_text(input);

    ASSERT_TRUE(result.is_ok());
    ASSERT_EQ(result.document.sections.size(), 2U);
    EXPECT_EQ(result.document.sections[0].name, QStringLiteral("Alpha"));
    EXPECT_EQ(result.document.sections[0].fields[0].value, QStringLiteral("1"));
    EXPECT_EQ(result.document.sections[1].name, QStringLiteral("Beta"));
    EXPECT_EQ(result.document.sections[1].fields[0].value, QStringLiteral("2"));
}

TEST(ItoParserTests, ParseTextReportsFieldOutsideSection) {
    const ItoParser parser;
    const ItoParseResult result = parser.parse_text(QStringLiteral("k=\"v\""));

    ASSERT_FALSE(result.is_ok());
    ASSERT_EQ(result.diagnostics.size(), 1U);
    const ItoDiagnostic& diagnostic = diagnostic_at(result.diagnostics, 0);
    EXPECT_EQ(diagnostic.location.line, 1);
    EXPECT_EQ(diagnostic.message, QStringLiteral("Field is outside of any section"));
    EXPECT_EQ(std::get<ItoParserDiagnosticCode>(diagnostic.code),
              ItoParserDiagnosticCode::field_outside_section);
}

TEST(ItoParserTests, ParseTextReportsEmptySectionName) {
    const ItoParser parser;
    const ItoParseResult result = parser.parse_text(QStringLiteral("[   ]\nkey=\"v\""));

    ASSERT_FALSE(result.is_ok());
    ASSERT_EQ(result.diagnostics.size(), 2U);
    const ItoDiagnostic& first = diagnostic_at(result.diagnostics, 0);
    const ItoDiagnostic& second = diagnostic_at(result.diagnostics, 1);
    EXPECT_EQ(first.location.line, 1);
    EXPECT_EQ(first.message, QStringLiteral("Empty section name"));
    EXPECT_EQ(std::get<ItoParserDiagnosticCode>(first.code),
              ItoParserDiagnosticCode::empty_section_name);
    EXPECT_EQ(second.location.line, 2);
    EXPECT_EQ(second.message, QStringLiteral("Field is outside of any section"));
    EXPECT_EQ(std::get<ItoParserDiagnosticCode>(second.code),
              ItoParserDiagnosticCode::field_outside_section);
}

TEST(ItoParserTests, ParseTextReportsFieldSyntaxErrors) {
    const ItoParser parser;
    const QString input = QStringLiteral("[S]\n"
                                         "bad_no_equal\n"
                                         " = \"value\"\n"
                                         "key = value\n"
                                         "bad_quote = \"a\"b\"\n"
                                         "bad_escape = \"abc\\\"");

    const ItoParseResult result = parser.parse_text(input);

    ASSERT_FALSE(result.is_ok());
    ASSERT_EQ(result.diagnostics.size(), 5U);
    EXPECT_EQ(diagnostic_at(result.diagnostics, 0).location.line, 2);
    EXPECT_EQ(diagnostic_at(result.diagnostics, 0).message, QStringLiteral("Expected key=\"value\""));
    EXPECT_EQ(std::get<ItoParserDiagnosticCode>(diagnostic_at(result.diagnostics, 0).code),
              ItoParserDiagnosticCode::expected_key_value);
    EXPECT_EQ(diagnostic_at(result.diagnostics, 1).location.line, 3);
    EXPECT_EQ(diagnostic_at(result.diagnostics, 1).message, QStringLiteral("Field key is empty"));
    EXPECT_EQ(std::get<ItoParserDiagnosticCode>(diagnostic_at(result.diagnostics, 1).code),
              ItoParserDiagnosticCode::empty_field_key);
    EXPECT_EQ(diagnostic_at(result.diagnostics, 2).location.line, 4);
    EXPECT_EQ(diagnostic_at(result.diagnostics, 2).message,
              QStringLiteral("Field value must be quoted"));
    EXPECT_EQ(std::get<ItoParserDiagnosticCode>(diagnostic_at(result.diagnostics, 2).code),
              ItoParserDiagnosticCode::value_must_be_quoted);
    EXPECT_EQ(diagnostic_at(result.diagnostics, 3).location.line, 5);
    EXPECT_EQ(diagnostic_at(result.diagnostics, 3).message,
              QStringLiteral("Unescaped quote inside field value"));
    EXPECT_EQ(std::get<ItoParserDiagnosticCode>(diagnostic_at(result.diagnostics, 3).code),
              ItoParserDiagnosticCode::unescaped_quote);
    EXPECT_EQ(diagnostic_at(result.diagnostics, 4).location.line, 6);
    EXPECT_EQ(diagnostic_at(result.diagnostics, 4).message,
              QStringLiteral("Unterminated escape sequence in field value"));
    EXPECT_EQ(std::get<ItoParserDiagnosticCode>(diagnostic_at(result.diagnostics, 4).code),
              ItoParserDiagnosticCode::unterminated_escape);
}

TEST(ItoParserTests, ParseFileReadsUtf8Content) {
    const ItoParser parser;
    QTemporaryDir temporary_dir;
    ASSERT_TRUE(temporary_dir.isValid());

    const QString file_path = QDir(temporary_dir.path()).filePath(QStringLiteral("sample.ito"));
    QFile file(file_path);
    ASSERT_TRUE(file.open(QIODevice::WriteOnly));
    const QByteArray content = QByteArrayLiteral("[S]\nname=\"тест\"\n");
    ASSERT_EQ(file.write(content), content.size());
    file.close();

    const ItoParseResult result = parser.parse_file(file_path);

    ASSERT_TRUE(result.is_ok());
    ASSERT_EQ(result.document.sections.size(), 1U);
    ASSERT_EQ(result.document.sections[0].fields.size(), 1U);
    EXPECT_EQ(result.document.sections[0].fields[0].value, QString::fromUtf8("тест"));
}

TEST(ItoParserTests, ParseFileReturnsErrorWhenFileCannotBeOpened) {
    const ItoParser parser;
    const QString missing_file =
        QDir::temp().filePath(QStringLiteral("woh_mod_creator_missing_file.ito"));

    const ItoParseResult result = parser.parse_file(missing_file);

    ASSERT_FALSE(result.is_ok());
    ASSERT_EQ(result.diagnostics.size(), 1U);
    const ItoDiagnostic& diagnostic = diagnostic_at(result.diagnostics, 0);
    EXPECT_EQ(diagnostic.location.line, 0);
    EXPECT_TRUE(diagnostic.message.startsWith(QStringLiteral("Failed to open file: ")));
    EXPECT_TRUE(diagnostic.message.endsWith(missing_file));
    EXPECT_EQ(std::get<ItoParserDiagnosticCode>(diagnostic.code),
              ItoParserDiagnosticCode::file_open_failed);
}
