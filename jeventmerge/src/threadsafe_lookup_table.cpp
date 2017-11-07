#include "threadsafe_lookup_table.h"


template<typename Key, typename Value>
Value TSLT::bucket_type<Key,Value>::value_for(Key const& key,Value const& default_value) const {
    boost::shared_lock<boost::shared_mutex> lock(mutex);
    bucket_iterator const found_entry=find_entry_for(key);
    return (found_entry==data.end())? default_value:found_entry->second;
}


template<typename Key, typename Value>
void TSLT::bucket_type<Key,Value>::add_or_update_mapping(Key const& key,Value const& value){
    std::unique_lock<boost::shared_mutex> lock(mutex);
    bucket_iterator const found_entry=find_entry_for(key);
    if(found_entry==data.end()) {
        data.push_back(bucket_value(key,value));
    } else {
        found_entry->second=value;
    }
}

template<typename Key, typename Value>
void TSLT::bucket_type<Key,Value>::remove_mapping(Key const& key) {
    std::unique_lock<boost::shared_mutex> lock(mutex);
    bucket_iterator const found_entry=find_entry_for(key);
    if(found_entry!=data.end()) {
        data.erase(found_entry);
    }
}

template<typename Key, typename Value, typename Hash>
Value TSLT::threadsafe_lookup_table<Key,Value,Hash>::value_for(Key const& key, Value const& default_value) const {
    return get_bucket(key).value_for(key,default_value);
}


template<typename Key, typename Value, typename Hash>
void TSLT::threadsafe_lookup_table<Key,Value,Hash>::add_or_update_mapping(Key const& key,Value const& value) {
    get_bucket(key).add_or_update_mapping(key,value);
}


template<typename Key, typename Value, typename Hash>
void TSLT::threadsafe_lookup_table<Key,Value,Hash>::remove_mapping(Key const& key){
    get_bucket(key).remove_mapping(key);
}
