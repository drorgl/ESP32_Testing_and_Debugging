/*
 Copyright (c) 2014-present PlatformIO <contact@platformio.org>

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
**/

#include <calculator.h>


int Calculator::add(int a, int b)
{
    return a + b;
}

int Calculator::sub(int a, int b)
{
    return a - b;
}

int Calculator::mul(int a, int b)
{
    return a * b;
}


int Calculator::div(int a, int b)
{
    return a / b;
}

//https://www.geeksforgeeks.org/square-root-of-an-integer/
int Calculator::floorSqrt(int x)
{
    // Base cases
    if (x == 0 || x == 1)
        return x;
 
    // Do Binary Search for floor(sqrt(x))
    int start = 1, end = x / 2, ans;
    while (start <= end) {
        int mid = (start + end) / 2;
 
        // If x is a perfect square
        int sqr = mid * mid;
        if (sqr == x)
            return mid;
 
        // Since we need floor, we update answer when
        // mid*mid is smaller than x, and move closer to
        // sqrt(x)
 
        /*
           if(mid*mid<=x)
                   {
                           start = mid+1;
                           ans = mid;
                   }
            Here basically if we multiply mid with itself so
           there will be integer overflow which will throw
           tle for larger input so to overcome this
           situation we can use long or we can just divide
            the number by mid which is same as checking
           mid*mid < x
 
           */
        if (sqr <= x) {
            start = mid + 1;
            ans = mid;
        }
        else // If mid*mid is greater than x
            end = mid - 1;
    }
    return ans;
}

static void tester(){
    int a = 0;
    int b = 1;
    if(a<b);
}