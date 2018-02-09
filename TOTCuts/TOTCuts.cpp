/**
 *  @copyright Copyright 2017 The J-PET Framework Authors. All rights reserved.
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
 *  @file TOTCuts.cpp
 */

#include <iostream>
#include <JPetWriter/JPetWriter.h>
#include "TOTCuts.h"

using namespace std;

TOTCuts::TOTCuts(const char* name): JPetUserTask(name) {}


bool TOTCuts::init()
{
  INFO("TOT cuts estimation started.");
  INFO("Looking at two Hit Events  from the same layer with ID = 24 / 48 (only back to back for central source).");
  INFO("Plotting TOT for each module, calculating mean value and parsing to ascii file.");
  
  fOutputEvents = new JPetTimeWindow("JPetEvent");
  
  //TOT histo for each strip
  for(int i=1;i<=192;i++)
  {
    getStatistics().createHistogram(new TH1F(Form("TOT_%d",i), Form("TOT_%d",i), 100, 0, 100));
    getStatistics().getHisto1D(Form("TOT_%d",i))->GetXaxis()->SetTitle("TOT [ns]");
    getStatistics().getHisto1D(Form("TOT_%d",i))->GetYaxis()->SetTitle("Counts");
    getStatistics().getHisto1D(Form("TOT_%d",i))->SetLineWidth(3);
  }  
    getStatistics().createHistogram(new TH2F("MeanTOTvsID","Mean TOT vs ID",101, 0, 100, 193, -0.5, 192.5));
    getStatistics().getHisto2D("MeanTOTvsID")->GetXaxis()->SetTitle("Mean TOT [ns]");
    getStatistics().getHisto2D("MeanTOTvsID")->GetYaxis()->SetTitle("ID");
                
  fOutputFile.open(fOutputName.c_str());

  return true;
}

bool TOTCuts::exec()
{
  if (auto timeWindow = dynamic_cast<const JPetTimeWindow* const>(fEvent)) {
    uint n = timeWindow->getNumberOfEvents();
    for (uint i = 0; i < n; ++i) {

      const auto& event = dynamic_cast<const JPetEvent&>(timeWindow->operator[](i));

      if(event.getHits().size() == 2 ){
	JPetHit& firstHit = event.getHits().at(0);
	
	
	JPetHit& secondHit = event.getHits().at(1);
	if(isBackToBack(firstHit, secondHit)){
	  cout << firstHit.getScintillator().getID() << endl;
	  getStatistics().getHisto1D(Form("TOT_%d",firstHit.getScintillator().getID()))->Fill(calculateSumOfTOTs(firstHit));
	  cout << secondHit.getScintillator().getID() << endl;
	  getStatistics().getHisto1D(Form("TOT_%d",secondHit.getScintillator().getID()))->Fill(calculateSumOfTOTs(secondHit));
	}
	
      }
    }
  }
  
  return true;
}

bool TOTCuts::terminate()
{
  double mean = -999.0;
  for(int i=1;i<=192;i++)
  {
    TCanvas *c1 = new TCanvas("c1");
    mean = getStatistics().getHisto1D(Form("TOT_%d",i))->GetMean();
    getStatistics().getHisto1D(Form("TOT_%d",i))->Draw();
    TLine *line = new TLine(mean, 0, mean, getStatistics().getHisto1D(Form("TOT_%d",i))->GetMaximum());
    line->SetLineColor(kRed);
    line->Draw();
    line = new TLine(mean, 0, mean, getStatistics().getHisto1D(Form("TOT_%d",i))->GetMaximum());
    line->Draw();
    getStatistics().getHisto2D("MeanTOTvsID")->Fill(mean,i);
    savingTOT(mean,i);
  }
       
  INFO(Form("Finished with finding TOT cuts values, results are in %s", fOutputName.c_str()));
  fOutputFile.close();

  return true;
}

bool TOTCuts::isBackToBack(const JPetHit& firstHit, const JPetHit& secondHit)
{
  int deltaID = fabs(firstHit.getScintillator().getID() - secondHit.getScintillator().getID());
  bool backToBack = false;
  if( firstHit.getBarrelSlot().getLayer().getID() == 1 && secondHit.getBarrelSlot().getLayer().getID() == 1 )
      backToBack = (deltaID == 24);
  else if( firstHit.getBarrelSlot().getLayer().getID() == 2 && secondHit.getBarrelSlot().getLayer().getID() == 2 )
      backToBack = (deltaID == 24);
  else if( firstHit.getBarrelSlot().getLayer().getID() == 3 && secondHit.getBarrelSlot().getLayer().getID() == 3 )
      backToBack = (deltaID == 48);
  cout << backToBack << "\t" << firstHit.getScintillator().getID() << "\t" << firstHit.getBarrelSlot().getLayer().getID() << endl;
  return backToBack;
}

void TOTCuts::savingTOT(double meanTOT, int ID)
{
  fOutputFile << ID << "\t" << meanTOT << "\n";
}


double TOTCuts::calculateSumOfTOTs(const JPetHit& hit)
{
  cout << hit.getScintillator().getID() << endl;
  cout << "Side A\n" << endl;
  double TOT = calculateSumOfTOTsForSignal(hit, 'A');
  cout << "Side B\n" << endl;
  TOT+= calculateSumOfTOTsForSignal(hit, 'B');

  return TOT;
}

double TOTCuts::calculateSumOfTOTsForSignal(const JPetHit& hit, char label)
{
  std::map<int,double> leadingPoints, trailingPoints;
  if('A' == label)
  //calculate for A
  {
	cout << "Side A\n" << endl;
    leadingPoints = hit.getSignalA().getRecoSignal().getRawSignal().getTimesVsThresholdNumber(JPetSigCh::Leading);
    cout << "leading points taken\n";
    trailingPoints = hit.getSignalA().getRecoSignal().getRawSignal().getTimesVsThresholdNumber(JPetSigCh::Trailing);
    cout << "trailing points taken\n";
  }
  else if ('B' == label)
  //calculate for B
  {
	cout << "Side B\n" << endl;
    leadingPoints = hit.getSignalB().getRecoSignal().getRawSignal().getTimesVsThresholdNumber(JPetSigCh::Leading);
    trailingPoints = hit.getSignalB().getRecoSignal().getRawSignal().getTimesVsThresholdNumber(JPetSigCh::Trailing);
  }
  else
  {
    ERROR("Wrong label provided to EventCategorizer::calculateSumOfTOTsForSignal");
    return -10;
  }
  cout << "Map acquired\n";
  double TOT = 0;
  //finding TOT for every threshold 1-4
  for(int i=1;i<5;i++){
    auto leadSearch = leadingPoints.find(i);
    auto trailSearch = trailingPoints.find(i);
    if (leadSearch != leadingPoints.end() && trailSearch != trailingPoints.end())
      TOT+=(trailSearch->second - leadSearch->second)/1000;
  }
  return TOT;
}




