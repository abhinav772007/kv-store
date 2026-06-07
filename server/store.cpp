#include "store.h"
std::string KeyValueStore::set(const std::string& key,const std::string& value){
    std::lock_guard<std::mutex> lock(mtx);
    store[key] =value;
    return "ok";
}
std::string KeyValueStore::get(const std::string& key){
    std::lock_guard<std::mutex> lock(mtx);
    if(store.find(key)!=store.end()){
        return "value "+store[key];
    }
    return "not found";
}
std::string KeyValueStore::del(const std::string & key){
    std::lock_guard<std::mutex> lock(mtx);
    if(store.erase(key))return "deleted";
    return "not found";
}