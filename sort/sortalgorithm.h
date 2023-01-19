#pragma once

#include <vector>
#include <assert.h>
#include <time.h>
using namespace std;

/**
 * Time Complexity && Space Complexity
 */

class SortAlgorithm {
public:
    // minmum
    //
    void bubbleSort(std::vector<int>& vec) {
        if (vec.empty()) {
            return;
        }
        for (int i = 0; i < vec.size() - 1; i++) {
            for (int j = 1; j < vec.size() - i; j++) {
                if (vec[j - 1] > vec[j]) {
                    swap(vec[j - 1], vec[j]);
                }
            }
        }
    }

    void selectSort(std::vector<int>& vec) {
        if (vec.empty())
            return;

        for (int i = 1; i < vec.size(); i++) {
            int& chosen = vec[i - 1];
            for (int j = i; j < vec.size(); j++) {
                if (chosen > vec[j]) {
                    swap(chosen, vec[j]);
                }
            }
        }
    }

    void insertSort(std::vector<int>& vec) {
        for (int i = 1; i < vec.size(); i++) {
            int key = vec[i];
            int j = i - 1;
            while ((j >= 0) && (key < vec[j])) {
                vec[j + 1] = vec[j];
                j--;
            }
            vec[j + 1] = key;
        }
    }

    void shellSort(std::vector<int>& vec) {
        for (int gap = vec.size() / 2; gap > 0; gap /= 2) {
            for (int i = 0; i < vec.size(); i++) {
                int j = i;
                // pass the first element
                while (j - gap >= 0 && vec[j] < vec[j - gap]) {
                    swap(vec[j], vec[j - gap]);
                    j -= gap;
                }
            }
        }
    }

    void merge(int* a, int* l, int* r, int leftcount, int rightcount) {
        int i, j, k;

        // i - to mark the index of left aubarray (l)
        // j - to mark the index of right sub-raay (r)
        // k - to mark the index of merged subarray (a)
        i = 0; j = 0; k = 0;

        while (i < leftcount && j < rightcount) {
            if (l[i] < r[j]) a[k++] = l[i++];
            else a[k++] = r[j++];
        }
        while (i < leftcount) a[k++] = l[i++];
        while (j < rightcount) a[k++] = r[j++];
    }

    void mergeSort(int* vec, int n) {
        if (n < 2)
            return;
        int mid = n / 2;
        int* l = new int[mid];
        int* r = new int[n - mid];

        for (int i = 0; i < mid; i++)l[i] = vec[i];
        for (int i = mid; i < n; i++)r[i - mid] = vec[i];

        mergeSort(l, mid);
        mergeSort(r, n - mid);
        //merge
        merge(vec, l, r, mid, n - mid);

        delete[] l;
        delete[] r;
    }

    void quicksort(std::vector<int>& vec, int low, int high) {
        if (low < high) {
            int left = low;
            int right = high + 1;
            int pivot = vec[low];
            while (1) {
                while (left < high && vec[++left] < pivot);
                while (right > low && vec[--right] > pivot);
                if (left >= right)break;
                swap(vec[left], vec[right]);
            }
            vec[low] = vec[right];
            vec[right] = pivot;
            quicksort(vec, low, right - 1);
            quicksort(vec, right + 1, high);
        }
    }
    void quicksort2(int* a, int leftend, int rightend) {
        if (leftend >= rightend) return;

        int leftcursor = leftend;
        int rightcursor = rightend + 1;
        int pivot = a[leftend];
        while (true)
        {
            do {
                leftcursor++;
            } while (a[leftcursor] < pivot);

            do {
                rightcursor--;
            } while (a[rightcursor] > pivot);

            if (leftcursor >= rightcursor)break;
            //swap(a[leftcursor], a[rightcursor]);
            int t = a[leftcursor];
            a[leftcursor] = a[rightcursor];
            a[rightcursor] = t;
        }

        a[leftend] = a[rightcursor];
        a[rightcursor] = pivot;

        quicksort2(a, leftend, rightcursor - 1);
        quicksort2(a, rightcursor + 1, rightend);
    }

    void maxHeapify(std::vector<int>& vec, int start, int end) {
        int dad = start;
        int son = dad * 2 + 1;
        while (son <= end) {
            if (son + 1 <= end && vec[son] < vec[son + 1]) {
                son++;
            }
            if (vec[dad] > vec[son]) {
                return;
            }
            else {
                swap(vec[dad], vec[son]);
                dad = son;
                son = dad * 2 + 1;
            }
        }
    }

    void heapSort(std::vector<int>& vec) {
        for (int i = vec.size() / 2 - 1; i >= 0; i--) {
            maxHeapify(vec, i, vec.size() - 1);
        }
        for (int i = vec.size() - 1; i > 0; i--) {
            swap(vec[0], vec[i]);
            maxHeapify(vec, 0, i - 1);
        }
    }

