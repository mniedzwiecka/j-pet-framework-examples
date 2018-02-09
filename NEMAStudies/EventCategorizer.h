/**
 *  @copyright Copyright 2017 MPN & SN TEAM. All rights reserved.
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may find a copy of the License in the LICENCE file.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *  @file EventCategorizer.h
 */

#ifndef EVENTCATEGORIZER_H
#define EVENTCATEGORIZER_H

#include <vector>
#include <map>
#include <JPetUserTask/JPetUserTask.h>
#include <JPetHit/JPetHit.h>
#include <JPetEvent/JPetEvent.h>
#include <JPetHitUtils/JPetHitUtils.h>
#include <utility>
#include <fstream>
#include "TLine.h"
#include "TStyle.h"

class JPetWriter;

#ifdef __CINT__
#	define override
#endif

struct point
{
  double x; double y; double z;
};

class EventCategorizer : public JPetUserTask{
public:
	EventCategorizer(const char * name);
	virtual ~EventCategorizer(){}
	virtual bool init() override;
	virtual bool exec() override;
	virtual bool terminate() override;

protected:
	void saveEvents(const std::vector<JPetEvent>& event);
	bool fSaveControlHistos = true;
		bool makeTotCut(JPetHit& firstHit, JPetHit& secondHit);
	point findAnnihilationPoint(const JPetHit& firstHit, const JPetHit& secondHit);
        bool cutOnLorDistanceFromCenter(const JPetHit& firstHit, const JPetHit& secondHit);
	double calculateXYDistance(const JPetHit& firstHit, const JPetHit& secondHit);
	double LORDistance(const JPetHit& firstHit, const JPetHit& secondHit);
	bool isBackToBack(const JPetHit& firstHit, const JPetHit& secondHit);
	int DeltaID(const JPetHit& firstHit, const JPetHit& secondHit);
	int DeltaAngle(const JPetHit& firstHit, const JPetHit& secondHit);
	void SSJExtraction(const JPetEvent* event);
	double calculateSumOfTOTsForSignal(const JPetHit& hit, char label = 'X');
	double calculateSumOfTOTs(const JPetHit& hit);
	void savingToRecoFile(JPetHit hit_one, JPetHit hit_two);
	void savingTOT(double meanTOT, int ID);
	bool cutOnZAxis(const JPetHit& firstHit, const JPetHit& secondHit);
	std::ofstream outputFile;
	point fPoint;
	std::string foutname; 
	double interLayerTOFCoeff[3][3];
	std::map< int, double> fTotCuts;
};
#endif /*  !EVENTCATEGORIZER_H */
