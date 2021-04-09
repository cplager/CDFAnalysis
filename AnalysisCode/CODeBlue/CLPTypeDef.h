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
typedef std::map< std::string, double > SDMap;

//typedef std::vector<  std::vector< double > > DVecVec;
//typedef std::map<std::string, *DVecVec> StringDVecVecMap;

extern std::ostream& operator<< (std::ostream& o_stream, const DVec &right);

extern std::ostream& operator<< (std::ostream& o_stream, const IVec &right);

extern std::ostream& operator<< (std::ostream& o_stream, const SVec &right);


#endif // CLPTypeDef_H
