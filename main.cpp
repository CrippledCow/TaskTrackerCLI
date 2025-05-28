#include <iostream>
#include <time.h>
#include <vector>
#include <unordered_map>
using namespace std;

class Task{

    private:
        int id;
        string description;
        string status;
        string createdAt;
        string updatedAt;
    public:
        static int objectCount;
        Task(string description)
        {
            objectCount++;
            this->id = objectCount;
            this->description = description;
            this->status = "todo";
            createdAt = __DATE__ " " __TIME__;
            updatedAt = __DATE__ " " __TIME__;
        }
        void printDetails()
        {
            cout << "\nID: " << id << "\nDescription: " << description << "\nStatus: " << status <<  "\ncreatedAt: " << createdAt << "\nupdatedAt: " << updatedAt;
        }
        void markInProgress()
        {
            this->status = "in-progress";
        }
        void markDone()
        {
            this->status = "done";
        }
        int getId()
        {
            return id;
        }
};

class TaskManager{
    private:
        unordered_map <int, Task> taskContainer;
    public:
        void addTask(string description)
        {
            Task addedTask(description);
            taskContainer.emplace(addedTask.getId(), addedTask);
        }
        void printTaskDetails(int id)
        {
            taskContainer.at(id).printDetails();
        }
};

int Task::objectCount = 0;

int main()
{
    TaskManager TaskManager1;
    TaskManager1.addTask("This is a basic description");
    TaskManager1.addTask("This is another basic description");
    TaskManager1.printTaskDetails(1);
    TaskManager1.printTaskDetails(2);
}