//
// Created by dylan liang on 2023/1/7.
//

#include <cstdlib>
#include <vector>

#define SKIPLIST_MAXLEVEL 32 /* Should be enough for 2^32 elements */
#define SKIPLIST_P 0.25      /* Skiplist P = 1/4 */
class SkipListNode;
struct SkipListLevel{
    SkipListNode *next;
    int span;
};

struct SkipListNode{
    int score;
    SkipListNode *back;
    std::vector<SkipListLevel> level;
};

struct SkiplistInner{
    SkipListNode *head;
    SkipListNode *tail;
    int level; //max level in skiplist
    int length;
};


class Skiplist {
public:
    SkiplistInner *skiplistInner;
    Skiplist() {
        skiplistInner = new SkiplistInner;
        skiplistInner->head = createNode(SKIPLIST_MAXLEVEL,0);
        for (int i = 0;i < SKIPLIST_MAXLEVEL;i++){
            skiplistInner->head->level[i].next = nullptr;
            skiplistInner->head->level[i].span = 0;
        }
        skiplistInner->head->back = nullptr;
        skiplistInner->tail = nullptr;
        skiplistInner->level = 1;
        skiplistInner->length = 0;
    }

    bool search(int target) {
        SkiplistInner *sl = skiplistInner;
        SkipListNode *x = sl->head;
        //保存小于num的最大节点
        SkipListNode *update[SKIPLIST_MAXLEVEL];
        for(int32_t i = sl->level -1;i >= 0;i--){
            while (x->level[i].next && x->level[i].next->score < target){
                //rank
                x = x->level[i].next;
            }
            update[i] = x;
        }
        return x->level[0].next && x->level[0].next->score == target;

    }

    void add(int num) {
        SkiplistInner *sl = skiplistInner;
        SkipListNode *x = sl->head;
        //保存小于num的最大节点
        SkipListNode *update[SKIPLIST_MAXLEVEL];
        uint32_t rank[SKIPLIST_MAXLEVEL];
        rank[SKIPLIST_MAXLEVEL-1] = 0;
        //find appropriate place to insert this
        //level  num
        //4      1 -->start place
        //3      1     3
        //2      1     3
        //1      1     3    4
        //每一层都要去找
        for(int32_t i = sl->level -1;i >= 0;i--){
//            rank[i] = i == (sl->level-1) ? 0 : rank[i+1];
            rank[i] = rank[i+1];
            while (x->level[i].next && x->level[i].next->score < num){
                //rank
                rank[i] += x->level[i].span;
                x = x->level[i].next;
            }
            update[i] = x;
        }

        int32_t level = randomLevel();
        //if the new level bigger than current's
        if (level > sl->level){
            for(int i = sl->level; i < level;i++){
                rank[i] = 0;
                update[i] = sl->head;
                //@TODO
                update[i]->level[i].span = sl->length;
            }
            sl->level = level;
        }
        x = createNode(level,num);
        for(int i = 0;i < level;i++){
            x->level[i].next = update[i]->level[i].next;
            update[i]->level[i].next = x;
            x->level[i].span = update[i]->level[i].span - (rank[0]-rank[i]);
            update[i]->level[i].span = (rank[0] - rank[i]) + 1;
        }
        for(int i = level;i < sl->level;i++){
            update[i]->level[i].span++;
        }
        x->back = (update[0] == sl->head) ? NULL : update[0];
        if (x->level[0].next)
            x->level[0].next->back = x;
        else
            sl->tail = x;
        sl->length++;
    }

    SkipListNode *createNode(int32_t level,double score){
        SkipListNode *sln = new SkipListNode;
        sln->level = std::vector<SkipListLevel>(level);
        sln->score = score;
        return sln;
    }

    bool erase(int num) {
        if(!search(num)){
            return false;
        }
        SkipListNode *update[SKIPLIST_MAXLEVEL];
        SkiplistInner *sl = skiplistInner;
        SkipListNode *x = skiplistInner->head;
        for(int i = sl->level-1;i>=0;i--){
            while (x->level[i].next && x->level[i].next->score < num){
                x = x->level[i].next;
            }
            update[i] = x;
        }
//        SkipListNode *next;
        x = x->level[0].next;
//        do{
//            next = x->level[0].next;
        if (x && num == x->score) {
            removeNode(x, update);
            freeNode(x);
//            }else{
//                break;
        }
//            x = next;
//        } while (next);
        return true;
    }

    void freeNode(SkipListNode *sln){
        delete sln;
    }
    void removeNode(SkipListNode *x, SkipListNode **update) {
        SkiplistInner *sl = skiplistInner;
        int i;
        for (i = 0; i < sl->level; i++) {
            if (update[i]->level[i].next == x) {
                update[i]->level[i].span += x->level[i].span - 1;
                update[i]->level[i].next = x->level[i].next;
            } else {
                update[i]->level[i].span -= 1;
            }
        }
        if (x->level[0].next) {
            x->level[0].next->back = x->back;
        } else {
            sl->tail = x->back;
        }
        while(sl->level > 1 && sl->head->level[sl->level-1].next == NULL)
            sl->level--;
        sl->length--;
    }

    int32_t randomLevel(){
        int level = 1;
        while ((random()&0xFFFF) < (SKIPLIST_P * 0xFFFF))
            level += 1;
        return (level<SKIPLIST_MAXLEVEL) ? level : SKIPLIST_MAXLEVEL;
    }

    int size(){
        return skiplistInner->length;
    }

};

int main(){
    Skiplist skiplist;

    skiplist.add(1);
    skiplist.add(2);
    skiplist.add(3);
    assert(skiplist.search(0)==false);
    skiplist.add(4);
    assert(skiplist.search(1)==true);
    skiplist.erase(0);
    skiplist.erase(1);
    assert(skiplist.search(1)==false);

    return 0;
}