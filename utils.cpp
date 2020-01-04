//==================================================================================
// Name        : utils.cpp
// Author      : Julien Thibodeau
// Description : Utilitary for authentificate.cpp to keep it clean, Ansi-style
//==================================================================================

#include <string>
#include <experimental/filesystem>
#include <iostream>

std::string intToString(int p_entier)
{
    std::ostringstream oss;
    oss << p_entier;
    return oss.str();
}

bool validateData()
{
    char type;
    do
    {
        std::cout << "Change data ? [y/n] ";
        std::cin >> type;
    } while (type != 'y' && type != 'n');
    if (type == 'y')
    {
        std::cin.ignore();
        return true;
    }
    else
    {
        return false;
    }
}

int GetInputMainMenu()
{
    bool loop = true;
    std::string text;
    int choice;
    do
    {

        try
        {
            std::cin >> text;
            choice = std::stoi(text);
            if (choice == 1 || choice == 2 || choice == 3 || choice == 4 || choice == 5)
            {
                loop = false;
            }
            else
            {
                throw std::invalid_argument("invalid argument");
            }
        }
        catch (const std::exception &e)
        {
            std::cout << "invalid argument" << std::endl;
        }

    } while (loop);
    std::cin.ignore();
    return choice;
}

int GetInputSubMEnu()
{
    bool loop = true;
    std::string text;
    int choice;
    do
    {

        try
        {
            std::cin >> text;
            choice = std::stoi(text);
            if (choice == 1 || choice == 2 || choice == 3 || choice == 4)
            {
                loop = false;
            }
            else
            {
                throw std::invalid_argument("invalid argument");
            }
        }
        catch (const std::exception &e)
        {
            std::cout << "invalid argument" << std::endl;
        }

    } while (loop);
    std::cin.ignore();
    return choice;
}

std::string GetTextCinMax50char()
{
    bool loop = true;
    std::string text;
    do
    {
        std::getline(std::cin, text);
        if (text.length() != 0 && text.length() <= 50)
        {
            loop = false;
        }
        else
        {
            std::cout << "Maximum 50 characters" << std::endl;
            text = {};
        }
    } while (loop);
    return text;
}

void MainMenu()
{
    std::cout << "-------------------Menu---------------------" << std::endl;
    std::cout << "1 : Add an entry" << std::endl;
    std::cout << "2 : Remove an entry" << std::endl;
    std::cout << "3 : List Entries" << std::endl;
    std::cout << "4 : Manage accessing entries" << std::endl;
    std::cout << "5 : Quit" << std::endl;
    std::cout << "--------------------------------------------" << std::endl;
    std::cout << std::endl;
}

void ManagePasswordsMenu()
{
    std::cout << "----------Manage accessing entries----------" << std::endl;
    std::cout << "1 : Modify password decrypt/encrypt file" << std::endl;
    std::cout << "2 : Modify password accessing entries" << std::endl;
    std::cout << "3 : Main menu" << std::endl;
    std::cout << "4 : Quit" << std::endl;
    std::cout << "--------------------------------------------" << std::endl;
    std::cout << std::endl;
}
