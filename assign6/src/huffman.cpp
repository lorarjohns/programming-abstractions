#include "bits.h"
#include "treenode.h"
#include "huffman.h"
#include "map.h"
#include "vector.h"
#include "priorityqueue.h"
#include "strlib.h"
#include "testing/SimpleTest.h"
#include "pqueue.h"

using namespace std;

/**
 * Given a Queue<Bit> containing the compressed message bits and the encoding tree
 * used to encode those bits, decode the bits back to the original message text.
 *
 * You can assume that tree is a well-formed non-empty encoding tree and
 * bits queue contains a valid sequence of encoded bits. Relies on error handling
 * in Bit class.
 *
 * Returns a string representation of the decoded object.
 */
string decodeText(EncodingTreeNode* tree, Queue<Bit>& messageBits) {

    /* initialize string to hold the decoded result */
    string answer = "";

    /* set a pointer to the current tree node */
    EncodingTreeNode* cur = tree;

    /* while there are still characters left to decode */
    while (!messageBits.isEmpty()) {

        Bit nextBit = messageBits.dequeue();

        /*if next bit is 0, go left */
        if (nextBit == 0) {
            cur = cur->zero;
        } else {
            /* else go right */
            cur = cur->one;
        }
           /* if we are on a leaf, take the data */
        if (cur->zero == nullptr && cur->one == nullptr) {
            answer += cur->ch;
            // reset the current node pointer
            cur = tree;
        }
    }
    return answer;
}

/**
 * Reconstruct encoding tree from flattened form Queue<Bit> and Queue<char>.
 *
 * You can assume that the input Queues are well-formed and represent
 * a valid encoding tree.
 *
 * This is like `countleft.cpp` from section 7.
 * Proceeding left to right through the encoded string, if we see a 0, we know that this node has no children.
 * If we see a 1, we branch. The next number indicates the left child, so if that is a 1,
 * we recursively continue to process the string until that child's child nodes are full.
 * Then any remaining numbers are right children further up the tree.
 *
 */

EncodingTreeNode* unflattenTree(Queue<Bit>& treeBits, Queue<char>& treeLeaves) {
    /* TODO: Fill in the rest of this function. */

    /* while we still have nodes to deserialize */
    while (!treeBits.isEmpty()) {
        /* check whether the next node is a leaf node */
        if (treeLeaves.peek() == 0) {
            /* if it is, put a character in it */
            EncodingTreeNode* newLeafNode = new EncodingTreeNode(treeLeaves.dequeue());
        }
    }
}

/**
 * Decompress the given EncodedData and return the original text.
 *
 * You can assume the input data is well-formed and was created by a correct
 * implementation of compress.
 *
 * Your implementation may change the data parameter however you like. There
 * are no requirements about what it should look like after this function
 * returns.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
string decompress(EncodedData& data) {
    /* TODO: Fill in the rest of this function. */
    return "";
}

/**
 * Constructs an optimal Huffman coding tree for the given text, using
 * the algorithm described in lecture.
 *
 * Reports an error if the input text does not contain at least
 * two distinct characters.
 *
 * When assembling larger trees out of smaller ones, make sure to set the first
 * tree dequeued from the queue to be the zero subtree of the new tree and the
 * second tree as the one subtree.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
EncodingTreeNode* buildHuffmanTree(string text) {
    /* TODO: Fill in the rest of this function. */
    return nullptr;
}

/**
 * Given a string and an encoding tree, encode the text using the tree
 * and return a Queue<Bit> of the encoded bit sequence.
 *
 * You can assume tree is a valid non-empty encoding tree and contains an
 * encoding for every character in the text.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
Queue<Bit> encodeText(EncodingTreeNode* tree, string text) {
    /* TODO: Fill in the rest of this function. */
    return {};
}

