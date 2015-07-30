
#include <iostream>
#include <string>
#include <functional>
#include <algorithm>



int main () {
	std::function <const size_t& (const size_t&, const size_t&)> ff;
	const size_t& (*p)(const size_t&, const size_t&);
	
	//ff = std::max<size_t>; // error compile
	p = std::max<size_t>;
	
	
	return 0;
}
