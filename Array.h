#include <algorithm>

struct vec2
{
	int x;
	int y;
	vec2(int _x = 0, int _y = 0) : x(_x), y(_y) {}
	vec2& operator=(vec2 rhs) {
		x = rhs.x;
		y = rhs.y;
		return *this;
	}
};

//important to have arrays that are not fragmented
class Array {
	public:
	void push(int x, int y)
	{
		//we will segfault immediately if maxIdx is greater than cMaxArray
		//good thing we will make sure that it is impossible for the player to
		//create more than 128 objects in the time they have
		positions[maxIdx].x = x;
		positions[maxIdx].y = y;
		maxIdx++;
	}
	
	// this just swaps with the back end and decrements
	// so it is important to keep that in mind during the for loop
	void erase(uint32_t idx)
	{
		maxIdx--;
		std::swap(positions[idx], positions[maxIdx]);
	}
	
	vec2& operator[](uint32_t idx)
	{
		return positions[idx];
	}
	
	inline uint32_t getMaxIdx() const { return maxIdx; }
	private:
	static constexpr uint32_t cMaxArray = 128;
	vec2 positions[cMaxArray];
	uint32_t maxIdx;
};