/**
 * Flatten the given tree into a Queue<Bit> and Queue<char> in the manner
 * specified in the assignment writeup.
 *
 * You can assume the input Queues are empty on entry to this function.
 *
 * You can assume tree is a valid well-formed encoding tree.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
void flattenTree(EncodingTreeNode* tree, Queue<Bit>& treeBits, Queue<char>& treeLeaves) {
    /* TODO: Fill in the rest of this function. */
}

/**
 * Compress the input text using Huffman coding, producing as output
 * an EncodedData containing the encoded message and encoding tree used.
 *
 * Reports an error if the message text does not contain at least
 * two distinct characters.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
EncodedData compress(string messageText) {
    /* TODO: Fill in the rest of this function. */
    return {};
}

/* * * * * * Testing Helper Functions Below This Point * * * * * */

EncodingTreeNode* createExampleTree() {
    /* Example encoding tree used in multiple test cases:
     *                *
     *              /   \
     *             T     *
     *                  / \
     *                 *   E
     *                / \
     *               R   S
     */

    /* Create the leaf nodes */
    EncodingTreeNode* R = new EncodingTreeNode('R');
    EncodingTreeNode* S = new EncodingTreeNode('S');
    EncodingTreeNode* E = new EncodingTreeNode('E');
    EncodingTreeNode* T = new EncodingTreeNode('T');

    /* Create intermediate nodes */
    EncodingTreeNode* innerNode1 = new EncodingTreeNode(R, S);
    EncodingTreeNode* innerNode2 = new EncodingTreeNode(innerNode1, E);

    /* Create root node */
    EncodingTreeNode* root = new EncodingTreeNode(T, innerNode2);

    return root;

}


void deallocateTree(EncodingTreeNode* root) {

    if (root == nullptr) {
        return;
    }

    deallocateTree(root->zero);
    deallocateTree(root->one);

    delete root;
}

bool areEqual(EncodingTreeNode* a, EncodingTreeNode* b) {

    /* check base case emptyness */
    if (a == nullptr || b == nullptr) {
        return a == b;
    }

    /* check for if both are leaf nodes
     * if yes, then return whether they have equal values
     * if no, return false
     */

    if (a->zero == nullptr || b->zero == nullptr) {
        return a->ch == b->ch;
    }

    if (a->one == nullptr || b->one == nullptr) {
        return a->ch == b->ch;
    }

    /* if you have left or right children,
     * then there's no data and we just return
     * equvalence of left and right subtrees
     */

    return areEqual(a->zero, b->zero)
            && areEqual(a->one, b->one);


}

/* * * * * * Test Cases Below This Point * * * * * */

/* TODO: Write your own student tests. */


STUDENT_TEST("deallocateTree") {
    EncodingTreeNode* tree = createExampleTree();
    deallocateTree(tree);

}

STUDENT_TEST("areEqual") {
    EncodingTreeNode* a = createExampleTree();
    EncodingTreeNode* b = createExampleTree();

    EXPECT(areEqual(a, b));
    deallocateTree(a);
    deallocateTree(b);
}

STUDENT_TEST("areEqual false") {
    /* Create the leaf nodes */
    EncodingTreeNode* R = new EncodingTreeNode('R');
    EncodingTreeNode* S = new EncodingTreeNode('S');
    EncodingTreeNode* T = new EncodingTreeNode('T');

    /* Create intermediate nodes */
    EncodingTreeNode* innerNode1 = new EncodingTreeNode(R, S);

    /* Create root node */
    EncodingTreeNode* root = new EncodingTreeNode(T, innerNode1);

    EncodingTreeNode* tree = createExampleTree();

    EXPECT(!areEqual(root, tree));

    deallocateTree(root);
    deallocateTree(tree);
}

