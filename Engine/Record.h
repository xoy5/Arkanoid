#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

struct Record {
    int place;
    std::string name;
    int score;
};

Record parseLine(const std::string& line) {
    Record r;
    std::stringstream ss(line);
    std::string temp;

    std::getline(ss, temp, ';');
    r.place = std::stoi(temp);

    std::getline(ss, r.name, ';');

    std::getline(ss, temp, ';');
    r.score = std::stoi(temp);

    return r;
}

std::string formatLine(const Record& r) {
    return std::to_string(r.place) + ";" + r.name + ";" + std::to_string(r.score);
}

void updateRecords(const std::string& filename, const std::string& name, int score) {
    std::fstream file(filename, std::ios::in);
    std::vector<Record> records;

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            Record record = parseLine(line);
            if (name == record.name)
            {
                if (score <= record.score)
                {
                    return;
                }
            }
            else
            {
                records.push_back(record);
            }
        }
    }
    file.close();

    Record newRecord{ 0, name.substr(0, 8), score };
    records.push_back(newRecord);

    std::sort(records.begin(), records.end(), [](const Record& a, const Record& b) {
        return a.score > b.score;
    });

    for (size_t i = 0; i < records.size(); ++i) {
        records[i].place = static_cast<int>(i + 1);
    }

    file.open(filename, std::ios::out | std::ios::trunc);
    for (const auto& r : records) {
        file << formatLine(r) << '\n';
    }
    file.close();
}

std::vector<Record> getRecords(const std::string& filename, int n = 10)
{
    std::fstream file(filename, std::ios::in);
    std::vector<Record> records;

    std::string line;
    int i = 0;
    while (std::getline(file, line)) 
    {
		if (i++ >= n){
			break;
		}
        records.push_back(parseLine(line));
    }
    file.close();

    return records;
}