#ifndef __GUYOS__MEMORY_H
#define __GUYOS__MEMORY_H
/*

RAM structure:
[video memory|GRUB|kernel|stack?| free space]

MemoryChunck 
MemoryChunck next, prev-> points to the next and the previous MemoryChunck
bool allocated
size_t size

*/

#include <common/types.h>


namespace guyos
{
    
    struct MemoryChunk
    {
        MemoryChunk *next;
        MemoryChunk *prev;
        bool allocated;
        common::size_t size;
    };
    
    
    class MemoryManager
    {
        
    protected:
        MemoryChunk* first;
    public:
        
        static MemoryManager *activeMemoryManager;
        
        MemoryManager(common::size_t first, common::size_t size);
        ~MemoryManager();
        
        void* malloc(common::size_t size);
        void free(void* ptr);
    };
}


void* operator new(unsigned size);
void* operator new[](unsigned size);

// placement new
void* operator new(unsigned size, void* ptr);
void* operator new[](unsigned size, void* ptr);

void operator delete(void* ptr);
void operator delete[](void* ptr);


#endif