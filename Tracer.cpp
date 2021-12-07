#include <set>
#include <iostream>
#include "Tracer.h"

bool Tracer::isEmpty()
{
    return numUsers == 0;
}

void Tracer::selectUser(string firstN, string lastN, string nickN, bool isInfected)
{
    //If the person already exists in the graph
    if (takenNicknames.find(nickN) != takenNicknames.end())
    {
        currentUserIndex = nickNameAdjListIndex[nickN];
    }
    else //else the person doesn't exist in the graph: add them
    {
        addUser(firstN, lastN, nickN, isInfected);
        currentUserIndex = nickNameAdjListIndex[nickN]; //should be adjlist size -1
    }
}
void Tracer::addUser(string firstN, string lastN, string nickN, bool isInfected)
{
    //The nick name doesn't exist yet: add it
    if (takenNicknames.find(nickN) == takenNicknames.end())
    {
        Person newUser(firstN, lastN, nickN, isInfected);

        pair<Person, vector<pair<Person, int>>> userAdjList;

        userAdjList.first = newUser;

        adjList.push_back(userAdjList);

        takenNicknames.insert(nickN);
        nickNameAdjListIndex[nickN] = adjList.size()-1;

        numUsers++;
    }
    else //Nickname exists: throw error
    {
        cout << "Nickname is taken" << endl;
    }
}
void Tracer::addConnection(string firstN, string lastN, string nickN, bool isInfected, int daysLastMet)
{
    //Add the person to the user's adjacency list
    Person contact(firstN, lastN, nickN, isInfected);
    pair<Person, int> connectionToNewPerson;
    connectionToNewPerson.first = contact;
    connectionToNewPerson.second = daysLastMet;

    vector<pair<Person, int> > &userAdjList = adjList[currentUserIndex].second;
    userAdjList.push_back(connectionToNewPerson);


    //If the contact doesn't exist in the graph, add them
    if ( takenNicknames.find(nickN) == takenNicknames.end() )
    {
        addUser(firstN, lastN, nickN, isInfected);
    }
    //Add the user to the new person's adjacency list
    Person user = adjList[currentUserIndex].first;
    pair<Person, int> connectionToUser;
    connectionToUser.first = user;
    connectionToUser.second = daysLastMet;

    vector<pair<Person, int> > &newPersonAdjList = adjList[findPersonIndex(contact)].second;
    newPersonAdjList.push_back(connectionToUser);
}

int Tracer::findPersonIndex(Person target)
{
    int index = -1;

    for (int i=0; i<adjList.size(); i++)
    {
        if (adjList[i].first.nickName == target.nickName)
        {
            index = i;
            break;
        }
    }

    return index;
}

//Print Functions
void Tracer::printUserDetails()
{
    Person user = adjList[currentUserIndex].first;

    cout << "User: " << user.firstName << " " << user.lastName << endl;
    cout << "Status: ";
    if (user.infected)
        cout << "INFECTED" << endl;
    else
        cout << "NOT Infected" << endl;
}
void Tracer::printAllUserContacts()
{
    vector<pair<Person, int> > userAdjList = adjList[currentUserIndex].second;
    for (int i=0; i<userAdjList.size(); i++)
    {
        cout << endl;
        cout << "Recent Contacts:" << endl;

        Person contact = userAdjList[i].first;
        int daysLastMet = userAdjList[i].second;

        cout << endl;
        cout << "Name: " << contact.firstName << " " << contact.lastName << endl;
        if (contact.infected)
            cout << "Status: INFECTED" << endl;
        else
            cout << "Status: NOT Infected" << endl;
        cout << "Days Since Last Met: " << daysLastMet << endl;
    }
}
void Tracer::printRisk(Person src)
{

}

vector<Tracer::Person> Tracer::djikstraShortestPathToInfected(Person user)
{
    vector<pair<Person, int>> userAdjList = adjList[currentUserIndex].second;

    set<Person> waiting;
    for (int i=0; i<userAdjList.size(); i++)
    {
        Person index = userAdjList[i].first;

        waiting.insert(index);
    }

    vector<Person> weights(numUsers);
    vector<int> parents(numUsers);
    for (int i=0; i<numUsers; i++)
    {
        weights[i] = 2147483647;
        parents[i] = -1;
    }
    weights[user] = 0;

    while ( !waiting.empty() )
    {
        int u = *(waiting.begin());
        for (auto it = waiting.begin(); it != waiting.end(); ++it)
        {
            if (d[*it] < d[u])
                u = *it;
        }
        waiting.erase(u);

        vector<pair<int, int>> adj = graph.adjList[u];
        for (int i=0; i<adj.size(); i++)
        {
            int v = adj[i].first;
            int w = adj[i].second;

            if (d[u] + w  < d[v])
            {
                d[v] = d[u] + w;
                p[v] = u;
            }
        }
    }

    return d;
}

//vector<Tracer::Person> Tracer::bellmanShortestPathToInfected(Person user)
//{
//
//}