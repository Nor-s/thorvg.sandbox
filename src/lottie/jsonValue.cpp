#include "jsonValue.h"

void value(std::stringstream& sst, const MapValue& mapValue)
{
	size_t count = 0;
	size_t total = mapValue.size();

	sst << "{";
	for (const auto& [key, valPtr] : mapValue)
	{
		bool isEnd = (++count == total);
		property(sst, key, *valPtr, isEnd);
	}
	sst << "}";
}
void value(std::stringstream& sst, const ArrayValue& arr)
{
	size_t count = 0;
	size_t total = arr.size();

	sst << "[";
	for (auto& v : arr)
	{
		bool isEnd = (++count == total);
		visitValue(sst, *v.get(), isEnd);
	}
	sst << "]";
}