#include <iostream>
#include <vector>
#include <algorithm>

#include "omp.h"

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T> v){
  out << "{";
  for (auto iter = v.begin(); iter != v.end(); iter++){
    out << *iter << " ";
  }
  out << "}";
  return out;
}

template<typename T>
std::vector<T> linspace(T start_in, T end_in, int num_in){
  double start = static_cast<double>(start_in);
  double end = static_cast<double>(end_in);
  double num = static_cast<double>(num_in);
  double delta = (end - start) / (num - 1);

  std::vector<T> linspaced(num - 1);

  for (int i = 0; i < num; ++i){
    linspaced[i] = start + delta * i;
  }
  linspaced.push_back(end);
  return linspaced;
}

template<typename T>
std::vector<T> slice(std::vector<T> vector, int begin, int end){
  std::vector<T> ret;
  for (int i = begin; i < end; i++){
    ret.push_back(vector[i]);
  }
  return ret;
}

template<typename T>
std::vector<T> absolute_value(std::vector<T> v){
  int n = v.size();
  std::vector<T> out (n);
  for (int i = 0; i < n; i++){
    out[i] = std::abs(v[i]);
  }
  return out;
}

template<typename T>
int find(std::vector<T> vect, T target){
  int n = vect.size();
  int index, number = 0;
  std::sort(vect.begin(), vect.end());
  if (n == 0){
    return -1;
  }
  else{
    int middle = n/2;

    if (target == vect[middle]){
      return middle;
    }
    else if (target < vect[middle]){
      std::vector<T> new_vect = slice(vect, 0, middle);
      return find(new_vect, target);
    }
    else if (target > vect[middle]){
      std::vector<T> new_vect = slice(vect, middle, n-1);
      return find(new_vect, target);
    }
    else{
      return -1;
    }
  }
}
