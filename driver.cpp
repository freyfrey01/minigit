#include <iostream> 
#include "miniGit.hpp"
using namespace std;

void displayMenu()
{
    cout << "Select a numerical option:" << endl;
    cout << "=========Main Menu=========" << endl;
    cout << "1: init" << endl;
    cout << "2: add" << endl;
    cout << "3: rm" << endl;
    cout << "4: commit" << endl;
    cout << "5: checkout" << endl;
    cout << "6: quit" << endl;
    cout << "===========================" << endl;
};

int main()
{
    cout << "Welcome to the minigit 2021 project by Freddy Perez" << endl;
    cout << endl;

    int input=10;
    bool initiated=false;
    minigit Mini;
    while (input!=6)
    {
        displayMenu();
        cin >>input;


        if (input ==6)
        {
            break;
        }
        else
        {
            if (initiated==true)
            {
                switch (input)
                {
                    case 1: cout << "Repository already initiated." << endl;
                    break;
                    case 2: 
                    if (Mini.commitsVec().size()==1)
                    {
                        Mini.addfile();
                        break;
                    }
                    else
                    {
                        if (Mini.commitsVec().at(Mini.commitsVec().size()-1)==Mini.current())
                        {
                            Mini.addfile();
                            break;
                        }
                        else
                        {
                            cout << "Cannot add while not in most recent commit." << endl;
                            cout << "Most recent commit: " << Mini.commitsVec().at(Mini.commitsVec().size()-1)->commitNumber << endl;
                            break;
                        }
                    }
                    case 3:
                    if (Mini.commitsVec().size()==1)
                    {
                        Mini.removeFile();
                        break;
                    }
                    else
                    {
                        if (Mini.commitsVec().at(Mini.commitsVec().size()-1)==Mini.current())
                        {
                            Mini.removeFile();
                            break;
                        }
                        else
                        {
                            cout << "Cannot remove while not in most recent commit." << endl;
                            cout << "Most recent commit: " << Mini.commitsVec().at(Mini.commitsVec().size()-1)->commitNumber << endl;
                            break;
                        }
                    }
                    case 4:
                    if (Mini.commitsVec().size()==1)
                    {
                        Mini.commit();
                        break;
                    }
                    else
                    {
                        if (Mini.commitsVec().at(Mini.commitsVec().size()-1)==Mini.current())
                        {
                            Mini.commit();
                            break;
                        }
                        else
                        {
                            cout << "Cannot commit while not in most recent commit." << endl;
                            cout << "Most recent commit: " << Mini.commitsVec().at(Mini.commitsVec().size()-1)->commitNumber << endl;
                            break;
                        }
                    }
                    case 5: 
                    if (Mini.commitsVec().size()==1)
                    {
                        cout << "Nothing to checkout to." << endl;
                        break;
                    }
                    else
                    {
                        Mini.checkout();
                        break;
                    }
                    default: cout << "Invalid input" << endl;
                    break;
                }
            }
            else
            {
                switch (input)
                {
                    case 1: initiated=true;
                    cout << "Repository initiated." << endl;
                    break;
                    case 2: cout << "Must initiate repository first." << endl;
                    break;
                    case 3: cout << "Must initiate repository first." << endl;
                    break;
                    case 4: cout << "Must initiate repository first." << endl;
                    break;
                    case 5: cout << "Must initiate repository first." << endl;
                    break;
                    default: cout << "Invalid input" << endl;
                    break;
                }
            }
        }
    }
}