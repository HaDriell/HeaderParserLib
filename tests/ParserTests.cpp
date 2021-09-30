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

TEST(ParserTests, ParseForwardDeclaredClass)
{
    std::string source = R"(
        class ForwardDeclaration;
    )";

    Parser parser;
    parser.SetSource(source);

    ASSERT_TRUE(parser.ParseClass());
    ASSERT_TRUE(parser.GetGblobalNamespace().GetClasses().empty());
}

TEST(ParserTests, ParseEmptyClass)
{
    std::string source = R"(
        class EmptyClass {};
    )";

    Parser parser;
    parser.SetSource(source);

    ASSERT_TRUE(parser.ParseClass());
    ASSERT_NE(parser.GetGblobalNamespace().GetClass("EmptyClass"), nullptr);
}

TEST(ParserTests, ParseClassWithFields)
{
    std::string source = R"(
        class ClassWithFields {
            int a;
            std::string b;
        };
    )";

    Parser parser;
    parser.SetSource(source);

    ASSERT_TRUE(parser.ParseClass());
    const Class* clazz = parser.GetGblobalNamespace().GetClass("ClassWithFields");
    ASSERT_NE(clazz, nullptr);
    ASSERT_NE(clazz->GetField("a"), nullptr);
    ASSERT_NE(clazz->GetField("b"), nullptr);
}

TEST(ParserTests, ParseClassWithAnnotatedField)
{
    std::string source = R"(
        class ClassWithFields {
            int a;

            PROPERTY()
            Field m_Field;

            std::string b;
        };
    )";

    Parser parser;
    parser.SetSource(source);

    ASSERT_TRUE(parser.ParseClass());
    const Class* clazz = parser.GetGblobalNamespace().GetClass("ClassWithFields");
    ASSERT_NE(clazz, nullptr);
    const Field* field = clazz->GetField("m_Field");
    ASSERT_NE(field, nullptr);
    ASSERT_FALSE(field->GetAnnotations().empty());
}

TEST(ParserTests, ParseEmptyAnnotatedClass)
{
    std::string source = R"(
        CLASS()
        class EmptyClass {};
    )";

    Parser parser;
    parser.SetSource(source);

    ASSERT_TRUE(parser.ParseClass());
    ASSERT_NE(parser.GetGblobalNamespace().GetClass("EmptyClass"), nullptr);
}

TEST(ParserTests, ParseNamespaceWithClasses)
{
    std::string source = R"(
        namespace Namespace {

            class ForwardDeclaration;

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
    ASSERT_NE(parser.GetGblobalNamespace().GetNamespace("Namespace"), nullptr);
}

TEST(ParserTests, ParseNamespaceWithAnnotatedClasses)
{
    std::string source = R"(
        namespace Namespace {

            CLASS()
            class AnnotatedClass {};

            class ForwardDeclaredClass;
            struct ForwardDeclaredStruct;

            CLASS()
            struct AnnotatedStruct {};

            class NonAnnotatedClass {};
            struct NonAnnotatedStruct {};
        }
    )";

    Parser parser;
    parser.SetSource(source);

    ASSERT_TRUE(parser.ParseNamespace());
    ASSERT_TRUE(parser.GetGblobalNamespace().GetClasses().empty());
    const Namespace* ns = parser.GetGblobalNamespace().GetNamespace("Namespace"); 
    ASSERT_NE(ns, nullptr);
    ASSERT_NE(ns->GetClass("AnnotatedClass"), nullptr);
    ASSERT_NE(ns->GetClass("AnnotatedStruct"), nullptr);
    ASSERT_NE(ns->GetClass("NonAnnotatedClass"), nullptr);
    ASSERT_NE(ns->GetClass("NonAnnotatedStruct"), nullptr);
    ASSERT_EQ(ns->GetClass("ForwardDeclaredClass"), nullptr);
    ASSERT_EQ(ns->GetClass("ForwardDeclaredStruct"), nullptr);
}