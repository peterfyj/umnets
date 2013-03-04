#include "util/Math.h"
#include <cstdlib>
#include <utility>
#include <algorithm>

using namespace std;

bool Math::happen(int numerator, int denominator) {
  uniform_int_distribution<int> prob(0, denominator - 1);
  return prob(engine) < numerator;
}

bool Math::happen(double prob) {
  return prob_distribution(engine) < prob;
}

int Math::get_random_int(int start, int end) {
  uniform_int_distribution<int> prob(start, end);
  return prob(engine);
}

double Math::get_random_probability() {
  return prob_distribution(engine);
}

vector<int> Math::get_random_derangement(int size) {
  vector<int> ret(size);
  do {
    for (int i = 0; i < size; ++i) {
      ret[i] = i;
    }
    shuffle(ret.begin(), ret.end(), engine);
  } while(!is_derangement(ret));
  return ret;
}

bool Math::is_derangement(vector<int>& v) {
  for (int i = 0; i < (int) v.size(); ++i) {
    if (v[i] == i) {
      return false;
    }
  }
  return true;
}

default_random_engine Math::engine(random_device().operator()());
uniform_real_distribution<double> Math::prob_distribution(0, 1);

