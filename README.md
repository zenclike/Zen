# Zen Language
Zen is a procedural programming language based on C.
Most of the commands are shortened like this command, `printw`.
`printw` stands for **print** and **raw** which prints a string literal **NOT** a string variable.
Here is an another example, `txt`.
`txt` stands for **read text file** which is not the same as the original form.
Here is all what commands stand for.
```
* core
 deflib :: defines a library
 defpac :: defines a package
 func :: creates a function
 end :: ends a statement, loop or function
 run :: runs a function with an iteration
 continue :: skips commands ahead and runs loop again
 break :: kills the loop
* library io
 printw :: prints a string literal
 printr :: prints a string variable
 printd :: prints an integer variable
 printf :: prints a float variable
 deftxt :: defines a file variable name
 txt :: reads the text file and checks if file does not exists
 getlnf :: reads a line from the file and set the content to a string variable
 wrt :: writes the text file
 wrtctf :: writes a content to the file
 sys :: runs a bash command from a string literal or a string variable
 clear :: clear the screen
* library str
 defstr :: defines a string variable
 str :: sets a string content to a string variable
 streq :: checks if a string variable is the same as the another string variable
 strneq :: checks if a string variable is not the same as the another string variable
 strfch :: removes the first character of the string variable
 strlch :: removes the last character of the string variable
 strlen :: gets the length of a string variable and sets the length to an integer variable
* library int
 defint :: defines an integer variable
 int :: sets an integer content to an integer variable
 inteq :: check if an integer variable is the same as the another integer variable
 intneq :: check if an integer variable is not the same as the another integer variable
 intlw :: check if an integer variable is lower than the another integer variable
 intmr :: check if an integer variable is higher than the another integer variable
 add :: adds an integer literal or integer variable to an integer variable
 sub :: subtracts an integer literal or integer variable to an integer variable
 mul :: multiplies an integer literal or integer variable to an integer variable
 div :: divides an integer literal or integer variable to an integer variable
 rand :: randoms a number between 1 to your number that you choose and set the value to an integer variable
 strint :: changes from a string to an integer and set the value to an integer variable
 rem :: gets the remainings from the division and set the value to an integer variable
*library flt
 defflt :: defines a float variable
 fltadd :: adds a float literal or float variable to an float variable
 fltsub :: subtracts a float literal or float variable to the float variable
 fltmul :: multiplies a float literal or float variable to the float variable
 fltdiv :: divides a float literal or float variable to the float variable
 cos :: gets the float variable and changes to radians and set the value to the float variable
 tan :: gets the float variable and changes to tangent of float variable's value and set the value to the float variable
 sin :: gets the float variable and changes to the value of the sine of float variable's value and set the value to the float variable
 abs :: gets the absolute value of that float variable's value and set the value to the float variable
 sqrt :: gets the square root value of that float variable's value and set the value to the float variable
 ceil :: rounds the value of that float variable's value up to the nearest number and set the value to the float variable
 floor :: rounds the value of that float variable's value down to the nearest number and set the value to the float variable
 flteq :: check if a float variable is the same as the another float variable
 fltneq :: check if a float variable is not the same as the another float variable
 fltlw :: check if a float variable is lower than another float variable
 fltmr :: check if a float variable is higher than the another float variable
```
There is a few types of commands in **Zen**, here is all of them.
```
* core
 deflib <str> :: function
 defpac <str> :: function
 func <str> :: function
 end :: function
 run <str>, <int> :: function
 continue :: function
 break :: function
* library io
 printw <str> :: function
 printr <str> :: function
 printd <str> :: function
 deftxt <str> :: function
 txt <str>, <str> :: statement
 getlnf <str>, <str> :: statement
 wrt <str> :: function
 wrtctf <str> :: function
 sys <str> :: function
 clear :: function
* library str
 defstr <str> :: function
 str <str>, <str> :: function
 streq <str>, <str> :: statement
 strneq <str>, <str> :: statement
 strfch <str>, <str> :: function
 strlch <str> :: function
 strlen <str> :: function
* library int
 defint <str> :: function
 int <str>, <int> :: function
 inteq <str>, <str> :: statement
 intneq <str>, <str> :: statement
 intlw <str>, <str> :: statment
 intmr <str>, <str> :: statement
 add <str>, <int> :: function
 sub <str>, <int> :: function
 mul <str>, <int> :: function
 div <str>, <int> :: function
 rand <str>, <int> :: function
 strint <str>, <str> :: function
 rem <str>, <str> :: function
* library flt
 defflt <str> :: function
 flt <str> :: function
 fltadd <str>, <flt> :: function
 fltsub <str>, <flt> :: function
 fltmul <str>, <flt> :: function
 fltdiv <str>, <flt> :: function
 cos <str> :: function
 tan <str> :: function
 sin <str> :: function
 abs <str> :: function
 sqrt <str> :: function
 ceil <str> :: function
 floor <str> :: function
 flteq <str> :: function
 fltneq <str> :: function
 fltlw <str> :: function
 fltmr <str> :: function
```
What is a **function**?
A function is like a group of commands turns into a single command.
What is a **statement**?
A statement is like **If I am hungry, where should I go?**.
## Formatting
Formatting (^) is like if you want to add 2 strings together but you don't wan't to use a string literal.
**Note** :: Some formatting does not need a hat (^).
Here's an example of how to formatting strings in string functions.
```zf
deflib "str";
defstr "a";
defstr "b";

str "a", "z";
str "b", "^a";
stradd "b", "^a";
```
Here's an example of how to formatting strings in integer functions.
```zf
deflib "int";
defint "a";
defint "b";

int "a", 1;
int "b", 1;
add "b", "a";
```
***Copyrighted (C) The Zen Team 2025***
