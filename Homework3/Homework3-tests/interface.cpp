#include "interface.h"
#include <iostream>
#include <string>

using std::string;

void WordsMultiset::add(const std::string& word, size_t times)
{
	hashMap.insert(word,times);
}

bool WordsMultiset::contains(const std::string& word) const
{
	return hashMap.contains(word);
}

size_t WordsMultiset::countOf(const std::string& word) const
{
	size_t counter = 0;
	hashMap.for_each(
		[&](const Data<string, size_t>& pair) {
			if (pair.word == word) counter = pair.count;
		}
	);
	return counter;
}

size_t WordsMultiset::countOfUniqueWords() const
{
	return hashMap.getSize();
}

std::multiset<std::string> WordsMultiset::words() const
{
	std::multiset<string> stdMultiset;
	hashMap.for_each(
		[&](const Data<string, size_t>& pair) {
			for (size_t i = 0; i < pair.count; i++)
			{
				stdMultiset.insert(pair.word);
			}
		}
	);
	return stdMultiset;
}

WordsMultiset WordsMultiset::getCommonWords(const WordsMultiset& other) const
{
	WordsMultiset commonWordsMultiset;
	int minCounter = 0;
	int otherCounter = 0;
	hashMap.for_each(
		[&](const Data<string, size_t>& pair) {
			if (contains(pair.word) && other.contains(pair.word)) {
				otherCounter = other.countOf(pair.word);
				minCounter = (pair.count < otherCounter) ? pair.count : otherCounter;
				commonWordsMultiset.add(pair.word, minCounter);
			}
		}
	);
	return commonWordsMultiset;
}

WordsMultiset WordsMultiset::getUniqueWords(const WordsMultiset& other) const
{
	//get exclusion of current multiset and commonWordsMultiset;
	WordsMultiset uniqueWordsMultiset;
	int otherCounter = 0;
	hashMap.for_each(
		[&](const Data<string, size_t>& pair) {
			otherCounter = other.countOf(pair.word);
			if (!other.contains(pair.word)) {
				uniqueWordsMultiset.add(pair.word, pair.count);
			} else 
			if (pair.count > otherCounter)
			{
				uniqueWordsMultiset.add(pair.word, pair.count - otherCounter);
			}
			//if pair.count == otherCounter there are no unique words
		}
	);
	return uniqueWordsMultiset;
}

ComparisonReport Comparator::compare(std::istream& a, std::istream& b)
{
	if (!a || !b) {
		std::cout << "Error: fault stream" << std::endl;
		//return ;
	}
	//open file and put the words in a hashmap
	WordsMultiset m1 = readStream(a);
	WordsMultiset m2 = readStream(b);

	//iterates through both hashmaps
	WordsMultiset commonWords = getCommonWords(m1, m2);

	//iterates through single hashmap
	WordsMultiset uniqueWords1 = getUniqueWords(m1, commonWords);
	WordsMultiset uniqueWords2 = getUniqueWords(m2, commonWords);
	ComparisonReport result{ commonWords, uniqueWords1, uniqueWords2 };

	return result;
}

WordsMultiset Comparator::readStream(std::istream& a)
{
	string word;
	WordsMultiset myWordsMultiset;
	while (a >> word)
	{
		myWordsMultiset.add(word);
	}
	return myWordsMultiset;
}

WordsMultiset Comparator::getCommonWords(const WordsMultiset& m1, const WordsMultiset& m2)
{
	return m1.getCommonWords(m2);
}

WordsMultiset Comparator::getUniqueWords(const WordsMultiset& m1, const WordsMultiset& common)
{
	return m1.getUniqueWords(common);
}
