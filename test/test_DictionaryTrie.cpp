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
 *        / | \
 *      (a) x  o
 *       |  |  |
 *      (n) i  c
 *       |  |  |
 *      (t) s  t
 *      /   |  |
 *    (d)  (t) b
 *    /        |
 *   c         e
 *   |         |
 *  ...       (r)
 */
class SmallDictTrieFixture : public ::testing::Test {
  protected:
    DictionaryTrie dict;

  public:
    SmallDictTrieFixture() {
        vector<string> inputs{"exist",    "a",  "ant",     "and",
                              "octorber", "an", "ancester"};
        vector<int> freqs{200, 1000, 400, 400, 300, 800, 0};
        for (int i = 0; i < inputs.size(); i++) {
            dict.insert(inputs[i], freqs[i]);
        }
    }
};

TEST_F(SmallDictTrieFixture, SMALL_FIND_TEST) {
    // expect to find word which is in dict
    EXPECT_TRUE(dict.find("exist"));
    EXPECT_TRUE(dict.find("and"));
    EXPECT_TRUE(dict.find("octorber"));
    // expect not to find "not_exist" which is not in dict
    EXPECT_FALSE(dict.find("not_exist"));
    // expect not to find with empty input
    EXPECT_FALSE(dict.find(""));
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
    vector<string> vtr2{"a", "an"};
    EXPECT_EQ(dict.predictCompletions("a", 2), vtr2);
    // expect return first 4 words in trie according to their freq
    vector<string> vtr3{"a", "an", "and", "ant"};
    EXPECT_EQ(dict.predictCompletions("", 4), vtr3);
}

TEST_F(SmallDictTrieFixture, SMALL_PREDICT_UNDERSCORES_TEST) {
    // expect empty vector when no underscore-input
    EXPECT_EQ(dict.predictUnderscores("", 4).size(), 0);
    // expect empty vector when the completion is definetely not in trie
    EXPECT_EQ(dict.predictUnderscores("z_", 4).size(), 0);
    // expect empty vector when the completion is possibly in
    //      but actually not in trie [letter not match]
    EXPECT_EQ(dict.predictCompletions("ex_is", 4).size(), 0);
    // expect empty vector when the completion is possibly in
    //      but actually not in trie [size of underscore not match]
    EXPECT_EQ(dict.predictUnderscores("ex__st", 4).size(), 0);

    // expect find matched completion
    vector<string> vtr1{"exist"};
    //      [underscore in the middle]
    EXPECT_EQ(dict.predictUnderscores("ex_st", 2), vtr1);
    //      [underscore in the end]
    EXPECT_EQ(dict.predictUnderscores("exis_", 2), vtr1);
    //      [underscore in the beginning]
    EXPECT_EQ(dict.predictUnderscores("_xist", 2), vtr1);
    //      [mixed positions of underscore]
    EXPECT_EQ(dict.predictUnderscores("_x_s_", 2), vtr1);
    // expect find matched completion
    vector<string> vtr2{"and"};
    //      [completions with the same freq should be sorted by alphabet]
    EXPECT_EQ(dict.predictUnderscores("an_", 1), vtr2);

    // expect find matched completion with all-underscore
    vector<string> vtr3{"and", "ant"};
    EXPECT_EQ(dict.predictUnderscores("___", 5), vtr3);
    // expect return original word if in trie and pattern without underscore
    vector<string> vtr4{"a"};
    EXPECT_EQ(dict.predictUnderscores("a", 10), vtr4);
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
