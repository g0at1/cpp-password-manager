#ifndef CPP_PASSWORD_MANAGER_UI_H
#define CPP_PASSWORD_MANAGER_UI_H

#include <iostream>

class UI {
public:
    UI();

    static void showPanel();
    void searchPassword();
    void sortPassword();
    void addPassword();
    void editPassword();
    void deletePassword();
    void addCategory();
    void deleteCategory();

};




#endif //CPP_PASSWORD_MANAGER_UI_H
