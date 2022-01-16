#include <iostream>
#include <sstream>
#include <fstream>
#include "interface.h"

using namespace std;

int main(int argc, char* argv[]) {

	if (argc != 3) {
		cout << "wrong input streams";
		return -1;
	}

	std::string fileName1 = argv[1];
	std::string fileName2 = argv[2];

	ifstream stream1;
	stream1.open(fileName1);

	ifstream stream2;
	stream2.open(fileName2);

	Comparator c;
	ComparisonReport report = c.compare(stream1, stream2);

	stream1.clear();
	stream2.clear();
	stream1.close();
	stream2.close();

	return 0;
}
