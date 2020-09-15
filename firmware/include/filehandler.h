/**
 * @file filehandler.h
 * @author Philipp Köhler
 * @brief Provides read and write functions for the SPIFFS
 * @version 0.1
 * @date 2019-08-17 - created
 * @date 2020-08-04 - last modified
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <SPIFFS.h>

class FileHandler
{
public:
    FileHandler(String path): path(path){;};
    void readFile(String &fileContent);
    void writeFile(const String value);
    
private:
    String path;
};

#endif //FILEHANDLER_H
