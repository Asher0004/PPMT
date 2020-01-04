//==================================================================================
// Name        : FileReader.cpp
// Author      : Julien Thibodeau
// Description : Print saved password in C++, Ansi-style
//==================================================================================

#include "FileReader.h"
#include "VariadicTable.h"

FileReader::FileReader()
{
}

FileReader::~FileReader()
{
}

std::string FileReader::aesCTRDecrypt(std::string KEY, std::string IV, std::string infoToDecrypt)
{
    CryptoPP::CTR_Mode<CryptoPP::AES>::Decryption aes((unsigned char *)KEY.c_str(), CryptoPP::AES::DEFAULT_KEYLENGTH, (unsigned char *)IV.c_str());
    std::string decrypted;
    CryptoPP::StringSource decrypt(infoToDecrypt, true, new CryptoPP::HexDecoder(new CryptoPP::StreamTransformationFilter(aes, new CryptoPP::StringSink(decrypted))));
    return decrypted;
}

std::string FileReader::aesCBCDecrypt(std::string KEY, std::string IV, std::string infoToDecrypt)
{
    CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption aes((unsigned char *)KEY.c_str(), CryptoPP::AES::DEFAULT_KEYLENGTH, (unsigned char *)IV.c_str());
    std::string decrypted;
    CryptoPP::StringSource decrypt(infoToDecrypt, true, new CryptoPP::HexDecoder(new CryptoPP::StreamTransformationFilter(aes, new CryptoPP::StringSink(decrypted))));
    return decrypted;
}

std::string FileReader::DecryptJsonFile(SHA512keysReader sha512keys)
{
    std::ifstream file(FileReader::pathFile);
    std::string data((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));

    data.erase(std::remove_if(data.begin(), data.end(), [](unsigned char x) { return std::isspace(x); }), data.end());

    return FileReader::aesCTRDecrypt(sha512keys.key, sha512keys.iv, data);
}

SHA512keysReader FileReader::generateKeyIv(std::string pwdContent)
{
    unsigned char iv[CryptoPP::AES::BLOCKSIZE];
    CryptoPP::AutoSeededRandomPool generator;
    generator.GenerateBlock(iv, CryptoPP::AES::BLOCKSIZE);
    unsigned char key[CryptoPP::AES::DEFAULT_KEYLENGTH];

    CryptoPP::HKDF<CryptoPP::SHA512> hkdf;
    hkdf.DeriveKey(key, CryptoPP::AES::DEFAULT_KEYLENGTH, (unsigned char *)pwdContent.data(), pwdContent.size(), iv, CryptoPP::AES::BLOCKSIZE, NULL, 0);

    std::string encodedKey;
    CryptoPP::StringSource encodeKey(key, sizeof(key), true, new CryptoPP::HexEncoder(new CryptoPP::StringSink(encodedKey)));

    std::string encodedIv;
    CryptoPP::StringSource encodeIv(iv, sizeof(iv), true, new CryptoPP::HexEncoder(new CryptoPP::StringSink(encodedIv)));

    SHA512keysReader sha512keys;
    sha512keys.key = encodedKey;
    sha512keys.iv = encodedIv;

    return sha512keys;
}

SHA512keysReader FileReader::generateKeyFromPwdAndIv(std::string pwdContent, std::string encodedIv)
{
    unsigned char key[CryptoPP::AES::DEFAULT_KEYLENGTH];

    std::string decodedIvHex;
    CryptoPP::StringSource decodeIvHex(encodedIv, true, new CryptoPP::HexDecoder(new CryptoPP::StringSink(decodedIvHex)));
    byte *result = (byte *)decodedIvHex.data();

    CryptoPP::HKDF<CryptoPP::SHA512> hkdf;
    hkdf.DeriveKey(key, CryptoPP::AES::DEFAULT_KEYLENGTH, (unsigned char *)pwdContent.data(), pwdContent.size(), result, CryptoPP::AES::BLOCKSIZE, NULL, 0);

    std::string encodedKey;
    CryptoPP::StringSource encodeKey(key, sizeof(key), true, new CryptoPP::HexEncoder(new CryptoPP::StringSink(encodedKey)));

    SHA512keysReader sha512keys;
    sha512keys.key = encodedKey;
    sha512keys.iv = encodedIv;

    return sha512keys;
}

