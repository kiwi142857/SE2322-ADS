#ifndef UTILS_H
#define UTILS_H

#include <bitset>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
namespace utils
{
std::string getFileName(const std::string &fileName, const std::string &suffix)
{
    size_t final_pos = fileName.find_last_of("/\\");
    size_t suffix_pos = fileName.find_last_of('.');
    return final_pos == std::string::npos
               ? "./output/" + fileName.substr(0, suffix_pos) + suffix
               : "./output/" + fileName.substr(final_pos + 1, suffix_pos - final_pos - 1) + suffix;
}

std::string parseText(const std::string &input)
{
    std::string content;
    std::ifstream file(input, std::ios::binary);
    if (file.is_open()) {
        char ch[1];
        while (file.read(ch, 1)) {
            content += ch[0];
        }
        file.close();
    } else
        std::cout << "Unable to open file";

    return content;
}

void output(const std::string &output, const std::string &data)
{
    // TODO: Your code here
    std::ofstream file(output, std::ios::binary);
    if (file.is_open()) {
        file << data;
        file.close();
    } else
        std::cout << "Unable to open file";
}

std::string codingTable2String(const std::map<std::string, std::string> &codingTable)
{
    std::string result = "";
    // TODO: Your code here
    for (auto it = codingTable.begin(); it != codingTable.end(); ++it) {
        result += it->first + " " + it->second + "\n";
    }
    return result;
}

void loadCodingTable(const std::string &input, std::map<std::string, std::string> &codingTable)
{
    // TODO: Your code here
    std::ifstream file(input, std::ios::binary);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) {
                std::getline(file, line);
                // calculate the length of blank 
                int lengthOfblank = 0; 
                for(int i = 0; i < line.size(); i++) {
                    if (line[i] == ' ')
                        lengthOfblank++;
                }
                std::string value = line.substr(lengthOfblank);
                if(lengthOfblank == 1)
                    codingTable[std::string(1, '\n')] = value;
                else if(lengthOfblank == 2)
                    codingTable[std::string(1, '\n' )] = value;
                continue;
            }
            std::string key = line.substr(0, line.rfind(" "));
            if (key.empty())
                key = " ";
            std::string value = line.substr(line.rfind(" ") + 1);
            if (value.empty()) {
                std::getline(file, line);
                line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
                codingTable[std::string(" \n")] = line;
                continue;
            }
            codingTable[key] = value;
        }
        file.close();
    } else
        std::cout << "Unable to open file";
}

std::string compress(const std::map<std::string, std::string> &codingTable, const std::string &text)
{
    std::string result;
    // TODO: Your code here
    for (int i = 0; i < text.size(); i++) {
        // 先找两个字符的编码
        if (i + 1 < text.size()) {
            std::string key2 = text.substr(i, 2);
            if (codingTable.find(key2) != codingTable.end()) {
                result += codingTable.at(key2);
                i++;
                continue;
            }
        }
        // 再找一个字符的编码
        std::string key = text.substr(i, 1);
        if (codingTable.find(key) != codingTable.end()) {
            result += codingTable.at(key);
        }
    }
    // 将result补足到8的倍数
    std::string byte;
    int64_t length = result.length();
    // 用八个字节的小端序表示长度
    for (int i = 0; i < 8; ++i)
        byte += (char)(length >> (i * 8));
    for (int i = length; i < ((length / 8) + ((length % 8) != 0)) * 8; ++i)
        result += '0';
    // 将result转化为字节流

    for (int i = 0; i < result.size(); i += 8) {
        byte += (char)std::stoi(result.substr(i, 8), nullptr, 2);
    }

    return byte;
}
}; // namespace utils

#endif