#include "cachelab.h"
#include "getopt.h"
#include <string.h>
#include "stdio.h"
#include "stdlib.h"
#define _SVID_SOURCE

typedef struct{
    int valid_bits;
    int tag;
    int stamp;
}cache_line, *cache_asso, **cache;

int hit_count = 0;
int miss_count = 0;
int eviction_count = 0;

cache _cache_ = NULL;
int opt, s,E,b,h,v;
int S;

void init_cache(){
    _cache_ = (cache)malloc(sizeof(cache_asso) * S);
    for (int i = 0;i<S;i++){
        _cache_[i] = (cache_asso)malloc(sizeof(cache_line)*E);
        for (int j = 0;j<E;j++){
            _cache_[i][j].valid_bits = 0;
            _cache_[i][j].tag = -1;
            _cache_[i][j].stamp = -1;
        }
    }
};

void clean_cache(){
    for (int i = 0;i<S;i++){
        free(_cache_[i]);
    }
    free(_cache_);
}


void update(unsigned address){
    int tmp = address >> b;
    int tag =  tmp >> s;
    int set_index = tmp ^ (tag << s);

    for (int i = 0;i<E;i++){
        if (tag == _cache_[set_index][i].tag){
            _cache_[set_index][i].stamp = 0;
            hit_count++;
            return;
        }
    }

    miss_count++;
    for (int i = 0;i<E;i++){
        if (_cache_[set_index][i].valid_bits == 0){
            _cache_[set_index][i].stamp = 0;
            _cache_[set_index][i].tag = tag;
            _cache_[set_index][i].valid_bits = 1;
            return;
        }
    }

    // 需要进行替换操作了
    eviction_count++;

    int index = -1;
    int max_stamp = -1;
    for (int i = 0;i<E;i++){
        if (_cache_[set_index][i].stamp > max_stamp){
            index = i;
            max_stamp = _cache_[set_index][i].stamp;
        }
    }
    _cache_[set_index][index].tag = tag;
    _cache_[set_index][index].stamp = 0;
    return;
};


void update_stamp(){
    for (int i = 0;i<S;i++){
        for (int j = 0;j<E;j++){
            if (_cache_[i][j].valid_bits != 0){
                _cache_[i][j].stamp++;
            }
        }
    }
}

int main(int argc, char** argv)
{
    char t[2000];
    while(-1 != (opt = (getopt(argc,argv,"hvs:E:b:t:")))){
        switch (opt)
        {
        case 'h':
            h = 1;
            //TODO
            break;
        case 'v':
            v = 1;
            //TODO
            break;
        case 's':
            s = atoi(optarg);
            S = 1 << s;
            break;
        case 'E':
            E = atoi(optarg);
            break;
        case 'b':
            b = atoi(optarg);
            break;
        case 't':
            strcpy(t,optarg);
            break;
        default:
            printf("wrong argument\n");
            break;
        }
    }
    // printf("%d %d %d\n", s,e,b);
    init_cache();

    FILE* pFile;
    pFile = fopen(t,"r"); // open trace file for reading

    char identifier;
    unsigned address;
    int size;
    // read lines like " M 20,1" or " L 19,3"

    if (pFile == NULL){
        exit(-1);
    }

    while (fscanf(pFile," %c %x,%d", &identifier, &address, &size) > 0){
        //TODO
        switch (identifier)
        {
        case 'L':
            update(address);
            break;
        case 'M':
            update(address);
        case 'S':
            update(address);
            break;
        }
        update_stamp();
    }
    fclose(pFile);
    printSummary(hit_count, miss_count, eviction_count);

    clean_cache();
    return 0;
}