#pragma once

#include <list>
#include <mutex>
#include <stdexcept>
#include <unordered_map>
#include <utility>

template<typename K, typename V>
class LRUCache {
private:
	using ListKVIterator = typename std::list<std::pair<K, V>>::iterator;

	size_t m_maxCapacity;
	std::list<std::pair<K, V>> m_keyValueList;
	std::unordered_map<K, ListKVIterator> m_keyMap;
	std::mutex m_mutex;

public:
	explicit LRUCache(size_t capacity) : m_maxCapacity(capacity) {
		if (capacity == 0) {
			throw std::invalid_argument("Cache capacity has to be positive value.");
		}
	}

	bool Get(const K& key, V& value) {
		std::lock_guard<std::mutex> lock(m_mutex);
		auto findKey = m_keyMap.find(key);

		if (findKey == m_keyMap.end()) {
			return false;
		}
		else {
			m_keyValueList.splice(m_keyValueList.begin(), m_keyValueList, findKey->second);

			value = m_keyValueList.begin()->second;

			return true;
		}
	}

	void Put(const K& key, const V& value) {
		std::lock_guard<std::mutex> lock(m_mutex);
		auto findKey = m_keyMap.find(key);

		if (findKey != m_keyMap.end()) {
			m_keyValueList.splice(m_keyValueList.begin(), m_keyValueList, findKey->second);
			m_keyValueList.begin()->second = value;

			return;
		}
		else {
			if (m_keyValueList.size() >= m_maxCapacity) {
				m_keyMap.erase(m_keyValueList.back().first);
				m_keyValueList.pop_back();
			}

			m_keyValueList.emplace_front(key, value);
			m_keyMap[key] = m_keyValueList.begin();
			
			return;
		}
	}
};