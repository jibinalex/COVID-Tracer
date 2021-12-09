#include <set>
#include <algorithm>
#include <queue>
#include <random>
#include <unordered_map>
#include <iostream>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include "Tracer.h"

using namespace std;
using namespace std::chrono;

bool Tracer::isEmpty()
{
    return numUsers == 0;
}

void Tracer::selectUser(string name, string id, bool isInfected)
{
    //If the person already exists in the graph
    if (takenIDs.find(id) != takenIDs.end())
    {
        currentUserIndex = idAdjListIndex[id];
    }
    else //else the person doesn't exist in the graph: add them
    {
        addUser(name, id, isInfected);
        currentUserIndex = idAdjListIndex[id]; //should be adjlist size -1
    }
}
void Tracer::addUser(string name, string id, bool isInfected)
{
    //The nick name doesn't exist yet: add it
    if (takenIDs.find(id) == takenIDs.end())
    {
        Person newUser(name, id, isInfected);

        pair<Person, vector<pair<Person, int>>> userAdjList;

        userAdjList.first = newUser;

        adjList.push_back(userAdjList);

        takenIDs.insert(id);
        idAdjListIndex[id] = adjList.size()-1;

        numUsers++;
    }
    else //Nickname exists: throw error
    {
        cout << "Nickname is taken" << endl;
    }
}
void Tracer::addNewConnection(string name, string id, bool isInfected, int daysLastMet)
{
    if (id != adjList[currentUserIndex].first.id)
    {
        //If the contact doesn't exist in the graph, add them
        if ( takenIDs.find(id) == takenIDs.end() )
        {
            addUser(name, id, isInfected);
        }

        //Add the person to the user's adjacency list
        Person contact = adjList[idAdjListIndex[id]].first;
        pair<Person, int> connectionToNewPerson;
        connectionToNewPerson.first = contact;
        connectionToNewPerson.second = daysLastMet;

        vector<pair<Person, int> > &userAdjList = adjList[currentUserIndex].second;
        userAdjList.push_back(connectionToNewPerson);

        //Add the user to the contact's adjacency list
        Person user = adjList[currentUserIndex].first;
        pair<Person, int> connectionToUser;
        connectionToUser.first = user;
        connectionToUser.second = daysLastMet;

        vector<pair<Person, int> > &newPersonAdjList = adjList[findPersonIndex(contact)].second;
        newPersonAdjList.push_back(connectionToUser);
    }
    else if (!testing)
        cout << "Can't add connection to yourself!" << endl;
}
void Tracer::addExistingConnection(string id, int daysLastMet)
{
    if (id != adjList[currentUserIndex].first.id)
    {
        Person contact = adjList[idAdjListIndex[id]].first;
        pair<Person, int> connectionToNewPerson;
        connectionToNewPerson.first = contact;
        connectionToNewPerson.second = daysLastMet;

        vector<pair<Person, int> > &userAdjList = adjList[currentUserIndex].second;
        userAdjList.push_back(connectionToNewPerson);

        //Add the user to the contact's adjacency list
        Person user = adjList[currentUserIndex].first;
        pair<Person, int> connectionToUser;
        connectionToUser.first = user;
        connectionToUser.second = daysLastMet;

        vector<pair<Person, int> > &newPersonAdjList = adjList[findPersonIndex(contact)].second;
        newPersonAdjList.push_back(connectionToUser);
    }
    else if(!testing)
    {
        cout << "Can't add connection to yourself!" << endl;
    }
}

//Getters
int Tracer::findPersonIndex(Person target)
{
    int index = -1;

    for (int i=0; i<adjList.size(); i++)
    {
        if (adjList[i].first.id == target.id)
        {
            index = i;
            break;
        }
    }

    return index;
}
bool Tracer::idExists(string id)
{
    return takenIDs.find(id)!=takenIDs.end();
}
bool Tracer::isConnected(string id)
{
    bool alreadyConnected = false;
    vector<pair<Person, int> > &userAdjList = adjList[currentUserIndex].second;
    for (int i=0; i<userAdjList.size(); i++)
    {
        if (userAdjList[i].first.id == id)
            alreadyConnected = true;
    }

    return alreadyConnected;
}
int Tracer::getNumUsers()
{
    return adjList.size();
}

