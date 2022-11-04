//
// Created by dylan liang on 2022/10/8.
//

//not safe for concurrent
#include "cstring"
class Vector{

public:
    Vector(){
        _start = new int[8];
//        size = 8;
        _finish = _start;
    }

    Vector(int size){
    }

    ~Vector(){
        delete _start;
        _start = nullptr;
    }

    void add(int num){
        if(_finish-_start >= _length){
            _length = 2;
            int *t = new int[_length*2];
            memcpy(t,_start,_length);
            delete _start;
            _start = t;
            _finish = _start + _length;
            _length *= 2;
        }
        *_finish = num;
        _finish++;
    }

    int size(){
        return _finish-_start;
    }

    int capicity(){
        return _length;
    }

    void pop(){
        if(_finish>_start)
            --_finish;
    }


private:
    int *_start = nullptr;
    int *_finish = nullptr;
    int *_end = nullptr;
    int _length = 0;
};