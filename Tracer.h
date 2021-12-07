#include <vector>
#include <set>
#include <unordered_map>
using namespace std;

class Tracer
{
    struct Person
    {
        string firstName;
        string lastName;
        string nickName;
        bool infected;

        Person()
        {
            firstName = "no first name";
            lastName = "no last name";
            infected = false;
        }
        Person(string newFirstName, string newLastName, string newNickName, bool isInfected)
        {
            firstName = newFirstName;
            lastName = newLastName;
            nickName = newNickName;
            infected = isInfected;
        }
    };

    //Number of vertices (people)
    int numUsers = 0;
    int currentUserIndex = -1;

    unordered_map<string, int> nickNameAdjListIndex;//<nickname, adjlist index>
    set<string> takenNicknames; // set of all taken nicknames

    //pair< User, vector< pair<Person, days last met> >
    vector< pair<Person, vector<pair<Person, int>>> > adjList;

public:
    //Graph Functions
    bool isEmpty();
    void selectUser(string firstN, string lastN, string nickN, bool isInfected);
    void addUser(string firstN, string lastN, string nickN, bool isInfected);
    void addConnection(string firstN, string lastN, string nickN, bool infected, int daysSinceMet);
    int findPersonIndex(Person target);

    //Print Functions
    void printRisk(Person src);
    void printShortestPathToInfected();
    void printUserDetails();
    void printAllUserContacts();

private:
    //Shortest Path Getters
    vector<Person> djikstraShortestPathToInfected(Person user);
    vector<Person> bellmanShortestPathToInfected(Person user);
};