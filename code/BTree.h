#ifndef BTREE_H__
#define BTREE_H__

#include <string>
#include <memory>
#include <vector>

using namespace std;

// The BTREE_ORDER definition sets the B-tree order (using the Knuth
// definition). This is the number of children the node can have. The
// number of keys is one less than this value.
#define BTREE_ORDER 5

// Note that the keys and children arrays are OVERSIZED to allow for
// some approaches to work, where nodes are allowed to temporarily
// have too many keys or children. You do not have to use the extra
// slots if you don't want to. Don't take this as a subtle hint that I
// want you to do it this way.
// 
// A valid btree node can have at most:
//   BTREE_ORDER-1 keys.
//   BTREE_ORDER children.
struct btree {
  // num_keys is the number of in keys array that are currently valid.
  int num_keys;
  // keys is an array of values. valid indexes are in [0..num_keys)
  int keys[BTREE_ORDER];
  // is_leaf is true if this is a leaf, false otherwise
  bool is_leaf;
  // children is an array of pointers to b-tree subtrees. valid
  // indexes are in [0..num_keys].
  shared_ptr<btree> children[BTREE_ORDER + 1];
  // NOTE to self : let's try to avoid customizing btree nodes
  // this will simplify tree init (the problem is there isn't any)
  // parent of current node, is null when this node is root
  weak_ptr<btree> parent;
};

class BTree {
public:
  // constructor, initialize class variables and pointers here if need.
  BTree();

  // deconstructor, if you are using shared_pointers you don't need to keep track of
  // freeing memory and you can leave this blank
  ~BTree();

  // insert adds the given key into a b-tree rooted at 'root'.  If the
  // key is already contained in the btree this should do nothing.
  // 
  // On exit: 
  // -- the 'root' pointer should refer to the root of the
  //    tree. (the root may change when we insert or remove)
  // -- the btree pointed to by 'root' is valid.
  // pass root by ref?
  //  can we modify the original tree made in unittest functions?
  //  is this even desired?
  void insert(shared_ptr<btree>& root, int key);

  // remove deletes the given key from a b-tree rooted at 'root'. If the
  // key is not in the btree this should do nothing.
  //
  // On exit:
  // -- the 'root' pointer should refer to the root of the
  //    tree. (the root may change when we insert or delete)
  // -- the btree pointed to by 'root' is valid.
  void remove(shared_ptr<btree>& root, int key);

  // find locates the node that either: (a) currently contains this key,
  // or (b) the node that would contain it if we were to try to insert
  // it.  Note that this always returns a non-null node.
  shared_ptr<btree> find(shared_ptr<btree>& root, int key);

  // count_nodes returns the number of nodes referenced by this
  // btree. If this node is NULL, count_nodes returns zero; if it is a
  // root, it returns 1; otherwise it returns 1 plus however many nodes
  // are accessable via any valid child links.
  int count_nodes(shared_ptr<btree> root);

  // count_keys returns the total number of keys stored in this
  // btree. If the root node is null it returns zero; otherwise it
  // returns the number of keys in the root plus however many keys are
  // contained in valid child links.
  int count_keys(shared_ptr<btree> root);

  // you can add add more public member variables and member functions here if you need

private:
  // you can add add more private member variables and member functions here if you need
  // ## inter node operations ##
  void ensure_initialized(shared_ptr<btree> root);
  void set_parent(shared_ptr<btree> child, shared_ptr<btree> parent);
  shared_ptr<btree> find_recursively(shared_ptr<btree> current, int key);
  // param `key_position` is the index of our key of interest in target.keys
  // returns the leaf node that has the next higher key than k.o.i.
  shared_ptr<btree> find_node_with_next_key(const shared_ptr<btree>& target, int key_position);
  // return param `left` or `right` could be nullptr
  // return param `target_pos` is the index of the pointer from parent to target
  void find_siblings(const shared_ptr<btree>& target, shared_ptr<btree>& left, shared_ptr<btree>& right, int& target_pos);
  // param `current` cannot be null
  // this funtion is called recursively up the tree
  void fix_under_full(shared_ptr<btree> current);
  // param `parent_key_pos` is the index of the key in parent that is between `left` and `right` pointers
  void rotate_left(shared_ptr<btree> left, shared_ptr<btree> right, shared_ptr<btree>& parent, int parent_key_pos);
  void rotate_right(shared_ptr<btree> left, shared_ptr<btree> right, shared_ptr<btree>& parent, int parent_key_pos);
  void merge(shared_ptr<btree> left, shared_ptr<btree> right, shared_ptr<btree>& parent, int parent_key_pos);
  // if current is overfull (num_keys = BTREE_ORDER)
  // this function will split current into two half full nodes and insert middle key into parent
  // then `try_split` parent recursively
  void try_split(shared_ptr<btree> current);
  // ## node manipulation helpers ##
  // this function returns the index of the first item that is larger or equal to `key`,
  // if none is found `target_range` is returned
  void node_key_search(const shared_ptr<btree>& target, int key, int& position, bool& found_exact);
  // this group of functions finds appropriate position to insert key
  // optionally this function can insert a node pointer to the corresponding position
  // returns the index of the insertion
  int node_insert(shared_ptr<btree> target, int key);
  int node_insert(shared_ptr<btree> target, int key, shared_ptr<btree> child);
  // both functions below scans and inserts `value` at `position`, shifts all elements after one
  // these functions assumes there is room for new element
  void array_insert(shared_ptr<btree> target[],int target_range, int position, shared_ptr<btree> value);
  void array_insert(int target[],int target_range, int position, int value);
  // both functions below removes element at param `position`
  // shifts following elements left but DOES NOT change `target_range` counter
  void array_remove(shared_ptr<btree> target[],int target_range, int position);
  void array_remove(int target[],int target_range, int position);

  shared_ptr<btree> root_;
};

#endif  // BTREE_H__