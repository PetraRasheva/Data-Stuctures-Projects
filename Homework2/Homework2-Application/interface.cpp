#include "interface.h"
#include <iostream>

Hierarchy::Hierarchy(Hierarchy&& r) noexcept
{
}

Hierarchy::Hierarchy(const Hierarchy& r)
{
	clone(r.root, r.root->parent);
	name = r.name;
	root->children = r.root->children;
}

Hierarchy::Hierarchy(const string& data)
{
	string str = data;
	try
	{
		create_tree(str);
	}
	catch (const std::exception& error)
	{
		std::cout << error.what() << std::endl;
	}
}

Hierarchy::~Hierarchy() noexcept
{
	erase_tree(root);
}

TreeNode* Hierarchy::clone(TreeNode* other, TreeNode* parent) {
	if (!root) return nullptr;

	TreeNode* result = new TreeNode{ other->name, parent };

	for (TreeNode* p : other->children) {
		result->children.push_back(clone(p, result));
	}
	return result;
}

void Hierarchy::erase_tree(TreeNode*& root)
{
	if (!root)
		return;

	for (TreeNode*& child : root->children)
		erase_tree(child);

	delete root;
	root = nullptr;
}

void Hierarchy::create_tree(string& str) {
	if (str != "")
		root = new TreeNode{ "Uspeshnia", nullptr };

	string parent, name, line;
	unsigned delimPos = 0;
	unsigned newlinePos = 0;
	while (!str.empty()) {
		newlinePos = str.find("\n");
		line = str.substr(0, newlinePos);
		unsigned strLen = line.length() + 1;
		line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
		delimPos = line.find("-");
		if (delimPos == string::npos) {
			throw std::invalid_argument("Wrong input");
		}
		parent = line.substr(0, delimPos);
		name = line.substr(delimPos + 1, line.length() - 1 - delimPos);
		if (parent.empty() || name.empty()) {

			break;
		}
		else {
			TreeNode* pParent = getAddrByName(parent);
			if (!pParent) {
				throw std::invalid_argument("Wrong input");
			}
			TreeNode* node = new TreeNode(name, pParent);
			pParent->children.push_back(node);
			str.erase(0, strLen);
		}
	}
	sortLex(root);
}

void Hierarchy::read_input(const string& filename) 
{
	std::ifstream file;
	file.open(filename);
	if (!file) {
		std::cout << "Error: file could not be opened" << std::endl;
		return;
	}
	string data, line;
	while (getline(file, line))
	{
		data = data + line + '\n';
	}
	file.close();
	file.clear();
	
	create_tree(data);
}

void Hierarchy::sortLex(TreeNode* root)
{
	if (!root) return;

	if (root->children.size() <= 1) return;

	for (TreeNode* child : root->children) {
		sortLex(child);
		//if (child->children.size() <= 1) return;
	}
	list<string> names;
	list<TreeNode*> sortedChildren;
	for (list<TreeNode*>::iterator it = root->children.begin(); it != root->children.end(); ++it) {
		names.push_back((*it)->name);
	}
	names.sort();
	//iterate through the same list but change addresses
	while (!names.empty())
	{
		TreeNode* node = getAddrByName(names.front());
		sortedChildren.push_back(node);
		names.pop_front();
	}
	root->children = sortedChildren;
}

string Hierarchy::print() const
{
	string hierarchyData;

	if (!root) return "";

	queue<const TreeNode*>   wave;
	wave.push(root);

	while (!wave.empty())
	{
		const TreeNode* curr = wave.front();
		wave.pop();

		if(curr->name != "Uspeshnia")
		hierarchyData = hierarchyData + curr->parent->name + "-" + curr->name + "\n";
	
		//lexSortList(curr->children);
		for (const TreeNode* child : curr->children)
			wave.push(child);
	}
	return hierarchyData;
}

int Hierarchy::longest_chain(const TreeNode* root) const
{
	if (!root) return 0;

	int maxLen = 0;
	for (const TreeNode* child : root->children)
	{
		int maxChildLen = longest_chain(child);
		if (maxChildLen > maxLen)
			maxLen = maxChildLen;
	}
	return 1 + maxLen;
}

int Hierarchy::longest_chain() const
{
	return longest_chain(root);
}

bool Hierarchy::find(const string& name,const TreeNode* root) const
{
	if (!root) return false;
	if (root->name == name) return true;

	bool flag = false;
	for (const TreeNode* child : root->children)
	{
		flag = find(name, child);
		if (flag) return true;
	}
	return false;
}

bool Hierarchy::find(const string& name) const
{
	return find(name, root);
}

int Hierarchy::num_employees(const TreeNode* root) const
{
	if (root == nullptr) return 0;
	int counter = 1;

	queue<const TreeNode*>   wave;
	wave.push(root);

	while (!wave.empty())
	{
		const TreeNode* curr = wave.front();
		wave.pop();

		counter = counter + curr->children.size();

		for (const TreeNode* child : curr->children)
			wave.push(child);
	}
	return counter;
}

int Hierarchy::num_employees() const
{
	return num_employees(root);
}

int Hierarchy::num_overloaded(int level, const TreeNode* root) const
{
	if (root == nullptr) return 0;

	if (root->children.empty()) return 0;

	int counter = 0;
	for (const TreeNode* child : root->children)
	{
		counter += num_overloaded(level, child);
	}
	if ((num_employees(root) - 1) >= level) {
		std::cout << root->name << std::endl;
		return 1 + counter;
	}
	return counter;
}

int Hierarchy::num_overloaded(int level) const
{
	return num_overloaded(level, root);
}

