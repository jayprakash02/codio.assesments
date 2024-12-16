#include<stdio.h>
#include<stddef.h>

struct block {
    size_t size;
    int free;
    struct block *next;
};

#define MEMORY_SIZE 20000
char memory[MEMORY_SIZE];
struct block *freeList = (void*)memory;

void initialize() {
    freeList->size = MEMORY_SIZE - sizeof(struct block);
    freeList->free = 1;
    freeList->next = NULL;
}

void split(struct block *fitting_slot, size_t size) {
    struct block *new = (void*)((void*)fitting_slot + size + sizeof(struct block));
    new->size = (fitting_slot->size) - size - sizeof(struct block);
    new->free = 1;
    new->next = fitting_slot->next;
    fitting_slot->size = size;
    fitting_slot->free = 0;
    fitting_slot->next = new;
}

void merge() {
    struct block *curr = freeList;
    while ((curr != NULL) && (curr->next != NULL)) {
        if ((curr->free) && (curr->next->free)) {
            curr->size += (curr->next->size) + sizeof(struct block);
            curr->next = curr->next->next;
        }
        curr = curr->next;
    }
}

void *MyMalloc(size_t noOfBytes) {
    struct block *curr;
    void *result;

    if (!freeList->size) {
        initialize();
    }

    curr = freeList;
    while (curr != NULL) {
        if ((curr->free) && (curr->size >= noOfBytes)) {
            if ((curr->size) > (noOfBytes + sizeof(struct block))) {
                split(curr, noOfBytes);
            }
            curr->free = 0;
            result = (void*)(++curr);
            printf("Memory allocated at: %p\n", result);
            return result;
        }
        curr = curr->next;
    }

    printf("Sorry. No sufficient memory to allocate.\n");
    return NULL;
}

void MyFree(void* ptr) {
    if ((void*)memory <= ptr && ptr <= (void*)(memory + MEMORY_SIZE)) {
        struct block* curr = (struct block*)ptr - 1;
        curr->free = 1;
        printf("Memory freed at: %p\n", ptr);
        merge();
    } else {
        printf("Invalid pointer, unable to free memory.\n");
    }
}

int main() {
    // Test the custom memory allocator
    void *p1 = MyMalloc(5000);
    void *p2 = MyMalloc(2000);
    void *p3 = MyMalloc(7000);

    MyFree(p2);
    void *p4 = MyMalloc(1500);

    MyFree(p1);
    MyFree(p3);
    MyFree(p4);

    return 0;
}
