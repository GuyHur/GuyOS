
#include <memory.h>

using namespace guyos;
using namespace guyos::common;


MemoryManager* MemoryManager::activeMemoryManager = 0;
        
MemoryManager::MemoryManager(size_t start, size_t size)
{
    activeMemoryManager = this;//this object is the current active memory manager
    
    if(size < sizeof(MemoryChunk))// size isn't sufficient, could overwrite other things so don't allow it
    {
        first = 0;
    }
    else
    {
        first = (MemoryChunk*)start;//the first memory chunck
        
        first -> allocated = false;//first chunck isn't allocated
        first -> prev = 0;// doesn't have a previous chunck
        first -> next = 0;// or a next one
        first -> size = size - sizeof(MemoryChunk);// size 
    }
}

MemoryManager::~MemoryManager()
{
    if(activeMemoryManager == this)
        activeMemoryManager = 0;// deactivate the memory manager
}
        
void* MemoryManager::malloc(size_t size)
{
    MemoryChunk *result = 0;
    //looping over a linked list
    for(MemoryChunk* chunk = first; chunk != 0 && result == 0; chunk = chunk->next)
        if(chunk->size > size && !chunk->allocated)
            result = chunk;//when we find an area that is free and it is large enough
        
    if(result == 0)
        return 0;// found nothing
    
    //if we are here then we must of found an area.
    if(result->size >= size + sizeof(MemoryChunk) + 1)//memory large enough to split
    {
        MemoryChunk* temp = (MemoryChunk*)((size_t)result + sizeof(MemoryChunk) + size);
        
        temp->allocated = false;
        temp->size = result->size - size - sizeof(MemoryChunk);//new chunck would get the size of the old one + the requested one
        temp->prev = result;
        temp->next = result->next;
        if(temp->next != 0)// if theres something
            temp->next->prev = temp;// previous pointer = temp
        
        result->size = size;
        result->next = temp;
    }
    
    result->allocated = true;// memory allocated.
    return (void*)(((size_t)result) + sizeof(MemoryChunk));// return a pointer to the beginning of the allocated memory.
}

void MemoryManager::free(void* ptr)
{
    MemoryChunk* chunk = (MemoryChunk*)((size_t)ptr - sizeof(MemoryChunk));//get a pointer to the previous chunck so we can link
    
    chunk -> allocated = false;
    //merging with previous chunck
    if(chunk->prev != 0 && !chunk->prev->allocated)
    {
        chunk->prev->next = chunk->next;
        chunk->prev->size += chunk->size + sizeof(MemoryChunk);
        if(chunk->next != 0)
            chunk->next->prev = chunk->prev;
        
        chunk = chunk->prev;
    }
    //merging with next one
    if(chunk->next != 0 && !chunk->next->allocated)
    {
        chunk->size += chunk->next->size + sizeof(MemoryChunk);
        chunk->next = chunk->next->next;
        if(chunk->next != 0)
            chunk->next->prev = chunk;
    }
    
}




void* operator new(unsigned size)
{
    if(guyos::MemoryManager::activeMemoryManager == 0)
        return 0;
    return guyos::MemoryManager::activeMemoryManager->malloc(size);
}

void* operator new[](unsigned size)
{
    if(guyos::MemoryManager::activeMemoryManager == 0)
        return 0;
    return guyos::MemoryManager::activeMemoryManager->malloc(size);
}

void* operator new(unsigned size, void* ptr)
{
    return ptr;
}

void* operator new[](unsigned size, void* ptr)
{
    return ptr;
}

void operator delete(void* ptr)
{
    if(guyos::MemoryManager::activeMemoryManager != 0)
        guyos::MemoryManager::activeMemoryManager->free(ptr);
}

void operator delete[](void* ptr)
{
    if(guyos::MemoryManager::activeMemoryManager != 0)
        guyos::MemoryManager::activeMemoryManager->free(ptr);
}