STUDENT_TEST("areEqual subtrees") {
    EncodingTreeNode* R = new EncodingTreeNode('R');
    EncodingTreeNode* S = new EncodingTreeNode('S');
    EncodingTreeNode* r = new EncodingTreeNode('R');
    EncodingTreeNode* s = new EncodingTreeNode('S');
    EncodingTreeNode* innerNode1 = new EncodingTreeNode(R, S);
    EncodingTreeNode* innerNode2 = new EncodingTreeNode(r, s);

    EXPECT(areEqual(innerNode1, innerNode2));
    deallocateTree(innerNode1);
    deallocateTree(innerNode2);
}


STUDENT_TEST("decodeText, small example encoding tree") {
    /* Example encoding tree used in multiple test cases:
     *                *
     *              /   \
     *             T     *
     *                  / \
     *                 *   E
     *                / \
     *               R   S
     */

    EncodingTreeNode* tree = createExampleTree();

    Queue<Bit> messageBits = { 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1 }; // TERES
    EXPECT_EQUAL(decodeText(tree, messageBits), "TERES");

    //Queue<Bit> badBits = { 0, 1, 2 };
    //EXPECT_ERROR(decodeText(tree, badBits));
    deallocateTree(tree);

}


/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("decodeText, small example encoding tree") {
    EncodingTreeNode* tree = createExampleTree(); // see diagram above

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(decodeText(tree, messageBits), "E");

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(decodeText(tree, messageBits), "SET");

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1}; // STREETS
    EXPECT_EQUAL(decodeText(tree, messageBits), "STREETS");

    deallocateTree(tree);
}

PROVIDED_TEST("unflattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  treeBits   = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeLeaves = { 'T', 'R', 'S', 'E' };
    EncodingTreeNode* tree = unflattenTree(treeBits, treeLeaves);

    EXPECT(areEqual(tree, reference));

    deallocateTree(tree);
    deallocateTree(reference);
}

PROVIDED_TEST("decompress, small example input") {
    EncodedData data = {
        { 1, 0, 1, 1, 0, 0, 0 }, // treeBits
        { 'T', 'R', 'S', 'E' },  // treeLeaves
        { 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1 } // messageBits
    };

    EXPECT_EQUAL(decompress(data), "TRESS");
}

PROVIDED_TEST("buildHuffmanTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    EncodingTreeNode* tree = buildHuffmanTree("STREETTEST");
    EXPECT(areEqual(tree, reference));

    deallocateTree(reference);
    deallocateTree(tree);
}

PROVIDED_TEST("encodeText, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(encodeText(reference, "E"), messageBits);

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(encodeText(reference, "SET"), messageBits);

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1 }; // STREETS
    EXPECT_EQUAL(encodeText(reference, "STREETS"), messageBits);

    deallocateTree(reference);
}

PROVIDED_TEST("flattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  expectedBits   = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> expectedLeaves = { 'T', 'R', 'S', 'E' };

    Queue<Bit>  treeBits;
    Queue<char> treeLeaves;
    flattenTree(reference, treeBits, treeLeaves);

    EXPECT_EQUAL(treeBits,   expectedBits);
    EXPECT_EQUAL(treeLeaves, expectedLeaves);

    deallocateTree(reference);
}

PROVIDED_TEST("compress, small example input") {
    EncodedData data = compress("STREETTEST");
    Queue<Bit>  treeBits    = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeChars   = { 'T', 'R', 'S', 'E' };
    Queue<Bit>  messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0 };

    EXPECT_EQUAL(data.treeBits, treeBits);
    EXPECT_EQUAL(data.treeLeaves, treeChars);
    EXPECT_EQUAL(data.messageBits, messageBits);
}

PROVIDED_TEST("Test end-to-end compress -> decompress") {
    Vector<string> inputs = {
        "HAPPY HIP HOP",
        "The job requires extra pluck and zeal from every young wage earner.",
        ":-) :-D XD <(^_^)>",
    };

    for (string input: inputs) {
        EncodedData data = compress(input);
        string output = decompress(data);

        EXPECT_EQUAL(output.size(), input.size());

        /* Don't clobber the output with a huge string if there's a mismatch. */
        EXPECT(input == output);
    }
}
