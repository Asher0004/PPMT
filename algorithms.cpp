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
        std::cout << "Invalid password for decrypt file" << std::endl;
        return;
    }

    std::cout << "Password for data entries : ";
    std::string pwdContent = GetTextCinMax50char();

    if (!fileWriter.validatepwdContent(pwdFile, pwdContent))
    {
        std::cout << "Invalid password for data entries" << std::endl;
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
    // TODO
}

void ListEntries()
{
    FileReader fileReader;
    std::cout << "Password for decrypt file : ";
    std::string pwdFile = GetTextCinMax50char();

    if (!fileReader.validatepwdFile(pwdFile))
    {
        std::cout << "Invalid password for decrypt file" << std::endl;
        return;
    }

    fileReader.printListEntriesInfosCensored(pwdFile);

    int trial = 0;
    std::string pwdContent;
    do
    {
        std::cout << "Password for data entries : ";
        pwdContent = GetTextCinMax50char();

        if (!fileReader.validatepwdContent(pwdFile, pwdContent))
        {
            std::cout << "Invalid password for data entries" << std::endl;
        }
        else
        {
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
    // TODO
}

void ModifyPasswordAccessEntries()
{
    // TODO
}
