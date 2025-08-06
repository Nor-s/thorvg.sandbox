#ifndef _SAVER_JSON_VALUE_H_
#define _SAVER_JSON_VALUE_H_

#include "orderedMap.h"

#include <sstream>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <variant>
#include <string>

#include <thorvg.h>
#include <tvgLottieData.h>


struct BoolValue
{
	bool value;
};

#define VAL std::make_unique<Value>
#define VALMOV(a) VAL(std::move(a))

struct Value;
using MapValue = OrderedMap<std::unique_ptr<Value>>;
using ArrayValue = std::vector<std::unique_ptr<Value>>;

struct Value
{
	using ValueType = std::variant<int, bool, float, RGB24, tvg::Point, std::string, MapValue, ArrayValue>;
	ValueType data;

	// Value(const ValueType& v) : data(v) {}
};

static void value(std::stringstream& sst, int v)
{
	sst << v;
}
static void value(std::stringstream& sst, float v)
{
	sst << v;
}
static void value(std::stringstream& sst, bool v)
{
	sst << (v ? "true" : "false");
}
static void value(std::stringstream& sst, const std::string& str)
{
	sst << "\"" << str << "\"";
}
static void value(std::stringstream& sst, const RGB24& v)
{
	sst << "[" << v.rgb[0]/255.0 << "," << v.rgb[1]/255.0 << "," << v.rgb[2]/255.0 << "]";
}
static void value(std::stringstream& sst, const tvg::Point& v)
{
	sst << "[" << v.x << "," << v.y << "]";
}
void value(std::stringstream& sst, const MapValue& arr);
void value(std::stringstream& sst, const ArrayValue& arr);

static void separate(std::stringstream& sst)
{
	sst << ",";
}
static void propertyName(std::stringstream& sst, const std::string& str)
{
	value(sst, {str});
	sst << ":";
}

static void visitValue(std::stringstream& sst, const Value& val, bool isEnd)
{
	std::visit([&](auto&& v) { value(sst, v); }, val.data);
	if (isEnd == false)
		separate(sst);
}

static void property(std::stringstream& sst, const std::string& name, const Value& val, bool isEnd = false)
{
	propertyName(sst, name);
	visitValue(sst, val, isEnd);
}

#endif