//Print Functions
void Tracer::printShortestPathToInfected()
{
    vector<int> shortestPathDJ = djikstraShortestPathToInfected();
    vector<int> shortestPathBF = bellmanShortestPathToInfected();

//    if (shortestPathDJ == shortestPathDJ)
//        cout << "Same" << endl;
//    if (shortestPathDJ.size() == shortestPathDJ.size())
//        cout << "Same Length" << endl;

    if (adjList[currentUserIndex].first.infected)
    {
        cout << "You are infected." << endl;
    }
    else if (shortestPathDJ.size() > 1)
    {
        int count=1;
        Person src = adjList[currentUserIndex].first;
        cout << count << ". " << src.name ;

        for (int i = shortestPathDJ.size() - 2; i > 0; i--) {
            Person contact = adjList[shortestPathDJ[i]].first;

            cout << " contacted " << contact.name << "." << endl;
            cout << ++count << ". " << contact.name;
        }
        cout << " contacted " << adjList[shortestPathDJ[0]].first.name << " who is infected." << endl;
    }
    else if (adjList[currentUserIndex].second.empty())
    {
        cout << "You haven't been in contact with anyone recently." << endl;
    }
    else
    {
        cout << "No one that you've been in contact with has been in contact with someone infected." << endl;
    }

}
void Tracer::printUserDetails()
{
    Person user = adjList[currentUserIndex].first;

    cout << "User: " << user.name << endl;
    cout << "ID: " << user.id << endl;
    cout << "Status: ";
    if (user.infected)
        cout << "INFECTED" << endl;
    else
        cout << "NOT Infected" << endl;
}
void Tracer::printAllUserContacts()
{
    vector<pair<Person, int> > userAdjList = adjList[currentUserIndex].second;

    cout << endl;
    cout << "Recent Contacts:" << endl;

    if (userAdjList.empty())
        cout << "You haven't been in contact with anyone recently." << endl;

    for (int i=0; i<userAdjList.size(); i++)
    {
        Person contact = userAdjList[i].first;
        int daysLastMet = userAdjList[i].second;

        cout << endl;
        cout << "Name: " << contact.name << endl;
        cout << "ID: " << contact.id << endl;
        if (contact.infected)
            cout << "Status: INFECTED" << endl;
        else
            cout << "Status: NOT Infected" << endl;
        cout << "Days Since Last Met: " << daysLastMet << endl;
    }
}

//Test
void Tracer::randomGraph()
{
    testing = true;

    for (int i=0; i<100000; i++)
    {
        string name = "";
        string strId = "";
        bool infected = false;
        int days;


        string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        for (int i=0; i<3; i++)
        {
            name += alphabet.at((rand() % 51) + 0);
        }


        for (int i=0; i<7; i++)
        {
            strId += to_string((rand() % 9) + 0);
        }
        //strId += to_string((rand() % 99999) + 0);


        days = (rand() % 99) + 0;


        if (idExists(strId))
        {
            addExistingConnection(strId, days);
        }
        else
        {
            if (i%10==0)
            {
                selectUser(name, strId, infected);
            }
            else
            {
                if (i%41==0)
                    infected = true;

                addNewConnection(name, strId, infected, days);
            }
        }
    }
    cout << "Users generated: " <<  numUsers << endl;

    testing = false;
}

