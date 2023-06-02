#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <algorithm>
#include "main.h"


/**
 * Funkcja, która zapisuje hasła do pliku w postaci zaszyfrowanej.
 *
 * Wykorzystuje do tego funkcję write, która przekazuje binarne dane.
 *
 */
void PasswordManager::encryptFile() {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        for (const auto &password: passwords) {
            file.write(reinterpret_cast<const char *>(&password), sizeof(password));
        }
        file.close();
    } else {
        std::cout << "Error saving file.\n";
    }
}


/**
 * Funkcja odczytuje hasła z pliku dzięki metodzie read, która odczytuje binarne dane
 *
 *
 */
void PasswordManager::decryptFile() {
    passwords.clear();

    std::ifstream file(filename, std::ios::binary);
    if (file.is_open()) {
        while (!file.eof()) {
            Password password;
            file.read(reinterpret_cast<char *>(&password), sizeof(password));
            if (!file.eof()) {
                passwords.push_back(password);
            }
        }
        file.close();
    } else {
        std::cout << "Error reading file.\n";
    }
}


/**
 * Sprawdzenie czy hasło zostało już użyte.
 *
 * @param password Parametr, w którym sprawdzamy czy był już użyty
 * @return true jeśli parametr został użyty
 * @return false jeśli parametr nie został użyty
 */
bool PasswordManager::isPasswordUsed(const std::string &password) const {
    for (const auto &entry: passwords) {
        if (entry.password == password) {
            return true;
        }
    }
    return false;
}


/**
 * Konstruktor klasy PasswordManager
 *
 * @param file przekazujemy plik do konstruktora, który zostanie odszyfrowany
 */
PasswordManager::PasswordManager(const std::string &file) : filename(file) {
    decryptFile();
}


/**
 * Metoda, która wyszukuje hasło po odpowiednim zapytaniu przekazanym jako argument
 *
 * Używamy do tego funkcji find() dla typu std::string z przekazanym argumentem query
 *
 * Następnie zostają wyświetlone znalezione stringi z danych kategorii
 *
 * @param query podajemy zapytanie, pod którym chcemy żeby nasze hasło zostało znalezione
 */
void PasswordManager::searchPasswords(const std::string &query) const {
    std::cout << "Searched passwords:\n";

    for (const auto &password: passwords) {
        if (password.name.find(query) != std::string::npos ||
            password.password.find(query) != std::string::npos ||
            password.category.find(query) != std::string::npos ||
            password.website.find(query) != std::string::npos ||
            password.login.find(query) != std::string::npos) {

            std::cout << "Name: " << password.name << std::endl;
            std::cout << "Password: " << password.password << std::endl;
            std::cout << "Category: " << password.category << std::endl;
            std::cout << "Website: " << password.website << std::endl;
            std::cout << "Login: " << password.login << std::endl;
            std::cout << std::endl;
        }
    }
}

/**
 * Sortujemy hasła po różnych parametrach.
 *
 * Wykorzystujemy do tego bibliotekę <algorithm> i funkcje sort().
 *
 *
 *
 * @param fields wybieramy po jakich parametrach chcemy sortowac
 * @param result tworzymy wektor z posortowanymi hasłami
 * @return false jeśli nie znajdziemy nic
 */
void PasswordManager::sortPasswords(const std::vector<std::string> &fields) {
    std::vector<Password> result = passwords;

    sort(result.begin(), result.end(), [&fields](const Password &firstPasswd, const Password &secondPasswd) -> bool {
        for (const auto &field: fields) {
            if (field == "name") {
                if (firstPasswd.name != secondPasswd.name) {
                    return firstPasswd.name < secondPasswd.name;
                }
            } else if (field == "password") {
                if (firstPasswd.password != secondPasswd.password) {
                    return firstPasswd.password < secondPasswd.password;
                }
            } else if (field == "category") {
                if (firstPasswd.category != secondPasswd.category) {
                    return firstPasswd.category < secondPasswd.category;
                }
            } else if (field == "website") {
                if (firstPasswd.website != secondPasswd.website) {
                    return firstPasswd.website < secondPasswd.website;
                }
            } else if (field == "login") {
                if (firstPasswd.login != secondPasswd.login) {
                    return firstPasswd.login < secondPasswd.login;
                }
            }
        }
        return false;
    });

    std::cout << "Sorted passwords:\n";

    for (const auto &password: result) {
        std::cout << "Name: " << password.name << std::endl;
        std::cout << "Password: " << password.password << std::endl;
        std::cout << "Category: " << password.category << std::endl;
        std::cout << "Website: " << password.website << std::endl;
        std::cout << "Login: " << password.login << std::endl;
        std::cout << std::endl;
    }
}


