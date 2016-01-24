/*
	第一次使用 VS2015
*/

#include <iostream>
#include <cmath>
#include <string>
#include <iostream>
#include <vector>
#include <map>

struct vec
{
	double x, y;

	vec() {}
	vec(double x, double y)
	:x(x), y(y)
	{}

	double length()const
	{
		return sqrt(x*x + y*y);
	}
};

double angle_of_vec(const vec& a, const vec& b)
{
	double angle = acos( (a.x*b.x + a.y*b.y) / ( a.length()*b.length() ) );
	return angle * 180 / 3.14159265359;
}

int main()
{
	vec a(0, 100), b(-0.13,30.2);
	std::cout << b.length()<<' ' << angle_of_vec(b, a);

	//std::vector<std::string> words = {
	//	"this", "sentence", "is", "not", "a", "sentence",
	//	"this", "sentence", "is", "a", "hoax"
	//};

	//std::map<std::string, size_t>  word_map;
	//for ( auto &w : words) {
	//	++word_map[w];
	//}

	//for (const auto &pair : word_map) {
	//	std::cout << pair.second
	//		<< " occurrences of word '"
	//		<< pair.first << "'\n";
	//}
}