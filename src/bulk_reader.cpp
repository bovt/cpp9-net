/// @file
/// @brief Файл с реализацией класса читателя блока команд

#include "bulk_reader.h"
#include "bulk.h"
#include <algorithm>
#include <iostream>
#include <string>

bool BulkReader::read(Bulk& bulk)
{
    while (bulk.size() < n_ || state() == OPENED_BULK)
    {
        char ch = 0;
        std::string cmd;
        while (is_.get(ch) && ch != '\n')
        {
            buffer_.push_back(ch);
        }
        if (is_.eof())
        {
            return false;
        }

        cmd.swap(buffer_);

        ltrim(cmd);
        if (cmd.empty())
        {
            continue;
        }
        if (cmd[0] == '{')
        {
            openDepth_++;
            if (openDepth_ == 1)
            {
                return true;
            }
            continue;
        }
        else if (cmd[0] == '}')
        {
            if (openDepth_ == 0) // случай, если изменение размера блока начинается с символа '}'
            {
                continue;
            }
            openDepth_--;
            if (openDepth_ == 0)
            {
                return true;
            }
            continue;
        }
        else
        {
            bulk.push_back(std::move(cmd));
        }
    }
    return true;
}

BulkReader::State BulkReader::state() const
{
    return openDepth_? OPENED_BULK : CLOSED_BULK;
}

void BulkReader::ltrim(std::string& str) const
{
    auto it = std::find_if(str.begin() ,str.end(), [](char ch){ return !std::isspace(ch); } );
    str.erase(str.begin(), it);
}
