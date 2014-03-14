#ifndef __CHAINTOOLS_HH__
#define __CHAINTOOLS_HH__

/*! \file chaintools.hh
    \brief functions to work with TChain objects
*/ 

#include<TChain.h>
#include<TBranch.h>
#include<string>
#include<iostream>
#include<vector>

#if __cplusplus >= 201103L
#include<array>
#endif

//! template to create connections of variables to TTree/TChain branches
/*!
  this function takes an array of branch-pointers, the names of the branches
  as a vector of strings and an array of values to associate with the TChain
  *chain
 */
template<class T>
bool addtochain(TChain *chain, TBranch **branch, 
		std::vector<std::string> names, T **val){
  //std::cout << names.size() << " " << names[0] << std::endl << std::flush;
  if(chain == NULL) return false;
  for(size_t i=0; i<names.size(); i++){
    //std::cout << i << " " << names[i] << std::endl;
    chain->SetBranchAddress(names[i].c_str(), val[i], branch+i);
  }
  return true;
}

//! template to create connections of variables to TTree/TChain branches
/*!
  this function takes an array of branch-pointers, the names of the branches
  as a vector of strings and a vector of values to associate with the TChain
  *chain

 */
template<class T>
bool addtochain(TChain *chain, TBranch **branch[], 
		std::vector<std::string> names, std::vector<T> *val){
  if(chain == NULL) return false;
  for(size_t i=0; i<names.size(); i++){
    chain->SetBranchAddress(names[i].c_str(), &((*val)[i]), branch[i]);
  }
  return true;
}

//! template to create connections of variables to TTree/TChain branches
/*!
  this function takes a vector of branch-pointers, the names of the branches
  as a vector of strings and a vector of values to associate with the TChain
  *chain

 */
template<class T>
bool addtochain(TChain *chain, std::vector<TBranch **> *branch, 
		std::vector<std::string> names, std::vector<T> *val){
  if(chain == NULL) return false;
  for(size_t i=0; i<names.size(); i++){
    chain->SetBranchAddress(names[i].c_str(), &((*val)[i]), (*branch)[i]);
  }
  return true;
}

//! opens a TChain with name chainname
/*!
   opens a TChain with name chainname that is distributed over the files in 
   the vector filename.
 */
TChain *openchain(std::string chainname,std::vector<std::string> filename);

//! adds a vector of filenames to the TChain *chain
bool    addfiletochain(TChain *chain, std::vector<std::string> filename);

#if __cplusplus >= 201103L
//! calculates file boundries and divides the TChain into chuncks of 2000 events
std::vector<std::array<size_t, 2> > getfileboundry(TChain *chain);
#endif

#endif
