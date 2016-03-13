#include "calculator.h"
#include <iostream>
#include "fraction.h"
#include "mixed.h"
#include <string>
#include "Stack.h"
#include "Queue.h"
#include "Parser.h"
#include "memory.h"
#include <cstdlib>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::ostream;
using std::istream;

enum MEMORYERROR {MEMORYINDEXERROR};

calculator::calculator()
{
    m = new memory;
    expression = "";
    saved = false;
    isRunning =true;
    //p = new Parser;
}

void calculator::commands()
{
    string userInput = "werite<fSfacsfacsfas.txt>", lhs, rhs;
//    userInput = "write<a.txt>";
    string nochangestr;
    string fileName = "untitled";
    string clear = "CLEAR";
    string quit = "QUIT";
    string exit = "EXIT";
    string write = "WRITE";


    m->setMemory('M'-'A',1);
    m->setMemory('C'-'A',1);
    m->setMemory('D'-'A',1);

    cout<<"Enter an expression or command: ";
    getline(cin, userInput);

    //check if the user saved the memory or not
    saved = isSaved(userInput);

    //if user press enter
    if(userInput.length()==0)
        exitCalculator();

    nochangestr = userInput;
    userInput = trimBoth(userInput);

    for(unsigned int i = 0; i < userInput.length(); i++)
    {
        userInput[i] = toupper(userInput[i]);
    }

    int equalIndex = userInput.find_first_of("=");
    /**
//   1. //check for the equal sign;
//        1.1 // if there's an equal sign splite to two
//            1.1.1//lhs convert to a integer for memory location
//                    1.1.1.1 //trim both side and remove the equal side
//                    1.1.1.2 // convert the letter to an index for the memorty slot
//            1.1.2 //rhs pass it to the parser to be processed;
//                    //might have to check for letters
//                    // if there are letters convert them to mixed fraction
//                    1.1.2.1 //do the calculation and return a mixed fration
//            1.1.3 set the index from 1.1.1.2 to the mixed fraction from 1.1.2.1
//    //when there are no euqal sign check for "CLEAR"
//        //if there's a clear
//            if it match "CLEAR"
//                clear all the memory slots
//            if it does not match "CLEAR"
//                find the index of "<" then access that index +1
//                to find the letter's memeory location to clear
//     //exit or quit
//        when exit or quit is detected
//            check if the file is saved
//               if file is saved exit the program
//               else if not saved ask user to save the file
//                    if no exit
//                    if yes ask for file name and save the file with the given0 namespace
//        when exit <filename> or quit<filename> is detected
//            save memory to file
//            exit calculator
//     //write<filename>
//            save the memory with the give file name
    */

    //   1. //check for the equal sign;
    if(equalIndex!= -1)
    {
        lhs = trimBoth(userInput.substr(0,equalIndex-1));
        cout<<lhs<<endl;
        cout<<lhs.size()<<endl;

        //when there are more than one letter on the left of the equal
        // sign it will throw an error
        if(lhs.size() != 1)
            throw MEMORYINDEXERROR;


        rhs = trimBoth(userInput.substr(equalIndex+1, userInput.size()));
        cout<<rhs<<endl;
        cout<<rhs.size()<<endl;

        //if there are no operator
            // convert the rhs to a mixed fraction
        //process rhs to get a mixed fraction

        mixed mTemp(1,2,3);
        cout<<lhs[0]<<" "<<"size: "<<(lhs[0]-'A')<<endl;
        m->setMemory((lhs[0]-'A'),mTemp);

        cout<<*m<<endl;
    }
    else //where there are no equal sign
    {
        string clr = userInput.substr(0,clear.length());
        string qut = userInput.substr(0,quit.length());
        string wrt = userInput.substr(0, write.length());

        if(clr == clear)    //CLEAR or CLEAR<M> is entered
        {
            if(userInput.length() == clear.length())
            {
                m->clearMemory();
                cout<<"All memory slots set to 0."<<endl;
            }
            else if(userInput.length() >clear.length())
            {
                int indexOfLetter = userInput.find_first_of("<")+1;
                m->clearMemoryPos(userInput[indexOfLetter]-'A');
                cout<<"Memory slot "<<userInput[indexOfLetter]<<" cleared."<<endl;
            }
        }
        else if(qut == quit || qut == exit) // exit or quit
        {
            if(qut.length() == userInput.length()) //exit and ask user for file name to save memory
            {
                //ask if the user want to quit
                string qe = "z";
                string sav = "n";

                cout<<"Quit? (Y/N): ";
                getline(cin,qe);

                if(qe[0]=='y' || qe[0] == 'Y')
                {
                    if(!saved)
                    {
                        cout<<"Do you want to save the memory? : (Y/N)";
                        getline(cin,sav);

                        if((sav[0] == 'y' || sav[0] == 'Y'))
                        {
                            cout<<"Please enter a file name with extension: ";
                            getline(cin, fileName);

                            // need to check if the file name is proper or not
                            m->saveMemory(fileName);
                            cout<<"Memory saved to file: "<< fileName<<endl;
                        }
                        exitCalculator();
                    }
                    else
                    {
                        m->clearMemory();
                        exitCalculator();
                    }
                }
                //ask if the user want to saved to file or no
                //if yes ask for file name;
                    //check if the file name is proper
            }
            else if(userInput.length() > quit.length()) // exit<fileName> or quit<fileName> are enter
            {
                //prase the string from '<' to '>' to extract the file name
                //fileName = nochangestr.substr(nochangestr.find_first_of("<")+1,nochangestr.find_last_of(">") - nochangestr.find_first_of("<")-1);
                fileName = extractFileName(nochangestr);
                m->saveMemory(fileName);
                cout<<"Memory saved to: "<<fileName<<endl;
                exitCalculator();
            }
        }
        else if(wrt == write) // write memory to file
        {
            fileName = extractFileName(nochangestr);
            m->saveMemory(fileName);
            cout<<"Memory saved to: "<<fileName<<endl;
            saved = true;
        }
        else // some expression is enter need to prase it
        {
            // pass it down to the parser
        }
    }

    {
    //        1.1 // if there's an equal sign splite to two
    //            1.1.1//lhs convert to a integer for memory location
    //                    1.1.1.1 //trim both side and remove the equal side
    //                    1.1.1.2 // convert the letter to an index for the memorty slot
    //            1.1.2 //rhs pass it to the parser to be processed;
    //                    //might have to check for letters
    //                    // if there are letters convert them to mixed fraction
    //                    1.1.2.1 //do the calculation and return a mixed fration
    //            1.1.3 set the index from 1.1.1.2 to the mixed fraction from 1.1.2.1
    //    //when there are no euqal sign check for "CLEAR"
    //        //if there's a clear
    //            if it match "CLEAR"
    //                clear all the memory slots
    //            if it does not match "CLEAR"
    //                find the index of "<" then access that index +1
    //                to find the letter's memeory location to clear
    //     //exit or quit
    //        when exit or quit is detected
    //            check if the file is saved
    //               if file is saved exit the program
    //               else if not saved ask user to save the file
    //                    if no exit
    //                    if yes ask for file name and save the file with the given0 namespace
    //        when exit <filename> or quit<filename> is detected
    //            save memory to file
    //            exit calculator
    //     //write<filename>
    //            save the memory with the give file name
    }
}

string calculator::trimBoth(string str)
{
    str.erase(0, str.find_first_not_of(" \n\r\t"));
    str.erase(str.find_last_not_of(" \n\r\t")+1);
    return str;
}

string calculator::extractFileName(string str)
{
    return str.substr(str.find_first_of("<")+1,str.find_last_of(">") - str.find_first_of("<")-1);
}

bool calculator::isSaved(string str)
{
    string checkstr = "WRITE<";
    string temp = str;
    for(unsigned i = 0; i<str.length(); i++)
    {
        temp[i] = toupper(temp[i]);
    }

    if((temp.substr(0,checkstr.length()) != checkstr))
    {
        return false;
    }

    return true;
}

void calculator::exitCalculator()
{
    cout<<"Good Bye!"<<endl;
    exit(0);
}

