#ifndef _BINTREEEXCEPTION_H_
#define _BINTREEEXCEPTION_H_

#include "BinTreeBaseException.h"

using namespace std;

class TreePositionException : public BinTreeBaseException {
public:
    TreePositionException() : BinTreeBaseException("Tree iterator position does not enables to run this action") {};
};

//exceptions treatment
//...

#endif

#pragma once

