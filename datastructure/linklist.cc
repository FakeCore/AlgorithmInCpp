//
// Created by dylan liang on 2022/10/8.
//

class List{
    List(){

    }
    ~List(){

    }

    void push_back(int n){
        List *t = _next;
        if(t != nullptr && t->_next != nullptr){
            t = t->_next;
        }
        t->_next = new List;
        t->_next->val = n;
    }
    void push_front(int n){

    }
    void pop_front(){

    }
    void pop_back(){

    }

private:
    List *_front = nullptr;
    List *_next = nullptr;
    int val = 0;
    int size = 0;
    //T data;
};