#include "pch.h"
#include "CppUnitTest.h"
#include "../SpecialHashMap/SpecialHashMap.h"
#include <string>
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(iloc)
	{
	public:
		TEST_METHOD(test_default)
		{
			SpecialHashMap<int> map;
			map["value1"] = 1;
			map["value2"] = 2;
			map["value3"] = 3;
			map["1"] = 10;
			map["2"] = 20;
			map["3"] = 30;
			map["1, 5"] = 100;
			map["5, 5"] = 200;
			map["10, 5"] = 300;

			map.toSortedSet();
			Assert::AreEqual(10, map.iloc(0));
			Assert::AreEqual(300, map.iloc(2));
			Assert::AreEqual(200, map.iloc(5));
			Assert::AreEqual(3, map.iloc(8));
		}
		TEST_METHOD(test_notSorted)
		{
			SpecialHashMap<int> map;
			map["value1"] = 1;
			map["3"] = 30;
			auto er = [&] { map.iloc(0); };
			Assert::ExpectException<std::exception>(er);
		}
		TEST_METHOD(test_indexOutOfRange)
		{
			SpecialHashMap<int> map;
			map["value1"] = 1;
			map["3"] = 30;
			map.toSortedSet();
			Assert::AreEqual(30, map.iloc(0));
			Assert::AreEqual(1, map.iloc(1));
			auto er = [&] { map.iloc(2); };
			Assert::ExpectException<std::exception>(er);
		}
	};
	TEST_CLASS(ploc)
	{
	public:
		TEST_METHOD(test_default)
		{
			SpecialHashMap<int> map;
			map["value1"] = 1;
			map["value2"] = 2;
			map["value3"] = 3;
			map["1"] = 10;
			map["2"] = 20;
			map["3"] = 30;
			map["(1, 5)"] = 100;
			map["(5, 5)"] = 200;
			map["(10, 5)"] = 300;
			map["(1, 5, 3)"] = 400;
			map["(5, 5, 4)"] = 500;
			map["(10, 5, 5)"] = 600;

			SpecialHashMap<int> map1 = map.ploc(">=1");
			SpecialHashMap<int> map2 = map.ploc("<3");
			SpecialHashMap<int> map3 = map.ploc(">0, >0");
			SpecialHashMap<int> map4 = map.ploc(">=10, >0");
			SpecialHashMap<int> map5 = map.ploc("<5, >=5, >=3");

			std::string s;
			std::stringstream ss;
			std::string s1("{1=10, 2=20, 3=30}");
			std::string s2("{1=10, 2=20}");
			std::string s3("{(1, 5)=100, (5, 5)=200, (10, 5)=300}");
			std::string s4("{(10, 5)=300}");
			std::string s5("{(1, 5, 3)=400}");

			ss << map1;
			getline(ss, s);
			ss.clear();
			Assert::AreEqual(s1, s);
			ss << map2;
			getline(ss, s);
			ss.clear();
			Assert::AreEqual(s2, s);
			ss << map3;
			getline(ss, s);
			ss.clear();
			Assert::AreEqual(s3, s);
			ss << map4;
			getline(ss, s);
			ss.clear();
			Assert::AreEqual(s4, s);
			ss << map5;
			getline(ss, s);
			ss.clear();
			Assert::AreEqual(s5, s);
		}
		TEST_METHOD(test_advanced)
		{
			SpecialHashMap<int> map;
			map["value1"] = 1;
			map["value2"] = 2;
			map["value3"] = 3;
			map["1"] = 10;
			map["2"] = 20;
			map["3gfd"] = 30;
			map["(1, 5fsd)"] = 100;
			map["(5, 5)"] = 200;
			map["(10, 5)"] = 300;
			map["(    1,  5    ,3)"] = 400;
			map["(5, 5, 4, 14942.534"] = 500;
			map["(10, 5, 5)"] = 600;
			map["14.4234, 59r.543534"] = 14234;
			map["(     9.55.5, 12.8,   9.6"] = 700; // this'd be read as (9.55, 5, 12.8, 9.6)

			auto er = [&] { SpecialHashMap<int> map1 = map.ploc(""); };
			Assert::ExpectException<std::exception>(er);

			SpecialHashMap<int> map2 = map.ploc("<3.1 <153459 <=999.99");
			SpecialHashMap<int> map3 = map.ploc("5   =5   ==    4    <> 1");
			SpecialHashMap<int> map4 = map.ploc("    trhgudrfgdfoigh soghgshfug sdf < gfdosghortu 10 fdgkjhjgfsdg ==5 > 0");
			SpecialHashMap<int> map5 = map.ploc("(>1;>1;>1)");
			SpecialHashMap<int> map6 = map.ploc(">0 >0 <>0 >0");
			SpecialHashMap<int> map7 = map.ploc(">=4.99 >=5");

			std::string s;
			std::stringstream ss;
			std::string s2("{(    1,  5    ,3)=400}");
			std::string s3("{(5, 5, 4, 14942.534=500}");
			std::string s4("{(10, 5, 5)=600}");
			std::string s5("{(10, 5, 5)=600}");
			std::string s6("{(5, 5, 4, 14942.534=500, (     9.55.5, 12.8,   9.6=700}");
			std::string s7("{(5, 5)=200, (10, 5)=300}");

			ss << map2;
			getline(ss, s);
			ss.clear();
			Assert::AreEqual(s2, s);
			ss << map3;
			getline(ss, s);
			ss.clear();
			Assert::AreEqual(s3, s);
			ss << map4;
			getline(ss, s);
			ss.clear();
			Assert::AreEqual(s4, s);
			ss << map5;
			getline(ss, s);
			ss.clear();
			Assert::AreEqual(s5, s);
			ss << map6;
			getline(ss, s);
			ss.clear();
			Assert::AreEqual(s6, s);
			ss << map7;
			getline(ss, s);
			ss.clear();
			Assert::AreEqual(s7, s);
		}
	};
}
