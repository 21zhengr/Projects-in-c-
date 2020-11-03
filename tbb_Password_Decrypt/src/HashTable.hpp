#pragma once
#include <string>
#include <utility>
#include <functional>

template <typename ValueType, 
	typename HashFuncType = std::hash<std::string>>
class HashTable
{
public:
	// Constructs the hash table with the given capacity (size of underlying array)
	HashTable(size_t capacity)
		:mTableData(new std::pair<std::string, ValueType>[capacity])
		,mCapacity(capacity)
		,mSize(0)
	{ }

	// TODO: Implement "Rule of five" functions
	~HashTable() {
		delete[] mTableData;
		mSize = 0;
		mCapacity = 0;

		mTableData = nullptr;
	}

	//copy constructor
	HashTable(const HashTable &h) {
		mTableData = new std::pair<std::string, ValueType>[h.Capacity()];
		mCapacity = h.Capacity();
		mSize = h.Size();
		for (int i = 0; i < mCapacity; ++i) {
			mTableData[i].first = h.mTableData[i].first;
			mTableData[i].second = h.mTableData[i].second;
		}

	}

	//assignment operator
	HashTable& operator=(const HashTable& rhs) {
		delete[] mTableData;
		mCapacity = rhs.Capacity();
		mSize = rhs.Size();
		mTableData = new std::pair<std::string, ValueType>[rhs.Capacity()];
		for (int i = 0; i < mCapacity; ++i) {
			mTableData[i].first = rhs.mTableData[i].first;
			mTableData[i].second = rhs.mTableData[i].second;
		}

		return *this;
	}

	//move assignment
	HashTable& operator=(HashTable&& rhs) {
		mCapacity = std::move(rhs.mCapacity);
		mSize = std::move(rhs.mSize);
		mTableData = std::move(rhs.mTableData);

		rhs.mCapacity = 0;
		rhs.mSize = 0;
		rhs.mTableData = nullptr;
		return *this;
	}

	//move constructor
	HashTable(HashTable&& rhs) {
		mCapacity = std::move(rhs.mCapacity);
		mSize = std::move(rhs.mSize);
		mTableData = std::move(rhs.mTableData);

		rhs.mCapacity = 0;
		rhs.mSize = 0;
		rhs.mTableData = nullptr;
	}
	// Returns overall capacity of hash table (size of underlying array)
	size_t Capacity() const
	{
		return mCapacity;
	}

	// Returns the number of elements currently inserted in the hash table
	size_t Size() const
	{
		return mSize;
	}

	// Tries to insert (key, value) into hash table
	// Returns true if insert succeeds, false if not
	bool Insert(const std::string& key, const ValueType& value)
	{
		// TODO: Implement
		if (mSize == mCapacity) {
			return false; //no more space left
		}
		int insertIndex = mHashFunc(key) % mCapacity;
		if (mTableData[insertIndex].first.empty()) { //there is space for it here
			mTableData[insertIndex] = std::make_pair(key, value);
			mSize += 1;
			return true;
		}
		//else
		int oldIndex = insertIndex;
		while (!mTableData[insertIndex].first.empty()) {
			insertIndex += 1;
			if (insertIndex == mCapacity) {
				insertIndex = 0; //wrap around
			}
			if (insertIndex == oldIndex) {
				return false;
			}
		}
		//exits while loop with an index where mTableData[insertIndex] == nullptr
		mTableData[insertIndex] = std::make_pair(key, value);
		mSize += 1;
		return true;
	}

	// Returns pointer to value for given key, or nullptr
	// if not in hash table
	ValueType* Find(const std::string& key) const
	{
		int searchIndex = mHashFunc(key) % mCapacity;
		if (mTableData[searchIndex].first.empty()) {
			return nullptr;
		}
		if (mTableData[searchIndex].first == key) {
			return &(mTableData[searchIndex].second);
		}
		//else
		int oldSearchIndex = searchIndex;
		while (mTableData[searchIndex].first != key) {
			searchIndex += 1;
			if (searchIndex == mCapacity) {
				searchIndex = 0; //wrap around
			}
			if (searchIndex == oldSearchIndex) {
				return nullptr;
			}
			if (mTableData[searchIndex].first.empty()) {
				return nullptr;
			}
		}
		return &(mTableData[searchIndex].second);
	
		//return nullptr;
	}

	// Executes provided lambda expression on each element inserted into the table
	void ForEach(std::function<void(const std::string& key, ValueType& value)> func)
	{
		// TODO: Implement
		for (int i = 0; i < mCapacity; ++i) {
			if (!mTableData[i].first.empty()) {
				func(mTableData[i].first, mTableData[i].second);
			}
		}
	}

	// Gets a const pointer to underlying data (for testing)
	const std::pair<std::string, ValueType>* Data() const
	{
		return mTableData;
	}
private:
	// NOTE: you should not need any more member data!
	
	// Underlying hash table array
	std::pair<std::string, ValueType>* mTableData;
	// Allocated capacity of hash table array
	size_t mCapacity;
	// Number of elements in HashTable
	size_t mSize;
	// Hash function
	HashFuncType mHashFunc;
};
