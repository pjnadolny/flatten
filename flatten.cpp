// Name:   flatten
// Author: Paul Nadolny, https://github.com/pjnadolny
// Date:   2019-Jul-19
//
// Purpose: Show two ways of flattening a nested vector of integers.
//          One way is recursive, the other is iterative.
// Note:    I'm showing the overall algorithms without necessarily
//          optimizing the running time or space management.
//
// To compile: g++ -std=c++17 -o flatten flatten.cpp

// Copyright (C) 2019  Paul Nadolny
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.


#include <iostream>
#include <typeinfo>
#include <any>
#include <stack>
#include <vector>


// -----------------------------------------------------------------------
// Use a vector of any to represent a nested vector
typedef std::vector<std::any> Nested;
typedef std::vector<int> Flat;


// -----------------------------------------------------------------------
// flatten1() takes two arguments:
//   Arg 1: the nested vector to be flattened
//   Arg 2: the resulting flat vector
// flatten1() returns a boolean:
//   true:  the flat vector is valid
//   false: a problem was encountered and the flat vector is invalid
// Algorithm: traverse the vector, examining each element
//   If an int is found, copy it to the flat vector
//   If a vector is found, call flatten() recursively on it
bool flatten1(const Nested& nested_vector, Flat& flat) {
  bool valid = true;

  for (auto& element : nested_vector) {
    if (element.type() == typeid(int)) {

      // Found an int, add it to the flat vector
      flat.push_back(std::any_cast<int>(element));

    } else if (element.type() == typeid(Nested)) {

      // Found a vector, flatten it recursively
      if (false == flatten1(std::any_cast<Nested>(element), flat)) {
        valid = false;
      }

    } else {

      // Found something other than a vector or an int
      valid = false;

    }
  }
  return valid;
}  // end of flatten1()


// -----------------------------------------------------------------------
// flatten2() takes two arguments:
//   Arg 1: the nested vector to be flattened
//   Arg 2: the resulting flat vector
// flatten2() returns a boolean:
//   true:  the flat vector is valid
//   false: a problem was encountered and the flat vector is invalid
// Algorithm: traverse the vector, examining each element
//   If an int is found, copy it to the flat vector
//   If a vector is found, remember the current state in a stack,
//     and move the newly found vector to the current state.
bool flatten2(Nested& arg, Flat& flat) {
  bool valid = true;

  // These two variables comprise the state
  Nested& nv = arg;   // nested vector
  std::size_t i = 0;  // index to current item of nested vector

  typedef std::tuple<Nested, std::size_t> State;
  State state;

  std::stack<State> state_stack;  // This stack holds the previous states

  int count = 0;  // loop counter

  // Loop while not done with current vector, or there's a state on the stack
  while (i < nv.size() || !state_stack.empty() ) {

    if (i < nv.size()) { // Not done with vector

      if (nv.at(i).type() == typeid(int)) {
        // Found an int, so move it to the flat vector
        flat.push_back(std::any_cast<int>(nv.at(i)));
        // Move to next item
        ++i;
      } else if (nv.at(i).type() == typeid(Nested)) {
        // Found a vector to work on, so push current state for later processing
        state = std::make_tuple(nv, i);
        state_stack.push(state);
        // Move the found vector to the current state
        nv = std::any_cast<Nested>(nv.at(i));
        i = 0;
      } else {
        // Found an unknown type
        valid = false;
        // Move to next item anyway, process as much as possible
        ++i;
      }

    } else { // Done with vector, so pop previous state from stack

      // Pop the old state to the current state
      state = state_stack.top();
      state_stack.pop();
      nv = std::get<0>(state);
      i = std::get<1>(state);
      // Already processed this item, so move to next item
      ++i;

    }

    if (++count == 100) {
      break; // stop runaway loop during development
    }
  } // end while loop

  return valid;
}  // end  of flatten2()


// -----------------------------------------------------------------------
int main(int argc, char* argv[]) {
  Flat result1;
  Flat result2;
  bool valid1;
  bool valid2;

  // Set up some test data
  Nested a;
  Nested b;
  Nested c;
  Nested d;
  Nested e;
  Nested f;
  Nested g;
  Nested h;
  Nested empty;

  // Test data meaning: The last two digits are the sequence left-to-right
  // The hundreds digit is the nesting level,
  // or the "level" if you draw it like a general tree
  std::cout << "Test data: [[] 1 [102 [203 204 [305 306]] 107] [108 109] 10 [[211]] [112] 13 []]" << std::endl;

  h.push_back(305);
  h.push_back(306);

  f.push_back(203);
  f.push_back(204);
  f.push_back(h);

  b.push_back(102);
  b.push_back(f);
  b.push_back(107);

  c.push_back(108);
  c.push_back(109);

  g.push_back(211);

  d.push_back(g);

  e.push_back(112);

  a.push_back(empty);
  a.push_back(1);
  a.push_back(b);
  a.push_back(c);
  a.push_back(10);
  a.push_back(d);
  a.push_back(e);
  a.push_back(13);
  a.push_back(empty);

  std::cout << "------------------------------------------" << std::endl;
  std::cout << "Calling recursive flatten" << std::endl;

  valid1 = flatten1(a, result1);

  if (valid1) {
    std::cout << "Flattened: ";
    for (int i : result1) {
      std::cout << i << " ";
    }
    std::cout << std::endl;
  } else {
    std::cout << "ERROR: Could not flatten properly" << std::endl;
  }

  std::cout << "------------------------------------------" << std::endl;
  std::cout << "Calling iterative flatten" << std::endl;

  valid2 = flatten2(a, result2);

  if (valid2) {
    std::cout << "Flattened: ";
    for (int i : result2) {
      std::cout << i << " ";
    }
    std::cout << std::endl;
  } else {
    std::cout << "ERROR: Could not flatten properly" << std::endl;
  }

  if (valid1 && valid2) {
    std::cout << "------------------------------------------" << std::endl;
    if (result1 == result2) {
      std::cout << "Results match" << std::endl;
    } else {
      std::cout << "Results do not match" << std::endl;
    }
  }

  return 0;
}
