/**
 * This File shows the implementation details of
 * DictionaryTrie class methods, which are declared
 * in "DictionaryTrie.hpp"
 *
 * Author: Yuening YANG, Shenlang ZHOU
 * Email: y3yang@ucsd.edu, shzhou@ucsd.edu
 */
#include "DictionaryTrie.hpp"
#include <iostream>

/* It is the constructor*/
DictionaryTrie::DictionaryTrie() { root = 0; }

/* This is the function to insert the word into the trie.
    arguments: word to insert, frequency of that word
    return: true if insertion is successful, false otherwise
 */
bool DictionaryTrie::insert(string word, unsigned int freq) {
    if (word.length() == 0) {
        return false;
    }

    Node* ptr;
    char letter = word[0];
    int i = 1;
    if (root == 0) {
        root = new Node(letter);
        ptr = root;
    } else {
        ptr = root;
        while (true) {
            if (letter < ptr->letter) {
                // search the left subtree
                if (ptr->left != nullptr) {
                    ptr = ptr->left;
                } else {
                    // not found, create new left child
                    ptr->left = new Node(letter);
                    ptr->left->parent = ptr;
                    ptr = ptr->left;
                    break;
                }
            } else if (letter > ptr->letter) {
                // search the right subtree
                if (ptr->right != nullptr) {
                    ptr = ptr->right;
                } else {
                    // not found, create new right child
                    ptr->right = new Node(letter);
                    ptr->right->parent = ptr;
                    ptr = ptr->right;
                    break;
                }
            } else {
                // search the middle tree
                if (i == word.length()) {
                    if (ptr->is_word) {
                        // the word is already in the trie
                        return false;
                    } else {
                        // the word is not in the trie. mark it as 'word'
                        ptr->is_word = true;
                        ptr->freq = freq;
                        while (ptr != nullptr) {
                            if (freq > ptr->maxFreq) {
                                ptr->maxFreq = freq;
                            } else {
                                break;
                            }
                            ptr = ptr->parent;
                        }
                        return true;
                    }
                } else {
                    // next letter
                    letter = word[i];
                    i++;
                    if (ptr->mid != nullptr) {
                        ptr = ptr->mid;
                    } else {
                        // not found, create new mid node
                        ptr->mid = new Node(letter);
                        ptr->mid->parent = ptr;
                        ptr = ptr->mid;
                        break;
                    }
                }
            }
        }
    }

    // loop through the word, create new mid nodes for the rest letters
    while (i < word.length()) {
        letter = word[i];
        i++;
        ptr->mid = new Node(letter);
        ptr->mid->parent = ptr;
        ptr = ptr->mid;
    }
    // set the last letter as 'word'. store its frequency
    ptr->is_word = true;
    ptr->freq = freq;
    while (ptr != nullptr) {
        if (freq > ptr->maxFreq) {
            ptr->maxFreq = freq;
        } else {
            break;
        }
        ptr = ptr->parent;
    }
    return true;
}

/* This is the function to find whether the word is in the trie.
    arguments: the target word
    return true if the word is found, false otherwise
 */
bool DictionaryTrie::find(string word) const {
    if (word.length() == 0) {
        return false;
    }
    if (root == 0) {
        return false;
    }

    Node* ptr = root;
    char letter = word[0];
    int i = 1;
    while (true) {
        if (letter < ptr->letter) {
            // search the left subtree
            if (ptr->left != nullptr) {
                ptr = ptr->left;
            } else {
                return false;
            }
        } else if (letter > ptr->letter) {
            // search the right subtree
            if (ptr->right != nullptr) {
                ptr = ptr->right;
            } else {
                return false;
            }
        } else {
            // search the middle subtree
            if (i == word.length()) {
                return ptr->is_word;
            } else {
                if (ptr->mid != nullptr) {
                    letter = word[i];
                    ptr = ptr->mid;
                    i = i + 1;
                } else {
                    return false;
                }
            }
        }
    }
}

/* Use frequency to complete the predict completions.
    arguments: prefix, number of completions return.
    return: a list of completions, sorted by their frequency
 */
