#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "interface.h"
using namespace std;

class Application
{
public:
	Application() {};
	~Application() {};

	list<Hierarchy*> hiearachyList{};

	void start() {
		string input;
		string command;
		string arguments;
		unsigned delimPos;

		do
		{
			do
			{
			getline(cin, input);
			delimPos = input.find(" ");
			command = input.substr(0, delimPos);
			arguments = input.substr(delimPos + 1, input.length() - delimPos);
			} while (delimPos == string::npos || command.empty() || arguments.empty());
			executeCommand(command, arguments);
		} while (command != "exit");
	}

	void save(Hierarchy* h, string filename) {
		if (filename == "") {
			cout << h->print() << endl;
		} else {
			ofstream file;
			file.open(filename);
			if (!file) {
				cout << "Error: file could not be opened" << endl;
				return;
			}
			file << h->print();
			file.close();
		}
	}

	void incorporate(Hierarchy* h) {
		h->incorporate();
		std::cout << h->print();
	}

	string loadFromConsole() {
		string data;
		string line;
		while (getline(cin, line))
		{
			data = data + line + '\n';
		}
		return data;
	}

	string loadFromFile(string filename) {
		//open file
		ifstream file;
		file.open(filename);
		if (!file) {
			cout << "Error: file could not be opened" << endl;
			return "";
		}
		string data;
		string line;
		while (getline(file, line))
		{
			data = data + line + '\n';
		}
		file.close();
		return data;
	}

	void load(string hierarchy, string filename) {
		string data;
		if (filename == "NO") {
			data = loadFromConsole();
		}
		else {
			data = loadFromFile(filename);
		}
		Hierarchy* h = new Hierarchy(data);
		if (!h) cout << "Could not load" << endl;

		h->setName(hierarchy);
		hiearachyList.push_back(h);
	}

	void find(Hierarchy* h, string employee)
	{
		cout << h->find(employee) << endl;
	}

	void num_subordinates(Hierarchy* h, string employee)
	{
		cout << h->num_subordinates(employee);
	}

	void manager(Hierarchy* h, string employee)
	{
		cout << h->manager(employee) << endl;
	}

	void num_employees(Hierarchy* h)
	{
		cout << h->num_employees() << endl;
	}

	void overloaded(Hierarchy* h)
	{
		int overloaded = h->num_overloaded();
		if (overloaded) {
			cout << overloaded;
		}
		else {
			cout << "No overloaded employees in " << h->getName() << endl;
		}
	}

	void fire(Hierarchy* h, string employee)
	{
		cout << h->fire(employee) << endl;
	}

	void longest_chain(Hierarchy* h)
	{
		cout << h->longest_chain() << endl;
	}

	void hire(Hierarchy* h, string employee, string manager)
	{
		cout << h->hire(employee, manager);
	}

	void modernize(Hierarchy* h)
	{
		h->modernize();
	}

	void executeCommand(string command, string arguments)
	{
		unsigned delimPos = arguments.find(" ");
		unsigned argLen = arguments.length();
		string hierarchy = arguments.substr(0, delimPos);
		
		if (command == "load") {
			string fileName = "NO";
			if (delimPos != string::npos)
				fileName = arguments.substr(delimPos + 1, argLen - delimPos);
			load(hierarchy, fileName);
			return;
		} 
		Hierarchy* h = nullptr;
		for (list<Hierarchy*>::const_iterator it = hiearachyList.cbegin(); it != hiearachyList.cend(); ++it) {
			if ((*it)->getName() == hierarchy) {
				h = *it;
				break;
			}
		}
		if (!h) {
			cout << "There is no such hierarchy" << endl;
			return;
		}

		if (command == "save") {
			if (delimPos == string::npos) return;
			string fileName = arguments.substr(delimPos + 1, argLen - delimPos);
			save(h, fileName);
		}
		else
		if (command == "find") {
			if (delimPos == string::npos) return;
			string employee = arguments.substr(delimPos + 1, argLen - delimPos);
			find(h, employee);
		}
		else
		if (command == "num_subordinates") {
			if (delimPos == string::npos) return;
			string employee = arguments.substr(delimPos + 1, argLen - delimPos);
			num_subordinates(h, employee);
		}
		else
		if (command == "manager") {
			if (delimPos == string::npos) return;
			string employee = arguments.substr(delimPos + 1, argLen - delimPos);
			manager(h, employee);
		}
		else
		if (command == "fire") {
			if (delimPos == string::npos) return;
			string employee = arguments.substr(delimPos + 1, argLen - delimPos);
			fire(h, employee);
		}
		else
		if (command == "hire") {
			if (delimPos == string::npos) return;
			arguments = arguments.substr(delimPos + 1, argLen - delimPos);
			delimPos = arguments.find(" ");
			string employee = arguments.substr(0, delimPos);
			string manager = arguments.substr(delimPos + 1, argLen - delimPos);
			hire(h, employee, manager);
		}
		else
		if (command == "incorporate") {
			incorporate(h);
		}
		else
		if (command == "num_employees") {
			num_employees(h);
		}
		else
		if (command == "overloaded") {
			overloaded(h);
		}
		else
		if (command == "longest_chain") {
			longest_chain(h);
		}
		else
		if (command == "modernize") {
			modernize(h);
		}
	}
};