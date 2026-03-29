## Problem Statement 
The goal of this program is to convert a base-10 number into another base between 2 and 16. If the user enters a base outside this range, the program should show an error message.

## Solution
The solution uses a recursive formula function 'to_base_n()'. The function repeatedly divides the number by the base and calls itself with the quotient. The remainder is used to determine the digit to print. A charater array is used to represent difits from 0-15, allowing for hexadecimal values A-F.  The recursive makes sure the digits are printed in the right order. 

### Pros
- Supports multiple bases
- Clean and simple recursive logic
- Easy to understand if you know recursion

### Cons
- Recursion can be difficult
- Uses stack memory which isnt efficient for large numbers
