#include "BaconProd/Utils/interface/TriggerTools.hh"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Math/interface/deltaR.h"
#include <string>

using namespace baconhep;

//--------------------------------------------------------------------------------------------------
TriggerObjects TriggerTools::matchHLT(const double eta, const double phi, 
				      const std::vector<TriggerRecord>& triggerRecords,
				      const trigger::TriggerEvent& triggerEvent)
{
  const double dRMax = 0.2;

  TriggerObjects matchBits;
  //return matchBits;
  for(unsigned int irec=0; irec<triggerRecords.size(); irec++) {     
    for(unsigned int iobj=0; iobj<triggerRecords[irec].objectMap.size(); iobj++) {
      const std::string   filterName = triggerRecords[irec].objectMap[iobj].first;
      const unsigned int  filterBit  = triggerRecords[irec].objectMap[iobj].second;
      
      edm::InputTag filterTag(filterName,"","HLT");
      // filterIndex must be less than the size of trgEvent or you get a CMSException: _M_range_check
      if(triggerEvent.filterIndex(filterTag) < triggerEvent.sizeFilters()) {
        const trigger::TriggerObjectCollection& toc(triggerEvent.getObjects());      
        const trigger::Keys& keys(triggerEvent.filterKeys(triggerEvent.filterIndex(filterTag)));
        
        for(unsigned int hlto=0; hlto<keys.size(); hlto++) {
          trigger::size_type hltf = keys[hlto];
          const trigger::TriggerObject& tobj(toc[hltf]);
          if(reco::deltaR(eta,phi,tobj.eta(),tobj.phi()) < dRMax) {
            matchBits[filterBit] = 1;
	    break;
          }
        }
      }
    }
  }
  
  return matchBits;
}

TriggerObjects TriggerTools::matchHLT(const double eta, const double phi,  
                                      const std::vector<TriggerRecord> &triggerRecords,
                                      const pat::TriggerObjectStandAloneCollection &triggerObjects)
{
  const double dRMax = 0.2;

  TriggerObjects matchBits;
  //return matchBits;
  for(unsigned int irec=0; irec<triggerRecords.size(); irec++) {     
    for(unsigned int iobj=0; iobj<triggerRecords[irec].objectMap.size(); iobj++) {
      const std::string   filterName = triggerRecords[irec].objectMap[iobj].first;
      const unsigned int  filterBit  = triggerRecords[irec].objectMap[iobj].second;

      for(pat::TriggerObjectStandAlone tobj : triggerObjects) {
        if(tobj.hasFilterLabel(filterName)) {
          if(reco::deltaR(eta,phi,tobj.eta(),tobj.phi()) < dRMax) {
	    matchBits[filterBit] = 1; 
	    break;
          }
        }
      }
    }
  }

  return matchBits;
}
