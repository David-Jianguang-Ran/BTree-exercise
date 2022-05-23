#include "BTree.h"

// constructor, initialize class variables and pointers here if need.
BTree::BTree(){
  // Your code here
  root_ = nullptr;
}

//deconstructor,
BTree::~BTree(){
}

void BTree::insert(shared_ptr<btree>& root, int key){
  ensure_initialized(root);
  if (root == nullptr) {
    shared_ptr<btree> new_root (new btree);
    new_root->is_leaf = true;
    new_root->num_keys = 0;
    node_insert(new_root, key);
    root = new_root;
    root_ = new_root;
  }
  shared_ptr<btree> insert_at;
  insert_at = find_recursively(root, key);
  if (insert_at->is_leaf) {
    node_insert(insert_at, key);
    try_split(insert_at);
    root = this->root_;  // in case root has changed update the passed in root to new root stored in this->root_
  }
}

void BTree::remove(shared_ptr<btree>& root, int key){
  ensure_initialized(root);
  shared_ptr<btree> found;
  found = find_recursively(root, key);
  int position;
  bool found_exact;
  node_key_search(found, key, position, found_exact);
  if (found_exact && found->is_leaf) {
    array_remove(found->keys, found->num_keys, position);
    found->num_keys--;
    fix_under_full(found);
  } else if (found_exact) {
    shared_ptr<btree> next_leaf;
    next_leaf = find_node_with_next_key(found, position);
    found->keys[position] = next_leaf->keys[0];
    array_remove(next_leaf->keys, next_leaf->num_keys, 0);
    next_leaf->num_keys--;
    fix_under_full(next_leaf);
  }
  root = this->root_;
}

shared_ptr<btree> BTree::find(shared_ptr<btree>& root, int key){
  ensure_initialized(root);
  shared_ptr<btree> found;
  found = find_recursively(root, key);
  return found;
}

int BTree::count_nodes(shared_ptr<btree> root){
  if (root == nullptr) {
    return 0;
  } else if (root->is_leaf) {
    // base case, leaf is 1 node
    return 1;
  } else {
    int total = 1;
    for (int i = 0; i <= root->num_keys; i++) {
      total += count_nodes(root->children[i]);
    }
    return total;
  }
}

int BTree::count_keys(shared_ptr<btree> root){
  if (root == nullptr) {
    return 0;
  } else if (root->is_leaf) {
    // base case, no more children
    return root->num_keys;
  } else {
    int total = root->num_keys;
    for (int i = 0; i <= root->num_keys; i++) {
      total += count_keys(root->children[i]);
    }
    return total;
  }
}

// since each unittest always stick to a single test tree,
// here can make sure init is done only once when a public function is called
void BTree::ensure_initialized(shared_ptr<btree> root) {
  if (this->root_ != root) {
    set_parent(root, nullptr);  // <= using root_ as nullptr
    this->root_ = root;
  }
}

void BTree::set_parent(shared_ptr<btree> child, shared_ptr<btree> parent) {
  if (child == nullptr) {
    return;
  } else if (parent != nullptr) {
    child->parent = parent;
  }
  if (child->is_leaf) {
    return;  // base case, leaf node have no children to link
  } else {
    for (int i = 0; i <= child->num_keys; i++) {
      set_parent(child->children[i], child);
    }
  }
}

// this function is where recursive tree traversal
shared_ptr<btree> BTree::find_recursively(shared_ptr<btree> current, int key) {
  // guard case against empty root
  shared_ptr<btree> found;
  if (current == nullptr) {
    return found;
  }
  int position;
  bool found_exact;
  node_key_search(current, key, position, found_exact);
  if (current->is_leaf || found_exact) {
    found = current;
  } else {
    found = find_recursively(current->children[position], key);
  }
  return found;
}

// this function returns a pointer to the next leaf node
// param `target` MUST NOT be a leaf node
// param `key_position` is the index of key in target.keys
shared_ptr<btree> BTree::find_node_with_next_key(const shared_ptr<btree>& target, int key_position) {
  shared_ptr<btree> current = target->children[key_position + 1];
  while (!current->is_leaf) {
    current = current->children[0];
  }
  return current;
}

// return param `left` or `right` could be nullptr
// return param `target_pos` is the index of the pointer from parent to target
void BTree::find_siblings(const shared_ptr<btree>& target, shared_ptr<btree>& left, shared_ptr<btree>& right, int& target_pos) {
  if (target->parent.expired()) {
    return;
  }
  shared_ptr<btree> parent = target->parent.lock();
  for (int i = 0; i < parent->num_keys; i++) {
    if (parent->children[i] == target) {
      right = parent->children[i + 1];
      target_pos = i;
    } else if (parent->children[i + 1] == target) {
      left = parent->children[i];
      target_pos = i + 1;
    }
  }
}

void BTree::fix_under_full(shared_ptr<btree> current) {
  // base case, current is root or current is not under full
  if (current->parent.expired()) {
    return;
  } else if (current->num_keys >= (BTREE_ORDER / 2)) {
    return;
  }
  // fix under full node
  shared_ptr<btree> left_sibling;
  shared_ptr<btree> right_sibling;
  shared_ptr<btree> parent = current->parent.lock();
  int found_position; // this is the index of the pointer from parent to current
  find_siblings(current, left_sibling, right_sibling, found_position);
  if (right_sibling != nullptr && right_sibling->num_keys > (BTREE_ORDER / 2)) {
    rotate_left(current, right_sibling, parent, found_position);
  } else if (left_sibling != nullptr && left_sibling->num_keys > (BTREE_ORDER / 2)) {
    rotate_right(left_sibling, current, parent, found_position - 1);
  } else if (right_sibling != nullptr) {
    merge(current, right_sibling, parent, found_position);
  } else {
    merge(left_sibling, current, parent, found_position - 1);
  }
}