    int minimum(const std::vector<int>& vec) {
        int min = vec[0];
        for (auto c : vec) {
            min = min < c ? min : c;
        }
        return min;
    }
    int maximum(const std::vector<int>& vec) {
        int max = vec[0];
        for (auto c : vec) {
            max = max > c ? max : c;
        }
        return max;
    }
    void countSort(std::vector<int>& vec) {
        if (vec.empty())
            return;
        int min = minimum(vec);
        int max = maximum(vec);

        std::vector<int> v(max - min + 1);
        //memset
        for (int c : vec) {
            v[c - min]++;
        }
        int count = 0;
        for (int i = 0; i < v.size(); i++) {
            int cc = v[i];
            int value = i + min;
            while (cc--) {
                vec[count++] = value;
            }
        }
    }
    void bucketSort(std::vector<int>& vec,int max,int min) {
        if (vec.empty())
            return;

        int bucket_num = 10;
        int liner = (max - min) / bucket_num + 1;

        std::vector<std::vector<int>> v(bucket_num);

        for (auto c : vec) {
            int pos = (c - min) / liner;
            v[pos].push_back(c);
        }
        for (auto& c : v) {
            //quicksort(c, 0, c.size() - 1);
            // mergeSort(c.data(), c.size());
            countSort(c);
        }
        int count = 0;
        for (auto& c : v) {
            std::copy(c.begin(), c.end(), vec.data() + count);
            count += c.size();
        }
    }


    void countSort(vector<int>& vec, int exp)
    {
        vector<int> range(10, 0);

        int length = vec.size();
        vector<int> tmpVec(length, 0);

        for (int i = 0; i < length; ++i)
        {
            range[(vec[i] / exp) % 10]++;
        }
        for (int i = 1; i < range.size(); ++i)
        {
            range[i] += range[i - 1];//
        }
        for (int i = length - 1; i >= 0; --i)
        {
            tmpVec[range[(vec[i] / exp) % 10] - 1] = vec[i];
            range[(vec[i] / exp) % 10]--;
        }
        vec = tmpVec;
    }

    void radixSort(vector<int>& vec)
    {
        int length = vec.size();
        int max = -1;
        for (int i = 0; i < length; ++i)
        {
            if (vec[i] > max)
                max = vec[i];
        }
        for (int exp = 1; max / exp > 0; exp *= 10)
            countSort(vec, exp);
    }


    /*****************************************************************************************************************/
    /**************************************************for test*******************************************************/
    /*****************************************************************************************************************/

    bool checkVecOrder(const std::vector<int>& vec) {
        for (int j = 1; j < vec.size(); j++) {
            if (vec[j - 1] > vec[j]) {
                printf("%d %d %d\n", j, vec[j - 1], vec[j]);
                return false;
            }
        }
        return true;
    }

    void test() {
        int s = 100000000;
        std::vector<int> v;
        for (int i = 0; i < s; i++) {
            v.push_back(rand() % 10000);
        }
        std::vector<std::vector<int>> vs ;
//        = { {323,1,41,4,13,41,23,3,5,2,41,4,234,1,42,5,1,2,1},{},{1},{83,42} };
//        vs.push_back({ 10,1,2,3,4,5,6 });
        vs.push_back(v);
//        v.resize(1000000);
//        vs.push_back(v);
    
        printf("quciksort\n");
        for (auto s : vs) {
            clock_t start = clock();
            quicksort(s,0,s.size()-1);
            clock_t end = clock();
            assert(checkVecOrder(s));
            printf("pass size:%d  time:%f\n",s.size(),(float)(end-start)/CLOCKS_PER_SEC/1.0);
        }
        //printf("quciksort2\n");
        //for (auto& s : vs) {
        //    clock_t start = clock();
        //    quicksort2(s.data(),0,s.size()-1);
        //    clock_t end = clock();
        //    assert(checkVecOrder(s));
        //    printf("pass size:%d  time:%f\n",s.size(),(float)(end-start)/CLOCKS_PER_SEC/1.0);
        //}
        printf("mergesort\n");
        for (auto s : vs) {
            clock_t start = clock();
            mergeSort(s.data(),s.size());
            clock_t end = clock();
            assert(checkVecOrder(s));
            printf("pass size:%d  time:%f\n",s.size(),(float)(end-start)/CLOCKS_PER_SEC/1.0);
        }
        printf("bucketsort\n");
        for (auto s : vs) {
            int min = 0;
            int max = 0;
            if (!s.empty()) {
                min = minimum(s);
                max = maximum(s);
            }
            clock_t start = clock();
            bucketSort(s, max, min);
            clock_t end = clock();
            assert(checkVecOrder(s));
            printf("pass size:%d  time:%f\n",s.size(),(float)(end-start)/CLOCKS_PER_SEC/1.0);
        }
        printf("heapsort\n");
        for (auto s : vs) {
            clock_t start = clock();
            heapSort(s);
            clock_t end = clock();
            assert(checkVecOrder(s));
            printf("pass size:%d  time:%f\n",s.size(),(float)(end-start)/CLOCKS_PER_SEC/1.0);
        }
        printf("shellsort\n");
        for (auto s : vs) {
            clock_t start = clock();
            shellSort(s);
            clock_t end = clock();
            assert(checkVecOrder(s));
            printf("pass size:%d  time:%f\n",s.size(),(float)(end-start)/CLOCKS_PER_SEC/1.0);
        }
        printf("countsort\n");
        for (auto s : vs) {
            clock_t start = clock();
            countSort(s);
            clock_t end = clock();
            assert(checkVecOrder(s));
            printf("pass size:%d  time:%f\n",s.size(),(float)(end-start)/CLOCKS_PER_SEC/1.0);
        }
        printf("radixsort\n");
        for (auto s : vs) {
            clock_t start = clock();
            radixSort(s);
            clock_t end = clock();
            assert(checkVecOrder(s));
            printf("pass size:%d  time:%f\n",s.size(),(float)(end-start)/CLOCKS_PER_SEC/1.0);
        }
    }

    void swap(int& a, int& b) {
        int t = a;
        a = b;
        b = t;
    }
};
