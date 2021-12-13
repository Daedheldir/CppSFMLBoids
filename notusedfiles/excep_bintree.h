#ifndef _EXCEP_BINTREE_H_
#define _EXCEP_BINTREE_H_

#include "exception.h"

using namespace std;

class TreePositionException : public Exception {
public:
    TreePositionException() : Exception("Tree iterator position does not enables to run this action") {};
};

//exceptions treatment
/*class NoHaySiguienteMayor :public runtime_error {
public:
    NoHaySiguienteMayor(const string& err) :runtime_error(err) {}
    string msg() const {
        return what();
    }
};*/

#endif

#pragma once
