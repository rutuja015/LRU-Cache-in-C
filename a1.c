#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
// #include "new.h"
#define MAX_CITEMS 1000  

typedef struct LRUCache {
    int val;
    bool valid;
    struct LRUCache *prev;
    struct LRUCache *next;
} LRUCache;

struct cdata {
    int capacity;
    int size;
    struct LRUCache *cache;
    struct LRUCache *head;
    struct LRUCache *tail;
};

static inline void *alloc_cache(size_t capacity){
    struct cdata *cdata = calloc(1, sizeof *cdata);
    cdata->cache = calloc(1, sizeof(struct LRUCache [MAX_CITEMS]));
    cdata->capacity = capacity;
    return cdata;
}

static inline void dealloc_cache(void *obj){
    free(((struct cdata *)obj)->cache);
    free(obj);
}

static inline void refresh_cache(struct LRUCache *item, struct cdata *cdata, bool front){
    struct LRUCache **prev = &item->prev, **next = &item->next;
    struct LRUCache **head = &cdata->head, **tail = &cdata->tail;
    if (item->valid) { // Remove node 
        *prev ? (*prev)->next = *next : 0;
        *next ? (*next)->prev = *prev : 0;
    }
    item == *tail ? *tail = *prev : 0;// Fix tail
    item->valid = front;    
    if (front) { // Move front
        *prev = NULL;                       
        *next = cdata->head;
        *head ? (*head)->prev = item : 0;
        *head = item; // Fix head
        !*tail ? *tail = *head : 0; // First entry fix tail
    }
}

LRUCache* lRUCacheCreate(int capacity){
    return alloc_cache(capacity);
}


int lRUCacheGet(LRUCache* obj, int key){
    struct cdata *cdata = (void *)obj;
    struct LRUCache *item = &cdata->cache[key];    
    if (!cdata->size || !item->valid)           ///       if no ele present or the key is not valid return -1
        return -1;
    if (item == cdata->head)     ///if key already present ...or present at front
        return item->val;        // Already at front 
    refresh_cache(item, cdata, true); // Move front
    return item->val;
}

void lRUCachePut(LRUCache* obj, int key, int value){
    struct cdata *cdata = (void *)obj;
    struct LRUCache *item = &cdata->cache[key];
    if (item == cdata->head)
        return item->val = value, (void)0; // Already at front
        
    if (!item->valid)
        if (cdata->size == cdata->capacity)
            refresh_cache(cdata->tail, cdata, false); // Evict
        else
            cdata->size++;
    refresh_cache(item, cdata, true); // Move/add front
    item->val = value;            
}

void lRUCacheFree(LRUCache* obj){
    dealloc_cache(obj);
}


int main(){
    int size,key,value,ans;
    printf("Enter the size of the Cache: \n");
    scanf("%d",&size);

    LRUCache * lru_obj = lRUCacheCreate(size);


    printf("\n \n");
    printf("Cache of size %d is created! \n",size);

    printf("\n \n");
    printf("Enter \"get\" to get the value of the key\n");
    printf("Enter \"put\" to put the value to the key\n");
    printf("Enter \' -1 \' to stop");
    printf("\n \n");

    char s[3];
    scanf("%s",s);

    while(strcmp(s,"-1")){
        if(strcmp(s,"get") && strcmp(s,"put")){
            printf("INVALID COMMAND!! Please enter correctly \n");
        }
        else{
            if(!strcmp(s,"put")){
                printf("Enter the value of key: \n");
                scanf("%d",&key);
                printf("Enter the value of value: \n");
                scanf("%d",&value);
                printf("\n \n");
                lRUCachePut(lru_obj,key,value);
            }
            else if(!strcmp(s,"get")){
                printf("Enter the value of key: \n");
                scanf("%d",&key);
                ans = lRUCacheGet(lru_obj,key);
                printf("The value corresponding to key %d is: %d",key,ans);
                printf("\n \n");
            }
        }

        printf("Enter Command: \n");
        scanf("%s",s);
    }
    

    return 0;
}