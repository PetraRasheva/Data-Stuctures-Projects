#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "interface.h"

SCENARIO("WordsMultiset stores data correctly when adding words consecutively")
{
	GIVEN("An empty WordsMultiset object")
	{
		WordsMultiset wl;

		WHEN("We consecutively add the words 'one', 'two', 'three' and 'one'")
		{
			wl.add("one");
			wl.add("two");
			wl.add("three");
			wl.add("one");

			THEN("The number of unique words is 3")
			{
				REQUIRE(wl.countOfUniqueWords() == 3);
			}
			THEN("The number of occurrences of 'one' is 2")
			{
				REQUIRE(wl.countOf("one") == 2);
			}
			THEN("The number of occurrences of 'two' and 'three' is 1")
			{
				REQUIRE(wl.countOf("two") == 1);
				REQUIRE(wl.countOf("three") == 1);
			}
			THEN("The number of occurrences of 'notInTheList' is 0")
			{
				REQUIRE(wl.countOf("notInTheList") == 0);
			}
			THEN("The list reports that 'one', 'two' and 'three' are contained within it")
			{
				REQUIRE(wl.contains("one"));
				REQUIRE(wl.contains("two"));
				REQUIRE(wl.contains("three"));
			}
			THEN("The list reports that 'notInTheList' is NOT contained within it")
			{
				REQUIRE_FALSE(wl.contains("notInTheList"));
			}
			THEN("The multiset of words is {one,one,two,three}")
			{
				std::multiset<std::string> expected{ "one","one","two","three" };
				REQUIRE(expected == wl.words());
			}
		}
	}
}

SCENARIO("WordsMultiset stores data correctly when adding many occurrances in one step")
{
	GIVEN("An empty WordsMultiset objecy")
	{
		WordsMultiset wl;

		WHEN("We directly add 4 occurrances of 'one'")
		{
			wl.add("one", 4);

			THEN("The number of unique words is 1")
			{
				REQUIRE(wl.countOfUniqueWords() == 1);
			}
			THEN("The number of occurrences of 'one' is 4")
			{
				REQUIRE(wl.countOf("one") == 4);
			}
			THEN("The multiset of words is {one,one,one,one}")
			{
				std::multiset<std::string> expected{ "one","one","one","one" };
				REQUIRE(expected == wl.words());
			}
			WHEN("We add 'one' once more")
			{
				wl.add("one");
				THEN("the number of occurrances of 'one' is 5")
				{
					REQUIRE(wl.countOf("one") == 5);
				}
				THEN("The multiset of words is {one,one,one,one,one}")
				{
					std::multiset<std::string> expected{ "one","one","one","one","one" };
					REQUIRE(expected == wl.words());
				}
			}
		}
	}
}

TEST_CASE("The number of unique words in an empty WordsMultiset object is zero")
{
	WordsMultiset wl;
	REQUIRE(wl.countOfUniqueWords() == 0);
}

TEST_CASE("The multiset of words in an empty WordsMultiset object is also empty")
{
	WordsMultiset wl;
	REQUIRE(wl.words().empty());
}

TEST_CASE("Comparator works correctly for empty streams")
{
	std::stringstream a, b;

	Comparator c;
	ComparisonReport report = c.compare(a, b);

	REQUIRE(report.commonWords.countOfUniqueWords() == 0);
	REQUIRE(report.uniqueWords[0].countOfUniqueWords() == 0);
	REQUIRE(report.uniqueWords[1].countOfUniqueWords() == 0);
}

TEST_CASE("Comparator works correctly for the streams [one two three four two one] and [two one four one one]")
{
	std::stringstream a("one two three four two one");
	std::stringstream b("two one four one one");

	Comparator c;
	ComparisonReport report = c.compare(a, b);

	SECTION("Sizes are reported correctly")
	{
		REQUIRE(report.commonWords.countOfUniqueWords() == 3);
		REQUIRE(report.uniqueWords[0].countOfUniqueWords() == 2);
		REQUIRE(report.uniqueWords[1].countOfUniqueWords() == 1);
	}

	SECTION("The common words are {one,one,two,four}")
	{
		std::multiset<std::string> expected{ "one","one","two","four" };
		REQUIRE(report.commonWords.words() == expected);
	}

	SECTION("The first stream has unique words {two,three}")
	{
		std::multiset<std::string> expected{ "two","three" };
		REQUIRE(report.uniqueWords[0].words() == expected);
	}

	SECTION("The second stream has only one unique word {one}")
	{
		std::multiset<std::string> expected{ "one" };
		REQUIRE(report.uniqueWords[1].words() == expected);
	}
}

