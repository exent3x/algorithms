#include <iostream>
#include <vector>
int search(std::vector<int> a, size_t left, size_t right, int number)
{
    int result;
    size_t mid = (left + right) / 2;
    if (number == a[mid])
    {
        return mid;
    }
    if (right < left)
        return -1;

    if (a[left] < a[mid])
    {
        if (number >= a[left] && number < a[mid])
            return search(a, left, mid - 1, number);
        else
            return search(a, mid + 1, right, number);

    }
    else if (a[left] > a[mid])
    {
        if (number > a[mid] && number <= a[right])
            return search(a, mid + 1, right, number);
        else
            return search(a, left, mid - 1, number);

    }
    else if (a[left] == a[mid])
        if (mid != a[right])
            return search(a, mid + 1, right, number);
        else
        {
            result = search(a, left, mid - 1, number);
            if (result == -1)
                result = search(a, mid + 1, right, number);
            else
                return result;
        }


    return -1;


}

int main() {
    
    std::vector<int> vec = { 7,8,9,1,2,3,4,5,6 };
    std::cout << search(vec, 0, vec.size()-1, 3);
    return 0;
}