//
// Created by dylan liang on 2022/12/1.
//

#include <cstdint>
#include <cstdlib>
#include <vector>

#define SKIPLIST_MAXLEVEL 32 /* Should be enough for 2^32 elements */
#define SKIPLIST_P 0.25      /* Skiplist P = 1/4 */

//copy code from redis 3.0.0
//http://redis.io
/**
 * <------ backward arrow
 * ------> forward arrow
 *
 * @TODO template<class T>
 */
struct SkipListNode{
    int32_t score;
    SkipListNode *backward;
    struct SkipListLevel{
        SkipListNode *forward;
        uint32_t span;
    }level[];
};

struct SkipListInner{
    SkipListNode *head;
    SkipListNode *tail;
    uint32_t length;
    int32_t level;

};

class SkipList{
public:
    SkipList(){
        skipList = create();
    }

    ~SkipList(){
        free(skipList);
    }

    void insert(int32_t score){
        std::vector<SkipListNode*> update(SKIPLIST_MAXLEVEL);
        SkipListInner *sl = skipList;
        SkipListNode *x = sl->head;
        unsigned int rank[SKIPLIST_MAXLEVEL] = {0};
        for(int i = sl->level - 1; i >= 0; i--){
//            if(i == sl->level - 1){
//                rank[i] = 0;
//            }else{
            rank[i] = rank[i+1];
//            }
            while (x->level[i].forward && x->level[i].forward->score < score) {
                rank[i] += x->level[i].span;
                x = x->level[i].forward;
            }
            update[i] = x;
        }
        int32_t level = randomLevel();
        if (level > sl->level) {
            for (int32_t i = sl->level; i < level; i++) {
                rank[i] = 0;
                update[i] = sl->head;
                update[i]->level[i].span = sl->length;
            }
            sl->level = level;
        }

        x = createNode(level,score);
        for (int32_t i = 0; i < level; i++) {
            x->level[i].forward = update[i]->level[i].forward;
            update[i]->level[i].forward = x;

            /* update span covered by update[i] as x is inserted here */
            x->level[i].span = update[i]->level[i].span - (rank[0] - rank[i]);
            update[i]->level[i].span = (rank[0] - rank[i]) + 1;
        }

        /* increment span for untouched levels */
        for (int32_t i = level; i < sl->level; i++) {
            update[i]->level[i].span++;
        }

        x->backward = (update[0] == sl->head) ? NULL : update[0];
        if (x->level[0].forward)
            x->level[0].forward->backward = x;
        else
            sl->tail = x;
        sl->length++;
    }

    void remove(int32_t score){
        SkipListNode *update[SKIPLIST_MAXLEVEL], *x;
        SkipListInner *sl = skipList;
        x = sl->head;
        for (int32_t i = sl->level-1; i >= 0; i--) {
            while (x->level[i].forward && x->level[i].forward->score < score){
                x = x->level[i].forward;
            }
            update[i] = x;
        }
        /* We may have multiple elements with the same score, what we need
         * is to find the element with both the right score and object. */
        x = x->level[0].forward;
        if (x ) {
            removeNode(x, update);
            freeNode(x);
        }
    }

private:
    SkipListInner *create(){
        SkipListInner *sl = static_cast<SkipListInner *>(malloc(sizeof(SkipListInner)));
        sl->length = 0;
        sl->level = 1;
        sl->head = createNode(SKIPLIST_MAXLEVEL,0);
        for(int i = 0;i < SKIPLIST_MAXLEVEL;i++){
            sl->head->level[i].forward = nullptr;
            sl->head->level[i].span = 0;
        }
        sl->head->backward = nullptr;
        sl->tail = nullptr;
        return sl;
    }
    SkipListNode *createNode(int32_t level,double score){
        SkipListNode *sln = static_cast<SkipListNode*>(malloc(sizeof(SkipListNode) + level * sizeof(SkipListNode::SkipListLevel)));
        sln->score = score;
        return sln;
    }
    void freeNode(SkipListNode *sln){
        free(sln);
    }
    void removeNode(SkipListNode *x, SkipListNode **update) {
        SkipListInner *sl = skipList;
        int i;
        for (i = 0; i < sl->level; i++) {
            if (update[i]->level[i].forward == x) {
                update[i]->level[i].span += x->level[i].span - 1;
                update[i]->level[i].forward = x->level[i].forward;
            } else {
                update[i]->level[i].span -= 1;
            }
        }
        if (x->level[0].forward) {
            x->level[0].forward->backward = x->backward;
        } else {
            sl->tail = x->backward;
        }
        while(sl->level > 1 && sl->head->level[sl->level-1].forward == NULL)
            sl->level--;
        sl->length--;
    }

    int randomLevel() {
        int level = 1;
        while ((random()&0xFFFF) < (SKIPLIST_P * 0xFFFF))
            level += 1;
        return (level<SKIPLIST_MAXLEVEL) ? level : SKIPLIST_MAXLEVEL;
    }

private:
    SkipListInner *skipList;
};

