#include <chrono>

class ArtificialClock
{
public:
	using duration = std::chrono::duration<uint64_t, std::ratio<1, 1000000>>;
	using time_point = std::chrono::time_point<ArtificialClock>;

	static void advance(duration duration) noexcept { nowMicroseconds += duration; }
	static time_point now() noexcept { return nowMicroseconds; }

private:
	static time_point nowMicroseconds;
};