TEST_CASE("Comparator works correctly for the streams [one two three] and []")
{
	std::stringstream a("one two three");
	std::stringstream b;

	Comparator c;
	ComparisonReport report = c.compare(a, b);

	SECTION("Sizes are reported correctly")
	{
		REQUIRE(report.commonWords.countOfUniqueWords() == 0);
		REQUIRE(report.uniqueWords[0].countOfUniqueWords() == 3);
		REQUIRE(report.uniqueWords[1].countOfUniqueWords() == 0);
	}

	SECTION("The first stream has unique words {one,two,three}")
	{
		std::multiset<std::string> expected{ "one","two","three" };
		REQUIRE(report.uniqueWords[0].words() == expected);
	}
}

// Additional cases

SCENARIO("WordsMultiset stores data correctly when are no words added")
{
	GIVEN("An empty WordsMultiset object")
	{
		WordsMultiset wl;

		WHEN("We doesn't add any words")
		{

			THEN("The number of unique words is 0")
			{
				REQUIRE(wl.countOfUniqueWords() == 0);
			}
			THEN("The number of occurrences of 'one' is 0")
			{
				REQUIRE(wl.countOf("one") == 0);
			}
			THEN("The number of occurrences of 'two' and 'three' is 0")
			{
				REQUIRE(wl.countOf("two") == 0);
				REQUIRE(wl.countOf("three") == 0);
			}
			THEN("The list reports that 'one', 'two' and 'three' are NOT contained within it")
			{
				REQUIRE_FALSE(wl.contains("one"));
				REQUIRE_FALSE(wl.contains("two"));
				REQUIRE_FALSE(wl.contains("three"));
			}
			THEN("The multiset of words is {empty}")
			{
				std::multiset<std::string> expected{};
				REQUIRE(expected == wl.words());
			}
		}
	}
}


TEST_CASE("Comparator works correctly for the streams [1234 \t\t(*abc*(  \r 1234  \n aok \r*k*)\t34a\t34a\n] and [1234 \t\t(*abc*(  \r 1234 \n 1234\t aok \r aok\n ssss\t\t\t\t\n\nttt\n\n34a]")
{
	std::stringstream a("1234 \t\t(*abc*(  \r 1234  \n aok \r*k*)\t34a\t34a\n");
	std::stringstream b("1234 \t\t(*abc*(  \r 1234 \n 1234\t aok \r aok\n ssss\t\t\t\t\n\nttt\n\n34a");

	Comparator c;
	ComparisonReport report = c.compare(a, b);

	SECTION("Sizes are reported correctly")
	{
		REQUIRE(report.commonWords.countOfUniqueWords() == 4);
		REQUIRE(report.uniqueWords[0].countOfUniqueWords() == 2);
		REQUIRE(report.uniqueWords[1].countOfUniqueWords() == 4);
	}

	SECTION("The common words are {1234,1234,(*abc*(,aok,34a}")
	{
		std::multiset<std::string> expected{ "1234","1234","(*abc*(","aok", "34a" };
		REQUIRE(report.commonWords.words() == expected);
	}

	SECTION("The first stream has unique words {*k*),34a}")
	{
		std::multiset<std::string> expected{ "*k*)","34a" };
		REQUIRE(report.uniqueWords[0].words() == expected);
	}

	SECTION("The second stream has unique words {1234,aok,ssss,ttt}")
	{
		std::multiset<std::string> expected{ "1234", "aok", "ssss", "ttt" };
		REQUIRE(report.uniqueWords[1].words() == expected);
	}
}

TEST_CASE("Comparator works correctly for the streams [aa bb cc dd] and [ee ff gg]")
{
	std::stringstream a("aa bb cc dd");
	std::stringstream b("ee ff gg");

	Comparator c;
	ComparisonReport report = c.compare(a, b);

	SECTION("Sizes are reported correctly")
	{
		REQUIRE(report.commonWords.countOfUniqueWords() == 0);
		REQUIRE(report.uniqueWords[0].countOfUniqueWords() == 4);
		REQUIRE(report.uniqueWords[1].countOfUniqueWords() == 3);
	}

	SECTION("There are NO common words.")
	{
		std::multiset<std::string> expected{};
		REQUIRE(report.commonWords.words() == expected);
	}

	SECTION("The first stream has unique words {aa, bb, cc, dd}")
	{
		std::multiset<std::string> expected{ "aa","bb","cc","dd" };
		REQUIRE(report.uniqueWords[0].words() == expected);
	}

	SECTION("The second stream has only one unique word {ee, ff, gg }")
	{
		std::multiset<std::string> expected{ "ee", "ff", "gg" };
		REQUIRE(report.uniqueWords[1].words() == expected);
	}
}


