#ifndef TRIGGERPATHTOINDEX_HH
#define TRIGGERPATHTOINDEX_HH

#include <vector>
#include <string>
#include <unordered_map>

using std::vector;
using std::string;
using std::unordered_map;


namespace ran{
  class TriggerPathToIndex{

  public:
    TriggerPathToIndex(){}
    TriggerPathToIndex(const vector<string> addedTriggerPaths){
      triggerPaths_.insert(triggerPaths_.begin(),addedTriggerPaths.begin(),addedTriggerPaths.end());
      for (unsigned int i = 0, imax = triggerPaths_.size(); i < imax; ++i){ 
	pathToIndexMap_.insert(make_pair(triggerPaths_[i],i));
      }
    }

    size_t size(){ return triggerPaths_.size();}
    unsigned int getTrigIndex(const string triggerPath) 
    {
      auto failedToFind = pathToIndexMap_.end();
      if (failedToFind != pathToIndexMap_.find(triggerPath)){//make sure trigger is in map
	return pathToIndexMap_[triggerPath];
      } else {
	throw "Error: Asking for a trigger we diddnn't specify\n";
      }
    }

  private:

    vector<string> triggerPaths_;
    unordered_map<string, unsigned int> pathToIndexMap_;


  };
}
#endif
