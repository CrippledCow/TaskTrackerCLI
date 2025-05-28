#include <iostream>
#include <time.h>
#include <vector>
#include <unordered_map>
#include "json.hpp"
#include "fstream"
#include <string.h>
#include <print>
#include <string>

using json = nlohmann::json;
using namespace std;

class Task{

    private:
        int id;
        string description;
        string status;
        string createdAt;
        string updatedAt;
        // Private constructor to load tasks.json
        Task(int id, std::string description, std::string status, std::string createdAt, std::string updatedAt) {
        this->id = id;
        this->description = description;
        this->status = status;
        this->createdAt = createdAt;
        this->updatedAt = updatedAt;
        // NOTE: We do NOT increment objectCount here!
    }
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
        friend class TaskManager;
};

class TaskManager{
    private:
        unordered_map <int, Task> taskContainer;
        string json_filename = "tasks.json";
    public:
        void loadFromFile() 
        {
            ifstream file(json_filename);
            // Check if the file exists and is not empty
            if (!file.is_open() || file.peek() == std::ifstream::traits_type::eof()) 
            {
            std::cout << "-> No existing tasks file found, starting fresh." << std::endl;
            return; // Exit the function, nothing to load
            }
            json tasksJson = json::parse(file);
            file.close();

            int maxId = 0;

            for(const auto& taskObj : tasksJson)
            {
                int id = taskObj.at("id").get<int>();
                string description = taskObj.at("description").get<string>();
                string status = taskObj.at("status").get<string>();
                string createdAt = taskObj.at("createdAt").get<string>();
                string updatedAt = taskObj.at("updatedAt").get<string>();

                Task loadedTask(id, description, status, createdAt, updatedAt);

                taskContainer.emplace(id, loadedTask);

                if(id > maxId)
                {
                    maxId = id;
                }
            }
            Task::objectCount = maxId;

            std::cout << "-> Successfully loaded " << taskContainer.size() << " tasks from " << json_filename << std::endl;

        }

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
        void processPositionalArgument(string posArg)
        {
            string numberString;
            int j = 0;
            bool numberExists = false;
            int foundId;

            for(int i = 0; i < posArg.length(); i++)
            {
                if(posArg[i] >= '0' && posArg[i] <= '9')
                {
                    numberExists = true;
                    numberString.push_back(posArg[i]);
                    j++;
                }
            }

            if(numberExists)
            {
                foundId = stoi(numberString);
            }

            if(posArg.find("task-cli") != std::string::npos)
            {
                if(posArg.find("add") != std::string::npos)
                {
                    int quotationIndex1 = posArg.find('\"');
                    int quotationIndex2 = posArg.find('\"', quotationIndex1 + 1);
                    if (quotationIndex1 != std::string::npos && quotationIndex2 != std::string::npos && quotationIndex2 > quotationIndex1) {
                        string subStringAdded = posArg.substr(quotationIndex1 + 1, quotationIndex2 - quotationIndex1 - 1);
                        addTask(subStringAdded);
                    } else {
                        cout << "Error: Could not find quoted description." << endl;
                    }
                }
                if(posArg.find("mark-in-progress") != std::string::npos)
                {
                    markInProgress(foundId);
                }
                if(posArg.find("mark-done") != std::string::npos)
                {
                    markDone(foundId);
                }
                if(posArg.find("list todo") != std::string::npos)
                {
                    listTodo();
                    return;
                }
                if(posArg.find("list in-progress") != std::string::npos)
                {
                    listInProgress();
                    return;
                }
                if(posArg.find("list done") != std::string::npos)
                {
                    listDone();
                    return;
                }
                if(posArg.find("list") != std::string::npos)
                {
                    list();
                }
            }
        }
};

int Task::objectCount = 0;

int main()
{
    TaskManager TaskManager1;
    string userArgument;
    TaskManager1.loadFromFile();

    while(1)
    {
        cout << "Enter your command" << endl;
        getline(cin, userArgument);
        TaskManager1.processPositionalArgument(userArgument);
    }
}