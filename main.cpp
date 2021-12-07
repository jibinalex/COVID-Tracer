#include <iostream>
#include <sstream>
#include "Tracer.h"

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
    string firstName;
    string lastName;
    string nickName;
    string YN;
    bool isInfected;

    cout << "What is your first name?" << endl;
    getline(cin, firstName);
    cout << "What is your last name?" << endl;
    getline(cin, lastName);
    cout << "What is your nick name?" << endl;
    getline(cin,  nickName);

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

    graph.selectUser(firstName, lastName, nickName, isInfected);
}
void addConnectionPrompt(Tracer &graph)
{
    string firstName;
    string lastName;
    string nickName;
    bool isInfected;
    int daysSinceLastMet;

    cout << "What is their first name?" << endl;
    cin >> firstName;
    cout << "What is their last name?" << endl;
    cin >> lastName;
    cout << "What is their nick name?" << endl;
    cin >> nickName;

    bool validAnswer = false;
    while (!validAnswer)
    {
        string YN;

        cout << "Are they infected? Y/N" << endl;
        cin >> YN;

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
        cin >> answer;

        if (isNum(answer))
        {
            validAnswer = true;
            daysSinceLastMet = stoi(answer);
        }
        else
            cout << "Invalid input time. Input maybe too long ago or have invalid characters." << endl;
    }

    graph.addConnection(firstName, lastName, nickName, isInfected, daysSinceLastMet);
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
            cout << "0. Exit......................." << endl;
            cout << "1. Change User................" << endl;
            cout << "2. Add Connection............." << endl;
            cout << "3. Print User Details........." << endl;
            cout << "4. Print All User Contacts...." << endl;
            cout << "5. Print Risk................." << endl;


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
            else
                cout << "Invalid input." << endl << endl;

        }
    }

    return 0;
}