vector<string> DictionaryTrie::predictCompletions(string prefix,
                                                  unsigned int numCompletions) {
    vector<string> results;
    if (root == 0) {
        // empty tree, no completions
        return results;
    }

    if (numCompletions == 0) {
        return results;
    }

    Node* ptr = root;
    // if prefix is not empty string, then search whether completion exists
    if (prefix.length() != 0) {
        char letter = prefix[0];
        int i = 1;

        // search whether completion exists in the trie
        //      if exists, ptr pointing to the last letter of prefix
        //      if not exists, return an empty vector
        while (true) {
            if (letter < ptr->letter) {
                // into left subtree
                if (ptr->left != nullptr) {
                    ptr = ptr->left;
                } else {
                    // no completion exists
                    return results;
                }
            } else if (letter > ptr->letter) {
                // into right subtree
                if (ptr->right != nullptr) {
                    ptr = ptr->right;
                } else {
                    // no completion exists
                    return results;
                }
            } else {
                // into middle subtree
                if (i == prefix.length()) {
                    break;
                } else {
                    if (ptr->mid != nullptr) {
                        letter = prefix[i];
                        i++;
                        ptr = ptr->mid;
                    } else {
                        // no completion exists
                        return results;
                    }
                }
            }
        }
    }

    // implement PQ to improve efficiency
    priority_queue<pair<int, string>, vector<pair<int, string>>, CompFreq> q;
    if (prefix.length() == 0) {
        dfs(root, "", q, numCompletions);
    } else {
        if (ptr->is_word) {
            q.push(pair<int, string>(ptr->freq, prefix));
        }
        dfs(ptr->mid, prefix, q, numCompletions);
    }

    for (int i = 0; i < numCompletions; i++) {
        if (q.size() > 0) {
            results.push_back(q.top().second);
            q.pop();
        }
    }
    reverse(results.begin(), results.end());
    return results;
}

/* function for wildcard prediction
          arguments: pattern with (or without) underscore(s)
                      number of completions desired
          return: a list of completions.
          hints: if the patter is empty, the returned list is empty
                if the patter is without underscore, return that patter
                  if it is a word in the trie
 */
std::vector<string> DictionaryTrie::predictUnderscores(
    string pattern, unsigned int numCompletions) {
    vector<string> results;
    if (root == 0) {
        // empty tree, no completions
        return results;
    }
    if (numCompletions == 0) {
        return results;
    }
    if (pattern.length() == 0) {
        // if enter an empty string, return nothing
        return results;
    }

    priority_queue<pair<int, string>, vector<pair<int, string>>, CompFreq> q;
    underscoreHelper(pattern, root, q, numCompletions);
    for (int i = 0; i < numCompletions; i++) {
        if (q.size() > 0) {
            results.push_back(q.top().second);
            q.pop();
        }
    }
    reverse(results.begin(), results.end());
    return results;
}

/* This is the destructor */
DictionaryTrie::~DictionaryTrie() { deleteAll(root); }

/* Helper function for destructor. Recursively deletes all the nodes.
    argument: a pointer pointing to the root of the subtree to be deleted.
 */
void DictionaryTrie::deleteAll(Node* ptr) {
    if (ptr == 0) {
        return;
    }
    deleteAll(ptr->left);
    deleteAll(ptr->mid);
    deleteAll(ptr->right);
    delete ptr;
}

/* traverse through the subtree with given root, prune the branch if the
    root of that branch fail to meet the requirement of being pushed to PQ
        arguments: the root of the subtree,
        a path showing how to reach the parent of the root, or empty if no
      parent,
        a piority queue to store all of the words,
        the max size of PQ, which equals to numCompletions
*/
void DictionaryTrie::dfs(
    Node* ptr, string path,
    priority_queue<pair<int, string>, vector<pair<int, string>>, CompFreq>& q,
    int k) {
    if (ptr == nullptr) {
        // if empty tree, return
        return;
    }

    if (ptr->parent == nullptr || ptr->parent->mid == ptr) {
        path = path + ptr->letter;
    } else {
        path[path.length() - 1] = ptr->letter;
    }
    if (q.size() >= k) {
        // if PQ already full
        if (ptr->maxFreq < q.top().first) {
            // if maxFreq of the subtree < mimFreq in heap, return
            return;
        } else if (ptr->maxFreq == q.top().first && path > q.top().second) {
            // if maxFreq of the subtree < mimFreq in heap, and with larger
            // alphabet-order, return
            return;
        }
    }

    // check current node
    if (ptr->is_word) {
        if (q.size() < k) {
            // if PQ not full, push
            q.push(pair<int, string>(ptr->freq, path));
        } else {
            // if PQ full, compare with the min element
            if (ptr->freq > q.top().first) {
                q.pop();
                q.push(pair<int, string>(ptr->freq, path));
            } else if (ptr->freq == q.top().first && path < q.top().second) {
                q.pop();
                q.push(pair<int, string>(ptr->freq, path));
            }
        }
    }

    // sort the maxFreq(s) of children
    vector<Node*> vtr;
    for (Node* p : {ptr->left, ptr->mid, ptr->right}) {
        if (p != nullptr) {
            vtr.push_back(p);
        }
    }
    sort(vtr.begin(), vtr.end(), CompNodeByMaxFrequent());

    // recursive to children by decreasing maxFreq
    for (Node* p : vtr) {
        dfs(p, path, q, k);
    }
}

