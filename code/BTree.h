#ifndef BTREE_H__
#define BTREE_H__

#include <string>
#include <memory>
#include <vector>

using namespace std;


  // Binary search tree:
  //
  // From any subtree node t, the left subtree's data values must be
  // less than t's data value. The right subtree's data values must be
  // greater than or equal to t's data value.
class BTree {
public:
  // constructor, initialize class variables and pointers here if need.
  // Initially set root_ptr_ to a null shared_ptr of node
  BTree();

  // deconstructor, if you are using shared_pointers you don't need to keep track of
  // freeing memory and you can leave this blank
  ~BTree();

  // you can add add more public member variables and member functions here if you need

private:
  // you can add add more private member variables and member functions here if you need
};

#endif  // BTREE_H__