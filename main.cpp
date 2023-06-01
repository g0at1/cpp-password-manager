#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>
#include <algorithm>
#include "main.h"

using namespace std;

//struct Password {
//    string name;
//    string password;
//    string category;
//    string website;
//    string login;
//    time_t timestamp;
//};


void PasswordManager::encryptFile() {
    ofstream file(filename, ios::binary);
    if (file.is_open()) {
        for (const auto& password : passwords) {
            file.write(reinterpret_cast<const char*>(&password), sizeof(password));
        }
        file.close();
    } else {
        cout << "Błąd podczas zapisu pliku." << endl;
    }
}

void PasswordManager::decryptFile() {
    passwords.clear();

    ifstream file(filename, ios::binary);
    if (file.is_open()) {
        while (!file.eof()) {
            Password password;
            file.read(reinterpret_cast<char*>(&password), sizeof(password));
            if (!file.eof()) {
                passwords.push_back(password);
            }
        }
        file.close();
    } else {
        cout << "Błąd podczas odczytu pliku." << endl;
    }
}

bool PasswordManager::isPasswordUsed(const string& password) const {
    for (const auto& entry : passwords) {
        if (entry.password == password) {
            return true;
        }
    }
    return false;
}

bool PasswordManager::isPasswordSecure(const string& password) const {
    // Sprawdź tutaj złożoność hasła, np. długość, obecność wielkich liter, małych liter, znaków specjalnych itp.
    // Zwróć true, jeśli hasło jest wystarczająco bezpieczne, w przeciwnym razie false.


    return true;
}


PasswordManager::PasswordManager(const string& file) : filename(file) {
    decryptFile();
}

void PasswordManager::searchPasswords(const string& query) const {
    cout << "Wyszukane hasła:" << endl;

    for (const auto& password : passwords) {
        if (password.name.find(query) != string::npos ||
            password.password.find(query) != string::npos ||
            password.category.find(query) != string::npos ||
            password.website.find(query) != string::npos ||
            password.login.find(query) != string::npos) {
            cout << "Nazwa: " << password.name << endl;
            cout << "Hasło: " << password.password << endl;
            cout << "Kategoria: " << password.category << endl;
            cout << "Strona WWW: " << password.website << endl;
            cout << "Login: " << password.login << endl;
            cout << "Timestamp: " << asctime(localtime(&password.timestamp)) << endl;
            cout << endl;
        }
    }
}

void PasswordManager::sortPasswords(const vector<string>& fields) {
    vector<Password> sortedPasswords = passwords;

    sort(sortedPasswords.begin(), sortedPasswords.end(), [&fields](const Password& a, const Password& b) {
        for (const auto& field : fields) {
            if (field == "name") {
                if (a.name != b.name) {
                    return a.name < b.name;
                }
            } else if (field == "password") {
                if (a.password != b.password) {
                    return a.password < b.password;
                }
            } else if (field == "category") {
                if (a.category != b.category) {
                    return a.category < b.category;
                }
            } else if (field == "website") {
                if (a.website != b.website) {
                    return a.website < b.website;
                }
            } else if (field == "login") {
                if (a.login != b.login) {
                    return a.login < b.login;
                }
            }
        }
        return false;
    });

    cout << "Posortowane hasła:" << endl;

    for (const auto& password : sortedPasswords) {
        cout << "Nazwa: " << password.name << endl;
        cout << "Hasło: " << password.password << endl;
        cout << "Kategoria: " << password.category << endl;
        cout << "Strona WWW: " << password.website << endl;
        cout << "Login: " << password.login << endl;
        cout << "Timestamp: " << asctime(localtime(&password.timestamp)) << endl;
        cout << endl;
    }
}

string PasswordManager::randomPassword(int length, bool upperCase, bool lowerCase, bool specialChar) const {
    srand(time(NULL));
    const string lowerChars = "abcdefghijklmnopqrstuvwxyz";
    const string upperChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const string specialChars = "!@#$%^&*()";

    string combinedChars;

    if (upperCase) {
        combinedChars += upperChars;
    }
    if (lowerCase) {
        combinedChars += lowerChars;
    }
    if (specialChar) {
        combinedChars += specialChars;
    }

    string result;

    int combinedCharsLength = combinedChars.length();

    for (int i = 0; i < length; i++) {
        int randomIndex = rand() % combinedCharsLength;
        result += combinedChars[randomIndex];
    }

    return result;

}

