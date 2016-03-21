#include <iostream>
#include "vector_ops.hpp"

using namespace std;

int main(int argc, char const *argv[]) {
  vector<int> test_vector;
  test_vector = linspace(1, 35, 22);
  cout << slice(test_vector,1,3) << endl;
  auto v = find(test_vector, 4);
  cout << v << endl;
  return 0;
}
