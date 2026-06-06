#include "store.h"
std::string KeyValueStore::set(const std::string& key,const std::string& value){
    store[key] =value;
    return "ok";
}
std::string KeyValueStore::get(const std::string& key){
    if(store.find(key)!=store.end()){
        return "value "+store[key];
    }
    return "not found";
}
std::string KeyValueStore::del(const std::string & key){
    if(store.erase(key))return "deleted";
    return "not found";
}