void PasswordManager::addPassword() {
    Password password;
    cout << "Dodawanie nowego hasła:" << endl;

    do {
        cout << "Nazwa: ";
        getline(cin, password.name);
    } while (password.name.empty());

//        cout << "Hasło: ";
//        getline(cin, password.password);

    cout << "Automatycznie generować hasło? (T/N): ";
    char generateChoice;
    cin >> generateChoice;

    if (generateChoice == 'T' || generateChoice == 't') {
        int length;
        bool includeUppercase, includeLowercase, includeSpecialChars;

        cout << "Ilość znaków: ";
        cin >> length;

        cout << "Czy hasło ma zawierać wielkie litery? (T/N): ";
        char uppercaseChoice;
        cin >> uppercaseChoice;
        includeUppercase = (uppercaseChoice == 'T' || uppercaseChoice == 't');

        cout << "Czy hasło ma zawierać małe litery? (T/N): ";
        char lowercaseChoice;
        cin >> lowercaseChoice;
        includeLowercase = (lowercaseChoice == 'T' || lowercaseChoice == 't');

        cout << "Czy hasło ma zawierać znaki specjalne? (T/N): ";
        char specialCharsChoice;
        cin >> specialCharsChoice;
        includeSpecialChars = (specialCharsChoice == 'T' || specialCharsChoice == 't');

        cin.ignore();  // Clear input buffer

        password.password = randomPassword(length, includeUppercase, includeLowercase, includeSpecialChars);
    } else if (generateChoice == 'N' || generateChoice == 'n') {
        do {
            cout << "Hasło: ";
//            cin.ignore();  // Clear input buffer
            getline(cin, password.password);
            if (isPasswordUsed(password.password)) {
                cout << "Haslo jest juz uzyte i moze nie byc bezpieczne.\n";
            };
        } while (password.password.empty());
    } else {
        cout << "Blad\n";
    }

//    while (isPasswordUsed(password.password)) {
//        cout << "To hasło zostało już użyte. Podaj inne hasło: ";
//        getline(cin, password.password);
//    }

    do {
        cout << "Kategoria: ";
        getline(cin, password.category);
    } while (password.category.empty());

    cout << "Strona WWW (opcjonalnie): ";
    getline(cin, password.website);

    cout << "Login (opcjonalnie): ";
    getline(cin, password.login);

    time(&password.timestamp);

    passwords.push_back(password);
    encryptFile();
}

void PasswordManager::editPassword() {
    string name;
    cout << "Podaj nazwę hasła do edycji: ";
    getline(cin, name);

    bool found = false;

    for (auto& password : passwords) {
        if (password.name == name) {
            cout << "Edytujesz hasło:" << endl;
            cout << "Nazwa: " << password.name << endl;
            cout << "Hasło: " << password.password << endl;
            cout << "Kategoria: " << password.category << endl;
            cout << "Strona WWW: " << password.website << endl;
            cout << "Login: " << password.login << endl;

            cout << "Nowa nazwa (enter, jeśli bez zmian): ";
            getline(cin, name);
            if (!name.empty()) {
                password.name = name;
            }

            cout << "Nowe hasło (enter, jeśli bez zmian): ";
            getline(cin, password.password);

            cout << "Nowa kategoria (enter, jeśli bez zmian): ";
            getline(cin, password.category);

            cout << "Nowa strona WWW (enter, jeśli bez zmian): ";
            getline(cin, password.website);

            cout << "Nowy login (enter, jeśli bez zmian): ";
            getline(cin, password.login);

            encryptFile();
            cout << "Hasło zostało zaktualizowane." << endl;
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Nie znaleziono hasła o podanej nazwie." << endl;
    }
}

void PasswordManager::removePassword() {
    string name;
    cout << "Podaj nazwę hasła do usunięcia: ";
    getline(cin, name);

    auto it = remove_if(passwords.begin(), passwords.end(), [&name](const Password& password) {
        return password.name == name;
    });

    if (it != passwords.end()) {
        passwords.erase(it, passwords.end());
        encryptFile();
        cout << "Hasło zostało usunięte." << endl;
    } else {
        cout << "Nie znaleziono hasła o podanej nazwie." << endl;
    }
}

void PasswordManager::addCategory() {
    string category;
    cout << "Podaj nazwę nowej kategorii: ";
    getline(cin, category);
    categories.push_back(category);

    // Dodaj nową kategorię

    cout << "Kategoria została dodana." << endl;
}

void PasswordManager::removeCategory() {
//    string category;
//    cout << "Podaj nazwę kategorii do usunięcia: ";
//    getline(cin, category);
//
//    // Usuń kategorię wraz ze wszystkimi hasłami przypisanymi do niej
//    for (auto it = passwords.begin(); it != passwords.end(); ) {
//        if (it->category == category) {
//            it = passwords.erase(it);
//        } else {
//            ++it;
//        }
//    }
    string category;
    cout << "Podaj nazwę kategorii do usunięcia: ";
    getline(cin, category);

    // Usuń hasła przypisane do kategorii
    auto it = remove_if(passwords.begin(), passwords.end(), [&category](const Password& password) {
        return password.category == category;
    });
    passwords.erase(it, passwords.end());
    encryptFile();

    cout << "Kategoria została usunięta." << endl;
}

bool fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good(); // Returns true if the file exists
}


