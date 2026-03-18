# DataStructure

This is a header file library written by myself.

A C++ template library implementing common data structures.

## 📦 Components

- `List.h` - Doubly linked list with bidirectional iterator
- (More data structures coming soon...)

## 🚀 Quick Start

```cpp
#include "List.h"
#include <iostream>

int main() {
    List<int> list;
    
    // Insert elements
    list.push_back(10);
    list.push_back(20);
    list.push_front(5);
    
    // Iterate
    for (auto it = list.begin(); it != list.end(); ++it) {
        std::cout << *it << " ";
    }
    // Output: 5 10 20
    
    return 0;
}
