//
// Created by dylan liang on 2022/11/4.
//
#include "ringbuf.h"

int main(){
    RingBuf<int> ringBuf(10);
    ringBuf.push(111);
    ringBuf.push(11);
    assert(ringBuf.size() == 2);
    ringBuf.pop();
    assert(ringBuf.size() == 1);
    ringBuf.pop();
    bool geterr = false;
    try{
        ringBuf.pop();
    }catch (std::exception &e){
        printf("%s\n",e.what());
        geterr = true;
    }
    for(auto i = 0 ;i < 10;i++){
        ringBuf.push(i);
    }
    assert(ringBuf.capacity() == 19);
    assert(ringBuf.size() == 10);
    for(auto i = 0 ;i < 10;i++){
        ringBuf.push(i);
    }
    assert(ringBuf.size() == 20);
    assert(geterr == true);
    return 0;
}