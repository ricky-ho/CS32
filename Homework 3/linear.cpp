/*
#include <iostream>
#include <cmath>
#include <cassert>
using namespace std;

bool somePredicate(double x)
{
    return x >= 0;  // Returns false if there is a negative number
}
*/

// Return false if the somePredicate function returns false for at
// least one of the array elements; return true otherwise.
bool allTrue(const double a[], int n)
{
    if (n <= 0)
        return true;
    
    if (! somePredicate(a[0]))
        return false;
    else
        return (allTrue(a+1, n-1));
}


// Return the number of elements in the array for which the
// somePredicate function returns false.
int countFalse(const double a[], int n)
{
    if (n <= 0)
        return 0;
    
    int numFalse = countFalse(a+1, n-1);    // numFalse keeps track of the number of falses that occurred
                                            // Start at last element of array in which numFalse = 0
    if (! somePredicate(a[0]))              // If the value causes somePredicate to return false, we increment numFalse
        numFalse++;
    
    return numFalse;
}


// Return the subscript of the first element in the array for which
// the somePredicate function returns false.  If there is no such
// element, return -1.
int firstFalse(const double a[], int n)
{
    if (n <= 0)
        return -1;
    
    if (! somePredicate(a[n-1]))        // If the last element causes somePredicate to return false
        if (firstFalse(a, n-1) == -1)   // Then check if any of the previous elements caused it to return false
            return n-1;                 // At this point, none of prev. elem. were false, thus we return subscript of
                                        // our original match
    
    return firstFalse(a, n-1);          // Otherwise, skip last element and start from the previous element
}


// Return the subscript of the smallest double in the array (i.e.,
// the one whose value is <= the value of all elements).  If more
// than one element has the same smallest value, return the smallest
// subscript of such an element.  If the array has no elements to
// examine, return -1.
int indexOfMin(const double a[], int n)
{
    if (n <= 0)
        return -1;

    int pos = 1 + indexOfMin(a+1, n-1);     // Keeps track of the subscript position of the smallest double
    
    if (a[0] <= a[pos])     // Compare the first element with the element of the current smallest double
        return 0;           // If a[0] is smaller, then pos will now be at that element
    else
        return pos;         // Otherwise, pos is smaller so we return pos which is incremented by 1 later
}


// If all n2 elements of a2 appear in the n1 element array a1, in
// the same order (though not necessarily consecutively), then
// return true; otherwise (i.e., if the array a1 does not include
// a2 as a not-necessarily-contiguous subsequence), return false.
// (Of course, if a2 is empty (i.e., n2 is 0), return true.)
// For example, if a1 is the 7 element array
//    10 50 40 20 50 40 30
// then the function should return true if a2 is
//    50 20 30
// or
//    50 40 40
// and it should return false if a2 is
//    50 30 20
// or
//    10 20 20
bool includes(const double a1[], int n1, const double a2[], int n2)
{
    if (n2 == 0)            // If a2 is empty, we return true
        return true;
    
    if (n1 == 0 && n2 != 0) // If a1 is completely searched, but not all of a2 is found, return false
        return false;
    
    if (a1[0] == a2[0])                             // If the current element of a1 is equal to current element of a2
        return includes(a1+1, n1-1, a2+1, n2-1);    // then we move onto next element for both
    
    else
        return includes(a1+1, n1-1, a2, n2);        // Otherwise, we continue to look through a1 at next element
}

/*
int main()
{
    int n1 = 7;
    int n2 = 3;
    int n3 = 4;
    int n4 = 3;
    const double arr1[7] = {2,-3,-3,4,7,-5,6};
    const double arr2[3] = {-3,-3,2};
    const double arr3[4] = {-0,0,0,0};
    const double arr4[3] = {-3, 4, 6};
    
    assert(allTrue(arr1, n1) == false);
    assert(allTrue(arr2, n2) == false);
    assert(allTrue(arr3, n3) == true);
    
    assert(countFalse(arr1, n1) == 3);
    assert(countFalse(arr2, n2) == 2);
    assert(countFalse(arr3, n3) == 0);

    assert(firstFalse(arr1, n1) == 1);
    assert(firstFalse(arr2, n2) == 0);
    assert(firstFalse(arr3, n3) == -1);
    
    assert(indexOfMin(arr1, n1) == 5);
    assert(indexOfMin(arr2, n2) == 0);
    assert(indexOfMin(arr3, n3) == 0);
    
    assert(includes(arr1, n1, arr3, n3) == false);
    assert(includes(arr1, n1, arr2, n2) == false);
    assert(includes(arr1, n1, arr4, n4) == true);
    
    cout << "All tests passed" << endl;
}
 */
