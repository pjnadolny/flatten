# Flatten
by Paul Nadolny, https://github.com/pjnadolny

Also on my gist: https://gist.github.com/pjnadolny
## 1 Problem Statement
Write some code that will flatten an array of arbitrarily nested arrays of integers into a flat array of integers. e.g. [[1,2,[3]],4] -> [1,2,3,4].

When writing this code, you can use any language you're comfortable with. The code must be well tested and documented. Please include unit tests and any documentation you feel is necessary. In general, treat the quality of the code as if it was ready to ship to production.

Try to avoid using language defined methods like Ruby's Array#flatten.
## 2 Solutions
I decided to use C++17 and represent the array as a std::vector of std::any. I use typeid() to check whether I'm looking at a vector or an integer (or something else).

I implemented two different algorithms: recursive and iterative.

### 2.1 Recursive

The first function `flatten1()` implements a recursive algorithm:

Loop through the nested vector, examining each element
- If an integer is found, copy it to the flat vector
- If a vector is found, call `flatten1()` recursively on it

### 2.2 Iterative

The second function `flatten2()` implements an iterative algorithm:

Loop through the nested vector, examining each element
- If an integer is found, copy it to the flat vector
- If a vector is found, remember the current state in a stack,
and move the newly found vector to the current state.
- If we hit the end of the current array, pop the previous state (if any) from the stack.

Note: the state that gets pushed and popped on the stack is the current vector and the index to an element.

## 3 Testing

I tested my functions by calling them on multiple test cases, representing various levels of nesting, various combinations of integers and vectors, and the empty vector.

I included the following single test case in the main code as a demonstration:

```
[[] 1 [102 [203 204 [305 306]] 107] [108 109] 10 [[211]] [112] 13 []]
```

It flattens to:
```
[1 102 203 204 305 306 107 108 109 10 211 112 13]
```
## 4 Implementation Notes
### 4.1 Compilation

To compile:
```
$ g++ -std=c++17 -o flatten flatten.cpp
```

### 4.2 My system

My system is macOS HighSierra 10.13.6

The compiler version:
```
Configured with: --prefix=/Applications/Xcode.app/Contents/Developer/usr --with-gxx-include-dir=/usr/include/c++/4.2.1
Apple LLVM version 10.0.0 (clang-1000.11.45.5)
Target: x86_64-apple-darwin17.7.0
Thread model: posix
InstalledDir: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin
```
