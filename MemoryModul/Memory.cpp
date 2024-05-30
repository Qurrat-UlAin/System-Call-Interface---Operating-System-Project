//Contributed by Sana: 
#include <iostream>
#include <vector>
#include <algorithm> // this header for sort
#include <cstdlib>
#include <cstdio> // this header for C-style I/O functions if needed

using namespace std;

class MemoryPool {
public:
    MemoryPool(size_t size) : poolSize(size), pool(new char[size]) {
        freeList.push_back({ 0, size });
    }

    ~MemoryPool() {
        delete[] pool;
    }

    void* customMalloc(size_t size);
    void customFree(void* ptr);

private:
    struct Block {
        size_t start;
        size_t size;
    };

    size_t poolSize;
    char* pool;
    vector<Block> freeList;

    void mergeFreeBlocks();
};

void MemoryPool::mergeFreeBlocks() {
    sort(freeList.begin(), freeList.end(), [](const Block& a, const Block& b) {
        return a.start < b.start;
        });

    for (auto it = freeList.begin(); it != freeList.end() - 1; ++it) {
        auto nextIt = it + 1;
        if (it->start + it->size == nextIt->start) {
            it->size += nextIt->size;
            freeList.erase(nextIt);
            --it;
        }
    }
}

void* MemoryPool::customMalloc(size_t size) {
    for (auto it = freeList.begin(); it != freeList.end(); ++it) {
        if (it->size >= size) {
            size_t start = it->start;
            if (it->size > size) {
                it->start += size;
                it->size -= size;
            }
            else {
                freeList.erase(it);
            }
            return pool + start;
        }
    }
    return nullptr; // Out of memory
}

void MemoryPool::customFree(void* ptr) {
    size_t start = static_cast<char*>(ptr) - pool;
    freeList.push_back({ start, poolSize - start });
    mergeFreeBlocks();
}

int main() {
    MemoryPool memoryPool(1024); // 1 KB memory pool

    void* ptr1 = memoryPool.customMalloc(128); // Allocate 128 bytes
    void* ptr2 = memoryPool.customMalloc(256); // Allocate 256 bytes

    cout << "ptr1: " << ptr1 << endl;
    cout << "ptr2: " << ptr2 << endl;

    memoryPool.customFree(ptr1); // Free the 128 bytes block
    memoryPool.customFree(ptr2); // Free the 256 bytes block

    return 0;
}
