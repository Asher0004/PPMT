//==================================================================================
// Name        : FileWriter.cpp
// Author      : Julien Thibodeau
// Description : Manage passwords save in C++, Ansi-style
//==================================================================================

#include "FileWriter.h"

FileWriter::FileWriter()
{
}

FileWriter::~FileWriter()
{
}

std::string FileWriter::aesCTREncrypt(std::string KEY, std::string IV, std::string infoToEncrypt)
{
    CryptoPP::CTR_Mode<CryptoPP::AES>::Encryption aes((unsigned char *)KEY.c_str(), CryptoPP::AES::DEFAULT_KEYLENGTH, (unsigned char *)IV.c_str());
    std::string cipher;
    CryptoPP::StringSource encrypt(infoToEncrypt, true, new CryptoPP::StreamTransformationFilter(aes, new CryptoPP::HexEncoder(new CryptoPP::StringSink(cipher))));
    return cipher;
}

std::string FileWriter::aesCTRDecrypt(std::string KEY, std::string IV, std::string infoToDecrypt)
{
    CryptoPP::CTR_Mode<CryptoPP::AES>::Decryption aes((unsigned char *)KEY.c_str(), CryptoPP::AES::DEFAULT_KEYLENGTH, (unsigned char *)IV.c_str());
    std::string decrypted;
    CryptoPP::StringSource decrypt(infoToDecrypt, true, new CryptoPP::HexDecoder(new CryptoPP::StreamTransformationFilter(aes, new CryptoPP::StringSink(decrypted))));
    return decrypted;
}

std::string FileWriter::aesCBCEncrypt(std::string KEY, std::string IV, std::string infoToEncrypt)
{
    CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption aes((unsigned char *)KEY.c_str(), CryptoPP::AES::DEFAULT_KEYLENGTH, (unsigned char *)IV.c_str());
    std::string cipher;
    CryptoPP::StringSource encrypt(infoToEncrypt, true, new CryptoPP::StreamTransformationFilter(aes, new CryptoPP::HexEncoder(new CryptoPP::StringSink(cipher))));
    return cipher;
}

std::string FileWriter::aesCBCDecrypt(std::string KEY, std::string IV, std::string infoToDecrypt)
{
    CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption aes((unsigned char *)KEY.c_str(), CryptoPP::AES::DEFAULT_KEYLENGTH, (unsigned char *)IV.c_str());
    std::string decrypted;
    CryptoPP::StringSource decrypt(infoToDecrypt, true, new CryptoPP::HexDecoder(new CryptoPP::StreamTransformationFilter(aes, new CryptoPP::StringSink(decrypted))));
    return decrypted;
}

std::string FileWriter::DecryptJsonFile(SHA512keysWriter sha512keys)
{
    std::ifstream file(FileWriter::pathFile);
    std::string data((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));

    data.erase(std::remove_if(data.begin(), data.end(), [](unsigned char x) { return std::isspace(x); }), data.end());

    return FileWriter::aesCTRDecrypt(sha512keys.key, sha512keys.iv, data);
}

std::string FileWriter::addEntryToJsonString(std::string JsonString, DataInfosJsonRowWriter dataInfosJsonRow, SHA512keysWriter keys)
{
    Json::Value root;
    std::stringstream stream(JsonString);
    stream >> root;

    Json::Value infoArray = root["accounts"];
    Json::Value infosRow;
    infosRow["iv"] = keys.iv;
    infosRow["url"] = dataInfosJsonRow.url;
    infosRow["user"] = FileWriter::aesCBCEncrypt(keys.key, keys.iv, dataInfosJsonRow.user);
    infosRow["pwd"] = FileWriter::aesCBCEncrypt(keys.key, keys.iv, dataInfosJsonRow.pwd);

    infoArray.append(infosRow);
    root["accounts"] = infoArray;

    return root.toStyledString();
}

SHA512keysWriter FileWriter::generateKeyIv(std::string pwdContent)
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

    SHA512keysWriter sha512keys;
    sha512keys.key = encodedKey;
    sha512keys.iv = encodedIv;

    return sha512keys;
}

