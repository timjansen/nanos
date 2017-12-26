
// should consider a drain function
typedef struct heap {
    void *(*allocate)(struct heap *h, bytes b);
    void (*deallocate)(struct heap *h, void *a, bytes b);
    void (*destroy)();
    bytes pagesize;
    bytes allocated;
} *heap;

heap allocate_leaky_heap(heap parent);
heap allocate_pagechunk(heap h, bytes s);
heap allocate_pagecache(heap h);
heap allocate_rolling_heap(heap h);

// really internals

static inline void *page_of(void *x, bytes pagesize)
{
    return((void *)((unsigned long)x &
                    (~((unsigned long)pagesize-1))));
}

static inline int subdivide(int quantum, int per, int s, int o)
{
    // this overallocates
    int base = ((s-o)/quantum) * per;
    return (pad(o + base, quantum));
}

#define allocate(__h, __b) (__h->allocate(__h, __b))
#define deallocate(__h, __b, __s) (__h->deallocate(__h, __b, __s))

#define allocate_zero(__h, __b) ({\
        void *x = allocate(__h, __b);\
        memset(x, 0, __b);\
        x; })

