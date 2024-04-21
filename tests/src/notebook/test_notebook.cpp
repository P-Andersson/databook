#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>

#include <sstream>
#include <variant>

import databook;

using namespace databook;

TEMPLATE_TEST_CASE("write notebook primitive integer tests", "", int, char, int8_t, int16_t, int32_t, int64_t, uint8_t, uint16_t, uint32_t)
{
	auto inner = std::make_unique<WritingNotebookMemory>();
	auto test_tree = inner->data();
	Notebook notebook(std::move(inner));

	SECTION("Basic") {
		TestType value = 4;
		notebook.write("mytest", value);

		REQUIRE(std::holds_alternative<TestType>(test_tree->children.at("mytest")));
		REQUIRE(value == std::get<TestType>(test_tree->children.at("mytest")));
	}
	SECTION("Zero") {
		TestType value = 0;
		notebook.write("mytest", value);

		REQUIRE(std::holds_alternative<TestType>(test_tree->children.at("mytest")));
		REQUIRE(value == std::get<TestType>(test_tree->children.at("mytest")));
	}
	SECTION("Negative") {
		if constexpr (std::is_signed<TestType>::value) {
			TestType value = -12;
			notebook.write("mytest", value);

			REQUIRE(std::holds_alternative<TestType>(test_tree->children.at("mytest")));
			REQUIRE(value == std::get<TestType>(test_tree->children.at("mytest")));
		}
	}
	SECTION("Max") {
		TestType value = std::numeric_limits<TestType>::max();
		notebook.write("mytest",value);

		REQUIRE(std::holds_alternative<TestType>(test_tree->children.at("mytest")));
		REQUIRE(value == std::get<TestType>(test_tree->children.at("mytest")));
	}
	SECTION("Min") {
		TestType value = std::numeric_limits<TestType>::min();
		notebook.write("mytest", value);

		REQUIRE(std::holds_alternative<TestType>(test_tree->children.at("mytest")));
		REQUIRE(value == std::get<TestType>(test_tree->children.at("mytest")));
	}
}

TEMPLATE_TEST_CASE("load notebook primitive integer tests", "", int, char, int8_t, int16_t, int32_t, int64_t, uint8_t, uint16_t, uint32_t)
{
	auto root_node = std::make_shared<MemoryNode>();
	
	SECTION("Basic") {
		TestType value = 4;
		root_node->children.emplace("mytest", value);

		new LoadingNotebookMemory(std::move(root_node));
		//auto inner = std::make_unique<LoadingNotebookMemory>(std::move(root_node));
		/*Notebook notebook(std::move(inner));

		TestType target = 1;
		notebook.load("mytest", target);
		REQUIRE(target == value);*/
	}
}

TEMPLATE_TEST_CASE("write notebook primitive float test", "", float, double)
{
	auto inner = std::make_unique<WritingNotebookMemory>();
	auto test_tree = inner->data();
	Notebook notebook(std::move(inner));

	SECTION("Basic") {
		TestType value = 2.4;
		notebook.write("mytest", value);

		REQUIRE(std::holds_alternative<TestType>(test_tree->children.at("mytest")));
		REQUIRE(value == std::get<TestType>(test_tree->children.at("mytest")));
	}
	SECTION("Zero") {
		TestType value = 0;
		notebook.write("mytest", value);

		REQUIRE(std::holds_alternative<TestType>(test_tree->children.at("mytest")));
		REQUIRE(value == std::get<TestType>(test_tree->children.at("mytest")));
	}
	SECTION("Negative") {
		if constexpr (std::is_signed<TestType>::value) {
			TestType value = -12.5;
			notebook.write("mytest", value);

			REQUIRE(std::holds_alternative<TestType>(test_tree->children.at("mytest")));
			REQUIRE(value == std::get<TestType>(test_tree->children.at("mytest")));
		}
	}
	SECTION("Max") {
		TestType value = std::numeric_limits<TestType>::max();
		notebook.write("mytest", value);

		REQUIRE(std::holds_alternative<TestType>(test_tree->children.at("mytest")));
		REQUIRE(value == std::get<TestType>(test_tree->children.at("mytest")));
	}
	SECTION("Min") {
		TestType value = std::numeric_limits<TestType>::min();
		notebook.write("mytest", value);

		REQUIRE(std::holds_alternative<TestType>(test_tree->children.at("mytest")));
		REQUIRE(value == std::get<TestType>(test_tree->children.at("mytest")));
	}
}