SHA512keysWriter FileWriter::generateKeyFromPwdAndIv(std::string pwdContent, std::string encodedIv)
{
    unsigned char key[CryptoPP::AES::DEFAULT_KEYLENGTH];

    std::string decodedIvHex;
    CryptoPP::StringSource decodeIvHex(encodedIv, true, new CryptoPP::HexDecoder(new CryptoPP::StringSink(decodedIvHex)));
    byte *result = (byte *)decodedIvHex.data();

    CryptoPP::HKDF<CryptoPP::SHA512> hkdf;
    hkdf.DeriveKey(key, CryptoPP::AES::DEFAULT_KEYLENGTH, (unsigned char *)pwdContent.data(), pwdContent.size(), result, CryptoPP::AES::BLOCKSIZE, NULL, 0);

    std::string encodedKey;
    CryptoPP::StringSource encodeKey(key, sizeof(key), true, new CryptoPP::HexEncoder(new CryptoPP::StringSink(encodedKey)));

    SHA512keysWriter sha512keys;
    sha512keys.key = encodedKey;
    sha512keys.iv = encodedIv;

    return sha512keys;
}

bool FileWriter::validatepwdFile(std::string pwdFile)
{
    if (!isFileExistWriter(FileWriter::pathFile))
    {
        return true;
    }

    SHA512keysWriter sha512keysFile = FileWriter::generateKeyFromPwdAndIv(pwdFile, FileWriter::ivFile);
    std::string decryptedJson = FileWriter::DecryptJsonFile(sha512keysFile);

    if (decryptedJson[0] == '{')
    {
        return true;
    }

    return false;
}

bool FileWriter::validatepwdContent(std::string pwdFile, std::string pwdContent)
{
    if (!isFileExistWriter(FileWriter::pathFile))
    {
        return true;
    }

    SHA512keysWriter sha512keysFile = FileWriter::generateKeyFromPwdAndIv(pwdFile, FileWriter::ivFile);
    std::string decryptedJson = FileWriter::DecryptJsonFile(sha512keysFile);

    std::stringstream stream(decryptedJson);
    Json::Value root;
    stream >> root;
    Json::Value infoArray = root["accounts"];

    if (infoArray.size() == 0)
    {
        return true;
    }

    SHA512keysWriter sha512keysContent = FileWriter::generateKeyFromPwdAndIv(pwdContent, infoArray[0]["iv"].asString());

    try
    {
        FileWriter::aesCBCDecrypt(sha512keysContent.key, sha512keysContent.iv, infoArray[0]["user"].asString());
        return true;
    }
    catch (const std::exception &e)
    {
        return false;
    }
}

void FileWriter::initiateJsonFile(std::string pwdFile)
{
    Json::Value root;
    root["accounts"] = Json::arrayValue;

    Json::StreamWriterBuilder builder;
    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    std::ofstream outputFileStream(FileWriter::pathFile);
    writer->write(root, &outputFileStream);
    outputFileStream.close();

    std::ifstream stream(FileWriter::pathFile);
    std::string data((std::istreambuf_iterator<char>(stream)), (std::istreambuf_iterator<char>()));
    SHA512keysWriter sha512keys = FileWriter::generateKeyFromPwdAndIv(pwdFile, FileWriter::ivFile);
    std::string encrypted = FileWriter::aesCTREncrypt(sha512keys.key, sha512keys.iv, data);

    FileWriter::saveEncryptedData(encrypted);
}

void FileWriter::saveEntryInfos(DataInfosJsonRowWriter entryInfos, std::string pwdFile, std::string pwdContent)
{

    if (!isFileExistWriter(FileWriter::pathFile))
    {
        std::ofstream accounts(FileWriter::pathFile, std::ofstream::out);
        accounts.close();
        FileWriter::initiateJsonFile(pwdFile);
    }

    SHA512keysWriter sha512keysFile = FileWriter::generateKeyFromPwdAndIv(pwdFile, FileWriter::ivFile);
    std::string decryptedJson = FileWriter::DecryptJsonFile(sha512keysFile);

    SHA512keysWriter sha512keysContent = FileWriter::generateKeyIv(pwdContent);
    std::string decryptedJsonWithNewEntry = FileWriter::addEntryToJsonString(decryptedJson, entryInfos, sha512keysContent);
    std::string JsonEncrypted = FileWriter::aesCTREncrypt(sha512keysFile.key, sha512keysFile.iv, decryptedJsonWithNewEntry);

    FileWriter::saveEncryptedData(JsonEncrypted);
}

void FileWriter::saveEncryptedData(std::string EncryptedData)
{
    std::vector<std::string> formattedData;
    for (unsigned i = 0; i < EncryptedData.length(); i += 64)
    {
        formattedData.push_back(EncryptedData.substr(i, 64));
    }

    std::fstream file;
    file.open(FileWriter::pathFile);
    for (size_t i = 0; i < formattedData.size(); i++)
    {
        file << formattedData[i] << std::endl;
    }

    file.close();
}