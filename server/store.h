#pragma once
#include<iostream>
#include<string>

class KeyValueStore{
    private:
        std::unordered_map<std::string,std::string> store;
    public:
        std::string set(const std::string &key,const std::string &value);
        std::string get(const std::string &key);
        std::string del(const std::string& key);
};
