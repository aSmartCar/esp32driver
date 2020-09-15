/**
 * @file filehandler.cpp
 * @author Philipp Köhler
 * @version 0.1
 * @date 2019-08-17 - created
 * @date 2020-08-04 - last modified
 * @copyright Copyright (c) 2020
 * 
 */
#include "filehandler.h"

/**
 * @brief Read a file and return it as string in fileContent
 * 
 * @param fileContent 
 */
void FileHandler::readFile(String &fileContent)
{
    File file = SPIFFS.open(this->path, "r");

    if (!file)
    {
        Serial.println("Could not open File in read mode");
        return;
    }

    String buffer;
    while (file.available())
    {
        buffer.concat((char)file.read());
    }
    file.close();
    fileContent = buffer;
}

/**
 * @brief Write the value into a file
 * 
 * @param value 
 */
void FileHandler::writeFile(const String value)
{
    File file = SPIFFS.open(this->path, FILE_WRITE);

    if (!file)
    {
        Serial.println("Could not open File in write mode");
        return;
    }

    if (file.print(value))
        Serial.println("Write File Done");
    else
        Serial.println("Write File Error");
    file.close();
}