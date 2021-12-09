#include <vector>
#include <set>
#include <unordered_map>
using namespace std;

class Tracer
{
    struct Person
    {
        string name;
        string id;
        bool infected;

        Person()
        {
            name = "no name";
            infected = false;
        }
        Person(string newName, string newID, bool isInfected)
        {
            name = newName;
            id = newID;
            infected = isInfected;
        }
    };

    bool testing = false;

    //Number of vertices (people)
    int numUsers = 0;
    int currentUserIndex = -1;

    unordered_map<string, int> idAdjListIndex;//<id, adjlist index>
    set<string> takenIDs; // set of all taken nicknames

    //pair< User, vector< pair<Person, days last met> >
    vector< pair<Person, vector<pair<Person, int>>> > adjList;

public:
    //Graph Functions
    bool isEmpty();
    void selectUser(string name, string id, bool isInfected);
    void addUser(string name, string id, bool isInfected);
    void addNewConnection(string name, string id, bool infected, int daysSinceMet);
    void addExistingConnection(string id, int daysLastMet);

    //Getters
    int findPersonIndex(Person target);
    bool idExists(string id);
    bool isConnected(string id);
    int getNumUsers();

    //Print Functions
    void printShortestPathToInfected();
    void printUserDetails();
    void printAllUserContacts();

    //Test
    void randomGraph();

private:
    //Shortest Path Getters
    vector<int> djikstraShortestPathToInfected();
    vector<int> bellmanShortestPathToInfected();
};