int main() {
    string filename;
    int choice;
    string query;
    vector<string> sortFields;
    char input;
    cout << "Podaj nazwę pliku: ";
    getline(cin, filename);

    PasswordManager manager(filename);

    if (!fileExists(filename)) {
        cout << "File not exist";
    } else {
        while (true) {
            cout << "Menu:" << endl;
            cout << "1. Wyszukaj hasła" << endl;
            cout << "2. Posortuj hasła" << endl;
            cout << "3. Dodaj hasło" << endl;
            cout << "4. Edytuj hasło" << endl;
            cout << "5. Usuń hasło" << endl;
            cout << "6. Dodaj kategorię" << endl;
            cout << "7. Usuń kategorię" << endl;
            cout << "8. Zakończ program" << endl;

            cout << "Wybierz opcję: ";
            cin >> choice;
            cin.ignore(); // Ignoruj znak nowej linii po wprowadzeniu wyboru

            switch (choice) {
                case 1:
                    cout << "Podaj zapytanie: ";
                    getline(cin, query);
                    manager.searchPasswords(query);
                    cin >> input;
                    if (input == 'q' || input == 'Q')
                        break;
                case 2:
                    sortFields.clear();
                    while (true) {
                        string field;
                        cout << "Podaj pole do sortowania (lub wpisz 'q' aby zakończyć): ";
                        getline(cin, field);
                        if (field == "q") {
                            break;
                        }
                        sortFields.push_back(field);
                    }

                    manager.sortPasswords(sortFields);
                    cin >> input;
                    if (input == 'q' || input == 'Q')
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
                    cout << "Nieprawidłowa opcja. Spróbuj ponownie." << endl;
                    break;
            }

            cout << endl;
        }
    }



//    while (true) {
//        cout << "Menu:" << endl;
//        cout << "1. Wyszukaj hasła" << endl;
//        cout << "2. Posortuj hasła" << endl;
//        cout << "3. Dodaj hasło" << endl;
//        cout << "4. Edytuj hasło" << endl;
//        cout << "5. Usuń hasło" << endl;
//        cout << "6. Dodaj kategorię" << endl;
//        cout << "7. Usuń kategorię" << endl;
//        cout << "8. Zakończ program" << endl;
//
//        cout << "Wybierz opcję: ";
//        cin >> choice;
//        cin.ignore(); // Ignoruj znak nowej linii po wprowadzeniu wyboru
//
//        switch (choice) {
//            case 1:
//                cout << "Podaj zapytanie: ";
//                getline(cin, query);
//                manager.searchPasswords(query);
//                cin >> input;
//                if (input == 'q' || input == 'Q')
//                    break;
//            case 2:
//                sortFields.clear();
//                while (true) {
//                    string field;
//                    cout << "Podaj pole do sortowania (lub wpisz 'q' aby zakończyć): ";
//                    getline(cin, field);
//                    if (field == "q") {
//                        break;
//                    }
//                    sortFields.push_back(field);
//                }
//
//                manager.sortPasswords(sortFields);
//                cin >> input;
//                if (input == 'q' || input == 'Q')
//                    break;
//            case 3:
//                manager.addPassword();
//                break;
//            case 4:
//                manager.editPassword();
//                break;
//            case 5:
//                manager.removePassword();
//                break;
//            case 6:
//                manager.addCategory();
//                break;
//            case 7:
//                manager.removeCategory();
//                break;
//            case 8:
//                return 0;
//            default:
//                cout << "Nieprawidłowa opcja. Spróbuj ponownie." << endl;
//                break;
//        }
//
//        cout << endl;
//    }
}
