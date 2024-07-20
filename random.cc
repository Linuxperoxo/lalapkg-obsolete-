#include <random>

template <typename T>

T randomNum(T min, T max){

  std::random_device rd;
  std::mt19937 gen(rd());

  std::uniform_int_distribution<> distrib(min, max);

  return distrib(gen);

}
