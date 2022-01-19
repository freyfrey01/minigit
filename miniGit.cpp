#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>

#include "miniGit.hpp"
using namespace std;

minigit::minigit()
{
    namespace fs = std::filesystem;
    fs::remove_all(".minigit"); // removes a directory and its contents
    fs::create_directory(".minigit");
    commitNode* newNode=new commitNode;
    newNode->commitNumber=0;
    newNode->head=NULL;
    commits.push_back(newNode);
    currentCommit=newNode;
}
minigit::~minigit()
{
    namespace fs = std::filesystem;
    fs::remove_all(".minigit"); // removes a directory and its contents
    for (int i=commits.size()-1;i>=0;i--)
    {
        if (commits[i]->head!=NULL)
        {
            singlyNode *ptr=commits[i]->head;
            singlyNode *temp;
            while (ptr!=NULL)
            {
                temp=ptr->next;
                delete ptr;
                ptr=temp;
            }
            commits[i]->head=NULL;
        }
        commitNode *ptr=commits[i];
        commits.pop_back();
        delete ptr;
        ptr=NULL;
    }
    cout << "Deleted files and directory." << endl;
}

string initialFile(string filename)
{
    string fileVer=filename+"   ";
    fileVer[fileVer.size()]=*filename.end();
    fileVer[fileVer.size()-1]=*(filename.end()-1);
    fileVer[fileVer.size()-2]=*(filename.end()-2);
    fileVer[fileVer.size()-3]=*(filename.end()-3);
    fileVer[fileVer.size()-4]=*(filename.end()-4);   // create file name
    fileVer[filename.size()-4]='-';
    fileVer[filename.size()-3]='0';
    fileVer[filename.size()-2]='0';
    return fileVer;
}
singlyNode* newSLLNode(string filename, string fileversion, singlyNode* next)
{
    singlyNode* newNodeSLL= new singlyNode;
    newNodeSLL->fileName=filename;
    newNodeSLL->fileVersion=fileversion;
    newNodeSLL->next=next;
    return newNodeSLL;
}
singlyNode* mostRecentEdition(string filename,vector <commitNode*> commits)
{
    singlyNode* temp,*prev;
    bool found=false;
    for (int i=commits.size();i>0;i--)
    {
        temp=commits[i-1]->head;
        prev=NULL;
        while (temp!=NULL)
        {
            if (temp->fileName==filename)
            {
                found=true;
                break;
            }
            else  
            {
                prev=temp;
                temp=temp->next;
            }
        }
        if (found==true)
        {
            break;
        }
    }
    return temp;
}
singlyNode* currentCommitLastFile(commitNode* currentCommit)
{
    singlyNode* tempSLL=currentCommit->head;
    while (tempSLL->next!=NULL)
    {
        tempSLL=tempSLL->next;
    }
    return tempSLL;
}
void minigit::addfile()
{
    if (commits.empty()==true)
    {
        cout << "Must first initialize repo." << endl;
    }
    else
    {
        string filename;
        cout << "Enter a filename" << endl;
        cin >> filename;
        ifstream file;
        ifstream isopenfile;
        file.open(filename);
        int filevs;
        if (file.is_open())
        {
            singlyNode* tempSLL=currentCommit->head;
            while (tempSLL!=NULL)
            {
                if (tempSLL->fileName==filename)
                {
                    cout << "File is already added to commit." << endl; /// check of file is already
                    return;
                }
                else
                {
                    tempSLL=tempSLL->next;
                }
            }

            if (currentCommit==commits[0])
            {
                string fileVer=initialFile(filename); //create file name;
                if (currentCommit->head==NULL)
                {
                    
                    currentCommit->head=newSLLNode(filename,fileVer,NULL);  
                }
                else
                {
                    tempSLL=currentCommitLastFile(currentCommit);
                    tempSLL->next=newSLLNode(filename,fileVer,NULL);  
                }
            }
            else
            {
                singlyNode* temp= mostRecentEdition(filename,commits);

                if (temp==NULL)
                {
                    string fileVer=initialFile(filename); //create file name;  
                    if (currentCommit->head==NULL)
                    {
                        currentCommit->head=newSLLNode(filename,fileVer,NULL);
                    }
                    else
                    {
                        tempSLL=currentCommitLastFile(currentCommit);
                        tempSLL->next=newSLLNode(filename,fileVer,NULL);
                    }
                }
                else
                {
                    string fileVer=temp->fileVersion,filevss;
                    if (currentCommit->head==NULL)
                    {
                        currentCommit->head=newSLLNode(filename,fileVer,NULL);             //create head of LL
                    }
                    else
                    {
                        tempSLL=currentCommitLastFile(currentCommit);
                        tempSLL->next=newSLLNode(filename,fileVer,NULL);
                    }
                }
            }
            file.close();
        }
        else
        {
         cout << "Enter a correct file name." << endl;
        }
    } 
}