/**
 * Funkcja sprawdza, które opcje mają zostać rozpatrzone w tworzeniu losowego hasła.
 *
 * Następnie tworzy i zwraca losowe hasło.
 *
 * @param length dlugosc hasla
 * @param upperCase czy w hasle maja zostac uzyte wielkie litery
 * @param lowerCase czy w hasle maja zostac uzyte male litery
 * @param specialChar czy w hasle maja zostac uzyte znaki specjalne
 * @return zwraca losowe hasło
 */
std::string PasswordManager::randomPassword(int length, bool upperCase, bool lowerCase, bool specialChar) const {
    srand(time(NULL));
    const std::string lowerChars = "abcdefghijklmnopqrstuvwxyz";
    const std::string upperChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const std::string specialChars = "!@#$%^&*()-+=~`;:'?/";

    std::string combinedChars;

    if (upperCase) {
        combinedChars += upperChars;
    }
    if (lowerCase) {
        combinedChars += lowerChars;
    }
    if (specialChar) {
        combinedChars += specialChars;
    }

    std::string result;

    int combinedCharsLength = combinedChars.length();

    for (int i = 0; i < length; i++) {
        int randomIndex = rand() % combinedCharsLength;
        result += combinedChars[randomIndex];
    }

    return result;

}


void PasswordManager::printVector() {
    std::cout << "Available categories: " << std::endl;
    for (const auto &category: categories) {
        std::cout << "- " << category << std::endl;
    }
}

/**
 * Funkcja dodająca hasło z wybranymi przez nas parametrami.
 *
 * Funkcja pozwala na wygenerowanie losowego hasła z wybranymi przez nas parametrami takimi jak:
 * długość hasła, wielkie litery, małe litery oraz znaki specjalne.
 *
 *
 * Po wybraniu wszystkich parametrów tworzy hasło z odpowiednimi polami i dodaje do wektora.
 *
 */
void PasswordManager::addPassword() {
    Password password;
    std::cout << "Adding new password:" << std::endl;

    do {
        std::cout << "Name: ";
        getline(std::cin, password.name);
    } while (password.name.empty());

    std::cout << "Do you want to generate random password? (Y/N): ";
    char generateChoice;
    std::cin >> generateChoice;
    tolower(generateChoice);
    std::cin.ignore();

    if (generateChoice == 'y') {
        int length;
        bool includeUppercase, includeLowercase, includeSpecialChars;

        std::cout << "Password length: ";
        std::cin >> length;
        std::cin.ignore();

        std::cout << "Include uppercase letters? (Y/N): ";
        char upperChoice;
        std::cin >> upperChoice;
        std::cin.ignore();
        tolower(upperChoice);
        includeUppercase = (upperChoice == 'y');

        std::cout << "Include lowercase letters? (Y/N): ";
        char lowerChoice;
        std::cin >> lowerChoice;
        std::cin.ignore();
        tolower(lowerChoice);
        includeLowercase = (lowerChoice == 'y');

        std::cout << "Include special characters? (Y/N): ";
        char specialChoice;
        std::cin >> specialChoice;
        std::cin.ignore();
        tolower(specialChoice);
        includeSpecialChars = (specialChoice == 'y');

        password.password = randomPassword(length, includeUppercase, includeLowercase, includeSpecialChars);
    } else if (generateChoice == 'n') {
        do {
            std::cout << "Password: ";
            getline(std::cin, password.password);
            if (isPasswordUsed(password.password)) {
                std::cout << "Password is already used and can be unsafe.\n";
            }
        } while (password.password.empty());
    } else {
        std::cout << "Error. \n";
    }

    std::cout << "Do you want to add category that has been already created? (Y/n): ";
    char categoryChoice;
    std::cin >> categoryChoice;
    tolower(categoryChoice);
    std::cin.ignore();
    if (categoryChoice == 'y') {
        printVector();
        std::cout << "Category: ";
        getline(std::cin, password.category);
    } else {
        do {
            std::cout << "Category: ";
            getline(std::cin, password.category);
        } while (password.category.empty());
    }

    std::cout << "Website (optional): ";
    getline(std::cin, password.website);

    std::cout << "Login (optional): ";
    getline(std::cin, password.login);

    passwords.push_back(password);
    encryptFile();
}

/**
 * Funkcja, która pozwala użytkownikowi na zmiane wartości w haśle
 *
 * Pobiera od użytkownika nazwę hasła, a następnie użytkownik wybiera co chce zmienić
 *
 *
 * @param name nazwa hasła do edytowania
 */
void PasswordManager::editPassword() {
    std::string name;
    std::cout << "Enter name of the password to edit: ";
    getline(std::cin, name);

    bool found = false;

    for (auto &password: passwords) {
        if (password.name == name) {
            std::cout << "Editing password:" << std::endl;
            std::cout << "Name: " << password.name << std::endl;

            std::cout << "New name (click enter if stays the same): ";
            getline(std::cin, name);
            if (!name.empty()) {
                password.name = name;
            }

            std::cout << "New password (click enter if stays the same): ";
            getline(std::cin, password.password);

            std::cout << "New category (click enter if stays the same): ";
            getline(std::cin, password.category);

            std::cout << "New website (click enter if stays the same): ";
            getline(std::cin, password.website);

            std::cout << "New login (click enter if stays the same): ";
            getline(std::cin, password.login);

            encryptFile();

            std::cout << "Password edited." << std::endl;
            found = true;
            break;
        }
    }

    if (!found) {
        std::cout << "Password not found." << std::endl;
    }
}

