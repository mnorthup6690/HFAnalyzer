// -*- C++ -*-
//
// Package:    HFAnalyzer/HFAnalyzer
// Class:      HFAnalyzer
// 
/**\class HFAnalyzer HFAnalyzer.cc HFAnalyzer/HFAnalyzer/plugins/HFAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Michael Eugene Northup
//         Created:  Thu, 23 Jul 2015 08:16:39 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Common/interface/Handle.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"

#include <iostream>
//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class HFAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit HFAnalyzer(const edm::ParameterSet&);
      ~HFAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------

  edm::InputTag HFRecHitCollection_label;
  edm::InputTag HcalDigi_label;
  edm::EDGetTokenT<HFUpgradeDigiCollection> HFToken_;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
HFAnalyzer::HFAnalyzer(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
   usesResource("TFileService");
//   HcalDigi_label = iConfig.getParameter<edm::InputTag>("HcalDigi_label");
   HFToken_ = consumes<HFUpgradeDigiCollection>(iConfig.getParameter< edm::InputTag >("HcalDigi_label"));
}


HFAnalyzer::~HFAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//

// member functions
//

// ------------ method called for each event  ------------
void
HFAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace std;

/*
Handle<HFDigiCollection> hfdigicollection;
    iEvent.getByLabel(HcalDigi_label,hfdigicollection);
   
  HFDigiCollection::const_iterator hfdigi; 
   for(hfdigi = hfdigicollection->begin(); hfdigi != hfdigicollection->end(); hfdigi++) 
	{
		for (int i = 0; i < 15; i++)
		{
		cout << hfdigi->sample(i).adc() << endl;
		}
	}
*/

Handle<HFUpgradeDigiCollection> hfHandle;
iEvent.getByToken(HFToken_, hfHandle);

//   Handle<HFUpgradeDigiCollection> hfdigicollection;
//   iEvent.getByLabel(HcalDigi_label,hfdigicollection);
//cout << "preloop" << endl;

//HFDigiCollection::const_iterator hfdigi = hfHandle->begin();
//HFDigiCollection::const_iterator hfdigi2 = hfHandle->end();
     
// if (hfdigi == hfdigi2)
//{
//	cout << "equal" << endl;
//}
// if (hfdigi != hfdigi2)
//{
//	cout << "not equal" << endl;
//}
int hfsize = hfHandle->size();

int FG1plus[hfsize];
int FG1min[hfsize];
int FG2plus[hfsize];
int FG2min[hfsize];

int FG1plusFinal = 0;
int FG1minFinal = 0;

int FG2plusFinal = 0;
int FG2minFinal = 0;

for (int i = 0; i < hfsize; i++)
{
	FG1plus[i] = 0;
	FG1min[i] = 0;
	FG2plus[i] = 0;
        FG2min[i] = 0;
}

// thresholds are in QIE values which range from 0-127. These are just example thresholds until I can determine the actual thresholds applied
int thresh1 = 5;
int thresh2 = 20;

int iter = 0;

    for (HFUpgradeDigiCollection::const_iterator hfdigi = hfHandle->begin(); hfdigi != hfHandle->end(); hfdigi++) 
    {
	HcalDetId hcalid = HcalDetId(hfdigi->id()) ;
	
	// if the hf channel has a qie value above one of the thresholds set the fine grain bit. We choose time slice number 4 fo each digi since almost all of the 
	// charge is deposited at this time slice
	if (hcalid.ieta() >= 0)
	{
	if (hfdigi->adc(4) >= thresh1 && hfdigi->adc(4) < thresh2)
	{
		FG1plus[iter] = 1;
	}
	else if (hfdigi->adc(4) >= thresh2)
	{
		FG2plus[iter] = 1;
	}
	}
	
	if (hcalid.ieta() < 0)
         {
         if (hfdigi->adc(4) >= thresh1 && hfdigi->adc(4) < thresh2)
         {       
                 FG1min[iter] = 1;
         }
         else if (hfdigi->adc(4) >= thresh2)
         {       
                 FG2min[iter] = 1;
         }
         }

	iter++; 
   }

  // Calculate the final FG bit
   for (int i = 0; i < hfsize; i++)
	{
		FG1plusFinal = FG1plusFinal || (FG1plus[i]);
		FG2plusFinal = FG2plusFinal || (FG2plus[i]);
		FG1minFinal = FG1minFinal || (FG1min[i]);
                FG2minFinal = FG2minFinal || (FG2min[i]);
	}	  
   
#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
}


// ------------ method called once each job just before starting event loop  ------------
void 
HFAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
HFAnalyzer::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
HFAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(HFAnalyzer);
