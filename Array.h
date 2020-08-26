#pragma once

#include <algorithm>

//important to have arrays that are not fragmented
template<class T>
class Array {
	public:
	uint32_t push(T item)
	{
		//we will segfault immediately if maxIdx is greater than cMaxArray
		//game design must assure that more than 128 objects is impossible.
		#if defined(DEBUG)
		assert(maxIdx < cMaxArray);
		#endif
		data[maxIdx] = item;
		uint32_t idx = maxIdx;
		maxIdx++;
		return idx;
	}
	
	// this just swaps with the back end and decrements
	// so it is important to keep that in mind during the for loop
	uint32_t erase(uint32_t idx)
	{
		#if defined(DEBUG)
		assert(idx >= 0 && idx < cMaxArray);
		assert(maxIdx>0);
		#endif
		maxIdx--;
		std::swap(data[idx], data[maxIdx]);
		return idx;
	}
	
	void clear()
	{
		maxIdx = 0;
	}
	
	T& operator[](uint32_t idx)
	{
		#if defined(DEBUG)
		assert(idx >= 0 && idx < cMaxArray);
		#endif	
		return data[idx];
	}

	T operator[](uint32_t idx) const
	{
		#if defined(DEBUG)
		assert(idx >= 0 && idx < cMaxArray);
		#endif
		return data[idx];
	}
	
	inline uint32_t getMaxIdx() const { return maxIdx; }
	private:
	static constexpr uint32_t cMaxArray = 128;
	T data[cMaxArray];
	uint32_t maxIdx;
};