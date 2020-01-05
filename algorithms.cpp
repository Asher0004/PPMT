//==================================================================================
// Name        : algorithms.cpp
// Author      : Julien Thibodeau
// Description : Algorithms for authentificate.cpp in C++, Ansi-style
//==================================================================================

#include "utils.cpp"
#include "FileReader.cpp"
#include "FileWriter.cpp"

void addEntry()
{
    FileWriter fileWriter;

    std::cout << "Password for decrypt file : ";
    std::string pwdFile = GetTextCinMax50char();

    if (!fileWriter.validatepwdFile(pwdFile))
    {
        return;
    }

    std::cout << "Password for data entries : ";
    std::string pwdContent = GetTextCinMax50char();

    if (!fileWriter.validatepwdContent(pwdFile, pwdContent))
    {
        return;
    }

    DataInfosJsonRowWriter entryInfos;
    std::vector<std::string> data;
    do
    {
        std::cout << "Url : ";
        entryInfos.url = GetTextCinMax50char();

        std::cout << "User : ";
        entryInfos.user = GetTextCinMax50char();

        std::cout << "Password : ";
        entryInfos.pwd = GetTextCinMax50char();

    } while (validateData());

    fileWriter.saveEntryInfos(entryInfos, pwdFile, pwdContent);
}

void RemoveAnEntry()
{
    FileReader fileReader;
    std::cout << "Password for decrypt file : ";
    std::string pwdFile = GetTextCinMax50char();

    if (!fileReader.validatepwdFile(pwdFile))
    {
        return;
    }

    int numberData = fileReader.printListEntriesInfosCensored(pwdFile);

    int lineToSupress = GetLineToSupress(numberData);

    if (lineToSupress == 0)
    {
        return;
    }

    FileWriter fileWriter;

    fileWriter.saveDeletedRowEntryInfos(lineToSupress, pwdFile);

    std::cout << "Deleted row succesfully." << std::endl;
}

void ListEntries()
{
    FileReader fileReader;
    std::cout << "Password for decrypt file : ";
    std::string pwdFile = GetTextCinMax50char();

    if (!fileReader.validatepwdFile(pwdFile))
    {
        return;
    }

    fileReader.printListEntriesInfosCensored(pwdFile);

    int trial = 0;
    std::string pwdContent;
    bool success = false;
    do
    {
        std::cout << "Password for data entries : ";
        pwdContent = GetTextCinMax50char();

        if (!fileReader.validatepwdContent(pwdFile, pwdContent))
        {
        }
        else
        {
            success = true;
            break;
        }
        trial++;

    } while (trial < 3);

    if (!fileReader.validatepwdContent(pwdFile, pwdContent))
    {
        return;
    }

    fileReader.printListEntriesInfosDecrypted(pwdFile, pwdContent);
}

void ModifyPasswordFile()
{
    FileReader fileReader;

    std::cout << "Old password for decrypt/encrypt file : ";
    std::string oldPwdFile = GetTextCinMax50char();

    if (!fileReader.validatepwdFile(oldPwdFile))
    {
        return;
    }

    std::cout << "New password for decrypt/encrypt file : ";
    std::string newPwdFile = GetTextCinMax50char();

    FileWriter fileWriter;

    fileWriter.changepwdFile(oldPwdFile, newPwdFile);

    std::cout << "Changed password file succesfully." << std::endl;
}

void ModifyPasswordAccessEntries()
{
    // TODO
}
