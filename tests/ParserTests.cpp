#include <gtest/gtest.h>

#include "HeaderParserLib/Parser.h"


TEST(ParserTests, ParseEmptySource)
{
    std::string source = "";
    Parser parser;
    parser.SetSource(source);

    ASSERT_FALSE(parser.ParseStatement());
    ASSERT_TRUE(parser.GetGblobalNamespace().GetClasses().empty());
    ASSERT_TRUE(parser.GetGblobalNamespace().GetNamespaces().empty());
}

TEST(ParserTests, ParseTypeSimple)
{
    std::string source = R"(
        string
    )";

    Parser parser;
    parser.SetSource(source);

    std::string type;
    ASSERT_TRUE(parser.ParseType(type));
    ASSERT_EQ(type, "string");
}

TEST(ParserTests, ParseTypeWithNamespaces)
{
    std::string source = R"(
        std::string
    )";

    Parser parser;
    parser.SetSource(source);

    std::string type;
    ASSERT_TRUE(parser.ParseType(type));
    ASSERT_EQ(type, "std::string");
}

TEST(ParserTests, ParseTypeWithGlobalNamespace)
{
    std::string source = R"(
        ::std::string
    )";

    Parser parser;
    parser.SetSource(source);

    std::string type;
    ASSERT_TRUE(parser.ParseType(type));
    ASSERT_EQ(type, "::std::string");
}

TEST(ParserTests, ParseTypeWithConstModifier)
{
    std::string source = R"(
        const string
    )";

    Parser parser;
    parser.SetSource(source);

    std::string type;
    ASSERT_TRUE(parser.ParseType(type));
    ASSERT_EQ(type, "const string");
}

TEST(ParserTests, ParseTypeWithConstModifierPostfix)
{
    std::string source = R"(
        string const
    )";

    Parser parser;
    parser.SetSource(source);

    std::string type;
    ASSERT_TRUE(parser.ParseType(type));
    ASSERT_EQ(type, "string const");
}

TEST(ParserTests, ParseTypeWithCVModifiers)
{
    std::string source = R"(
        const volatile string 
    )";

    Parser parser;
    parser.SetSource(source);

    std::string type;
    ASSERT_TRUE(parser.ParseType(type));
    ASSERT_EQ(type, "const volatile string");
}

TEST(ParserTests, ParseTemplateType)
{
    std::string source = R"(
        std::vector<std::string> 
    )";

    Parser parser;
    parser.SetSource(source);

    std::string type;
    ASSERT_TRUE(parser.ParseType(type));
    ASSERT_EQ(type, "std::vector<std::string>");
}

TEST(ParserTests, ParsePointerType)
{
    std::string source = R"(
        std::vector<std::string> const**const&& 
    )";

    Parser parser;
    parser.SetSource(source);

    std::string type;
    ASSERT_TRUE(parser.ParseType(type));
    ASSERT_EQ(type, "std::vector<std::string> const * * const &&");
}

TEST(ParserTests, ParseQuiteComplexType)
{
    std::string source = R"(
        const volatile Map<uint32_t, const std::string*>& 
    )";

    Parser parser;
    parser.SetSource(source);

    std::string type;
    ASSERT_TRUE(parser.ParseType(type));
    ASSERT_EQ(type, "const volatile Map<uint32_t, const std::string *> &");
}

TEST(ParserTests, ParseEmptyNamespace)
{
    std::string source = R"(
        namespace Namespace {

        }

    )";

    Parser parser;
    parser.SetSource(source);

    ASSERT_TRUE(parser.ParseNamespace());
    ASSERT_TRUE(parser.GetGblobalNamespace().GetClasses().empty());
    ASSERT_TRUE(!parser.GetGblobalNamespace().GetNamespaces().empty());
}

TEST(ParserTests, ParseNonEmptyNamespace)
{
    std::string source = R"(
        namespace Namespace {

            class ForwardDeclaration;
            using Alias = ForwardDeclaration;

            struct Struct {
                int x = 0;
                ForwardDeclaration* ptr{ nullptr };
            };

        }
    )";

    Parser parser;
    parser.SetSource(source);

    ASSERT_TRUE(parser.ParseNamespace());
    ASSERT_TRUE(parser.GetGblobalNamespace().GetClasses().empty());
    ASSERT_TRUE(!parser.GetGblobalNamespace().GetNamespaces().empty());
    ASSERT_TRUE(!parser.GetGblobalNamespace().GetNamespaces().empty());
}