vector<int> Tracer::djikstraShortestPathToInfected()
{
    //--------------------------------Timer Start----------------------------//
    srand(time(NULL));
    auto start = high_resolution_clock::now();

    //Add all vectors of graph to waiting set
    set<int> waiting;
    queue<int> needsChecking;
    needsChecking.push(currentUserIndex);
    while (!needsChecking.empty())
    {
        int curr = needsChecking.front();
        needsChecking.pop();

        if (waiting.find(curr) == waiting.end())
        {
            waiting.insert(curr);

            vector<pair<Person, int>> currAdjList = adjList[curr].second;
            for (int i=0; i<currAdjList.size(); i++)
            {
                Person index = currAdjList[i].first;

                needsChecking.push(findPersonIndex(index));
            }
        }
    }

    vector<int> weights(numUsers, INT_MAX);
    vector<int> parents(numUsers, -1);
    weights[findPersonIndex(adjList[currentUserIndex].first)] = 0;

    while ( !waiting.empty() )
    {
        //Choose the vertex with shortest path
        int currentVertex = *(waiting.begin());
        for (auto it = waiting.begin(); it != waiting.end(); ++it)
        {
            if (weights[*it] < weights[currentVertex])
                currentVertex = *it;
        }
        waiting.erase(currentVertex);

        //Add the current path distance to all of the current vertex's neighbors
        vector<pair<Person, int>> adj = adjList[currentVertex].second;
        for (int i=0; i<adj.size(); i++)
        {
            int v = findPersonIndex(adj[i].first);
            int w = adj[i].second;

            if (weights[currentVertex] + w  < weights[v])
            {
                weights[v] = weights[currentVertex] + w;
                parents[v] = currentVertex;
            }
        }
    }

    int smallestIndex = currentUserIndex;
    for (int i=0; i<weights.size(); i++)
    {
        if (adjList[i].first.infected)
        {
            smallestIndex = i;
            break;
        }
    }

    for (int i=0; i<weights.size(); i++)
    {
        if (adjList[i].first.infected)
        {
            if (weights[i] < weights[smallestIndex])
                smallestIndex = i;
        }
    }

    vector<int> shortestPath;
    queue<int> q;
    q.push(smallestIndex);
    while (!q.empty())
    {
        shortestPath.push_back(q.front());
        if (parents[q.front()] != -1)
            q.push(parents[q.front()]);
        q.pop();
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop-start);
    cout << "Dijkstra's Algorithm: " << duration.count() << " microseconds" << endl;
    //--------------------------------Timer End-------------------------------//

    return shortestPath;
}
vector<int> Tracer::bellmanShortestPathToInfected()
{
    //--------------------------------Timer Start----------------------------//
    srand(time(NULL));
    auto start = high_resolution_clock::now();

    //Add all vectors of graph to waiting set
    set<int> waiting;
    queue<int> needsChecking;
    needsChecking.push(currentUserIndex);
    while (!needsChecking.empty())
    {
        int curr = needsChecking.front();
        needsChecking.pop();

        if (waiting.find(curr) == waiting.end())
        {
            waiting.insert(curr);

            vector<pair<Person, int>> currAdjList = adjList[curr].second;
            for (int i=0; i<currAdjList.size(); i++)
            {
                Person index = currAdjList[i].first;

                needsChecking.push(findPersonIndex(index));
            }
        }
    }

    vector<int> distances(numUsers, INT_MAX);
    vector<int> predecessors(numUsers, -1);

    unordered_map<int, int> help;
    vector<tuple<Person, Person, int>> edgeList;
    int i=0;
    for (auto it=waiting.begin(); it!=waiting.end(); ++it)
    {
        help[*it] = i++;

        for (int j = 0; j < adjList[*it].second.size(); j++)
        {
            tuple<Person, Person, int> edge;
            get<0>(edge) = adjList[*it].first;
            get<1>(edge) = adjList[*it].second[j].first;
            get<2>(edge) = adjList[*it].second[j].second;
            edgeList.push_back(edge);
        }
    }


    for (int i = 0; i < waiting.size() - 1; i++)
    {
        for (int j = 0; j < edgeList.size(); j++)
        {
            if (distances[help[findPersonIndex(get<0>(edgeList[j]))]] < INT_MAX)
            {
                int indexA = findPersonIndex(get<0>(edgeList[j]));
                int indexB = findPersonIndex(get<1>(edgeList[j]));
                if (distances[help[indexB]] > distances[help[indexA]] + get<2>(edgeList[j]))
                {
                    distances[help[indexB]] = distances[help[indexA]] + get<2>(edgeList[j]);
                    predecessors[help[indexB]] = indexA;
                }
            }
        }
    }

    int smallestIndex = currentUserIndex;
    for (int i=0; i<distances.size(); i++)
    {
        if (adjList[i].first.infected)
        {
            smallestIndex = i;
            break;
        }
    }

    for (int i=0; i<distances.size(); i++)
    {
        if (adjList[i].first.infected)
        {
            if (distances[i] < distances[smallestIndex])
                smallestIndex = i;
        }
    }

    vector<int> shortestPath;
    queue<int> q;
    q.push(smallestIndex);
    while (!q.empty())
    {
        shortestPath.push_back(q.front());
        if (predecessors[q.front()] != -1)
            q.push(predecessors[q.front()]);
        q.pop();
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop-start);
    cout << "Bellman Ford's Algorithm: " << duration.count() << " microseconds" << endl;
    //--------------------------------Timer End-------------------------------//

    return shortestPath;

//    for (int i = 0; i < distances.size(); i++) {
//        if (distances[i] != INT_MAX ) {
//            // if vertex corresponding to distance[i] meets conditions and is infected
//            vector<int> pathToInfected; // Pushes indices of predecessors into vector
//            for (int j = i; j != -1; j = predecessors[j]) {
//                pathToInfected.push_back(j);
//            }
//            reverse(pathToInfected.begin(), pathToInfected.end());
//        }
//    }
}


