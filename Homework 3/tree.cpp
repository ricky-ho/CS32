/*
#include <iostream>
#include <cassert>
using namespace std;
*/

// Return the number of ways that all n2 elements of a2 appear
// in the n1 element array a1 in the same order (though not
// necessarily consecutively).  The empty sequence appears in a
// sequence of length n1 in 1 way, even if n1 is 0.
// For example, if a1 is the 7 element array
//	10 50 40 20 50 40 30
// then for this value of a2     the function must return
//	10 20 40			1
//	10 40 30			2
//	20 10 40			0
//	50 40 30			3
int countIncludes(const double a1[], int n1, const double a2[], int n2)
{
    if (n2 <= 0)        // If n2 <= 0, we have found a sequence of a2 in a1
        return 1;
    else if (n1 <= 0)   // If n1 <= 0, then a1 does not contain any sequence of a2
        return 0;
    
    // cerr << "Pos2[" << n2-1 << "] = " << a2[n2-1] << " | " << "Pos1[" << n1-1 << "] = " << a1[n1-1] << endl;
    
    if (a2[n2-1] == a1[n1-1])                        // Examines both a1 and a2 from end to front
        return countIncludes(a1, n1-1, a2, n2-1) +   // If matching elements found, move to next elements in a1 & a2
               countIncludes(a1, n1-1, a2, n2);      // This adds up all the matching sequences and returns
        
    else
        return countIncludes(a1, n1-1, a2, n2);      // Otherwise, we move onto next element in a1
}


// Exchange two doubles
void exchange(double& x, double& y)
{
    double t = x;
    x = y;
    y = t;
}


// Rearrange the elements of the array so that all the elements
// whose value is > splitter come before all the other elements,
// and all the elements whose value is < splitter come after all
// the other elements.  Upon return, firstNotGreater is set to the
// index of the first element in the rearranged array that is
// <= splitter, or n if there is no such element, and firstLess is
// set to the index of the first element that is < splitter, or n
// if there is no such element.
// In other words, upon return from the function, the array is a
// permutation of its original value such that
//   * for 0 <= i < firstNotGreater, a[i] > splitter
//   * for firstNotGreater <= i < firstLess, a[i] == splitter
//   * for firstLess <= i < n, a[i] < splitter
// All the elements > splitter end up in no particular order.
// All the elements < splitter end up in no particular order.
void split(double a[], int n, double splitter,
           int& firstNotGreater, int& firstLess)
{
    if (n < 0)
        n = 0;
    
    // It will always be the case that just before evaluating the loop
    // condition:
    //  firstNotGreater <= firstUnknown and firstUnknown <= firstLess
    //  Every element earlier than position firstNotGreater is > splitter
    //  Every element from position firstNotGreater to firstUnknown-1 is
    //    == splitter
    //  Every element from firstUnknown to firstLess-1 is not known yet
    //  Every element at position firstLess or later is < splitter
    
    firstNotGreater = 0;
    firstLess = n;
    int firstUnknown = 0;
    while (firstUnknown < firstLess)
    {
        if (a[firstUnknown] < splitter)
        {
            firstLess--;
            exchange(a[firstUnknown], a[firstLess]);
        }
        else
        {
            if (a[firstUnknown] > splitter)
            {
                exchange(a[firstNotGreater], a[firstUnknown]);
                firstNotGreater++;
            }
            firstUnknown++;
        }
    }
}


// Rearrange the elements of the array so that
// a[0] >= a[1] >= a[2] >= ... >= a[n-2] >= a[n-1]
// If n <= 1, do nothing.
void order(double a[], int n)
{
    if (n <= 1)     // If n <= 1 then we do nothing and just return
        return;
    
    int firstNotGreater, firstLess;
    split(a, n, a[n-1], firstNotGreater, firstLess);    // Call split to arrange a[] using the last element's value as
                                                        // the splitter
    /*
    for (int k = 0; k < n; k++)
        cerr << a[k] << ","; cerr << endl;
    */
    
    return order(a, n-1);   // Continue ordering until we have gone through all but the first element
}

/*
int main()
{
    int n1 = 10;  int n2 = 3; int n3 = 2;
    double arr1[10] = {10, 50, 15, 20, 50, 40, 30, 15, 20, 35};
    double arr2[3] = {50, 40, 30};
    double arr3[2] = {15, 20};
    
    assert(countIncludes(arr1, n1, arr2, n2) == 2);
    assert(countIncludes(arr1, n1, arr3, n3) == 3);
    assert(countIncludes(arr1, n1, arr2, 0) == 1);
    assert(countIncludes(arr1, 0, arr2, n2) == 0);
    
    order(arr1, n1);

    for (int k = 0; k < n1; k++)
    {
        cout << arr1[k] << " ";
    }
    
    cout << endl;
    
    cout << "Passed All Tests" << endl;
}
*/
