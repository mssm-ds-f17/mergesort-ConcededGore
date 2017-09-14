#include <iostream>
#include <vector>
#include <functional>
#include "gtest/gtest.h"

using namespace std;

class Thing {
public:
    int id;
    int stableInt;
public:
    Thing(int id);
};

Thing::Thing(int id = -1) {
    this->id = id;
}

bool isSorted(const vector<int>& values) {

    if (values.size() < 2) {
        return true;
    }

    for (unsigned int i = 0; i < values.size() - 2; i++) {
        if (values[i] > values[i + 1]) {
            return false;
        }
    }

    return true;
}

bool compareThingsById(const Thing& a, const Thing& b) {
    return a.id < b.id;
}

void printVector(vector<Thing> vec) {
    cout << "{";
    for (unsigned int i = 0; i < vec.size(); i++) {
        cout << vec[i].id;
        if (i != vec.size() - 1) {
            cout << ", ";
        } else {
            cout << "}" << endl;
        }
    }
}

bool isSorted(const vector<Thing> &values, function<bool(const Thing& a, const Thing& b)> comp) {

    if (values.size() == 0) {
        return true;
    }

    for (unsigned int i = 0; i < values.size() - 1; i++) {
        if (comp(values[i], values[i + 1]) == comp(values[i + 1], values[i])) {
            // Testing if the values are equal
        } else if (comp(values[i + 1], values[i])) {
            return false;
        }
    }

    return true;
}

vector<Thing> sortThings(const vector<Thing>& values, function<bool(const Thing& a, const Thing& b)> f = compareThingsById) {
    // Split vector into 2
    vector<Thing> a;
    vector<Thing> b;

    if (values.size() == 1) {
        return values;
    }

    for (unsigned int i = 0; i < values.size(); i++) {
        if (i < values.size() / 2) {
            a.push_back(values[i]);
        } else {
            b.push_back(values[i]);
        }
    }

    a = sortThings(a, f);
    b = sortThings(b, f);
    // Merge vectors together
    unsigned int aInd = 0;
    unsigned int bInd = 0;

    vector<Thing> retval;
    while(true) {
        if (f(a[aInd], b[bInd])) {
            retval.push_back(a[aInd]);
            aInd++;
        } else if (f(b[bInd], a[aInd])) {
            retval.push_back(b[bInd]);
            bInd++;
        } else {
            retval.push_back(a[aInd]);
            aInd++;
        }

        if (aInd == a.size()) {
            for (unsigned int k = bInd; k < b.size(); k++) {
                retval.push_back(b[k]);
            }
            break;
        } else if (bInd == b.size()) {
            for (unsigned int k = aInd; k < a.size(); k++) {
                retval.push_back(a[k]);
            }
            break;
        }
    }
    return retval;
}

TEST(SortThing, sortVectorOfThings) {
    vector<Thing> v{3, 1, 2, 55, 2, 1, 77, 1, 0, 30};
    v = sortThings(v);
    ASSERT_TRUE(isSorted(v, compareThingsById));
}

TEST(SortTest, isEmptyVectorSorted) {
    vector<int> v;
    ASSERT_TRUE(isSorted(v));
}

TEST(SortTest, isSortedVectorSorted) {
    vector<int> v {1, 2, 5, 5, 9, 100};
    ASSERT_TRUE(isSorted(v));
}

TEST(SortTest, isUnsortedVectorSorted) {
    vector<int> v {1, 2, 5, 1, 9, 100};
    ASSERT_FALSE(isSorted(v));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
