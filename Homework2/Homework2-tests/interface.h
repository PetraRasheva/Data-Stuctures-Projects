#pragma once
#include <string>
#include <list>
#include <queue>
#include <fstream>

using std::string;
using std::list;
using std::queue;
using std::stringstream;

struct TreeNode
{
    TreeNode(string name, TreeNode* parent) : name(name), parent(parent) {};
    string name{};
    TreeNode* parent{};
    list<TreeNode*> children{};
};

class Hierarchy
{
public:
    //Hierarchy() { root = new TreeNode{ "Uspeshnia" , nullptr}; }
    Hierarchy(Hierarchy&& r) noexcept;
    Hierarchy(const Hierarchy& r);
    Hierarchy(const string& data);
    ~Hierarchy() noexcept;
    void operator=(const Hierarchy&) = delete;

    string print()const;

    int longest_chain() const; //done
    bool find(const string& name) const; //done
    int num_employees() const; //done
    int num_overloaded(int level = 2) const; //done

    string manager(const string& name) const; //done
    int num_subordinates(const string& name) const; //done
    unsigned long getSalary(const string& who) const;

    bool fire(const string& who);
    bool hire(const string& who, const string& boss);

    void incorporate();
    void modernize();

    Hierarchy join(const Hierarchy& right) const;

    //If you need it - add more public methods here
    TreeNode* clone(TreeNode* other, TreeNode* parent);
    void BFS(TreeNode* root,const Hierarchy& right, int height) const;
    TreeNode* findDuplicate(string name, int height, TreeNode* right) const;
    int get_level_node(const TreeNode* root) const;
    void modernize(TreeNode* root);
    int longest_chain(const TreeNode* root) const;
    void create_tree(string& data);
    void erase_tree(TreeNode*& root);
    void read_input(const string& filename);
    void sortLex(TreeNode* root);
    string getName() const { return name; };
    TreeNode* findMaxSalary(TreeNode* current);
    TreeNode* getAddrByName(const string& name) const;
    TreeNode* getAddrByName(TreeNode* root, const string& name) const;
    int num_employees(const TreeNode* root) const;
    bool find(const string& name, const TreeNode* root) const;
    int num_overloaded(int level, const TreeNode* root) const;
    string manager(const string& name,const TreeNode* root) const;
    void incorporate(TreeNode* root);
    void setName(string name) { this->name = name; };
private:
    TreeNode* root{};
    string name{};

    //Add whatever you need here
};
