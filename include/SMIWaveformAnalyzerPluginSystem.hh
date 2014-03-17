#ifndef __SMIWAVEFORMANALYZERPLUGINSYSTEM_HH__
#define __SMIWAVEFORMANALYZERPLUGINSYSTEM_HH__

#include<vector>

/*! \file SMIWaveformAnalyzerPluginSystem.hh
    \brief this file contains all base classes for the plugin system
    \author Clemens Sauerzopf
*/ 


#if __cplusplus >= 201103L
#include <memory>
using std::shared_ptr;
#else
#include <tr1/memory>
using std::tr1::shared_ptr;
#endif

#include"SMIWave.hh"

//! abstract base class for plugin system
/*!
  this class provides the basic functionality of a plugin, it is a functor 
  that operates on a single @ref WaveForm. This class is pure virtual, so don't
  try to use it directly, but ALL plugins need to inherit from this class.
 */
class SMIAnalyzerPluginBase {
public:
  //! standard constructor
  SMIAnalyzerPluginBase(){}
  
  //! virtual standrad destructor
  virtual ~SMIAnalyzerPluginBase(){};

  //! function call operator
  /*!
    this operator takes the reference to one @ref WaveForm as it's argument. 
    When deriving a real plugin from this function needs to be provided.
   */
  virtual bool operator()(WaveForm &wave) = 0;
};

//! list of plugins
/*!
  This class wraps around a std::vector to provide a convinient access to
  the pluginslist. The plugins are stored within this vectors as smart 
  pointers to newly created instanced of the plugins. The use of smart pointers
  ensures that the pluginlist is copy constructable without generating 
  more memory consumption. If a copy of a pluginlist is created and the 
  original list goes out of scope the pointers to the previously allocated
  plugins are still valid. The will only be released when the last copy 
  goes out of scope or it's destructor is called.

  usage e.g.:\n
       `SMIAnalyzerPluginList list;`\n
       `list.add(new SMIAnalyzerPluginDummy(1));`\n
       `list.add(new SMIAnalyzerPluginDummy(2));`\n
       `list.add(new SMIAnalyzerPluginDummy(22));`\n
 */
class SMIAnalyzerPluginList {
public:
  //! standard constructor
  SMIAnalyzerPluginList(){};
  //! stamdard destructtor
  ~SMIAnalyzerPluginList();

  //! adds a plugin to the list
  /*!
    this function adds a plugin to the list of plugins by calling the
    underlying std::vectors push_back function.

    @param plugin pointer to a plugin, use this with `new`
   */
  inline void add(SMIAnalyzerPluginBase* plugin) 
  {plugins.push_back(shared_ptr<SMIAnalyzerPluginBase>(plugin));}

  //! operator to access a plugin
  /*!
    @param pos position number of the plugin within the vector
    @return a shared pointer to the plugin
   */
  inline shared_ptr<SMIAnalyzerPluginBase> & operator[](size_t pos) {return plugins[pos];}
  
  //! returns the number of plugins in the list
  inline size_t size() const {return plugins.size();}

private:
  std::vector<shared_ptr<SMIAnalyzerPluginBase> > plugins; //!< storage container for the plugin smart pointers
};

//! a simple dummy plugin for demonstration purposes
class SMIAnalyzerPluginDummy : public SMIAnalyzerPluginBase {
public:
  //! standard constructor
  SMIAnalyzerPluginDummy() : dummy(0) {}
  //! constructor with 1 argument
  SMIAnalyzerPluginDummy(int i) : dummy(i) {}
  //! overloaded operator, the plugins functionality is implemented here
  virtual bool operator()(WaveForm &wave) {
    char tmp[20];
    sprintf(tmp,"Test%i",dummy);
    wave.result[tmp] = (double)dummy/10.;
    return true;
  }

private:
  int dummy; //!< a variable
};

#endif
