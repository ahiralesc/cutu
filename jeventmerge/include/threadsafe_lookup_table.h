#ifndef __TSLookupTable__
#define __TSLookupTable__

/**
* See Williams, A. C++ Concurrency in Action: Practical Multithreading Manning, 2012 (author)
*/

#include <vector>
#include <memory>
#include <mutex>
#include <functional>
#include <list>
#include <utility>
#include <algorithm>
#include <cstddef>
#include <boost/thread/shared_mutex.hpp>


template<typename Key, typename Value>
class bucket_type {
private:
    typedef std::pair<Key,Value> bucket_value;
    typedef std::list<bucket_value> bucket_data;
    typedef typename bucket_data::iterator bucket_iterator;
    bucket_data data;
    mutable boost::shared_mutex mutex;

    bucket_iterator find(Key const& key) {
        return std::find_if(data.begin(),data.end(),
        [&](bucket_value const& item){return item.first == key;});
    }



public:
    Value at(Key const& key,Value const& default_value) const {
        boost::shared_lock<boost::shared_mutex> lock(mutex);
        bucket_iterator const entry=find(key);
        return (entry==data.end())? default_value:entry->second;
    }

    void insert(Key const& key,Value const& value) {
   
        std::unique_lock<boost::shared_mutex> lock(mutex);
        bucket_iterator const entry=find(key);
        if(entry==data.end())
            data.push_back(bucket_value(key,value));
        else
            entry->second=value;
    
    }

    void remove(Key const& key) {
        std::unique_lock<boost::shared_mutex> lock(mutex);
        bucket_iterator const entry=find(key);
        if(entry!=data.end())
            data.erase(entry);
    }
};



template<typename Key,typename Value,typename Hash=std::hash<Key>>
class threadsafe_lookup_table {
private:
    std::vector<std::unique_ptr<bucket_type<Key,Value>> > buckets;
    Hash hasher;
    
    bucket_type<Key,Value>& get_bucket(Key const& key) const {
        std::size_t const index=hasher(key)%buckets.size();
        return *buckets[index];
    }

public:
    typedef Key key_type;typedef Value mapped_type;
    typedef Hash hash_type;
    
    threadsafe_lookup_table(unsigned num_buckets=3001, Hash const& hasher_=Hash()): buckets(num_buckets),hasher(hasher_){
        for(unsigned i=0;i<num_buckets;++i) {
            buckets[i].reset(new bucket_type<Key,Value>);
        }
    }

    threadsafe_lookup_table(threadsafe_lookup_table const& other) = delete;
    threadsafe_lookup_table& operator=(threadsafe_lookup_table const& other)=delete;
    Value at(Key const& key,Value const& default_value=Value()) const {
        return get_bucket(key).at(key,default_value);
    }
    
    void insert(Key const& key,Value const& value) {
        get_bucket(key).insert(key,value);
    }
    
    void remove(Key const& key) {
        get_bucket(key).remove(key);
    }
};


#endif
