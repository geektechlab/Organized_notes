/*******************************************************
 * C++ STL MASTER FILE (Apple / Arm UK Interview Level)
 * Covers:
 * - Templates (Generic programming)
 * - Containers (array, vector, list, map)
 * - Algorithms
 * - Iterators
 * - Pair, Tuple
 * - String (C-style vs std::string)
 * - All member functions from notes
 * - Edge cases + interview caveats
 *******************************************************/

#include <iostream>
#include <vector>
#include <list>
#include <array>
#include <map>
#include <tuple>
#include <algorithm>   // sort, reverse, find
#include <iterator>
#include <cstring>     // strcpy, strcmp
#include <string>

using namespace std;

/*******************************************************
 * 1. TEMPLATE BASICS (as used in STL)
 *******************************************************/
template <typename T>
class MyContainer {
public:
    T data;
    MyContainer(T val) : data(val) {}

    void print() {
        cout << "Template value: " << data << endl;
    }
};

/*******************************************************
 * 2. ARRAY (std::array) — FIXED SIZE
 *******************************************************/
void array_demo() {
    cout << "\n===== ARRAY DEMO =====\n";

    array<int, 4> arr = {2, 4, 6, 8};

    // Access
    cout << "arr[2]: " << arr[2] << endl;
    cout << "arr.at(2): " << arr.at(2) << endl;

    // at() throws exception if out of range
    try {
        cout << arr.at(10) << endl;
    } catch (out_of_range &e) {
        cout << "Exception: " << e.what() << endl;
    }

    // front, back
    cout << "front: " << arr.front() << endl;
    cout << "back: " << arr.back() << endl;

    // size
    cout << "size: " << arr.size() << endl;

    // fill
    arr.fill(1);

    // swap
    array<int, 4> arr2 = {9, 9, 9, 9};
    arr.swap(arr2);

    // iterators
    for (auto it = arr.begin(); it != arr.end(); ++it)
        cout << *it << " ";
    cout << endl;
}

/*******************************************************
 * 3. VECTOR — DYNAMIC ARRAY
 *******************************************************/
void vector_demo() {
    cout << "\n===== VECTOR DEMO =====\n";

    vector<int> v;

    // capacity growth (IMPORTANT INTERVIEW POINT)
    cout << "Initial capacity: " << v.capacity() << endl;

    v.push_back(10);
    v.push_back(20);
    v.push_back(30);

    cout << "Capacity after push: " << v.capacity() << endl;

    // Access
    cout << v[1] << " " << v.at(1) << endl;

    // pop_back
    v.pop_back();

    // size vs capacity
    cout << "Size: " << v.size() << endl;
    cout << "Capacity: " << v.capacity() << endl;

    // clear (does NOT reduce capacity)
    v.clear();

    // insert
    v = {1, 2, 3, 4};
    auto it = v.begin();
    v.insert(it + 2, 99);

    // iterators
    for (auto x : v) cout << x << " ";
    cout << endl;

    /***** IMPORTANT CAVEATS *****/
    // - vector reallocation invalidates ALL iterators
    // - push_back may trigger reallocation
    // - contiguous memory (cache friendly)
}

/*******************************************************
 * 4. LIST — DOUBLY LINKED LIST
 *******************************************************/
void list_demo() {
    cout << "\n===== LIST DEMO =====\n";

    list<int> l = {11, 22, 33};

    l.push_back(44);
    l.push_front(5);

    l.pop_back();
    l.pop_front();

    // traversal
    for (auto x : l) cout << x << " ";
    cout << endl;

    // sort (list has its own sort)
    l.sort();

    // reverse
    l.reverse();

    // remove value
    l.remove(22);

    // size
    cout << "Size: " << l.size() << endl;

    /***** IMPORTANT CAVEATS *****/
    // - No random access (no l[2])
    // - Iterator stability (better than vector)
}

/*******************************************************
 * 5. MAP — ASSOCIATIVE CONTAINER
 *******************************************************/