TEST_CASE("write notebook primitive bool test")
{
	auto inner = std::make_unique<WritingNotebookMemory>();
	auto test_tree = inner->data();
	Notebook notebook(std::move(inner));

	SECTION("True") {
		bool value = true;
		notebook.write("mytest", value);

		REQUIRE(std::holds_alternative<bool>(test_tree->children.at("mytest")));
		REQUIRE(value == std::get<bool>(test_tree->children.at("mytest")));
	}
	SECTION("False") {
		bool value = false;
		notebook.write("mytest", value);

		REQUIRE(std::holds_alternative<bool>(test_tree->children.at("mytest")));
		REQUIRE(value == std::get<bool>(test_tree->children.at("mytest")));
	}
}

TEST_CASE("write notebook primitive string test")
{
	auto inner = std::make_unique<WritingNotebookMemory>();
	auto test_tree = inner->data();
	Notebook notebook(std::move(inner));

	SECTION("Some text") {
		std::string value = "some text";
		notebook.write("mytest", value);

		REQUIRE(std::holds_alternative<std::string>(test_tree->children.at("mytest")));
		REQUIRE(value == std::get<std::string>(test_tree->children.at("mytest")));
	}
	SECTION("Empty") {
		std::string value = "";
		notebook.write("mytest", value);

		REQUIRE(std::holds_alternative<std::string>(test_tree->children.at("mytest")));
		REQUIRE(value == std::get<std::string>(test_tree->children.at("mytest")));
	}
}

namespace {
	class Composite {
	public:
		Composite(int val1, std::string val2, bool val3)
			: val1(val1)
			, val2(val2)
			, val3(val3) {

		}

		int val1;
		std::string val2;
		bool val3;

		void store(Notebook& archive) {
			archive.write("val1", val1);
			archive.write("val2", val2);
			archive.write("val3", val3);
		}


		static Composite load(Notebook& archive) {
			return Composite(1, "", false);
		}

		std::strong_ordering operator<=>(const Composite& other) const = default;
	};

	static_assert(MemberFunctionStorable<Composite, Notebook>);
	static_assert(MemberFunctionLoadable<Composite, Notebook>);

	class CompositeSerializeable {
	public:
		CompositeSerializeable(int val1, std::string val2, bool val3)
			: val1(val1)
			, val2(val2)
			, val3(val3) {

		}

		int val1;
		std::string val2;
		bool val3;

		void serialize(Notebook& archive) {
			archive.serialize("val1", val1);
			archive.serialize("val2", val2);
			archive.serialize("val3", val3);
		}

		std::strong_ordering operator<=>(const CompositeSerializeable& other) const = default;
	};

	static_assert(MemberFunctionSerializable<CompositeSerializeable, Notebook>);
	static_assert(MemberFunctionSerializable<CompositeSerializeable, Notebook>);
}

TEMPLATE_TEST_CASE("notebook composite tests", "", Composite, CompositeSerializeable)
{
	auto inner = std::make_unique<WritingNotebookMemory>();
	auto test_tree = inner->data();
	Notebook notebook(std::move(inner));

	SECTION("Write") {

		TestType value(65, "sad apple", true);
		notebook.write("mytest", value);

		REQUIRE(std::holds_alternative<std::shared_ptr<MemoryNode>>(test_tree->children.at("mytest")));
		auto unpacked = std::get<std::shared_ptr<MemoryNode>>(test_tree->children.at("mytest"));
		REQUIRE(std::get<int>( unpacked->children.at("val1")) == 65);
		REQUIRE(std::get<std::string>(unpacked->children.at("val2")) == "sad apple");
		REQUIRE(std::get<bool>(unpacked->children.at("val3")) == true);
	}
}
