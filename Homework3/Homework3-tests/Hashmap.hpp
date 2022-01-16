#include <iostream>
#include <vector>
#include <forward_list>

using std::forward_list;
using std::vector;

template<class Key, class Value>
struct Data
{
	Data(const Key& word, const Value& count) : word(word), count(count) {};
	Key word;
	Value count;
};

template<class Key, class Value, class Hash = std::hash<Key>>
class HashMap
{
public:
	HashMap();
	HashMap( const HashMap& ) = default;
	~HashMap() = default;

	HashMap& operator=(const HashMap&) = default;

	bool            insert(const Key& word, const Value& count);
	void            rehash(size_t bucketsCount);

	bool			contains(const Key& word) const;

	size_t			getSize()           const { return size; }
	size_t          bucket_count()      const { return hArr.size(); }
	void			increment(Data<Key, Value>* pair, size_t times);

	template<class Func>
	void            for_each(Func func) const;

private:

	size_t						hash(const Key& word) const;
	const Data<Key, Value>*		find(const Key& word) const;
	Data<Key, Value>*			find(const Key& key);
	Data<Key, Value>*			add(const Data<Key, Value>& pair);

	using HashRow		= std::forward_list< Data <Key,Value> >;
	using HashTableArr	= std::vector< HashRow >;
	HashTableArr		hArr;
	size_t				size;
	Hash				hashFunc;
};

const size_t    DEFAULT_BUCKET_COUNT = 8;

template<class Key, class Value, class Hash>
inline HashMap<Key, Value, Hash>::HashMap() : hArr(DEFAULT_BUCKET_COUNT), size(0) {}

template<class Key, class Value, class Hash>
inline bool HashMap<Key, Value, Hash>::insert(const Key& word, const Value& count)
{
	Data<Key, Value> current(word, count);

	if (Data<Key, Value>* found = find(current.word))
	{
		increment(found, current.count);
		return true;
	}
	else 
	{
		this->add(current);
		size++;
		return true;
	}
	return false;
}

template<class Key, class Value, class Hash>
inline void HashMap<Key, Value, Hash>::rehash(size_t bucketsCount)
{
	size_t minBuckets = std::ceil(this->size() / this->max_load_factor());
	if (bucketsCount < minBuckets)
		bucketsCount = minBuckets;

	if (this->bucket_count() == bucketsCount)
		return;

	HashTableArr newArr(bucketsCount);
	std::swap(hArr, newArr);
	size = 0;

	for (const std::forward_list< Data<Key,Value> >& list : newArr)
		for (const Data<Key,Value> & elem : list)
			this->insert(elem.word, elem.count);
}

template<class Key, class Value, class Hash>
inline bool HashMap<Key, Value, Hash>::contains(const Key& word) const
{
	if (const Data<Key, Value>* found = find(word))
		return true;
	return false;
}

template<class Key, class Value, class Hash>
template<class Func>
inline void HashMap<Key, Value, Hash>::for_each(Func func) const
{
	for (const std::forward_list< Data<Key, Value> >& list : hArr)
		for (const Data<Key,Value>& elem : list)
			func(elem);
}

template<class Key, class Value, class Hash>
inline void HashMap<Key, Value, Hash>::increment(Data<Key, Value>* pair, size_t times)
{
	pair->count += times;
}

template<class Key, class Value, class Hash>
inline size_t HashMap<Key, Value, Hash>::hash(const Key& word) const
{
	return hashFunc(word) % this->bucket_count();
}

template<class Key, class Value, class Hash>
inline typename const Data<Key, Value>*
HashMap<Key, Value, Hash>::find(const Key& word) const
{
	size_t index = this->hash(word);

	for (const Data<Key, Value>& elem : hArr[index])
		if (elem.word == word)
			return &elem;

	return nullptr;
}

template<class Key, class Value, class Hash>
inline typename Data<Key,Value>*
HashMap<Key, Value, Hash>::find(const Key& key)
{
	return const_cast<Data<Key,Value>*>(std::as_const(*this).find(key));
}


template<class Key, class Value, class Hash>
inline typename Data<Key, Value>* HashMap<Key, Value, Hash>::add(const Data<Key,Value>& pair)
{
	size_t index = this->hash(pair.word);

	hArr[index].push_front(pair);
	return &hArr[index].front();
}