//vector<int> Tracer::bellmanShortestPathToInfected()
//{
//    //Add all vectors of graph to waiting set
//    set<int> waiting;
//    queue<int> needsChecking;
//    needsChecking.push(currentUserIndex);
//    while (!needsChecking.empty())
//    {
//        int curr = needsChecking.front();
//        needsChecking.pop();
//
//        if (waiting.find(curr) == waiting.end())
//        {
//            waiting.insert(curr);
//
//            vector<pair<Person, int>> currAdjList = adjList[curr].second;
//            for (int i=0; i<currAdjList.size(); i++)
//            {
//                Person index = currAdjList[i].first;
//
//                needsChecking.push(findPersonIndex(index));
//            }
//        }
//    }
//
//    unordered_map<int, int> indexToVertex;
//    vector<int> weights(waiting.size(), INT_MAX);
//    vector<int> parents(waiting.size(), -1);
//    int i=0;
//    for (auto it=waiting.begin(); it!=waiting.end(); ++it)
//    {
//        int vertex = *it;
//
//        indexToVertex[i] = vertex;
//    }
//    weights[findPersonIndex(adjList[currentUserIndex].first)] = 0;
//
//    for (auto it=weights.begin(); it!=weights.end(); ++it)
//    {
//        vector<pair<Person, int>> adj = adjList[].second;
//        for (int i=0; i<adj.size(); i++)
//        {
//            int v = findPersonIndex(adj[i].first);
//            int w = adj[i].second;
//
//            if (weights[currentVertex] + w  < weights[v])
//            {
//                weights[v] = weights[currentVertex] + w;
//                parents[v] = currentVertex;
//            }
//        }
//    }
//
//
//
//
//
//    int smallestIndex = currentUserIndex;
//    for (int i=0; i<weights.size(); i++)
//    {
//        if (adjList[i].first.infected)
//        {
//            smallestIndex = i;
//            break;
//        }
//    }
//
//    for (int i=0; i<weights.size(); i++)
//    {
//        if (adjList[i].first.infected)
//        {
//            if (weights[i] < weights[smallestIndex])
//                smallestIndex = i;
//        }
//    }
//
//    vector<int> shortestPath;
//    queue<int> q;
//    q.push(smallestIndex);
//    while (!q.empty())
//    {
//        shortestPath.push_back(q.front());
//        if (parents[q.front()] != -1)
//            q.push(parents[q.front()]);
//        q.pop();
//    }
//
//    return shortestPath;
//}