// takes a key from right, add a key to left (not the same key)
void BTree::rotate_left(shared_ptr<btree> left, shared_ptr<btree> right, shared_ptr<btree>& parent, int parent_key_pos) {
  node_insert(left, parent->keys[parent_key_pos], right->children[right->num_keys]);
  parent->keys[parent_key_pos] = right->keys[0];
  array_remove(right->keys, right->num_keys, 0);
  array_remove(right->children, right->num_keys + 1, 0);
  right->num_keys--;
}

// takes a key from left, add a key to right (not the same key)
void BTree::rotate_right(shared_ptr<btree> left, shared_ptr<btree> right, shared_ptr<btree>& parent, int parent_key_pos) {
  node_insert(right, parent->keys[parent_key_pos], left->children[left->num_keys]);
  parent->keys[parent_key_pos] = left->keys[left->num_keys - 1];
  left->num_keys--;
}

void BTree::merge(shared_ptr<btree> left, shared_ptr<btree> right, shared_ptr<btree>& parent, int parent_key_pos) {
  left->keys[left->num_keys] = parent->keys[parent_key_pos];
  left->num_keys++;
  for (int right_i = 0; right_i < right->num_keys; right_i++) {
    left->keys[left->num_keys] = right->keys[right_i];
    left->children[left->num_keys] = right->children[right_i];
    left->num_keys++;
  }
  left->children[left->num_keys] = right->children[right->num_keys];
  array_remove(parent->keys, parent->num_keys, parent_key_pos);
  array_remove(parent->children, parent->num_keys + 1, parent_key_pos + 1);
  parent->num_keys--;
  if (!parent->parent.expired()) {
    fix_under_full(parent);
  } else if (parent->num_keys == 0){
    this->root_ = left;
  }
}

// this function can only split nodes that are overfull by 1
void BTree::try_split(shared_ptr<btree> current) {
  if (current->num_keys != BTREE_ORDER) {
    return;
  }
  int split_at = BTREE_ORDER / 2;
  int split_key = current->keys[split_at];
  // make new left node and clear pointer from current node
  shared_ptr<btree> new_right(new btree);
  new_right->num_keys = 0;
  new_right->is_leaf = current->is_leaf;
  for (int i = split_at + 1; i <= BTREE_ORDER; i++) {
    if (i != BTREE_ORDER) {
      new_right->keys[i - split_at - 1] = current->keys[i];
      new_right->num_keys++;
    }
    if (!new_right->is_leaf) {
      new_right->children[i - split_at - 1] = current->children[i];
      current->children[i] = nullptr;
    }
  }
  current->num_keys = split_at;
  // raise split key to parent
  if (!current->parent.expired()) {
    new_right->parent = current->parent;
    node_insert(current->parent.lock(), split_key, new_right);
    try_split(current->parent.lock());
  } else {
    // no parent, make new root parent, no more recursion
    shared_ptr<btree> new_root (new btree);
    new_root->is_leaf = false;
    new_root->num_keys = 1;
    new_root->keys[0] = split_key;
    new_root->children[0] = current;
    new_root->children[1] = new_right;
    new_right->parent = new_root;
    current->parent = new_root;
    this->root_ = new_root;
  }
}

// this function returns the index of the first item that is larger or equal to `key`,
// if an exact match is found `found_exact` would be set to true
// if none is found `target_range` is returned
void BTree::node_key_search(const shared_ptr<btree>& target, int key, int& position, bool& found_exact) {
  // find insert location
  position = 0;
  while (position < target->num_keys) {
    if (target->keys[position] < key) {
      position++;
    } else {
      found_exact = (target->keys[position] == key);
      return;
    }
  }
  found_exact = false;
}

// this group of functions finds appropriate position to insert key
// optionally this function can insert a node pointer to the corresponding position
// returns the index of the insertion
int BTree::node_insert(shared_ptr<btree> target, int key) {
  int position;
  bool found_exact;
  node_key_search(target, key, position, found_exact);
  array_insert(target->keys, target->num_keys, position, key);
  target->num_keys++;
  return position;
}
// NOTE the child is inserted in index position + 1
// this mean the child node should be a node that has keys greater than `key`
int BTree::node_insert(shared_ptr<btree> target, int key, shared_ptr<btree> child) {
  int position;
  bool found_exact;
  node_key_search(target, key, position, found_exact);
  array_insert(target->keys, target->num_keys, position, key);
  array_insert(target->children, target->num_keys + 1, position + 1, child);
  target->num_keys++;
  return position;
}

// both functions of this name iterates backwards to shift elements
// then insert `value` at `position`
// these functions do no bounds checking at all
// TODO is there a way to avoid writing the exact function twice?
void BTree::array_insert(int *target, int target_range, int position, int value) {
  int i;
  for (i = target_range - 1; i >= position; i--) {
    target[i + 1] = target[i];
  }
  target[position] = value;
}

void BTree::array_insert(shared_ptr <btree> *target, int target_range, int position, shared_ptr <btree> value) {
  int i;
  for (i = target_range - 1; i >= position; i--) {
    target[i + 1] = target[i];
  }
  target[position] = value;
}

void BTree::array_remove(int *target, int target_range, int position) {
  for (int i = position + 1; i < target_range; i++) {
    target[i - 1] = target[i];
  }
}

void BTree::array_remove(shared_ptr<btree> *target, int target_range, int position) {
  for (int i = position + 1; i < target_range; i++) {
    target[i - 1] = target[i];
  }
}
