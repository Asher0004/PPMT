//==================================================================================
// Name        : FileWriter.h
// Author      : Julien Thibodeau
// Description : Declaration for FileWriter.cpp in C++, Ansi-style
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


inline bool isFileExistWriter(const std::string &name)
{
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

std::string getCurrentDirWriter()
{
    char buff[FILENAME_MAX];
    getcwd(buff, FILENAME_MAX);
    std::string currentWorkingDir(buff);
    return currentWorkingDir;
}

struct SHA512keysWriter
{
    std::string key;
    std::string iv;
};

struct DataInfosJsonRowWriter
{
    std::string url;
    std::string user;
    std::string pwd;
};

class FileWriter
{
private:
    inline static const std::string ivFile = "D04BF8250278335D017A5FE311151DE2";
    inline static const std::string pathFile = getCurrentDirWriter() + "/accounts.encrypted";

    std::string aesCTREncrypt(std::string KEY, std::string IV, std::string infoToEncrypt);
    std::string aesCTRDecrypt(std::string KEY, std::string IV, std::string infoToDecrypt);

    std::string aesCBCEncrypt(std::string KEY, std::string IV, std::string infoToEncrypt);
    std::string aesCBCDecrypt(std::string KEY, std::string IV, std::string infoToDecrypt);

    std::string addEntryToJsonString(std::string JsonString, DataInfosJsonRowWriter dataInfosJsonRow, SHA512keysWriter keys);
    std::string deletedRowToJsonString(std::string JsonString, int lineToSupress);
    std::string DecryptJsonFile(SHA512keysWriter sha512keys);

    SHA512keysWriter generateKeyIv(std::string pwdContent);
    SHA512keysWriter generateKeyFromPwdAndIv(std::string pwdContent, std::string encodedIv);

    void initiateJsonFile(std::string pwdFile);
    void saveEncryptedData(std::string EncryptedData);

public:
    FileWriter();
    ~FileWriter();

    bool validatepwdFile(std::string pwdFile);
    bool validatepwdContent(std::string pwdFile, std::string pwdContent);

    void saveEntryInfos(DataInfosJsonRowWriter entryInfos, std::string pwdFile, std::string pwdContent);
    void saveDeletedRowEntryInfos(int lineToSupress, std::string pwdFile);
};
