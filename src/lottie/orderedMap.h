#ifndef _SAVER_ORDERED_MAP_H_
#define _SAVER_ORDERED_MAP_H_

#include <vector>
#include <unordered_map>

template<typename V>
class OrderedMap
{
    using KeyValue = std::pair<std::string, V>;

public:
	KeyValue& insert(const std::string& key, V&& val)
	{
        auto it = mIndexMap.find(key);
        if(it != mIndexMap.end())
        {
            return mStorage[it->second];
        }
        auto idx = mStorage.size();
        mIndexMap[key] = idx;
        mStorage.emplace_back(key, std::forward<V>(val));

        return mStorage[idx];
	}

    V& operator[](const std::string& key)
    {
        return insert(key, {}).second;
    }

	// 순회
	auto begin() const
	{
		return mStorage.begin();
	}
	auto end() const
	{
		return mStorage.end();
	}
    auto size() const 
    {
        return mStorage.size();
    }

private:
	std::vector<std::pair<std::string, V>> mStorage;
    std::unordered_map<std::string, int> mIndexMap;
};

#endif