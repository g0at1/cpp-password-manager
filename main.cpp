#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include "ui.h"

void UI::showPanel() {
    std::cout << "1. Search for password." << std::endl;
    std::cout << "2. Sort password." << std::endl;
    std::cout << "3. Add password." << std::endl;
    std::cout << "4. Edit password." << std::endl;
    std::cout << "5. Delete password." << std::endl;
    std::cout << "6. Add category." << std::endl;
    std::cout << "7. Delete category." << std::endl;
    std::cout << "8. Show files in folder." << std::endl;
    std::cout << "9. Choose your path." << std::endl;
}



int main() {


    int option = 0;
    while (option != 10) {
        UI::showPanel();
        std::cin >> option;
        std::string dirPath = "/Users/michallendzion/Desktop/Workspace/cpp-password-manager";
        std::string input;
        auto dirIter = std::filesystem::recursive_directory_iterator(dirPath);
        switch (option) {
            case 1:
                std::cout << "Searching for passwords..." << std::endl;
                break;
            case 2:
                std::cout << "Sorting passwords..." << std::endl;
                break;
            case 3:
                std::cout << "Adding passwords..." << std::endl;
                break;
            case 4:
                std::cout << "Editing passwords..." << std::endl;
                break;
            case 5:
                std::cout << "Deleting passwords..." << std::endl;
                break;
            case 6:
                std::cout << "Adding categories..." << std::endl;
                break;
            case 7:
                std::cout << "Editing categories..." << std::endl;
                break;
            case 8:
                for (const auto &entry : dirIter) {
                    if (entry.is_regular_file()) {
                        std::cout << entry.path().filename() << std::endl;
                    }
                }
                break;
            case 9:
                std::cin >> dirPath;
                break;
        }
    }



    return 0;
}
