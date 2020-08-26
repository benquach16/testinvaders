#pragma once

#include <algorithm>

// Array class that enforces contiguous memory, but is not stable
// Do not store indices from this class
template<class T>
class Array {
	public:
	void push(T item)
	{
		// we will segfault immediately if maxIdx is greater than cMaxArray
		// game design must assure that more than cMaxArray objects is impossible.
		#if defined(DEBUG)
		assert(maxIdx < cMaxArray);
		#endif
		data[maxIdx] = item;
		maxIdx++;
	}
	
	// this just swaps with the back end and decrements
	// so it is important to keep that in mind during the for loop
	void erase(uint32_t idx)
	{
		#if defined(DEBUG)
		assert(idx >= 0 && idx < cMaxArray);
		assert(maxIdx>0);
		#endif
		maxIdx--;
		std::swap(data[idx], data[maxIdx]);
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
	uint32_t maxIdx = 0;
};