/**
 * Funkcja, która pobiera od użytkownika nazwe hasła, a później je usuwa.
 *
 * @param name nazwa hasła do usunięcia
 *
 */
void PasswordManager::removePassword() {
    std::string name;
    std::cout << "Enter the name of the password to delete: ";
    getline(std::cin, name);

    std::cout << "You are about to delete a password. Are you sure? (Y/N): " << std::endl;
    char confirmation;
    std::cin >> confirmation;
    tolower(confirmation);
    std::cin.ignore();

    if (confirmation == 'y') {
        auto it = remove_if(passwords.begin(), passwords.end(), [&name](const Password &password) {
            return password.name == name;
        });

        if (it != passwords.end()) {
            passwords.erase(it, passwords.end());
            encryptFile();
            std::cout << "Password deleted." << std::endl;
        } else {
            std::cout << "Password not found." << std::endl;
        }
    } else {
        std::cout << "Operation canceled." << std::endl;
    }
}


/**
 * Funkcja, która dodaje nową kategorię do wektora kategorii.
 *
 * Pobiera od użytkownika nazwę kategorii
 *
 * @param category nazwa kategorii do dodania
 */
void PasswordManager::addCategory() {
    std::string category;
    std::cout << "Enter name of the category: ";
    getline(std::cin, category);
    categories.push_back(category);

    std::cout << "Category added." << std::endl;
}


/**
 * Funkcja, która pozwala na usunięcie kategorii wraz ze wszystkimi hasłami przypisanymi do tej kategorii
 *
 * Pobiera od użytkownika nazwę kategorii.
 *
 * @param category nazwa kategorii do usunięcia
 */
void PasswordManager::removeCategory() {
    std::string category;
    std::cout << "Enter name of the category to delete: ";
    getline(std::cin, category);

    auto it = remove_if(passwords.begin(), passwords.end(), [&category](const Password &password) {
        return password.category == category;
    });
    passwords.erase(it, passwords.end());
    encryptFile();

    std::cout << "Category deleted." << std::endl;


}

/**
 * Funkcja sprawdzająca czy podany plik z parametru istnieje
 *
 *
 * @param filename wczytujemy nazwę pliku
 * @return true jeśli plik istnieje
 */
bool fileExists(const std::string &filename) {
    std::ifstream file(filename);
    return file.good();
}

int main() {
    std::string filename;
    int choice;
    std::string query;
    std::vector<std::string> sortFields;
    char input;
    std::cout << "Enter name of the file: ";
    getline(std::cin, filename);
    std::cout << std::endl;

    PasswordManager manager(filename);

    if (!fileExists(filename)) {
        std::cout << "File not exist.\n";
    } else {
        while (true) {
            std::cout << "1. Search password" << std::endl;
            std::cout << "2. Sort password" << std::endl;
            std::cout << "3. Add password" << std::endl;
            std::cout << "4. Edit password" << std::endl;
            std::cout << "5. Delete password" << std::endl;
            std::cout << "6. Add category" << std::endl;
            std::cout << "7. Delete category" << std::endl;
            std::cout << "8. Close program" << std::endl;

            std::cout << "Choose an option: ";
            std::cin >> choice;
            std::cin.ignore();

            switch (choice) {
                case 1:
                    std::cout << "Enter query: ";
                    getline(std::cin, query);
                    manager.searchPasswords(query);
                    std::cout << "Type q to continue: ";
                    std::cin >> input;
                    tolower(input);
                    if (input == 'q')
                        break;
                case 2:
                    sortFields.clear();
                    while (true) {
                        std::string field;
                        std::cout << "Enter field/fields to sort (type q to close): ";
                        getline(std::cin, field);
                        if (field == "q" || field == "Q") {
                            break;
                        }
                        sortFields.push_back(field);
                    }
                    manager.sortPasswords(sortFields);
                    std::cout << "Type q to continue: ";
                    std::cin >> input;
                    tolower(input);
                    if (input == 'q')
                        break;
                case 3:
                    manager.addPassword();
                    break;
                case 4:
                    manager.editPassword();
                    break;
                case 5:
                    manager.removePassword();
                    break;
                case 6:
                    manager.addCategory();
                    break;
                case 7:
                    manager.removeCategory();
                    break;
                case 8:
                    return 0;
                default:
                    std::cout << "Invalid option." << std::endl;
                    break;
            }
            std::cout << std::endl;
        }
    }
}
