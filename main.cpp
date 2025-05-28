#include <iostream>
#include <time.h>
#include <vector>
#include <unordered_map>
#include "json.hpp"
#include "fstream"

using json = nlohmann::json;
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
        void printDetails() const
        {
            cout << "\nID: " << id << "\nDescription: " << description << "\nStatus: " << status <<  "\ncreatedAt: " << createdAt << "\nupdatedAt: " << updatedAt << endl;
        }
        void markInProgress()
        {
            this->status = "in-progress";
        }
        void markDone()
        {
            this->status = "done";
        }
        string getStatus() const 
        {
            return status;
        }
        int getId() const 
        {
            return id;
        }
        string getDescription() const 
        {
            return description;
        }
        string getCreatedAt() const
        {
            return createdAt;
        }
        string getUpdatedAt() const
        {
            return updatedAt;
        }
        void updateTime()
        {
            updatedAt = __DATE__ " " __TIME__;
        }
};

class TaskManager{
    private:
        unordered_map <int, Task> taskContainer;
        string json_filename = "tasks.json";
    public:
        void saveToFile() const
        {
            json tasksJson = json::array(); // Create a JSON array

            // Convert task object in the unordered_map to a JSON object
            for(const auto& pair : taskContainer)
            {
                const Task& task = pair.second;
                json taskObj;
                taskObj["id"] = task.getId();
                taskObj["description"] = task.getDescription();
                taskObj["status"] = task.getStatus();
                taskObj["createdAt"] = task.getCreatedAt();
                taskObj["updatedAt"] = task.getUpdatedAt();
                tasksJson.push_back(taskObj);
            }
            // Write the JSON array to the file
            ofstream file(json_filename);
            file << tasksJson.dump(4);
            file.close();
            cout << "-> State saved to " << json_filename << endl;
        }

        void addTask(string description)
        {
            Task addedTask(description);
            taskContainer.emplace(addedTask.getId(), addedTask);
            cout << "\nTask added successfully (ID: " << addedTask.getId() << ")" << endl;
            saveToFile();
        }
        void printTaskDetails(int id) const
        {
            taskContainer.at(id).printDetails();
        }
        void markInProgress(int id)
        {
            taskContainer.at(id).markInProgress();
            saveToFile();
        }
        void markDone(int id)
        {
            taskContainer.at(id).markDone();
            saveToFile();
        }
        void deleteTask(int id)
        {
            taskContainer.erase(id);
            saveToFile();
        }
        void updateTaskTime(int id)
        {
            taskContainer.at(id).updateTime();
            saveToFile();
        }
        void listTodo() const
        {
            for(auto &pair : taskContainer)
            {
                if(pair.second.getStatus() == "todo")
                {
                    cout << "ID: " << pair.first << " - " << pair.second.getDescription() << endl;
                }
            }
        }
        void listInProgress() 
        {
            for(auto &pair : taskContainer)
            {
                if(pair.second.getStatus() == "in-progress")
                {
                    cout << "ID: " << pair.first << " - " << pair.second.getDescription() << endl;
                    updateTaskTime(pair.first);
                }
            }
        }
        void listDone() 
        {
            for(auto &pair : taskContainer)
            {
                if(pair.second.getStatus() == "done")
                {
                    cout << "ID: " << pair.first << " - " << pair.second.getDescription() << endl;
                    updateTaskTime(pair.first);
                }
            }
        }
        void list() const
        {
            for(auto &pair : taskContainer)
            {
                cout << "ID: " << pair.first << " - " << pair.second.getDescription() << endl;
            }
        }

};

int Task::objectCount = 0;

int main()
{
    TaskManager TaskManager1;
    string userArgument;
    TaskManager1.addTask("This is a basic description");
    TaskManager1.addTask("This is another basic description");
    TaskManager1.printTaskDetails(1);
    TaskManager1.printTaskDetails(2);
    TaskManager1.listTodo();
    TaskManager1.markInProgress(1);
    TaskManager1.listInProgress();
    TaskManager1.addTask("This is a funny description");
    TaskManager1.markInProgress(2);
    TaskManager1.list();
    TaskManager1.saveToFile();
    TaskManager1.addTask("Hi, how are you?");
    TaskManager1.saveToFile();
}