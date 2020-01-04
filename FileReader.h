//==================================================================================
// Name        : FileReader.h
// Author      : Julien Thibodeau
// Description : Declaration for FileReader.cpp in C++, Ansi-style
//==================================================================================

#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <unistd.h>
#include <experimental/filesystem>
#include <iostream>
#include <sys/stat.h>
#include "cryptopp/modes.h"
#include "cryptopp/aes.h"
#include "cryptopp/osrng.h"
#include "cryptopp/hex.h"
#include "cryptopp/hkdf.h"
#include "cryptopp/pssr.h"
#include <jsoncpp/json/json.h>

inline bool isFileExistReader(const std::string &name)
{
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

std::string getCurrentDirReader()
{
    char buff[FILENAME_MAX];
    getcwd(buff, FILENAME_MAX);
    std::string currentWorkingDir(buff);
    return currentWorkingDir;
}

struct SHA512keysReader
{
    std::string key;
    std::string iv;
};

struct DataInfosJsonRowReader
{
    std::string url;
    std::string user;
    std::string pwd;
};


class FileReader
{
private:
    inline static const std::string ivFile = "D04BF8250278335D017A5FE311151DE2";
    inline static const std::string pathFile = getCurrentDirReader() + "/accounts.encrypted";

    std::string aesCTRDecrypt(std::string KEY, std::string IV, std::string infoToDecrypt);
    std::string aesCBCDecrypt(std::string KEY, std::string IV, std::string infoToDecrypt);

    std::string DecryptJsonFile(SHA512keysReader sha512keys);

    SHA512keysReader generateKeyIv(std::string pwdContent);
    SHA512keysReader generateKeyFromPwdAndIv(std::string pwdContent, std::string encodedIv);

public:
    FileReader();
    ~FileReader();

    bool validatepwdFile(std::string pwdFile);
    bool validatepwdContent(std::string pwdFile, std::string pwdContent);

    void printListEntriesInfosCensored(std::string pwdFile);
    void printListEntriesInfosDecrypted(std::string pwdFile, std::string pwdContent);
};
