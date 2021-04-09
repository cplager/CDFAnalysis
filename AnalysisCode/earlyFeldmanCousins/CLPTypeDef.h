// -*- C++ -*-

#if !defined(CLPTypeDef_H)
#define CLPTypeDef_H

#include <vector>
#include <map>
#include <string>
#include <iostream>

typedef std::vector< std::string> SVec;
typedef std::vector< double > DVec;
typedef std::vector< float > FVec;
typedef std::vector< int > IVec;
typedef std::vector< DVec > DVecVec;
typedef std::map< int, DVec > IDVecMap;
typedef IDVecMap::iterator IDVecMapIter;
//typedef std::vector<  std::vector< double > > DVecVec;
//typedef std::map<std::string, *DVecVec> StringDVecVecMap;

extern ostream& operator<< (ostream& o_stream, const DVec &right);

extern ostream& operator<< (ostream& o_stream, const IVec &right);

extern ostream& operator<< (ostream& o_stream, const SVec &right);


#endif // CLPTypeDef_H
