///////////////////////////////////////////////////////////////////////////////
// disks.hpp
//
// Definitions for two algorithms that each solve the
// alternating disks problem.
//
// As provided, this header has four functions marked with
// TODO comments.
// You need to write in your own implementation of these
// functions.
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <sstream>
#include <string>
#include <vector>

// State of one disk, either light or dark.
enum disk_color { DISK_LIGHT, DISK_DARK };

// Data structure for the state of one row of disks.
class disk_state {
private:
  std::vector<disk_color> _colors;

public:

  disk_state(size_t light_count)
    : _colors(light_count * 2, DISK_LIGHT) {

      assert(dark_count() > 0);

      for (size_t i = 1; i < _colors.size(); i += 2) {
        _colors[i] = DISK_DARK;
      }
  }

  // Equality operator for unit tests.
  bool operator== (const disk_state& rhs) const {
    return std::equal(_colors.begin(), _colors.end(), rhs._colors.begin());
  }

  size_t total_count() const {
    return _colors.size();
  }

  size_t dark_count() const {
    return total_count() / 2;
  }

  size_t light_count() const {
    return dark_count();
  }

  bool is_index(size_t i) const {
    return (i < total_count());
  }

  disk_color get(size_t index) const {
    assert(is_index(index));
    return _colors[index];
  }

  void swap(size_t left_index) {
    assert(is_index(left_index));
    auto right_index = left_index + 1;
    assert(is_index(right_index));
    std::swap(_colors[left_index], _colors[right_index]);
  }

  std::string to_string() const {
    std::stringstream ss;
    bool first = true;
    for (auto color : _colors) {
      if (!first) {
        ss << " ";
      }

      if (color == DISK_LIGHT) {
        ss << "L";
      } else {
        ss << "D";
      }

      first = false;
    }
    return ss.str();
  }

  // Return true when this disk_state is in alternating format. That means
  // that the first disk at index 0 is light, the second disk at index 1
  // is dark, and so on for the entire row of disks.
  bool is_alternating() const {
    for (size_t x = 0; x < _colors.size(); x++){
      // if any even index is dark, return false
      if(x%2 == 0) {
        if(_colors[x] == DISK_DARK) return false;
      }
      // if any odd index is light, return false;
      else {
        if (_colors[x] == DISK_LIGHT) return false;
      }
    }
    return true;
  }

  // Return true when this disk_state is fully sorted, with all light disks
  // on the left (low indices) and all dark disks on the right (high
  // indices).
  bool is_sorted() const {
    for (size_t x = 0; x < total_count(); x++){
      // checking the lower half of the disks and returning false if any are dark
      if (x < total_count()/2) {
        if(_colors[x] == DISK_DARK) return false;
      }
      // checking the upper half of the disks and returning false if any are light
      else {
        if(_colors[x] == DISK_LIGHT) return false;
      }
    }
   return true;
  }
};

// Data structure for the output of the alternating disks problem. That
// includes both the final disk_state, as well as a count of the number
// of swaps performed.
class sorted_disks {
private:
  disk_state _after;
  unsigned _swap_count;

public:

  sorted_disks(const disk_state& after, unsigned swap_count)
    : _after(after), _swap_count(swap_count) { }

  sorted_disks(disk_state&& after, unsigned swap_count)
    : _after(after), _swap_count(swap_count) { }

  const disk_state& after() const {
    return _after;
  }

  unsigned swap_count() const {
    return _swap_count;
  }
};

// Algorithm that sorts disks using the left-to-right algorithm.
sorted_disks sort_left_to_right(const disk_state& before) {

  disk_state temporary = before;
  int swapCounter = 0; // keep track of number of swaps

  for (size_t x = 0; x < temporary.total_count(); x++){
    for (size_t y = x; y < temporary.total_count()-1; x++){ // -1 for bound error
      if (temporary.get(y) > temporary.get(y+1)) { // if j is dark and j+1 is light, swap
        temporary.swap(y);
        swapCounter++;
      }
    }
  }
  return sorted_disks(disk_state(temporary), swapCounter);
}

// Algorithm that sorts disks using the lawnmower algorithm.
sorted_disks sort_lawnmower(const disk_state& before) {

  disk_state temporary = before;
  int swapCounter = 0; // keep track of number of swaps

  for (int x = 0; x < (int) temporary.total_count()/2; x++){
    // forward for-loop
    for (int y = 0; y < (int) temporary.total_count()-1; y++){
      if(temporary.get(y) > temporary.get(y+1)){
        temporary.swap(y);
        swapCounter++;
      }
    }
    // backward for-loop
    for(int z = temporary.total_count(); z > 0; z--){
      if(temporary.get(z-1) > temporary.get(z)){ // swap if left index is greater than right index
        temporary.swap(z-1);
        swapCounter++;
      }
    }
  }
  return sorted_disks(disk_state(temporary), swapCounter);
}
