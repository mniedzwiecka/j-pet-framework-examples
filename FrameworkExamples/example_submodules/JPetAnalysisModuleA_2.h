//
//  JPetAnalysisModuleA_2.h
//  
//
//  Created by Karol Stola on 22.04.2014.
//
//

#ifndef ____JPetAnalysisModuleA_2__
#define ____JPetAnalysisModuleA_2__

#include <iostream>
#include <TTree.h>
#include <JPetAnalysisModule/JPetAnalysisModule.h>


class JPetAnalysisModuleA_2 : public JPetAnalysisModule {

public:
	JPetAnalysisModuleA_2(TTree* super_tree);
	virtual void createInputObjects(const char* inputFilename=0){} //
	virtual void createOutputObjects(const char* outputFilename=0){} //
	virtual void exec();
	virtual long long getEventNb(){}
	virtual void terminate(){}

};

#endif /* defined(____JPetAnalysisModuleA_2__) */