string Hierarchy::manager(const string& name,const TreeNode* root) const
{
	if (!root) return "";

	if (root->parent && root->name == name) return root->parent->name;

	string managerName = "";
	for (TreeNode* child : root->children)
	{
		managerName = manager(name, child);
		if (managerName != "") return managerName;
	}
	return "";
}

string Hierarchy::manager(const string& name) const
{
	return manager(name, root);
}

TreeNode* Hierarchy::getAddrByName(TreeNode* root, const string& name) const
{
	if (!root) return nullptr;

	if (name == "") return nullptr;

	if (root->name == name) return root;

	TreeNode* node = nullptr;

	for (TreeNode* child : root->children)
	{
		node = getAddrByName(child, name);
		if (node) return node;
	}
	return nullptr;
}

TreeNode* Hierarchy::getAddrByName(const string& name) const
{
	//if (name == root->name) return root;

	return getAddrByName(root, name);
}

int Hierarchy::num_subordinates(const string& name) const
{
	const TreeNode* match = getAddrByName(name);
	if(match) return match->children.size();
	return -1;
}

//като тя се определя по следната формула: 500 * <брой преки подчинени> + 50 * <брой не преки подчинени>;
unsigned long Hierarchy::getSalary(const string& who) const
{
	TreeNode* curr = getAddrByName(who);
	if (!curr) return -1;
	int numSubordinates = num_subordinates(who);
	return  500 * numSubordinates + 50 * (num_employees(curr) - numSubordinates - 1);
}

//По подадена йерархия и име на служител трябва да уволните (премахнете) служителя, като всички негови 
//подчинени стават съответно подчинени на ръководителя му. Разбира се, няма как да се уволни Успешния;
bool Hierarchy::fire(const string& who)
{
	if (who == "Uspeshnia") return false;

	TreeNode* toFire = getAddrByName(who);
	if (toFire) 
	{
		int numSubordinates = num_subordinates(who);
		for (size_t i = 0; i < numSubordinates; i++)
		{
			TreeNode* node = toFire->children.front();
			node->parent = toFire->parent;
			toFire->children.pop_front();
			toFire->parent->children.push_back(node);
		}
		numSubordinates = num_subordinates(toFire->parent->name);
		toFire->parent->children.remove(toFire);
		return true;
	}
	return false;
}

//По подадена йерархия, име на служител и име на ръководител трябва да назначите служителя като 
//подчинен на този ръководител. Проверете дали служителят вече не работи с друг ръководител. 
//Тогава преместете служителя на новата му позиция (преназначаване). Ако в този случай служителят е 
//имал подчинени, то те остават под него в йерархията;
bool Hierarchy::hire(const string& who, const string& boss)
{
	TreeNode* pBoss = getAddrByName(boss);
	if (!pBoss) return false;
	if (find(who)) {
		TreeNode* toMove = getAddrByName(who);
		if (toMove->parent->name == boss) return true; //already exists

		toMove->parent->children.remove(toMove);
		toMove->parent = pBoss;
		pBoss->children.push_back(toMove);
		return true;
	}
	else {
		TreeNode* toHire = new TreeNode{ who, pBoss };
		pBoss->children.push_back(toHire);
		return true;
	}
	return false;
}

TreeNode* Hierarchy::findMaxSalary(TreeNode* current)
{
	list<TreeNode*>::const_iterator it = current->children.cbegin();
	TreeNode* newBoss = *(it);
	++it;
	for (; it != current->children.cend(); ++it) {
		if (getSalary(((*it)->name)) > getSalary(newBoss->name)) newBoss = *it;
	}
	return newBoss;
}

void Hierarchy::incorporate(TreeNode* root)
{
	if (!root) return;

	if (root->children.empty()) return;

	for (TreeNode* child : root->children)
	{
		incorporate(child);
	}
	if (root->children.size() >= 2)
	{
		TreeNode* newBoss = findMaxSalary(root);
		for (list<TreeNode*>::iterator it = root->children.begin(); it != root->children.end();) {
			if (*it != newBoss) {
				TreeNode* toMove = *it;
				it = root->children.erase(it);
				toMove->parent = newBoss;
				newBoss->children.push_back(toMove);
			}
			else {
				++it;
			}
		}
	}
}

void Hierarchy::incorporate()
{
	incorporate(root);
}

int Hierarchy::get_level_node(const TreeNode* root) const {
	if (!root || root->name == "Uspeshnia") {
		return 0;
	}
	else {
		return 1 + get_level_node(root->parent);
	}
}

void Hierarchy::modernize(TreeNode* root) {
	if (!root) 
		return;
	for (TreeNode* p : root->children) {
		modernize(p);
	}
	int level = get_level_node(root);
	if (level % 2 == 0) {

		std::list<TreeNode*>::iterator it;
		int children_count = root->children.size();
		for (it = root->children.begin(); it != root->children.end() && children_count > 0; children_count--) {
			if ((*it)->children.size() != 0) {
				std::list<TreeNode*>::iterator iter;
				for (iter = (*it)->children.begin(); iter != (*it)->children.end(); iter++) {
					(*iter)->parent = root;
					root->children.push_back(*iter);
				}
				(*it)->children.clear();
				it = root->children.erase(it);
			}
			else {
				it++;
			}
		}
	}
}

void Hierarchy::modernize()
{
	modernize(root);
	sortLex(root);
}

TreeNode* Hierarchy::findDuplicate(string name, int height, TreeNode* right) const
{
	//End
	if (right == nullptr)
		return nullptr;
	if (right->name == name)
		return right;

	//Traverse
	TreeNode* duplicate;
	for (TreeNode* child : right->children)
	{
		duplicate = findDuplicate(name, height + 1, child);
		if (duplicate) return duplicate; //stops the loop if name is found
	}
}