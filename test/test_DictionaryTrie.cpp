/**
 * This File tests all the methods implemented in
 * DictionaryTrie.
 * Tests can be roughly divided into 2 categories:
 *      DictTrieTests: tests on an empty trie & on destructor
 *      SmallDictTrieFixture: tests on a small dictionary trie
 *
 * Author: Yuening YANG
 * Email: y3yang@ucsd.edu
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include "DictionaryTrie.hpp"
#include "util.hpp"

using namespace std;
using namespace testing;

/* Empty test */
TEST(DictTrieTests, EMPTY_TEST) {
    DictionaryTrie dict;
    ASSERT_EQ(dict.find("abrakadabra"), false);
}

TEST(DictTrieTests, EMPTY_PREDICT_COMPLETIONS_TEST) {
    // expect predict_completions() on empty trie
    //      returns an empty vector
    DictionaryTrie dict;
    EXPECT_EQ(dict.predictCompletions("a", 4).size(), 0);
}

TEST(DictTrieTests, EMPTY_PREDICT_UNDERSCORES_TEST) {
    // yet to be implemented in part2
}

/* Small DictTree test */

/**
 * A simple test fixture from which multiple tests can be
 * written. Rebuit after every test.
 *
 * Builds the following Dictionary Trie:
 *          e
 *        / |
 *      (a) x
 *       |  |
 *       n  i
 *       |  |
 *      (t) s
 *      /   |
 *    (d)  (t)
 */
class SmallDictTrieFixture : public ::testing::Test {
  protected:
    DictionaryTrie dict;

  public:
    SmallDictTrieFixture() {
        vector<string> inputs{"exist", "a", "ant", "and"};
        vector<int> freqs{200, 1000, 400, 400};
        for (int i = 0; i < inputs.size(); i++) {
            dict.insert(inputs[i], freqs[i]);
        }
    }
};

TEST_F(SmallDictTrieFixture, SMALL_FIND_TEST) {
    // expect to find "exist" which is in dict
    EXPECT_TRUE(dict.find("exist"));
    // expect not to find "not_exist" which is not in dict
    EXPECT_FALSE(dict.find("not_exist"));
}

TEST_F(SmallDictTrieFixture, SMALL_INSERT_TEST) {
    // expect successful insertion as "new" is not in dict
    EXPECT_TRUE(dict.insert("new", 150));
    // expect false as the string is empty
    EXPECT_FALSE(dict.insert("", 500));
    // expect false as "exist" is already in dict
    EXPECT_FALSE(dict.insert("exist", 100));
}

TEST_F(SmallDictTrieFixture, SMALL_PREDICT_COMPLETIONS_TEST) {
    // expect empty vector when no completion exists
    EXPECT_EQ(dict.predictCompletions("z", 10).size(), 0);

    // expect vector of size 1, as there are fewer than ​
    //      numCompletions legal completions
    vector<string> vtr1{"exist"};
    EXPECT_EQ(dict.predictCompletions("e", 5), vtr1);
    // expect return "add" rather than "ant" (the same freq)
    //      according to the alphabet
    vector<string> vtr2{"a", "and"};
    EXPECT_EQ(dict.predictCompletions("a", 2), vtr2);
    // expect return first 4 words in trie according to their freq
    vector<string> vtr3{"a", "and", "ant", "exist"};
    EXPECT_EQ(dict.predictCompletions("", 4), vtr3);
}

TEST_F(SmallDictTrieFixture, SMALL_PREDICT_UNDERSCORES_TEST) {
    // yet to be implemented in part2
}

/* Destructor test */
TEST(DictTrieTests, DESTRUCTOR_TEST) {
    // test whether there's error in destructing empty trie
    DictionaryTrie* dict = new DictionaryTrie();
    delete dict;
    // test whether there's error in destructing small trie
    dict = new DictionaryTrie();
    vector<string> inputs{"exist", "a", "ant", "and"};
    vector<int> freqs{200, 1000, 400, 400};
    for (int i = 0; i < inputs.size(); i++) {
        dict->insert(inputs[i], freqs[i]);
    }
    delete dict;

    // success when no error met in previous procedure
    EXPECT_TRUE(1);
}
