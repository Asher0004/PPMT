//===============================================================================================
// Name        : authentificate.cpp
// Author      : Julien Thibodeau
// Description : Add/Remove/Show passwords from all your websites securitly in C++, Ansi-style
//===============================================================================================

#include "algorithms.cpp"

int main()
{
    bool executeProgram = true;
    while (executeProgram)
    {
        MainMenu();

        switch (GetInputMainMenu())
        {
        case 1:
            addEntry();
            break;

        case 2:
            RemoveAnEntry();
            break;

        case 3:
            ListEntries();
            break;

        case 4:
            ManagePasswordsMenu();
            switch (GetInputSubMEnu())
            {
            case 1:
                ModifyPasswordFile();
                break;

            case 2:
                ModifyPasswordAccessEntries();
                break;

            case 3:
                // MainMenu
                break;

            case 4:
                executeProgram = false;
                break;
            }
            break;

        case 5:
            executeProgram = false;
            break;

        default:
            break;
        }
    }

    return 0;
}
