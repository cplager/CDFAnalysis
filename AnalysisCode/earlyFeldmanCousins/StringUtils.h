// -*- C++ -*-

#if !defined(STRINGUTILS_H)
#define STRINGUTILS_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <iomanip>
#include "CLPTypeDef.h"

void split (SVec &retval, std::string line);
std::string toLowerCase(std::string const &word);

#endif // STRINGUTILS_H