void map_demo() {
    cout << "\n===== MAP DEMO =====\n";

    map<int, string> m;

    // insert
    m[100] = "abc";
    m[200] = "xyz";

    // insert via pair
    m.insert({300, "pqr"});

    // access
    cout << m[100] << endl;

    // at() safer
    try {
        cout << m.at(999) << endl;
    } catch (...) {
        cout << "Key not found\n";
    }

    // iteration (sorted by key)
    for (auto &p : m) {
        cout << p.first << " -> " << p.second << endl;
    }

    // size
    cout << "Size: " << m.size() << endl;

    // erase
    m.erase(100);

    /***** IMPORTANT CAVEATS *****/
    // - keys are unique
    // - sorted (RB-tree internally)
    // - O(log n) operations
}

/*******************************************************
 * 6. PAIR
 *******************************************************/
void pair_demo() {
    cout << "\n===== PAIR DEMO =====\n";

    pair<string, int> p1 = make_pair("Rahul", 6);

    cout << p1.first << " " << p1.second << endl;

    // comparison
    pair<int, int> a = {1, 2};
    pair<int, int> b = {1, 3};

    cout << (a < b) << endl;  // lexicographical
}

/*******************************************************
 * 7. TUPLE
 *******************************************************/
void tuple_demo() {
    cout << "\n===== TUPLE DEMO =====\n";

    tuple<string, int, int> t1 = make_tuple("India", 16, 10);

    cout << get<0>(t1) << endl;
    cout << get<1>(t1) << endl;
    cout << get<2>(t1) << endl;
}

/*******************************************************
 * 8. ALGORITHMS
 *******************************************************/
void algorithm_demo() {
    cout << "\n===== ALGORITHM DEMO =====\n";

    vector<int> v = {5, 2, 8, 1};

    // sort
    sort(v.begin(), v.end());

    // reverse
    reverse(v.begin(), v.end());

    // binary search (requires sorted array)
    sort(v.begin(), v.end());
    bool found = binary_search(v.begin(), v.end(), 5);

    cout << "Found 5? " << found << endl;

    /***** IMPORTANT CAVEATS *****/
    // - binary_search requires sorted container
    // - algorithms operate via iterators
}

/*******************************************************
 * 9. ITERATORS (BRIDGE between container & algorithm)
 *******************************************************/
void iterator_demo() {
    cout << "\n===== ITERATOR DEMO =====\n";

    vector<int> v = {1, 2, 3};

    vector<int>::iterator it;

    for (it = v.begin(); it != v.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;

    /***** KEY INSIGHT *****/
    // sort(begin, end) → works for ANY container
}

/*******************************************************
 * 10. C-STYLE STRING vs std::string
 *******************************************************/
void string_demo() {
    cout << "\n===== STRING DEMO =====\n";

    /***** C-STYLE STRING *****/
    char s1[10] = "Hello";
    char s2[10];

    strcpy(s2, s1);
    cout << strcmp(s1, s2) << endl;

    /***** PROBLEM *****/
    // No bounds checking → buffer overflow risk

    /***** std::string *****/
    string str1 = "Hello";
    string str2 = "World";

    // concatenation
    string str3 = str1 + " " + str2;

    // size
    cout << str3.size() << endl;

    // find
    cout << str3.find("World") << endl;

    // replace
    str3.replace(0, 5, "Hi");

    // insert
    str3.insert(2, "123");

    // erase
    str3.erase(2, 3);

    cout << str3 << endl;

    /***** IMPORTANT CAVEATS *****/
    // - std::string is safe (RAII)
    // - supports operator overloading
    // - C-style faster but unsafe
}

/*******************************************************
 * MAIN
 *******************************************************/
int main() {

    cout << "===== STL MASTER PROGRAM =====\n";

    // Template
    MyContainer<int> obj(10);
    obj.print();

    array_demo();
    vector_demo();
    list_demo();
    map_demo();
    pair_demo();
    tuple_demo();
    algorithm_demo();
    iterator_demo();
    string_demo();

    return 0;
}