void deleteNode(singlyNode*& prev, singlyNode*&current, singlyNode*& next)
{
    prev->next=next;
    delete current;
    current=NULL;
}
singlyNode* searchNode(commitNode* currentCommit,string filename)
{
    singlyNode* tempSLL=currentCommit->head;
    while (tempSLL!=NULL)
    {
        if (tempSLL->fileName==filename)
        {
            return tempSLL;
        }
        else
        {
            tempSLL=tempSLL->next;
        }
    }
    return NULL;
}
void minigit::removeFile()
{
    string filename;
    bool found=false;
    cout << "Enter a file name" << endl;
    cin >>filename;
    
    
    if (currentCommit->head==NULL)
    {
        cout << "File list already empty" << endl;
    }
    else
    {
        singlyNode* tempSLL=searchNode(currentCommit,filename);
        if (tempSLL==NULL)
        {
            cout << "File not found in current commit." << endl;
        }
        else
        {
            if (tempSLL==currentCommit->head)
            {
                currentCommit->head=tempSLL->next;
                delete tempSLL;
                tempSLL=NULL;
            }
            else
            {
                tempSLL=currentCommit->head;
                singlyNode* prev=NULL;
                while (tempSLL!=NULL)
                {
                    if (tempSLL->fileName==filename)
                    {
                        break;
                    }
                    else
                    {
                        prev=tempSLL;
                        tempSLL=tempSLL->next;
                    }
                }
                deleteNode(prev,tempSLL,tempSLL->next);
            }
        }
    }
}
void minigit::commit()
{
    ifstream fileRead;
    ifstream infile;
    ofstream fileOut;
    bool sameList=true;
    int commitnumber=currentCommit->commitNumber;
    if (currentCommit->head==NULL)
    {
        cout << "Nothing to commit" << endl;                //checking whethere git is keeping track of files
    }
    else
    {
        singlyNode* tempSLL=currentCommit->head;
        while (tempSLL!=NULL)                           //go through list of files of currentCommmit
        {
            string line;
            string line2;
            bool same=true;
            bool sameLines;
            int filevs;
            string filevss;
            string fileVer;
            infile.open(tempSLL->fileName);
            infile.clear();
            infile.seekg(0);
            fileRead.open(".minigit/"+tempSLL->fileVersion);
            if (fileRead.is_open()==false)
            {
                fileOut.open(".minigit/"+tempSLL->fileVersion);    //if file with corresponding file version doesn't exist
                while (getline(infile,line))
                {
                    fileOut << line << endl;
                }
                infile.clear();
                infile.seekg(0);
            }
            else
            {
                int c1=0,c2=0;
                while (getline(infile,line))
                {
                    c1++;
                }
                infile.clear();
                infile.seekg(0);
                while (getline(fileRead,line2))
                {
                    c2++;
                }
                fileRead.clear();
                fileRead.seekg(0);
                if (c1==c2)
                {
                    while (getline(infile,line)&& getline(fileRead,line2))
                    {
                        if (line!=line2)
                        {
                            same=false;     
                            sameList=false;                                        //if exists check whethere the files are the same
                            break;
                        }
                        line="";
                        line2="";
                    }
                    infile.clear();
                    infile.seekg(0);
                    fileRead.clear();
                    fileRead.seekg(0);
                    fileRead.close();
                }
                else
                {
                    same=false;
                    sameList=false;
                }


                if (same==true)
                {
                    cout << tempSLL->fileName << " was unchanged." << endl;             //files were unchanged
                }
                else
                {
                    fileVer=tempSLL->fileVersion;
                    for (int i=0;i<fileVer.size();i++)
                    {                                               //file was changed
                        if (fileVer[i]=='-')
                        {
                            string sub=fileVer.substr(i+1,2);
                            filevs=stoi(sub);
                            filevs++;
                            filevss=to_string(filevs);
                            if (filevs<10)
                            {
                                filevss="0"+filevss;
                            }
                            fileVer[i+1]=filevss[0];                    //updating fileversion
                            fileVer[i+2]=filevss[1];
                            break;
                        }
                    }
                    fileOut.open(".minigit/"+fileVer);
                    while (getline(infile,line))
                    {
                        fileOut << line << endl;                //writing to new file
                    }

                    tempSLL->fileVersion=fileVer;               //sets new file version of file
                }
            }
            infile.clear();
            infile.seekg(0);
            infile.close();
            fileOut.close();
            tempSLL=tempSLL->next;
        }
        tempSLL=NULL;
        if(sameList==false||currentCommit==commits[0])
        {
            commitNode* newNodeDLL=new commitNode;
            newNodeDLL->commitNumber=commitnumber+1;            //create new commit

            tempSLL=currentCommit->head;
            singlyNode * newNodeSLL= new singlyNode;
            newNodeSLL->fileName=tempSLL->fileName;
            newNodeSLL->fileVersion=tempSLL->fileVersion;
            newNodeSLL->next=NULL;                              //copy head node
            newNodeDLL->head=newNodeSLL;
            tempSLL=tempSLL->next;                              //move temp to ll heads next
            singlyNode* newNodeCrawler=newNodeDLL->head;

            while (tempSLL!=NULL)
            {
                singlyNode * newNodeSLL= new singlyNode;
                newNodeSLL->fileName=tempSLL->fileName;             //while temp!=NULL
                newNodeSLL->fileVersion=tempSLL->fileVersion;           //deep copy to new DLL
                newNodeSLL->next=NULL;
                newNodeCrawler->next=newNodeSLL;
                newNodeCrawler=newNodeCrawler->next;
                tempSLL=tempSLL->next;
            }

            commits.push_back(newNodeDLL);
            currentCommit=newNodeDLL;
        }
    }    
    cout << "Current commit number: " << currentCommit->commitNumber << endl;
}
void minigit::checkout()
{
    int commitnumber;
    ofstream fileOut;
    ifstream fileIn;
    singlyNode* tempSLL;
    string line;
    char choice='x';
    cout << "Please enter a commit number" << endl;
    cin >>commitnumber;
    if (commitnumber<0||commitnumber>=commits.size())
    {
        cout << "No commit matches given commit number (" << commitnumber << ")." << endl;
    }
    else
    {
        cout << "Are you sure you would like to overwrite current directories?" << endl;
        cout << "Y/N" << endl;
        while (choice!='N'&& choice!='Y'&&choice!='n'&&choice!='y')
        {
            cin >> choice;
            switch (choice)
            {
                case 'Y':
                case 'y': 
                tempSLL=commits[commitnumber]->head;
                while (tempSLL!=NULL)
                {
                    fileOut.open(tempSLL->fileName);
                    fileOut.clear();
                    fileIn.open(".minigit/"+tempSLL->fileVersion);
                    while (getline(fileIn,line))
                    {
                        fileOut<<line<<endl;
                    }
                    tempSLL=tempSLL->next;
                    fileOut.close();
                    fileIn.close();
                }
                currentCommit=commits[commitnumber];
                break;
                case 'N':
                case 'n': cout << "No changes made" << endl;
                break;break;
                default: cout << "Invalid option (Y/N)" << endl;
            }
        }
    }
}

vector <commitNode*> minigit::commitsVec()
{
    return commits;
}

commitNode* minigit::current()
{
    return currentCommit;
}