/* helper method for predictUnderscore, so that we could fill in the
   underscore and recursive down. arguments: subpattern (ignore the part
   before the undersocre we just filled in)， a pointer pointing to node the
   lettter of which is used to fill in the underscore a priority_queue used
   to store the completions k (numOfCompletions) determines the max-size of
   PQ we want, in order to improve efficiency
 */
void DictionaryTrie::underscoreHelper(
    string pattern, Node* ptr,
    priority_queue<pair<int, string>, vector<pair<int, string>>, CompFreq>& q,
    int k) {
    if (ptr == 0) {
        return;
    }
    if (pattern.length() == 0) {
        return;
    }

    char letter = pattern[0];
    int i = 1;
    while (true) {
        if (letter == '_') {
            // if 'underscore', replace it with possible letter and do
            // recursion
            string nextPattern = ptr->letter + pattern.substr(i);
            underscoreHelper(nextPattern, ptr, q, k);
            underscoreHelper(pattern.substr(i - 1), ptr->left, q, k);
            underscoreHelper(pattern.substr(i - 1), ptr->right, q, k);
            return;
        }
        if (letter < ptr->letter) {
            // into left subtree
            if (ptr->left != nullptr) {
                ptr = ptr->left;
            } else {
                return;
            }
        } else if (letter > ptr->letter) {
            // into right subtree
            if (ptr->right != nullptr) {
                ptr = ptr->right;
            } else {
                return;
            }
        } else {
            // into middle subtree
            if (i == pattern.length()) {
                if (ptr->is_word) {
                    if (q.size() < k) {
                        q.push(pair<int, string>(ptr->freq, ptr->getWord()));
                    } else {
                        if (ptr->freq > q.top().first) {
                            q.pop();
                            q.push(
                                pair<int, string>(ptr->freq, ptr->getWord()));
                        } else if (ptr->freq == q.top().first &&
                                   ptr->getWord() < q.top().second) {
                            q.pop();
                            q.push(
                                pair<int, string>(ptr->freq, ptr->getWord()));
                        }
                    }
                }
                return;
            } else {
                if (ptr->mid != nullptr) {
                    letter = pattern[i];
                    ptr = ptr->mid;
                    i = i + 1;
                } else {
                    return;
                }
            }
        }
    }
}

/* the comparator used in sorting nodes based on their frequecy.
        arguments: two nodes to be compared
 */
bool DictionaryTrie::CompFreq::operator()(const pair<int, string>& p1,
                                          const pair<int, string>& p2) {
    if (p1.first == p2.first) {
        return p1.second < p2.second;
    } else {
        return p1.first > p2.first;
    }
}
bool DictionaryTrie::CompNodeByMaxFrequent::operator()(const Node* ptr1,
                                                       const Node* ptr2) {
    return ptr1->maxFreq < ptr2->maxFreq;
}

/*  Create a node. Argument: a letter to be inserted  */
DictionaryTrie::Node::Node(char letter)
    : letter(letter), is_word(false), freq(0), maxFreq(0) {
    left = nullptr;
    mid = nullptr;
    right = nullptr;
    parent = nullptr;
}
/* get the word from the chosen word node by iteration to the root  */
string DictionaryTrie::Node::getWord() {
    if (!is_word) {
        return "";
    }
    string s = "";
    Node* ptr = this;
    s = ptr->letter + s;
    while (ptr->parent != nullptr) {
        if (ptr->parent->mid == ptr) {
            s = ptr->parent->letter + s;
        }
        ptr = ptr->parent;
    }
    return s;
}