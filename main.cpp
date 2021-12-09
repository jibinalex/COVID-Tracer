#include <iostream>
#include <sstream>
#include "Tracer.h"

using namespace std;

bool isNum(string id) //Returns true if id is 8 digits and is only numbers
{
    bool valid = true;

    if (id.size()<3)
    {
        for (int i=0; i<id.size(); i++)
        {
            if ( !isdigit(id.at(i)) )
            {
                valid = false;
            }
        }
    }
    else
    {
        valid = false;
    }

    return valid;
}

void addUserPrompt(Tracer &graph)
{
    string id;
    cout << "What is your ID?" << endl;
    getline(cin, id);

    if (!graph.idExists(id))
    {
        string name;
        string YN;
        bool isInfected;

        cout << "What is your name?" << endl;
        getline(cin, name);


        bool validAnswer = false;
        while (!validAnswer)
        {
            string YN;

            cout << "Are you infected? Y/N" << endl;
            getline(cin, YN);

            if (YN == "Y")
            {
                validAnswer = true;
                isInfected = true;
            }
            else if (YN == "N")
            {
                validAnswer = true;
                isInfected = false;
            }
            else
                cout << "Answer 'Y' or 'N'." << endl;
        }

        graph.selectUser(name, id, isInfected);
    }
    else
    {
        graph.selectUser("", id, false);
    }
}
void addConnectionPrompt(Tracer &graph)
{
    string id;
    cout << "What is their ID?" << endl;
    getline(cin,  id);

    if (!graph.idExists(id))
    {
        string name;
        bool isInfected;
        int daysSinceLastMet;

        cout << "What is their name?" << endl;
        getline(cin, name);

        bool validAnswer = false;
        while (!validAnswer)
        {
            string YN;

            cout << "Are they infected? Y/N" << endl;
            getline(cin, YN);;

            if (YN == "Y")
            {
                validAnswer = true;
                isInfected = true;
            }
            else if (YN == "N")
            {
                validAnswer = true;
                isInfected = false;
            }
            else
                cout << "Answer 'Y' or 'N'." << endl;
        }

        validAnswer = false;
        while (!validAnswer)
        {
            string answer;

            cout << "How many days since you last meet them?" << endl;
            getline(cin, answer);;

            if (isNum(answer))
            {
                validAnswer = true;
                daysSinceLastMet = stoi(answer);
            }
            else
                cout << "Invalid input time. Input maybe too long ago or have invalid characters." << endl;
        }

        graph.addNewConnection(name, id, isInfected, daysSinceLastMet);
    }
    else
    {
        if (!graph.isConnected(id))
        {
            bool validAnswer = false;
            int daysSinceLastMet;

            validAnswer = false;
            while (!validAnswer)
            {
                string answer;

                cout << "How many days since you last meet them?" << endl;
                getline(cin, answer);;

                if (isNum(answer))
                {
                    validAnswer = true;
                    daysSinceLastMet = stoi(answer);
                }
                else
                    cout << "Invalid input time. Input maybe too long ago or have invalid characters." << endl;
            }

            graph.addExistingConnection(id, daysSinceLastMet);
        }
        else
            cout << "Already connected!" << endl;

    }
}

int main()
{
    string opening = "This is the Opening Prompt...........";
    cout << opening << endl;

    Tracer graph;

    bool on = true;

    while (on)
    {
        if (graph.isEmpty())
        {
            addUserPrompt(graph);
        }
        else
        {
            //Print options
            cout << endl;
            cout << "Main Menu:" << endl;
            cout << "0. Exit.........................................................." << endl;
            cout << "1. Change User..................................................." << endl;
            cout << "2. Add Connection................................................" << endl;
            cout << "3. Print User Details............................................" << endl;
            cout << "4. Print All User Contacts......................................." << endl;
            cout << "5. Print Shortest Path to Infected..............................." << endl;
            cout << "6. Show Number of Users in the Graph............................." << endl;
            cout << "7. Generate Random Graph 100k Vertices (Debugging)..............." << endl;

            string command;
            getline(cin,  command);
            if (command == "0")
            {
                cout << "Goodbye" << endl;

                on = false;
            }
            else if (command == "1")
            {
                addUserPrompt(graph);
            }
            else if (command == "2")
            {
                addConnectionPrompt(graph);
            }
            else if (command == "3")
            {
                graph.printUserDetails();
            }
            else if (command == "4")
            {
                graph.printAllUserContacts();
            }
            else if (command == "5")
            {
                graph.printShortestPathToInfected();
            }
            else if (command == "6")
            {
                cout << graph.getNumUsers() << endl;
            }
            else if (command == "7")
            {
                graph.randomGraph();
            }
            else
                cout << "Invalid input." << endl << endl;

        }
    }

    return 0;
}
