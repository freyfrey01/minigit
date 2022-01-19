#ifndef MINIGIT_HPP
#define MINIGIT_HPP
#include <string>
#include <vector>
using namespace std;

struct singlyNode{
string fileName; // Name of local file
string fileVersion; // Name of file in .minigit folder
singlyNode * next;
};

struct commitNode{
int commitNumber;
singlyNode * head;
};

class minigit
{
    private:
    vector <commitNode*> commits;
    commitNode* currentCommit;    


    public:
    minigit();
    ~minigit();
    void addfile();
    void removeFile();
    void commit();
    void checkout();

    vector <commitNode*> commitsVec();
    commitNode* current();
};

#endif
