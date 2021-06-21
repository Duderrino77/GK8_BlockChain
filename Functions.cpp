#include <iostream>
#include <array>
#include <initializer_list>

//An implementation of sum function getting unknown number of parameters.
int32_t sum(std::initializer_list<int32_t> a_args)
{
    int32_t iSum = 0;
    for (auto i : a_args)
        iSum += i;

    return iSum;
}

//An implementation of a template function that returns manipulated std::array
constexpr auto GetArraySize(std::size_t size)
{
    return size / 2;
}

template <int N>
std::array<int32_t, GetArraySize(N)> sum_array(const std::array<int32_t, N>& numArray)
{
    std::array<int32_t, GetArraySize(N)> retArray{};

    for (int i = 0; i < retArray.size(); ++i)
        retArray[i] = numArray[2 * i] + numArray[2 * i + 1];
       
   
    return retArray;
}

int main() 
{
  
   // std::cout << sum({ 11, -22, 11, 3 });

    
    std::array<int, 6> arr = {1, 2, 3, 4, 5, 6};

    sum_array(arr);

    for (auto i : sum_array(arr))
        std::cout << i << " ";
    
    return 0;
}