#include "../json/json.h"
#include "../json/parser.h"
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

int main(int argc, char *argv[]) {
  cout << "======= constructor =======" << endl;
  iaee::json::Json obj1(123);
  iaee::json::Json obj2(true);
  iaee::json::Json obj3(1.23);
  iaee::json::Json obj4("hhhhhhhh");

  iaee::json::Json obj5(obj1);
  iaee::json::Json obj6;
  obj6 = obj2;

  cout << obj1.toString() << endl;
  cout << obj2.toString() << endl;
  cout << obj3.toString() << endl;
  cout << obj4.toString() << endl;
  cout << obj5.toString() << endl;
  cout << obj6.toString() << endl;

  cout << "======= Equal sign operator overload =======" << endl;
  iaee::json::Json a(true);
  cout << a.toString() << endl;
  a = false;
  cout << a.toString() << endl;

  iaee::json::Json b(3424);
  cout << b.toString() << endl;
  b = 2343;
  cout << b.toString() << endl;

  iaee::json::Json c(1.23);
  cout << c.toString() << endl;
  c = 3.21;
  cout << c.toString() << endl;

  iaee::json::Json d("sfhal");
  cout << d.toString() << endl;
  // 这里的这个字符串是一个char*类型，在GNU g++ 编译器下会给警告
  d = "fhklasjf";
  cout << d.toString() << endl;

  iaee::json::Json aa;
  iaee::json::Json bb;
  iaee::json::Json cc;
  iaee::json::Json dd;

  aa = true;
  bb = 333;
  cc = 3.3;
  dd = "sdfhalk";
  cout << aa.toString() << endl;
  cout << bb.toString() << endl;
  cout << cc.toString() << endl;
  cout << dd.toString() << endl;
  // aa = 333; // bool->int error

  cout << "======= Data type overload =======" << endl;

  bool flag = a;
  cout << flag << endl;
  int integer = b;
  cout << integer << endl;
  double dbl = c;
  cout << dbl << endl;
  string str = d;
  cout << str << endl;

  cout << "======= Bracket operator overloading =======" << endl;

  iaee::json::Json arr;
  arr[0] = true;
  arr[1] = 123;
  arr[2] = 1.23;
  arr[3] = "fasf";
  arr[4] = iaee::json::Json(321);
  for (int i = 0; i < 5; ++i) {
    cout << arr[i].toString() << endl;
  }

  cout << "======= Identical to and not equal to operator overloading ======="
       << endl;

  iaee::json::Json iobj1(true);
  iaee::json::Json iobj2(123);
  iaee::json::Json iobj3(1.23);
  iaee::json::Json iobj4("cccc");
  iaee::json::Json iobj5(iobj1);

  if (iobj1 == iobj5) {
    cout << "iobj1 == iobj5" << endl;
  }
  if (iobj1 != iobj5) {
    cout << "iobj1 != iobj5" << endl;
  }
  if (iobj2 == iobj4) {
    cout << "iobj2 == iobj4" << endl;
  }
  if (iobj2 != iobj4) {
    cout << "iobj2 != iobj4" << endl;
  }
  if (iobj3 == 1.23) {
    cout << "The value of iobj3 object is '1.23'" << endl;
  }
  if (iobj3 != 1.23) {
    cout << "The value of iobj3 object is not '1.23'" << endl;
  }
  if (iobj3 != 2.0) {
    cout << "The value of iobj3 object is not '2.0'" << endl;
  }

  cout << "======= public function =======" << endl;
  iaee::json::Json pfArr;
  pfArr.append(iaee::json::Json(true));
  pfArr.append(iaee::json::Json(123));
  pfArr.append(iaee::json::Json("qwertyuiop"));
  cout << pfArr.toString() << endl;
  cout << pfArr[2].toString() << endl;

  iaee::json::Json pfObj;
  pfObj["bool"] = true;
  pfObj["int"] = 999;
  pfObj["double"] = 9.99;
  pfObj["string"] = "asdfghjkl";
  pfObj["arr"] = pfArr;
  pfObj["obj"] = pfObj;
  cout << pfObj.toString() << endl;

  pfArr.remove(1);
  pfObj.remove("obj");
  cout << pfArr.toString() << endl;
  cout << pfObj.toString() << endl;

  pfObj.pformat();

  if (pfArr.isArray()) {
    cout << "pfArr is an array type obj" << endl;
  }
  if (!pfArr.isObject()) {
    cout << "pfArr is not an array type obj" << endl;
  }

  iaee::json::Json pfInt(123);
  iaee::json::Json pfString("fdsa");
  cout << pfInt.asInt() << endl;
  cout << pfString.asString() << endl;

  if (!pfObj.empty() && !pfArr.empty()) {
    int na = pfArr.size();
    int no = pfObj.size();
    cout << "pfArr size: " << na << endl;
    cout << "pfObj size: " << no << endl;
  }

  if (pfArr.has(1)) {
    cout << "pfArr has an index of size 1" << endl;
  }
  if (!pfArr.has(8)) {
    cout << "pfArr does not have an index of size 8" << endl;
  }

  ifstream fin("./test.json");
  stringstream os;
  os << fin.rdbuf();
  const std::string parserStr = os.str();

  iaee::json::Json hh;
  hh.parser(parserStr);
  hh.pformat();
  return 0;
}