bool FileReader::validatepwdFile(std::string pwdFile)
{
    if (!isFileExistReader(FileReader::pathFile))
    {
        std::cout << "Need to have data. Use option 1." << std::endl;
        return false;
    }

    SHA512keysReader sha512keysFile = FileReader::generateKeyFromPwdAndIv(pwdFile, FileReader::ivFile);
    std::string decryptedJson = FileReader::DecryptJsonFile(sha512keysFile);

    if (decryptedJson[0] == '{')
    {
        return true;
    }

    return false;
}

bool FileReader::validatepwdContent(std::string pwdFile, std::string pwdContent)
{
    if (!isFileExistReader(FileReader::pathFile))
    {
        std::cout << "Need to have data. Use option 1." << std::endl;
        return false;
    }

    SHA512keysReader sha512keysFile = FileReader::generateKeyFromPwdAndIv(pwdFile, FileReader::ivFile);
    std::string decryptedJson = FileReader::DecryptJsonFile(sha512keysFile);

    std::stringstream stream(decryptedJson);
    Json::Value root;
    stream >> root;
    Json::Value infoArray = root["accounts"];

    if (infoArray.size() == 0)
    {
        return true;
    }

    SHA512keysReader sha512keysContent = FileReader::generateKeyFromPwdAndIv(pwdContent, infoArray[0]["iv"].asString());

    try
    {
        FileReader::aesCBCDecrypt(sha512keysContent.key, sha512keysContent.iv, infoArray[0]["user"].asString());
        return true;
    }
    catch (const std::exception &e)
    {
        return false;
    }
}

void FileReader::printListEntriesInfosCensored(std::string pwdFile)
{

    SHA512keysReader sha512keysFile = FileReader::generateKeyFromPwdAndIv(pwdFile, FileReader::ivFile);
    std::string decryptedJson = FileReader::DecryptJsonFile(sha512keysFile);

    std::stringstream stream(decryptedJson);
    Json::Value root;
    stream >> root;
    Json::Value infoArray = root["accounts"];

    VariadicTable<int, std::string, std::string, std::string> table({"line", "url", "user", "pwd"});

    int numberLine = 1;
    std::string url;
    for (int i = 0; i < infoArray.size(); i++)
    {
        url = infoArray[i]["url"].toStyledString().substr(1, infoArray[i]["url"].toStyledString().length() - 3);
        table.addRow({numberLine, url, "*****", "*****"});
        numberLine++;
    }

    table.print(std::cout);
}

void FileReader::printListEntriesInfosDecrypted(std::string pwdFile, std::string pwdContent)
{
    SHA512keysReader sha512keysFile = FileReader::generateKeyFromPwdAndIv(pwdFile, FileReader::ivFile);
    std::string decryptedJson = FileReader::DecryptJsonFile(sha512keysFile);

    std::stringstream stream(decryptedJson);
    Json::Value root;
    stream >> root;
    Json::Value infoArray = root["accounts"];

    VariadicTable<int, std::string, std::string, std::string> table({"line", "url", "user", "pwd"});

    int numberLine = 1;
    std::string url;

    std::string encodedIv;
    SHA512keysReader sha512KeysReader;

    std::string encodedUser;
    std::string encodedPwd;

    std::string user;
    std::string pwd;

    for (int i = 0; i < infoArray.size(); i++)
    {
        url = infoArray[i]["url"].toStyledString().substr(1, infoArray[i]["url"].toStyledString().length() - 3);

        encodedIv = infoArray[i]["iv"].toStyledString().substr(1, infoArray[i]["iv"].toStyledString().length() - 3);
        sha512KeysReader = FileReader::generateKeyFromPwdAndIv(pwdContent, encodedIv);

        encodedUser = infoArray[i]["user"].toStyledString().substr(1, infoArray[i]["user"].toStyledString().length() - 3);
        encodedPwd = infoArray[i]["pwd"].toStyledString().substr(1, infoArray[i]["pwd"].toStyledString().length() - 3);

        user = FileReader::aesCBCDecrypt(sha512KeysReader.key, sha512KeysReader.iv, encodedUser);
        pwd = FileReader::aesCBCDecrypt(sha512KeysReader.key, sha512KeysReader.iv, encodedPwd);

        table.addRow({numberLine, url, user, pwd});
        numberLine++;
    }

    table.print(std::cout);
}