//
// Created by dylan liang on 2022/11/3.
//
#include "vector"

struct NoSpaceException : public std::exception
{
    const char * what () const throw ()
    {
        return "No available buf space for reading";
    }
};
// not thread safe
template<class T>
class RingBuf{
public:
    RingBuf(int capacity,bool enableScale = false){
        buf.resize(capacity);
        isScale = enableScale;
    }
    void push(T t){
//        if(size() == capacity()){
//            throw "No available buf space for writing";
//        }
        buf[wIndex] = t;
        wIndex++;
        if(size() == capacity()){
            //full
            int oldSize = buf.size();
            buf.resize(buf.size()*2);
            if(rIndex != 0){
                std::vector<T> tmp(wIndex);
                memmove(&buf[0]+oldSize,&buf[0],wIndex);
                memmove(&buf[0],&buf[0]+rIndex,oldSize-1);
            }
            rIndex = 0;
            wIndex = oldSize;
        }
        wIndex %= buf.size();
    }

    T pop(){
        if(empty()){
            throw NoSpaceException();
        }
        int tIndex = rIndex;
        rIndex++;
        rIndex = rIndex % buf.size();
        return buf[tIndex];
    }

    bool empty(){
        return size() == 0;
    }
    int32_t size(){
        if(wIndex > rIndex){
            return wIndex - rIndex - 1;
        }else{
            return capacity() - (rIndex-wIndex);
        }
    }
    int32_t capacity(){
        return buf.size() - 1;
    }

    int32_t availableSize(){
        capacity()-size();
    }
private:
    int rIndex = 0;
    int wIndex = 1;
    std::vector<T> buf;
    bool isScale = false;
};