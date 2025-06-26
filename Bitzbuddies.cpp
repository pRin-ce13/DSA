#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <vector>
#include <algorithm>
using namespace std;

const int N_MONTHS = 6; // Files older than 6 months will be considered for cleaning
const int M_ACCESS_COUNT = 3; // Files accessed less than 3 times will be considered for cleaning

void checkIfFileIsOld(const string& filePath) {
    struct stat result;
    if (stat(filePath.c_str(), &result) == 0) {
        time_t currentTime = time(nullptr);
        double secondsSinceLastAccess = difftime(currentTime, result.st_atime);
        double monthsSinceLastAccess = secondsSinceLastAccess / (60 * 60 * 24 * 30); // Approximation of months
        if (monthsSinceLastAccess > N_MONTHS) {
            cout << "File is old: " << filePath << endl;
        }
    }
}
void checkIfFileIsEmpty(const string& filePath) {
    ifstream file(filePath);
    if (file.peek() == ifstream::traits_type::eof()) {
        cout << "File is empty: " << filePath << endl;
        // Perform cleaning if necessary
        remove(filePath.c_str()); // Delete the file
    }
}

void checkFileAccessCount(const string& filePath) {
    // Implement a function to get the access count of a file using platform-specific methods
    // For simplicity, we'll print a message
    cout << "Checking access count for: " << filePath << endl;
}

struct FileData {
    string name;
    off_t size;
};

bool compareFileSize(const FileData& a, const FileData& b) {
    return a.size < b.size;
}

void sortFilesBySize(const string& directory) {
    DIR *dir;
    struct dirent *entry;

    vector<FileData> files;

    if ((dir = opendir(directory.c_str())) != nullptr) {
        while ((entry = readdir(dir)) != nullptr) {
            string full_path = directory + "/" + entry->d_name;
            struct stat file_stat;
            if (stat(full_path.c_str(), &file_stat) == 0) {
                if (S_ISREG(file_stat.st_mode)) {
                    files.push_back({entry->d_name, file_stat.st_size});
                }
            }
        }
        closedir(dir);
    } else {
        cerr << "Failed to open directory" << endl;
        return;
    }

    sort(files.begin(), files.end(), compareFileSize);

    for (const auto& file : files) {
        cout << file.size << " bytes\t" << file.name << endl;
    }
}

void displayFileDates(const string& directory) {
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;

    if ((dir = opendir(directory.c_str())) != nullptr) {
        while ((entry = readdir(dir)) != nullptr) {
            string full_path = directory + "/" + entry->d_name;
            if (stat(full_path.c_str(), &file_stat) == 0) {
                // Check if it's a regular file
                if (S_ISREG(file_stat.st_mode)) {
                    // Get last modification time
                    time_t lastModified = file_stat.st_mtime;
                    cout << "File: " << full_path << ", Last Modified: " << ctime(&lastModified);
                }
            }
        }
        closedir(dir);
    } else {
        cerr << "Failed to open directory" << endl;
    }
}

void deleteEmptyFiles(const string& directory) {
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;

    if ((dir = opendir(directory.c_str())) != nullptr) {
        while ((entry = readdir(dir)) != nullptr) {
            string full_path = directory + "/" + entry->d_name;
            if (stat(full_path.c_str(), &file_stat) == 0) {
                // Check if it's a regular file and its size is zero
                if (S_ISREG(file_stat.st_mode) && file_stat.st_size == 0) {
                    // Delete the empty file
                    unlink(full_path.c_str());
                    cout << "Deleted empty file: " << full_path << endl;
                }
            }
        }
        closedir(dir);
    } else {
        cerr << "Failed to open directory" << endl;
    }
}

void deleteOldFiles(const string& directory) {
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;

    if ((dir = opendir(directory.c_str())) != nullptr) {
        while ((entry = readdir(dir)) != nullptr) {
            string full_path = directory + "/" + entry->d_name;
            if (stat(full_path.c_str(), &file_stat) == 0) {
                // Check if it's a regular file and its last access time is older than N_MONTHS
                if (S_ISREG(file_stat.st_mode)) {
                    time_t currentTime = time(nullptr);
                    double secondsSinceLastAccess = difftime(currentTime, file_stat.st_atime);
                    double monthsSinceLastAccess = secondsSinceLastAccess / (60 * 60 * 24 * 30); // Approximation of months
                    if (monthsSinceLastAccess > N_MONTHS) {
                        // Delete the old file
                        unlink(full_path.c_str());
                        cout << "Deleted old file: " << full_path << endl;
                    }
                }
            }
        }
        closedir(dir);
    } else {
        cerr << "Failed to open directory" << endl;
    }
}

bool fileExists(const string& filePath) {
    struct stat buffer;
    return (stat(filePath.c_str(), &buffer) == 0);
}

void cleanFolder(const string& folderPath) {
    // Delete empty files
    deleteEmptyFiles(folderPath);

    // Delete old files
    deleteOldFiles(folderPath);
    
    // Display file dates
    displayFileDates(folderPath);
    
    //DIsplay order of files in ascending order by size  
    sortFilesBySize(folderPath);
    
}

int main() {
    string folderPath = "c:/Users/VICTUS/OneDrive/Desktop/study/MAYBE DSA"; // Change this to the folder you want to clean
    cleanFolder(folderPath);
    return 0;
}
