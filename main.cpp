#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <vector>

//void UI::showPanel() {
//    std::cout << "1. Search for password." << std::endl;
//    std::cout << "2. Sort password." << std::endl;
//    std::cout << "3. Add password." << std::endl;
//    std::cout << "4. Edit password." << std::endl;
//    std::cout << "5. Delete password." << std::endl;
//    std::cout << "6. Add category." << std::endl;
//    std::cout << "7. Delete category." << std::endl;
//    std::cout << "8. Show files in folder." << std::endl;
//    std::cout << "9. Choose your path." << std::endl;
//}

struct Password {
    std::string name;
    std::string password;
    std::string category;
    std::string website;
    std::string login;
};

void logOperation(std::string operation) {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    std::string timestamp = std::to_string(ltm->tm_year + 1900) + "-" + std::to_string(ltm->tm_mon + 1) + "-" + std::to_string(ltm->tm_mday) + " " + std::to_string(ltm->tm_hour) + ":" + std::to_string(ltm->tm_min) + ":" + std::to_string(ltm->tm_sec);

    std::ofstream logFile("log.txt", std::ios::app);
    if (!logFile) {
        std::cerr << "Could not open log file" << std::endl;
        return;
    }

    logFile << timestamp << " - " << operation << std::endl;
}

void savePassword(const Password& password) {
    std::ofstream passwordsFile("passwords.txt", std::ios::app);
    if (!passwordsFile) {
        std::cerr << "Could not open passwords file" << std::endl;
        return;
    }

    passwordsFile << password.name << std::endl;
    passwordsFile << password.password << std::endl;
    passwordsFile << password.category << std::endl;
    passwordsFile << password.website << std::endl;
    passwordsFile << password.login << std::endl;

    passwordsFile.close();

    std::string operation = "Saved password: " + password.name + " (" + password.category + ")";
    logOperation(operation);

    std::cout << "Password saved" << std::endl;
}

void encryptFile(std::string filePath) {
    std::ifstream inFile(filePath, std::ios::binary);
    if (!inFile) {
        std::cerr << "Could not open file: " << filePath << std::endl;
        return;
    }

    std::string encryptedFilePath = filePath + ".enc";
    std::ofstream outFile(encryptedFilePath, std::ios::binary);
    if (!outFile) {
        std::cerr << "Could not create encrypted file: " << encryptedFilePath << std::endl;
        return;
    }

    char key = 'K';
    char c;
    while (inFile.get(c)) {
        c = c ^ key;
        outFile.put(c);
    }

    std::cout << "File encrypted and saved as: " << encryptedFilePath << std::endl;
}

void readPasswords() {
    std::ifstream passwordsFile("passwords.txt");
    if (!passwordsFile) {
        std::cerr << "Could not open passwords file" << std::endl;
        return;
    }

    std::vector<Password> passwords;

    while (!passwordsFile.eof()) {
        Password password;

        getline(passwordsFile, password.name);
        getline(passwordsFile, password.password);
        getline(passwordsFile, password.category);
        getline(passwordsFile, password.website);
        getline(passwordsFile, password.login);

        if (!password.name.empty()) {
            passwords.push_back(password);
        }
    }

    passwordsFile.close();

    if (passwords.empty()) {
        std::cout << "No passwords saved yet" << std::endl;
    } else {
        std::cout << "Saved passwords:" << std::endl;

        for (const auto& password : passwords) {
            std::cout << "Name: " << password.name << std::endl;
            std::cout << "Category: " << password.category << std::endl;
            std::cout << "Password: " << password.password << std::endl;
            if (!password.website.empty()) {
                std::cout << "Website: " << password.website << std::endl;
            }
            if (!password.login.empty()) {
                std::cout << "Login: " << password.login << std::endl;
            }
            std::cout << std::endl;
        }
    }
}





int main() {


    std::cout << "Choose an option:" << std::endl;
    std::cout << "1. Select source file" << std::endl;
    std::cout << "2. Enter absolute path to file" << std::endl;

    int choice;
    std::cin >> choice;

    std::string filePath;
    switch (choice) {
        case 1:
            std::cout << "Select a file from the program folder:" << std::endl;
            std::cin >> filePath;
            break;
        case 2:
            std::cout << "Enter the absolute path to the file:" << std::endl;
            std::cin >> filePath;
            break;
        default:
            std::cerr << "Invalid choice" << std::endl;
            return 1;
    }

    encryptFile(filePath);

    std::cout << "Enter a name for the password entry:" << std::endl;
    std::string name;
    std::cin >> name;

    std::cout << "Enter the password:" << std::endl;
    std::string password;
    std::cin >> password;

    std::cout << "Enter the category:" << std::endl;
    std::string category;
    std::cin >> category;

    std::cout << "Enter the website (optional):" << std::endl;
    std::string website;
    std::cin >> website;

    std::cout << "Enter the login (optional):" << std::endl;
    std::string login;
    std::cin >> login;

    Password newPassword = {name, password, category, website, login};
    savePassword(newPassword);

    std::cout << "Saved password entry:" << std::endl;
    std::cout << "Name: " << newPassword.name << std::endl;
    std::cout << "Category: " << newPassword.category << std::endl;
    std::cout << "Password: " << newPassword.password << std::endl;
    if (!newPassword.website.empty()) {
        std::cout << "Website: " << newPassword.website << std::endl;
    }
    if (!newPassword.login.empty()) {
        std::cout << "Login: " << newPassword.login << std::endl;
    }

    readPasswords();
//    int option = 0;
//    while (option != 10) {
//        UI::showPanel();
//        std::cin >> option;
//        std::string dirPath = "/Users/michallendzion/Desktop/Workspace/cpp-password-manager";
//        std::string input;
//        auto dirIter = std::filesystem::recursive_directory_iterator(dirPath);
//        switch (option) {
//            case 1:
//                std::cout << "Searching for passwords..." << std::endl;
//                break;
//            case 2:
//                std::cout << "Sorting passwords..." << std::endl;
//                break;
//            case 3:
//                std::cout << "Adding passwords..." << std::endl;
//                break;
//            case 4:
//                std::cout << "Editing passwords..." << std::endl;
//                break;
//            case 5:
//                std::cout << "Deleting passwords..." << std::endl;
//                break;
//            case 6:
//                std::cout << "Adding categories..." << std::endl;
//                break;
//            case 7:
//                std::cout << "Editing categories..." << std::endl;
//                break;
//            case 8:
//                for (const auto &entry : dirIter) {
//                    if (entry.is_regular_file()) {
//                        std::cout << entry.path().filename() << std::endl;
//                    }
//                }
//                break;
//            case 9:
//                std::cin >> dirPath;
//                break;
//        }
//    }



    return 0;
}
