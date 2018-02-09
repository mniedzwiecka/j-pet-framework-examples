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
 *  @file EventCategorizer.cpp
 */

#include <iostream>
#include <JPetWriter/JPetWriter.h>
#include "EventCategorizer.h"


using namespace std;

EventCategorizer::EventCategorizer(const char* name): JPetUserTask(name) {}

bool EventCategorizer::init()
{

  INFO("Event categorization started.");
  INFO("Looking at two hit Events on Layer 1&2 only - creating only control histograms");

  fOutputEvents = new JPetTimeWindow("JPetEvent");

  if (fSaveControlHistos) {
    getStatistics().createHistogram(
      new TH1F("two_hit_event_theta_diff",
               "Abs Theta Difference Between Two Hits in Event",
               360, -0.5, 359.5)
    );
    getStatistics().createHistogram(
      new TH1F("two_hit_event_theta_diff_cut",
               "Abs Theta Difference Between Opposite Hits in Event",
               360, -0.5, 359.5)
    );
    getStatistics().createHistogram(
      new TH1F("hits_x_pos",
               "Hits X position",
               100, -60.0, 60.0)
    );
    getStatistics().createHistogram(
      new TH1F("hits_y_pos",
               "Hits Y position",
               100, -60.0, 60.0)
    );
    getStatistics().createHistogram(
      new TH1F("hits_z_pos",
               "Hits Z position",
               100, -60.0, 60.0)
    );
    getStatistics().createHistogram(
      new TH1F("hits_x_pos_cut",
               "Opposite Hits X position",
               100, -60.0, 60.0)
    );
    getStatistics().createHistogram(
      new TH1F("hits_y_pos_cut",
               "Opposite Hits Y position",
               100, -60.0, 60.0)
    );
    getStatistics().createHistogram(
      new TH1F("hits_z_pos_cut",
               "Opposite Hits Z position",
               100, -60.0, 60.0)
    );
    getStatistics().createHistogram(
      new TH2F("hit_distanece_vs_time_diff",
               "Two Hit distance vs. abs time difference",
               100, 0.0, 150.0,
               100, 0.0, 6000.0)
    );
    getStatistics().createHistogram(
      new TH2F("hit_distanece_vs_theta_diff",
               "Two Hit distance vs. abs theta difference",
               100, 0.0, 150.0,
               360, -0.5, 359.5)
    );
    getStatistics().createHistogram(
      new TH2F("hit_distanece_vs_time_diff_cut",
               "Opposite Hits distance vs. abs time difference",
               100, 0.0, 150.0,
               100, 0.0, 6000.0)
    );
    getStatistics().createHistogram(
      new TH2F("hit_distanece_vs_theta_diff_cut",
               "Opposite Hit distance vs. abs theta difference",
               100, 0.0, 150.0,
               360, -0.5, 359.5)
    );

    getStatistics().createHistogram(
      new TH2F("3_hit_angles",
               "3 Hit angles difference 1-2, 2-3",
               360, -0.5, 359.5,
               360, -0.5, 359.5)
    );
  }
  
  
  
  foutname = "6_sources_5_step"; 
	//1 - brak ciec, 2 - tot cut, 3 - ciecie na z hitu/anihilacji w zaleznosci od kombinacji, 5 - ciecie na z axis, bez ciecia na z punktu anihilacji, collimator_0_0_0_new_without

	interLayerTOFCoeff[0][0] = interLayerTOFCoeff[1][1] = interLayerTOFCoeff[2][2] = 0;
	interLayerTOFCoeff[0][1] = interLayerTOFCoeff[1][0] = 0.1417;
	interLayerTOFCoeff[0][2] = interLayerTOFCoeff[2][0] = 0.5003;
	interLayerTOFCoeff[1][2] = interLayerTOFCoeff[2][1] = 0.3585;


	if (fSaveControlHistos){
		
		
		//TOF histograms for each layer
		getStatistics().createHistogram(new TH1F("TOF_layer_1", "TOF for layer 1", 100, -5, 5));
		getStatistics().getHisto1D("TOF_layer_1")->GetXaxis()->SetTitle("TOF [ns]");
		getStatistics().getHisto1D("TOF_layer_1")->GetYaxis()->SetTitle("Counts");
		getStatistics().getHisto1D("TOF_layer_1")->SetLineWidth(3);
		
		getStatistics().createHistogram(new TH1F("TOF_layer_2", "TOF for layer 2", 100, -5, 5));
		getStatistics().getHisto1D("TOF_layer_2")->GetXaxis()->SetTitle("TOF [ns]");
		getStatistics().getHisto1D("TOF_layer_2")->GetYaxis()->SetTitle("Counts");
		getStatistics().getHisto1D("TOF_layer_2")->SetLineWidth(3);
		
		getStatistics().createHistogram(new TH1F("TOF_layer_3", "TOF for layer 3", 100, -5, 5));
		getStatistics().getHisto1D("TOF_layer_3")->GetXaxis()->SetTitle("TOF [ns]");
		getStatistics().getHisto1D("TOF_layer_3")->GetYaxis()->SetTitle("Counts");
		getStatistics().getHisto1D("TOF_layer_3")->SetLineWidth(3);

		getStatistics().createHistogram(new TH2F("TOF_vs_ID", "TOF vs ID", 192, 0.5 , 192.5, 200, -5, 5));
		getStatistics().getHisto2D("TOF_vs_ID")->GetXaxis()->SetTitle("ID");
		getStatistics().getHisto2D("TOF_vs_ID")->GetYaxis()->SetTitle("TOF [ns]");
		
		//TOF histogram between layers
		
		getStatistics().createHistogram(new TH1F("TOF_layer_1_2", "TOF between layer 1 and 2", 100, -5, 5));
		getStatistics().getHisto1D("TOF_layer_1_2")->GetXaxis()->SetTitle("TOF [ns]");
		getStatistics().getHisto1D("TOF_layer_1_2")->GetYaxis()->SetTitle("Counts");
		getStatistics().getHisto1D("TOF_layer_1_2")->SetLineWidth(3);
		
		getStatistics().createHistogram(new TH1F("TOF_layer_1_3", "TOF between layer 1 and 3", 100, -5, 5));
		getStatistics().getHisto1D("TOF_layer_1_3")->GetXaxis()->SetTitle("TOF [ns]");
		getStatistics().getHisto1D("TOF_layer_1_3")->GetYaxis()->SetTitle("Counts");
		getStatistics().getHisto1D("TOF_layer_1_3")->SetLineWidth(3);
		
		getStatistics().createHistogram(new TH1F("TOF_layer_2_3", "TOF between layer 2 and 3", 100, -5, 5));
		getStatistics().getHisto1D("TOF_layer_2_3")->GetXaxis()->SetTitle("TOF [ns]");
		getStatistics().getHisto1D("TOF_layer_2_3")->GetYaxis()->SetTitle("Counts");
		getStatistics().getHisto1D("TOF_layer_2_3")->SetLineWidth(3);
		
		//No. of events
		
		getStatistics().createHistogram(new TH1F("NoEvents", "No. of events", 192 , 0.5, 192.5));
		getStatistics().getHisto1D("NoEvents")->GetXaxis()->SetTitle("ID");
		getStatistics().getHisto1D("NoEvents")->GetYaxis()->SetTitle("Counts");
		getStatistics().getHisto1D("NoEvents")->SetLineWidth(3);
		
		//LOR
		getStatistics().createHistogram(new TH1F("LOR_distance", "LOR distance", 66 , -0.5, 65.5));
		getStatistics().getHisto1D("LOR_distance")->GetXaxis()->SetTitle("Distance [cm]");
		getStatistics().getHisto1D("LOR_distance")->GetYaxis()->SetTitle("Counts");
		getStatistics().getHisto1D("LOR_distance")->SetLineWidth(3);
		
		//Delta T AB vs ID
		
		getStatistics().createHistogram(new TH2F("TimeDiffBA_vs_ID", "TimeDiff BA vs ID", 192, 0.5 , 192.5, 100, -5, 5));
		getStatistics().getHisto2D("TimeDiffBA_vs_ID")->GetXaxis()->SetTitle("ID");		
		getStatistics().getHisto2D("TimeDiffBA_vs_ID")->GetYaxis()->SetTitle("TimeDiffBA [ns]");

		// Annihilation point in XY and YZ projection
		getStatistics().createHistogram(new TH2F("Annihilation_pointXY","X and Y of Annihilation point",101, -50.5, 50.5,101, -50.5, 50.5));
		getStatistics().getHisto2D("Annihilation_pointXY")->GetXaxis()->SetTitle("X axis position [cm]");
		getStatistics().getHisto2D("Annihilation_pointXY")->GetYaxis()->SetTitle("Y axis position [cm]");
		
		getStatistics().createHistogram(new TH2F("Annihilation_pointYZ","Y and Z of Annihilation point",101, -50.5, 50.5,101, -50.5, 50.5));
		getStatistics().getHisto2D("Annihilation_pointYZ")->GetXaxis()->SetTitle("Y axis position [cm]");
		getStatistics().getHisto2D("Annihilation_pointYZ")->GetYaxis()->SetTitle("Z axis position [cm]");
		
		getStatistics().createHistogram(new TH1F("X_plane", "X plane",  101, -50.5, 50.5));
		getStatistics().getHisto1D("X_plane")->GetXaxis()->SetTitle("X axis position [cm]");
		getStatistics().getHisto1D("X_plane")->GetYaxis()->SetTitle("Counts");
		getStatistics().getHisto1D("X_plane")->SetLineWidth(3);
		
		getStatistics().createHistogram(new TH1F("Y_plane", "Y plane",  101, -50.5, 50.5));
		getStatistics().getHisto1D("Y_plane")->GetXaxis()->SetTitle("Y axis position [cm]");
		getStatistics().getHisto1D("Y_plane")->GetYaxis()->SetTitle("Counts");
		getStatistics().getHisto1D("Y_plane")->SetLineWidth(3);
		
		getStatistics().createHistogram(new TH1F("Z_plane", "Z plane",  101, -50.5, 50.5));
		getStatistics().getHisto1D("Z_plane")->GetXaxis()->SetTitle("Z axis position [cm]");
		getStatistics().getHisto1D("Z_plane")->GetYaxis()->SetTitle("Counts");
		getStatistics().getHisto1D("Z_plane")->SetLineWidth(3);
	
		//Delta ID vs TOF
		getStatistics().createHistogram(new TH2F("TOFvsDeltaID_1","TOF vs delta ID layer 1",101, -5.5, 5.5, 25, -0.5, 24.5));
		getStatistics().getHisto2D("TOFvsDeltaID_1")->GetXaxis()->SetTitle("TOF [ns]");
		getStatistics().getHisto2D("TOFvsDeltaID_1")->GetYaxis()->SetTitle("Delta ID");
		
		getStatistics().createHistogram(new TH2F("TOFvsDeltaID_2","TOF vs delta ID layer 2",101, -5.5, 5.5, 25, -0.5, 24.5));
		getStatistics().getHisto2D("TOFvsDeltaID_2")->GetXaxis()->SetTitle("TOF [ns]");
		getStatistics().getHisto2D("TOFvsDeltaID_2")->GetYaxis()->SetTitle("Delta ID");
		
		getStatistics().createHistogram(new TH2F("TOFvsDeltaID_3","TOF vs delta ID layer 3",101, -5.5, 5.5, 49, -0.5, 48.5));
		getStatistics().getHisto2D("TOFvsDeltaID_3")->GetXaxis()->SetTitle("TOF [ns]");
		getStatistics().getHisto2D("TOFvsDeltaID_3")->GetYaxis()->SetTitle("Delta ID");
		
		//ID vs TOF
		getStatistics().createHistogram(new TH2F("TOFvsID_1","TOF vs ID layer 1",101, -5.5, 5.5, 48, 0.5, 48.5));
		getStatistics().getHisto2D("TOFvsID_1")->GetXaxis()->SetTitle("TOF [ns]");
		getStatistics().getHisto2D("TOFvsID_1")->GetYaxis()->SetTitle("ID");
		
		getStatistics().createHistogram(new TH2F("TOFvsID_2","TOF vs ID layer 2",101, -5.5, 5.5, 48, 48.5, 96.5));
		getStatistics().getHisto2D("TOFvsID_2")->GetXaxis()->SetTitle("TOF [ns]");
		getStatistics().getHisto2D("TOFvsID_2")->GetYaxis()->SetTitle("ID");
		
		getStatistics().createHistogram(new TH2F("TOFvsID_3","TOF vs ID layer 3",101, -5.5, 5.5, 96, 96.5, 192.5));
		getStatistics().getHisto2D("TOFvsID_3")->GetXaxis()->SetTitle("TOF [ns]");
		getStatistics().getHisto2D("TOFvsID_3")->GetYaxis()->SetTitle("ID");
		
		getStatistics().createHistogram(new TH2F("TOFvsID_all","TOF vs ID layer all",101, -5.5, 5.5, 192, 0.5, 192.5));
		getStatistics().getHisto2D("TOFvsID_all")->GetXaxis()->SetTitle("TOF [ns]");
		getStatistics().getHisto2D("TOFvsID_all")->GetYaxis()->SetTitle("ID");
		
		//ID vs TOF - porownanie ID and Angle
		getStatistics().createHistogram(new TH2F("TOFvsID_ID_1","TOF vs ID layer 1 (ID)",101, -5.5, 5.5, 48, 0.5, 48.5));
		getStatistics().getHisto2D("TOFvsID_ID_1")->GetXaxis()->SetTitle("TOF [ns]");
		getStatistics().getHisto2D("TOFvsID_ID_1")->GetYaxis()->SetTitle("ID");
		
		getStatistics().createHistogram(new TH2F("TOFvsID_ID_2","TOF vs ID layer 2 (ID)",101, -5.5, 5.5, 48, 48.5, 96.5));
		getStatistics().getHisto2D("TOFvsID_ID_2")->GetXaxis()->SetTitle("TOF [ns]");
		getStatistics().getHisto2D("TOFvsID_ID_2")->GetYaxis()->SetTitle("ID");
		
		getStatistics().createHistogram(new TH2F("TOFvsID_ID_3","TOF vs ID layer 3 (ID)",101, -5.5, 5.5, 96, 96.5, 192.5));
		getStatistics().getHisto2D("TOFvsID_ID_3")->GetXaxis()->SetTitle("TOF [ns]");
		getStatistics().getHisto2D("TOFvsID_ID_3")->GetYaxis()->SetTitle("ID");
		
		
		getStatistics().createHistogram(new TH2F("TOFvsID_Angle_1","TOF vs ID layer 1 (Angle)",101, -5.5, 5.5, 48, 0.5, 48.5));
		getStatistics().getHisto2D("TOFvsID_Angle_1")->GetXaxis()->SetTitle("TOF [ns]");
		getStatistics().getHisto2D("TOFvsID_Angle_1")->GetYaxis()->SetTitle("ID");
		
		getStatistics().createHistogram(new TH2F("TOFvsID_Angle_2","TOF vs ID layer 2 (Angle)",101, -5.5, 5.5, 48, 48.5, 96.5));
		getStatistics().getHisto2D("TOFvsID_Angle_2")->GetXaxis()->SetTitle("TOF [ns]");
		getStatistics().getHisto2D("TOFvsID_Angle_2")->GetYaxis()->SetTitle("ID");
		
		getStatistics().createHistogram(new TH2F("TOFvsID_Angle_3","TOF vs ID layer 3 (Angle)",101, -5.5, 5.5, 96, 96.5, 192.5));
		getStatistics().getHisto2D("TOFvsID_Angle_3")->GetXaxis()->SetTitle("TOF [ns]");
		getStatistics().getHisto2D("TOFvsID_Angle_3")->GetYaxis()->SetTitle("ID");
		
		//Delta Angle vs TOF
		getStatistics().createHistogram(new TH2F("TOFvsDeltaAngle","TOF vs delta angle layer",101, -0.5, 5.5, 180, -0.5, 180.5));
		getStatistics().getHisto2D("TOFvsDeltaAngle")->GetXaxis()->SetTitle("TOF [ns]");
		getStatistics().getHisto2D("TOFvsDeltaAngle")->GetYaxis()->SetTitle("Delta Angle [deg]");

		
		//Delta ID vs TOF fabs
		getStatistics().createHistogram(new TH2F("TOFvsID_1_fabs","TOF vs delta ID layer 1",101, 0, 5.5, 25, -0.5, 24.5));
		getStatistics().getHisto2D("TOFvsID_1_fabs")->GetXaxis()->SetTitle("TOF [ns]");
		getStatistics().getHisto2D("TOFvsID_1_fabs")->GetYaxis()->SetTitle("Delta ID");
		
		getStatistics().createHistogram(new TH2F("TOFvsID_2_fabs","TOF vs delta ID layer 2",101, 0, 5.5, 25, -0.5, 24.5));
		getStatistics().getHisto2D("TOFvsID_2_fabs")->GetXaxis()->SetTitle("TOF [ns]");
		getStatistics().getHisto2D("TOFvsID_2_fabs")->GetYaxis()->SetTitle("Delta ID");
		
		getStatistics().createHistogram(new TH2F("TOFvsID_3_fabs","TOF vs delta ID layer 3",101, 0, 5.5, 49, -0.5, 48.5));
		getStatistics().getHisto2D("TOFvsID_3_fabs")->GetXaxis()->SetTitle("TOF [ns]");
		getStatistics().getHisto2D("TOFvsID_3_fabs")->GetYaxis()->SetTitle("Delta ID");
	
		//TOT
		getStatistics().createHistogram(new TH1F("Sum_TOTs", "Sum of TOTs", 100, 0, 100));
		getStatistics().getHisto1D("Sum_TOTs")->GetXaxis()->SetTitle("TOT [ns]");
		getStatistics().getHisto1D("Sum_TOTs")->GetYaxis()->SetTitle("Counts");
		getStatistics().getHisto1D("Sum_TOTs")->SetLineWidth(3);
		
	    //Delta ID
	    getStatistics().createHistogram(new TH1F("DeltaID_layer_1", "DeltaID for layer 1",  25, -0.5, 24.5));
		getStatistics().getHisto1D("DeltaID_layer_1")->GetXaxis()->SetTitle("DeltaID");
		getStatistics().getHisto1D("DeltaID_layer_1")->GetYaxis()->SetTitle("Counts");
		getStatistics().getHisto1D("DeltaID_layer_1")->SetLineWidth(3);
		
		getStatistics().createHistogram(new TH1F("DeltaID_layer_2", "DeltaID for layer 2", 25, -0.5, 24.5));
		getStatistics().getHisto1D("DeltaID_layer_2")->GetXaxis()->SetTitle("DeltaID");
		getStatistics().getHisto1D("DeltaID_layer_2")->GetYaxis()->SetTitle("Counts");
		getStatistics().getHisto1D("DeltaID_layer_2")->SetLineWidth(3);
		
		getStatistics().createHistogram(new TH1F("DeltaID_layer_3", "DeltaID for layer 3", 49, -0.5, 48.5));
		getStatistics().getHisto1D("DeltaID_layer_3")->GetXaxis()->SetTitle("DeltaID");
		getStatistics().getHisto1D("DeltaID_layer_3")->GetYaxis()->SetTitle("Counts");
		getStatistics().getHisto1D("DeltaID_layer_3")->SetLineWidth(3);
		
		//Delta Angle
		getStatistics().createHistogram(new TH1F("DeltaAngle", "Delta_Angle",  180, -0.5, 180.5));
		getStatistics().getHisto1D("DeltaAngle")->GetXaxis()->SetTitle("Delta Angle [deg]");
		getStatistics().getHisto1D("DeltaAngle")->GetYaxis()->SetTitle("Counts");
		getStatistics().getHisto1D("DeltaAngle")->SetLineWidth(3);
		
		//TOT histo for each strip
		
		for(int i=1;i<=192;i++)
		{
		getStatistics().createHistogram(new TH1F(Form("TOT_%d",i), Form("TOT_%d",i), 100, 0, 100));
		getStatistics().getHisto1D(Form("TOT_%d",i))->GetXaxis()->SetTitle("TOT [ns]");
		getStatistics().getHisto1D(Form("TOT_%d",i))->GetYaxis()->SetTitle("Counts");
		getStatistics().getHisto1D(Form("TOT_%d",i))->SetLineWidth(3);
		
		//~ getStatistics().createHistogram(new TH1F(Form("TimeDiff_%d",i), Form("TimeDiff_%d",i), 100, -10, 10));
		//~ getStatistics().getHisto1D(Form("TimeDiff_%d",i))->GetXaxis()->SetTitle("TimeDiffAB [ns]");
		//~ getStatistics().getHisto1D(Form("TimeDiff_%d",i))->GetYaxis()->SetTitle("Counts");
		//~ getStatistics().getHisto1D(Form("TimeDiff_%d",i))->SetLineWidth(3);
		
		}
		
		for (int thr=1;thr<=4;thr++){
		for(int i=1;i<=192;i++)
		{
		getStatistics().createHistogram(new TH1F(Form("TimeDiffAB_ID_%d_thr_%d",i,thr), Form("TimeDiffAB_ID_%d_thr_%d",i,thr), 100, -10, 10));
		getStatistics().getHisto1D(Form("TimeDiffAB_ID_%d_thr_%d",i,thr))->GetXaxis()->SetTitle("TimeDiffAB [ns]");
		getStatistics().getHisto1D(Form("TimeDiffAB_ID_%d_thr_%d",i,thr))->GetYaxis()->SetTitle("Counts");
		getStatistics().getHisto1D(Form("TimeDiffAB_ID_%d_thr_%d",i,thr))->SetLineWidth(3);
		}
		}
		
		getStatistics().createHistogram(new TH2F("TOTvsID","TOT vs ID",101, 0, 100, 192, 0.5, 192.5));
		getStatistics().getHisto2D("TOTvsID")->GetXaxis()->SetTitle("TOT [ns]");
		getStatistics().getHisto2D("TOTvsID")->GetYaxis()->SetTitle("ID");

	
		getStatistics().createHistogram(new TH2F("z_vs_ID", "hit z coordinate vs ID", 192,0.5, 192.5, 50, -25, 25));
		getStatistics().getHisto2D("z_vs_ID")->GetXaxis()->SetTitle("ID");
		getStatistics().getHisto2D("z_vs_ID")->GetYaxis()->SetTitle("z");

	}
	
	    getStatistics().createHistogram(new TH2F("MeanTOTvsID","Mean TOT vs ID",101, 0, 100, 193, -0.5, 192.5));
		getStatistics().getHisto2D("MeanTOTvsID")->GetXaxis()->SetTitle("Mean TOT [ns]");
		getStatistics().getHisto2D("MeanTOTvsID")->GetYaxis()->SetTitle("ID");
		
		
//Magda		
		getStatistics().createHistogram(new TH2F("hit_oposite_time_diff_layer1","Two Oposite Hit time difference - Layer1",24, 0.5, 24.5,120, -6., 6.));
		getStatistics().createHistogram(new TH2F("hit_oposite_time_diff_layer2","Two Oposite Hit time difference - Layer2",24, 0.5, 24.5,120, -6., 6.));
		getStatistics().createHistogram(new TH2F("hit_oposite_time_diff_layer3","Two Oposite Hit time difference - Layer3",48, 0.5, 48.5,120, -6., 6.));

//Multiplicity

		getStatistics().createHistogram(new TH1F("multiplicity","Multiplicity",10, -0.5, 9.5));
	
	//~ ifstream totCutsFile;
	//~ totCutsFile.open("MeanTOT_collimator_0.txt");
//~ 
	//~ int id; double cut_down, cut_up;
	//~ while( totCutsFile >> id >> cut_down >> cut_up)
		//~ fTotCuts[id] = cut_down;
	//~ 
	//~ if(fTotCuts.size() != 192)
		//~ cout << "Problem with size of tot cuts vector, its size is " << fTotCuts.size() << " instead of 192 \n";
//~ 
	//~ totCutsFile.close();	

	outputFile.open(foutname+".txt");
  
  
  
  return true;
}

void EventCategorizer::savingToRecoFile(JPetHit hit_one, JPetHit hit_two)
{
	
	outputFile << hit_one.getPosX() << "\t" << hit_one.getPosY() << "\t" << hit_one.getPosZ() << "\t" << hit_one.getTime()*1000 << "\t" << hit_two.getPosX() << "\t" << hit_two.getPosY() << "\t" << hit_two.getPosZ() << "\t" << hit_two.getTime()*1000 << "\n";
	
}

void EventCategorizer::savingTOT(double meanTOT, int ID)
{
	
	outputFile << ID << "\t" << meanTOT << "\t" << (38.0/20.0)*meanTOT << "\n";
	
}

double EventCategorizer::calculateSumOfTOTs(const JPetHit& hit)
{
	
	double TOT = calculateSumOfTOTsForSignal(hit, 'A');
	TOT+= calculateSumOfTOTsForSignal(hit, 'B');
	
	return TOT;
}

bool EventCategorizer::cutOnZAxis(const JPetHit& firstHit, const JPetHit& secondHit)
{
return fabs(firstHit.getPosZ()) < 23 && fabs(secondHit.getPosZ()) < 23;  
}


double EventCategorizer::calculateSumOfTOTsForSignal(const JPetHit& hit, char label)
{
	std::map<int,double> leadingPoints, trailingPoints;
	if('A' == label)
	//calculate for A
	{
		leadingPoints = hit.getSignalA().getRecoSignal().getRawSignal().getTimesVsThresholdNumber(JPetSigCh::Leading);
		trailingPoints = hit.getSignalA().getRecoSignal().getRawSignal().getTimesVsThresholdNumber(JPetSigCh::Trailing);
	}		
	else if ('B' == label)
	//calculate for B
	{
		leadingPoints = hit.getSignalB().getRecoSignal().getRawSignal().getTimesVsThresholdNumber(JPetSigCh::Leading);
		trailingPoints = hit.getSignalB().getRecoSignal().getRawSignal().getTimesVsThresholdNumber(JPetSigCh::Trailing);
	}		
	else
	{
		ERROR("Wrong label provided to EventCategorizer::calculateSumOfTOTsForSignal");
		return -10;
	}
	double TOT = 0;
			//finding TOT for every threshold 1-4
				for(int i=1;i<5;i++){
					auto leadSearch = leadingPoints.find(i);
					auto trailSearch = trailingPoints.find(i);
					if (leadSearch != leadingPoints.end()
						&& trailSearch != trailingPoints.end())
							TOT+=(trailSearch->second - leadSearch->second)/1000;
				}
				
	return TOT;
	
}

point EventCategorizer::findAnnihilationPoint(const JPetHit& firstHit, const JPetHit& secondHit)
{
	double tof = fabs( firstHit.getTime() - secondHit.getTime())/1000;
	double VecLength = sqrt( pow(firstHit.getPosX()-secondHit.getPosX(),2)	// Pos in cm
			+pow(firstHit.getPosY()-secondHit.getPosY(),2)
			+pow(firstHit.getPosZ()-secondHit.getPosZ(),2) );
	
	double middleX = (firstHit.getPosX()+secondHit.getPosX() )/2;
	double middleY = (firstHit.getPosY()+secondHit.getPosY() )/2;
	double middleZ = (firstHit.getPosZ()+secondHit.getPosZ() )/2;
	double Fraction = 2.0*tof*29.979246/VecLength;
	if( firstHit.getTime() >= secondHit.getTime() )
	{
	fPoint.x = middleX + Fraction*( firstHit.getPosX()-middleX );
	fPoint.y = middleY + Fraction*( firstHit.getPosY()-middleY ) ;
	fPoint.z = middleZ + Fraction*( firstHit.getPosZ()-middleZ );
	}
	else
	 {
	fPoint.x = middleX + Fraction*( secondHit.getPosX()-middleX ); 
	fPoint.y = middleY + Fraction*( secondHit.getPosY()-middleY );
	fPoint.z = middleZ + Fraction*( secondHit.getPosZ()-middleZ );
	}
	
	return fPoint;
}

bool EventCategorizer::exec()
{
//TUTAJ MOZESZ MODYFIKOWAC :)
  //Analysis of Events consisting of two hits that come from Layer 1 or 2
  //Layer 3 is ignored, since it is not callibrated
  if (auto timeWindow = dynamic_cast<const JPetTimeWindow* const>(fEvent)) {
    uint n = timeWindow->getNumberOfEvents();
    for (uint i = 0; i < n; ++i) {

      const auto& event = dynamic_cast<const JPetEvent&>(timeWindow->operator[](i));
		
		//getStatistics().getHisto1D("multiplicity").Fill(event.getHits().size());
       
     //if(false){  
     if(event.getHits().size() == 2 ){
	//getStatistics().getHisto1D("multiplicity").Fill(event.getHits().size());

		  vector<JPetHit> hits = event.getHits();
		  for(int i=0;i<hits.size();i++){
		    for(int j=i+1;j<hits.size();j++){
          JPetHit firstHit = hits.at(i);
			    JPetHit secondHit = hits.at(j);
			    
            float thetaDiff = fabs(firstHit.getBarrelSlot().getTheta()-secondHit.getBarrelSlot().getTheta());
    
            
            float distance = sqrt(pow(firstHit.getPosX()-secondHit.getPosX(),2)
              +pow(firstHit.getPosY()-secondHit.getPosY(),2)
              +pow(firstHit.getPosZ()-secondHit.getPosZ(),2));
              
              double TOT1 = calculateSumOfTOTs(firstHit);
              double TOT2 = calculateSumOfTOTs(secondHit);
               
  	
			  
              double_t slot_no; 
              int slot_number_first = firstHit.getBarrelSlot().getID();
			  int slot_number_second = secondHit.getBarrelSlot().getID();
            
             
              int deltaID = DeltaID(firstHit,secondHit);
		fPoint = findAnnihilationPoint(firstHit,secondHit);
	
//		if (fSaveControlHistos && makeTotCut(firstHit, secondHit) && cutOnZAxis(firstHit, secondHit) && cutOnLorDistanceFromCenter(firstHit, secondHit)){

		if (fSaveControlHistos){
		// && cutOnZAxis(firstHit, secondHit) && fabs(fPoint.z)< 23 && cutOnLorDistanceFromCenter(firstHit, secondHit) && DeltaAngle(firstHit, secondHit)>20 && TOT1>15 && TOT1<25 && TOT2>15 && TOT2<25){
		
	getStatistics().getHisto1D("multiplicity")->Fill(event.getHits().size());
 			//if(false){
		  			
				    float timeDiff = secondHit.getTime()-firstHit.getTime();
		//-------TOF for each layer AND Delta ID vs TOF----------------------------------------------------
				if(firstHit.getBarrelSlot().getLayer().getID() == secondHit.getBarrelSlot().getLayer().getID()){

				    getStatistics().getHisto1D(Form("DeltaID_layer_%d",secondHit.getBarrelSlot().getLayer().getID()))->Fill(deltaID);
					if(firstHit.getBarrelSlot().getID() < secondHit.getBarrelSlot().getID()){
					 
					 getStatistics().getHisto1D(Form("TOF_layer_%d", secondHit.getBarrelSlot().getLayer().getID()))->Fill(timeDiff/1000.);
					 getStatistics().getHisto2D(Form("TOFvsDeltaID_%d", secondHit.getBarrelSlot().getLayer().getID()))->Fill(timeDiff/1000., deltaID);
					 getStatistics().getHisto2D(Form("TOFvsID_%d_fabs", secondHit.getBarrelSlot().getLayer().getID()))->Fill(fabs(timeDiff/1000.), deltaID);

					}
					else{
					    
					 getStatistics().getHisto1D(Form("TOF_layer_%d", secondHit.getBarrelSlot().getLayer().getID()))->Fill(-1.0*timeDiff/1000.);
					 getStatistics().getHisto2D(Form("TOFvsDeltaID_%d", secondHit.getBarrelSlot().getLayer().getID()))->Fill(-1.0*timeDiff/1000., deltaID);
					 getStatistics().getHisto2D(Form("TOFvsID_%d_fabs", secondHit.getBarrelSlot().getLayer().getID()))->Fill(fabs(-1.0*timeDiff/1000.), deltaID);
					}
				}
				
						for (int thres=1;thres<=4;thres++){
				getStatistics().getHisto1D(Form("TimeDiffAB_ID_%d_thr_%d", firstHit.getScintillator().getID(), thres))->Fill(JPetHitUtils::getTimeDiffAtThr(firstHit, thres)/1000.);
				getStatistics().getHisto1D(Form("TimeDiffAB_ID_%d_thr_%d", secondHit.getScintillator().getID(), thres))->Fill(JPetHitUtils::getTimeDiffAtThr(secondHit, thres)/1000.);
				}
				
				//getStatistics().getHisto1D(Form("TimeDiff_%d",firstHit.getScintillator().getID()))->Fill(firstHit.getTimeDiff()/1000.);
				//getStatistics().getHisto1D(Form("TimeDiff_%d",secondHit.getScintillator().getID()))->Fill(secondHit.getTimeDiff()/1000.);
	
				getStatistics().getHisto1D("NoEvents")->Fill(firstHit.getBarrelSlot().getID());
				getStatistics().getHisto1D("NoEvents")->Fill(secondHit.getBarrelSlot().getID());
				
				if(firstHit.getBarrelSlot().getLayer().getID()== 1 && secondHit.getBarrelSlot().getLayer().getID()==2){
					if(firstHit.getBarrelSlot().getTheta() < secondHit.getBarrelSlot().getTheta())
						getStatistics().getHisto1D("TOF_layer_1_2")->Fill(timeDiff/1000.);
					else
						getStatistics().getHisto1D("TOF_layer_1_2")->Fill(-1.0*timeDiff/1000.);
				}
				else if(firstHit.getBarrelSlot().getLayer().getID()== 2 && secondHit.getBarrelSlot().getLayer().getID()==1){
					if(firstHit.getBarrelSlot().getTheta() < secondHit.getBarrelSlot().getTheta())
						getStatistics().getHisto1D("TOF_layer_1_2")->Fill(timeDiff/1000.);
					else
						getStatistics().getHisto1D("TOF_layer_1_2")->Fill(-1.0*timeDiff/1000.);
				}
				
				if(firstHit.getBarrelSlot().getLayer().getID()== 1 && secondHit.getBarrelSlot().getLayer().getID()==3){
					if(firstHit.getBarrelSlot().getTheta() < secondHit.getBarrelSlot().getTheta())
						getStatistics().getHisto1D("TOF_layer_1_3")->Fill(timeDiff/1000.);
					else
						getStatistics().getHisto1D("TOF_layer_1_3")->Fill(-1.0*timeDiff/1000.);
				}
				else if(firstHit.getBarrelSlot().getLayer().getID()== 3 && secondHit.getBarrelSlot().getLayer().getID()==1){
					if(firstHit.getBarrelSlot().getTheta() < secondHit.getBarrelSlot().getTheta())
						getStatistics().getHisto1D("TOF_layer_1_3")->Fill(timeDiff/1000.);
					else
						getStatistics().getHisto1D("TOF_layer_1_3")->Fill(-1.0*timeDiff/1000.);
				}
				
				if(firstHit.getBarrelSlot().getLayer().getID()== 2 && secondHit.getBarrelSlot().getLayer().getID()==3){
					if(firstHit.getBarrelSlot().getTheta() < secondHit.getBarrelSlot().getTheta())
						getStatistics().getHisto1D("TOF_layer_2_3")->Fill(timeDiff/1000.);
					else
						getStatistics().getHisto1D("TOF_layer_2_3")->Fill(-1.0*timeDiff/1000.);
				}
				else if(firstHit.getBarrelSlot().getLayer().getID()== 3 && secondHit.getBarrelSlot().getLayer().getID()==2){
					if(firstHit.getBarrelSlot().getTheta() < secondHit.getBarrelSlot().getTheta())
						getStatistics().getHisto1D("TOF_layer_2_3")->Fill(timeDiff/1000.);
					else
						getStatistics().getHisto1D("TOF_layer_2_3")->Fill(-1.0*timeDiff/1000.);
				}
	
		getStatistics().getHisto2D("TimeDiffBA_vs_ID")->Fill(firstHit.getScintillator().getID(), firstHit.getTimeDiff()/1000.);
		getStatistics().getHisto2D("TimeDiffBA_vs_ID")->Fill(secondHit.getScintillator().getID(), secondHit.getTimeDiff()/1000.);

		//Annihilation point
				getStatistics().getHisto2D("Annihilation_pointXY")->Fill(fPoint.x, fPoint.y);
				getStatistics().getHisto2D("Annihilation_pointYZ")->Fill(fPoint.y, fPoint.z);
				
				getStatistics().getHisto1D("X_plane")->Fill(fPoint.x);
				getStatistics().getHisto1D("Y_plane")->Fill(fPoint.y);
				getStatistics().getHisto1D("Z_plane")->Fill(fPoint.z);
				
		//
		
		getStatistics().getHisto1D("LOR_distance")->Fill(LORDistance(firstHit, secondHit));
		//TOT and Sum of TOT
				getStatistics().getHisto1D("Sum_TOTs")->Fill(TOT1);
				getStatistics().getHisto1D("Sum_TOTs")->Fill(TOT2);
				//getStatistics().getHisto1D("Sum_TOTs").
				getStatistics().getHisto1D(Form("TOT_%d", firstHit.getScintillator().getID()))->Fill(TOT1); 
				getStatistics().getHisto1D(Form("TOT_%d", secondHit.getScintillator().getID()))->Fill(TOT2);
				
				getStatistics().getHisto2D("TOTvsID")->Fill(TOT1,firstHit.getScintillator().getID());
				getStatistics().getHisto2D("TOTvsID")->Fill(TOT2,secondHit.getScintillator().getID());
		//Angle
				double deltaAngle = DeltaAngle(firstHit, secondHit);
				getStatistics().getHisto2D("TOFvsDeltaAngle")->Fill(timeDiff/1000,deltaAngle);
				getStatistics().getHisto1D("DeltaAngle")->Fill(deltaAngle);
		
					
				getStatistics().getHisto2D("z_vs_ID")->Fill(firstHit.getScintillator().getID(), firstHit.getPosZ());	
				getStatistics().getHisto2D("z_vs_ID")->Fill(secondHit.getScintillator().getID(), secondHit.getPosZ());
				
			getStatistics().getHisto1D("hits_z_pos")->Fill(firstHit.getPosZ());
			getStatistics().getHisto1D("hits_z_pos")->Fill(secondHit.getPosZ());
		
				
		//TOF vs ID for different layers separately
		
		if(firstHit.getBarrelSlot().getLayer().getID() == secondHit.getBarrelSlot().getLayer().getID()){
			
					if(firstHit.getBarrelSlot().getID() < secondHit.getBarrelSlot().getID()){
					 getStatistics().getHisto2D(Form("TOFvsID_%d", secondHit.getBarrelSlot().getLayer().getID()))->Fill(timeDiff/1000., firstHit.getScintillator().getID());
					 getStatistics().getHisto2D(Form("TOFvsID_%d", secondHit.getBarrelSlot().getLayer().getID()))->Fill(timeDiff/1000., secondHit.getScintillator().getID());
					 
					  getStatistics().getHisto2D("TOFvsID_all")->Fill(timeDiff/1000., firstHit.getScintillator().getID());
					 getStatistics().getHisto2D("TOFvsID_all")->Fill(timeDiff/1000., secondHit.getScintillator().getID());
					}
					else{
					 getStatistics().getHisto2D(Form("TOFvsID_%d", secondHit.getBarrelSlot().getLayer().getID()))->Fill(-1.0*timeDiff/1000., secondHit.getScintillator().getID());
					 getStatistics().getHisto2D(Form("TOFvsID_%d", secondHit.getBarrelSlot().getLayer().getID()))->Fill(-1.0*timeDiff/1000., firstHit.getScintillator().getID());
					 
					 getStatistics().getHisto2D("TOFvsID_all")->Fill(-1.0*timeDiff/1000., secondHit.getScintillator().getID());
					 getStatistics().getHisto2D("TOFvsID_all")->Fill(-1.0*timeDiff/1000., firstHit.getScintillator().getID());
					}
					
					if(isBackToBack(firstHit,secondHit)){ 
								if(slot_number_first<25){
									slot_no=slot_number_first;
									getStatistics().getHisto2D("hit_oposite_time_diff_layer1")->Fill(slot_no,-timeDiff/1000.);

								}
								if(slot_number_first>24){
									slot_no=slot_number_first-24;
									getStatistics().getHisto2D("hit_oposite_time_diff_layer1")->Fill(slot_no,timeDiff/1000.);
								}
						}
//Layer 2
							if(isBackToBack(firstHit,secondHit)){
								if(slot_number_first<73){
									slot_no=slot_number_first-48;
									getStatistics().getHisto2D("hit_oposite_time_diff_layer2")->Fill(slot_no,-timeDiff/1000.);
								}
								if(slot_number_first>72){
								slot_no=slot_number_first-72;
								getStatistics().getHisto2D("hit_oposite_time_diff_layer2")->Fill(slot_no,timeDiff/1000.);
								}
							}
//Layer 3
							if(isBackToBack(firstHit,secondHit)){
								if(slot_number_first<145){
								slot_no=slot_number_first-96;
								getStatistics().getHisto2D("hit_oposite_time_diff_layer3")->Fill(slot_no,-timeDiff/1000.);
								}
								if(slot_number_first>144){
								slot_no=slot_number_first-144;
								getStatistics().getHisto2D("hit_oposite_time_diff_layer3")->Fill(slot_no,timeDiff/1000.);
								}
							}
				
				}
				
				//--------------------------------------------
		if(firstHit.getBarrelSlot().getLayer().getID() == secondHit.getBarrelSlot().getLayer().getID()){
			
			if(firstHit.getBarrelSlot().getID() < secondHit.getBarrelSlot().getID())
				getStatistics().getHisto2D(Form("TOFvsID_ID_%d", secondHit.getBarrelSlot().getLayer().getID()))->Fill(-1.0*timeDiff/1000., firstHit.getScintillator().getID());
			else
				getStatistics().getHisto2D(Form("TOFvsID_ID_%d", secondHit.getBarrelSlot().getLayer().getID()))->Fill(timeDiff/1000., secondHit.getScintillator().getID());
			
			if( firstHit.getBarrelSlot().getTheta() < secondHit.getBarrelSlot().getTheta() )
				getStatistics().getHisto2D(Form("TOFvsID_Angle_%d", secondHit.getBarrelSlot().getLayer().getID()))->Fill(-1.0*timeDiff/1000., firstHit.getScintillator().getID());
			else
				getStatistics().getHisto2D(Form("TOFvsID_Angle_%d", secondHit.getBarrelSlot().getLayer().getID()))->Fill(timeDiff/1000., secondHit.getScintillator().getID());
		}
		

		//	int firstHitLayer = firstHit.getBarrelSlot().getLayer().getID();      
		//	int secondHitLayer = secondHit.getBarrelSlot().getLayer().getID();      

		//	double timeCorrection = interLayerTOFCoeff[firstHitLayer-1][secondHitLayer-1];
			
		//	if(firstHitLayer > secondHitLayer)
		//		timeDiff+= timeCorrection*1000.0;	
		//	else
		//		timeDiff-= timeCorrection*1000.0;

			if( firstHit.getBarrelSlot().getTheta() < secondHit.getBarrelSlot().getTheta() )
			{	getStatistics().getHisto2D("TOF_vs_ID")->Fill(firstHit.getScintillator().getID(), timeDiff/1000.);	
				getStatistics().getHisto2D("TOF_vs_ID")->Fill(secondHit.getScintillator().getID(), timeDiff/1000.);	
			}
			else
			{
				getStatistics().getHisto2D("TOF_vs_ID")->Fill(firstHit.getScintillator().getID(),-1.0*timeDiff/1000.);	
				getStatistics().getHisto2D("TOF_vs_ID")->Fill(secondHit.getScintillator().getID(),-1.0*timeDiff/1000.);	
			}

		}
          
             
	   //   if(sqrt(fPoint.x*fPoint.x + fPoint.y*fPoint.y) < 44 && makeTotCut(firstHit, secondHit) && fabs(fPoint.z) <23) 

	//~ if(cutOnZAxis(firstHit, secondHit)  && fabs(fPoint.z)< 23 && cutOnLorDistanceFromCenter(firstHit, secondHit) && DeltaAngle(firstHit, secondHit)>20 && TOT1>15 && TOT1<25 && TOT2>15 && TOT2<25)
	//~ savingToRecoFile(firstHit,secondHit);
    
		    }
		  }
	}

      if (event.getHits().size() > 1) {

        vector<JPetHit> hits = event.getHits();
        for (size_t i = 0; i < hits.size(); i++) {
          for (size_t j = i + 1; j < hits.size(); j++) {
            JPetHit firstHit = hits.at(i);
            JPetHit secondHit = hits.at(j);

            if (firstHit.getBarrelSlot().getLayer().getID() != 3
                && secondHit.getBarrelSlot().getLayer().getID() != 3) {

              float thetaDiff = fabs(firstHit.getBarrelSlot().getTheta()
                                     - secondHit.getBarrelSlot().getTheta());
              float timeDiff = fabs(firstHit.getTime() - secondHit.getTime());
              float distance = sqrt(pow(firstHit.getPosX() - secondHit.getPosX(), 2)
                                    + pow(firstHit.getPosY() - secondHit.getPosY(), 2)
                                    + pow(firstHit.getPosZ() - secondHit.getPosZ(), 2));
            if (false) {
              //if (fSaveControlHistos) {
                getStatistics().getHisto1D("two_hit_event_theta_diff")
                ->Fill(thetaDiff);
                getStatistics().getHisto1D("hits_x_pos")
                ->Fill(firstHit.getPosX());
                getStatistics().getHisto1D("hits_y_pos")
                ->Fill(firstHit.getPosY());
                getStatistics().getHisto1D("hits_z_pos")
                ->Fill(firstHit.getPosZ());
                getStatistics().getHisto1D("hits_x_pos")
                ->Fill(secondHit.getPosX());
                getStatistics().getHisto1D("hits_y_pos")
                ->Fill(secondHit.getPosY());
                getStatistics().getHisto1D("hits_z_pos")
                ->Fill(secondHit.getPosZ());
                getStatistics().getHisto2D("hit_distanece_vs_time_diff")
                ->Fill(distance, timeDiff);
                getStatistics().getHisto2D("hit_distanece_vs_theta_diff")
                ->Fill(distance, thetaDiff);
                if (thetaDiff >= 180.0 && thetaDiff < 181.0) {
                  getStatistics().getHisto1D("two_hit_event_theta_diff_cut")
                  ->Fill(thetaDiff);
                  getStatistics().getHisto1D("hits_x_pos_cut")
                  ->Fill(firstHit.getPosX());
                  getStatistics().getHisto1D("hits_y_pos_cut")
                  ->Fill(firstHit.getPosY());
                  getStatistics().getHisto1D("hits_z_pos_cut")
                  ->Fill(firstHit.getPosZ());
                  getStatistics().getHisto1D("hits_x_pos_cut")
                  ->Fill(secondHit.getPosX());
                  getStatistics().getHisto1D("hits_y_pos_cut")
                  ->Fill(secondHit.getPosY());
                  getStatistics().getHisto1D("hits_z_pos_cut")
                  ->Fill(secondHit.getPosZ());
                  getStatistics().getHisto2D("hit_distanece_vs_time_diff_cut")
                  ->Fill(distance, timeDiff);
                  getStatistics().getHisto2D("hit_distanece_vs_theta_diff_cut")
                  ->Fill(distance, thetaDiff);
                }
              }
            }
          }
        }
      }

      if (event.getHits().size() == 3) {
        JPetHit firstHit = event.getHits().at(0);
        JPetHit secondHit = event.getHits().at(1);
        JPetHit thirdHit = event.getHits().at(2);

        float theta_1_2 = fabs(firstHit.getBarrelSlot().getTheta()
                               - secondHit.getBarrelSlot().getTheta());
        float theta_2_3 = fabs(secondHit.getBarrelSlot().getTheta()
                               - thirdHit.getBarrelSlot().getTheta());

        getStatistics().getHisto2D("3_hit_angles")
        ->Fill(theta_1_2, theta_2_3);
      }
    }
  } else {
    return false;
  }
  return true;
}


bool EventCategorizer::makeTotCut(JPetHit& firstHit, JPetHit& secondHit)
{
	double TOT1 = calculateSumOfTOTs(firstHit);
        double TOT2 = calculateSumOfTOTs(secondHit);
	return (fTotCuts[firstHit.getScintillator().getID()] < TOT1 && (38.0/20)*fTotCuts[firstHit.getScintillator().getID()] > TOT1 && fTotCuts[secondHit.getScintillator().getID()] < TOT2  && (38.0/20)* fTotCuts[secondHit.getScintillator().getID()] > TOT2 );
	//return (fTotCuts[firstHit.getScintillator().getID()] < TOT1 && (38.0/20)*fTotCuts[firstHit.getScintillator().getID()] > TOT1 && fTotCuts[secondHit.getScintillator().getID()] < TOT2  && (38.0/20)* fTotCuts[secondHit.getScintillator().getID()] > TOT2 );

}

bool EventCategorizer::terminate()
{
	//outputFile.close();
	double mean = -999.0;

	//outputFile.open("MeanTOT_collimator_0.txt");
	
	for(int i=1;i<=192;i++)
		{
			//TCanvas *c1 = new TCanvas("c1");
	        mean = getStatistics().getHisto1D(Form("TOT_%d",i))->GetMean();
	        getStatistics().getHisto1D(Form("TOT_%d",i))->Draw();
	        TLine *line = new TLine(mean, 0, mean, getStatistics().getHisto1D(Form("TOT_%d",i))->GetMaximum());
	        line->SetLineColor(kRed);
	        line->Draw();
	        line = new TLine((38.0/20)*mean, 0, (38.0/20)*mean, getStatistics().getHisto1D(Form("TOT_%d",i))->GetMaximum());
	        line->SetLineColor(kRed);
	        line->Draw();
	        getStatistics().getHisto2D("MeanTOTvsID")->Fill(mean,i);
	       // c1->SaveAs(Form("TOT_%d.png",i));
	     //   savingTOT(mean,i);
		}
		TCanvas *c2 = new TCanvas("c1");
		mean = getStatistics().getHisto1D("Sum_TOTs")->GetMean();
		getStatistics().getHisto1D("Sum_TOTs")->Draw();
		//TLine *line = new TLine(mean, 0, mean, getStatistics().getHisto1D("Sum_TOTs")->GetMaximum());
		TLine *line = new TLine(15, 0, 15, getStatistics().getHisto1D("Sum_TOTs")->GetMaximum());
	    line->SetLineColor(kRed);
		line->Draw();
	  //  line = new TLine((38.0/20)*mean, 0, (38.0/20)*mean, getStatistics().getHisto1D("Sum_TOTs")->GetMaximum());
		line = new TLine(25, 0, 25, getStatistics().getHisto1D("Sum_TOTs")->GetMaximum());
	    line->SetLineColor(kRed);
	    line->Draw();
		c2->SaveAs("Sum_TOTs.png");
		//c2->SaveAs("Sum_TOTs.root");
	
	
	INFO("More than one hit Events done. Writing conrtrol histograms.");
	outputFile.close();
	
	//~ TH2F* histo = &(getStatistics().getHisto2D("Annihilation_pointXY"));
	//~ 
	//~ int maxBin = histo->GetMaximumBin();
	//~ int nx = 101;
	//~ int ny = nx;
	//~ int binX = maxBin%(nx+2);
	//~ int binY = ((maxBin-binX)/(nx+2))%(ny+2);
	//~ std::cout << maxBin << std::endl;
	//~ std::cout<< "Max bin on X: " << histo->GetXaxis()->GetBinCenter(binX) << std::endl;
	//~ std::cout<< "Max bin on Y: " << histo->GetYaxis()->GetBinCenter(binY) << std::endl;
	//~ 
	//~ TH1D* projectionX = histo->ProjectionY("Y_projection", binX,binX);
	//~ TCanvas* c = new TCanvas();
	//~ projectionX->Draw();
	//~ projectionX->Fit("gaus");
	//~ gStyle->SetOptFit(1);
//	c->SaveAs((foutname+"_x.png").c_str());
	//c->SaveAs((foutname+"_x.root").c_str());
	
	//~ TH1D* projectionY = histo->ProjectionX("X_projection", binY,binY);
	//~ projectionY->Draw();
	//~ projectionY->Fit("gaus");
	//c->SaveAs((foutname+"_y.png").c_str());
	//c->SaveAs((foutname+"_y.root").c_str());
	
	//~ TH2F* histo1 = &(getStatistics().getHisto2D("Annihilation_pointYZ"));
	//~ maxBin = histo1->GetMaximumBin();
	//~ nx = 101;
	//~ binX = maxBin%(nx+2);
	//~ TH1D* projectionZ = histo1->ProjectionY("Z_projection", binX,binX);
	//~ projectionZ->Draw();
	//~ projectionZ->Fit("gaus");
	//c->SaveAs((foutname+"_z.png").c_str());
	//c->SaveAs((foutname+"_z.root").c_str());
	
	
				//~ getStatistics().getHisto2D("Annihilation_pointYZ").Fill(fPoint.y, fPoint.z);

	for (int thres=1;thres<=4;thres++)
	{
		for(int id=1;id<=192;id++)
		{		int highestBin = getStatistics().getHisto1D(Form("TimeDiffAB_ID_%d_thr_%d", id, thres))->GetBinCenter( getStatistics().getHisto1D(Form("TimeDiffAB_ID_%d_thr_%d", id, thres))->GetMaximumBin() );
				getStatistics().getHisto1D(Form("TimeDiffAB_ID_%d_thr_%d", id, thres))->Fit("gaus","","", highestBin-1.5, highestBin+1.5);
				gStyle->SetOptFit(1);			
		}
	}
		
  return true;
}

double EventCategorizer::calculateXYDistance(const JPetHit& firstHit, const JPetHit& secondHit)
{
	return sqrt(pow(firstHit.getPosX()-secondHit.getPosX(),2) + pow(firstHit.getPosY()-secondHit.getPosY(),2) );
}

bool EventCategorizer::cutOnLorDistanceFromCenter(const JPetHit& firstHit, const JPetHit& secondHit)
{

double x_a = firstHit.getPosX();
double x_b = secondHit.getPosX();

double y_a = firstHit.getPosY();
double y_b = secondHit.getPosY();

double a = (y_a - y_b)/(x_a - x_b);
double b = 1.0;
double c = y_a - ((y_a - y_b)/(x_a - x_b))*x_a;

double distance = fabs(c)/sqrt(a*a + b*b);
return distance < 25;
}

double EventCategorizer::LORDistance(const JPetHit& firstHit, const JPetHit& secondHit)
{
double x_a = firstHit.getPosX();
double x_b = secondHit.getPosX();

double y_a = firstHit.getPosY();
double y_b = secondHit.getPosY();

double a = (y_a - y_b)/(x_a - x_b);
double b = 1.0;
double c = y_a - ((y_a - y_b)/(x_a - x_b))*x_a;

double distance = fabs(c)/sqrt(a*a + b*b);
return distance;
}

void EventCategorizer::SSJExtraction(const JPetEvent* event)
{
	if(3 != event->getHits().size() )
		return;
	std::vector<JPetHit> hits = event->getHits();
	
	//Find backToBackhits and set the remaining one as scattered
	std::pair<int,int> backToBackHits = std::make_pair<int,int> (0, 0);
	int scatteredHit = 0;
	if( isBackToBack( hits[0], hits[1] ) )
	{
		backToBackHits = std::make_pair<int,int> (0, 1);
		scatteredHit = 2;
	}
	else if ( isBackToBack( hits[0], hits[2] ) )
	{
		backToBackHits = std::make_pair<int,int> (0, 2);
		scatteredHit = 1;
	}
	else if ( isBackToBack( hits[1], hits[2] ) )
	{
		backToBackHits = std::make_pair<int,int> (1, 2);
		scatteredHit = 0;
	}
		
	//If not set backToBackHits then event is unknown
	if( 0 == backToBackHits.first && 0 == backToBackHits.second )
		return;
	
	//Connect initial hit with hit after scattering
	std::pair<int,int> initialAndScatteredHit = std::make_pair<int,int> (0,0);
	if( calculateXYDistance( hits[ backToBackHits.first ], hits[ scatteredHit ] ) >   calculateXYDistance(hits[ backToBackHits.second ], hits[ scatteredHit ] ) )
		initialAndScatteredHit = std::make_pair<int&,int&> ( backToBackHits.second, scatteredHit );
	else 
		initialAndScatteredHit = std::make_pair<int&,int&>( backToBackHits.first, scatteredHit );
	
}

bool EventCategorizer::isBackToBack(const JPetHit& firstHit, const JPetHit& secondHit)
{
	int deltaID = fabs(firstHit.getScintillator().getID() - secondHit.getScintillator().getID());
			    bool backToBack = false;
			    if( firstHit.getBarrelSlot().getLayer().getID() == 1 && secondHit.getBarrelSlot().getLayer().getID() == 1 )
					backToBack = (deltaID == 24);
				else if( firstHit.getBarrelSlot().getLayer().getID() == 2 && secondHit.getBarrelSlot().getLayer().getID() == 2 )
					backToBack = (deltaID == 24);
				else if( firstHit.getBarrelSlot().getLayer().getID() == 3 && secondHit.getBarrelSlot().getLayer().getID() == 3 )
					backToBack = (deltaID == 48);
	return backToBack;
}

int EventCategorizer::DeltaID(const JPetHit& firstHit, const JPetHit& secondHit)
{
	int deltaID1=-999, deltaID2=-999, deltaID = -999;
	
	if(firstHit.getBarrelSlot().getLayer().getID()==1 || firstHit.getBarrelSlot().getLayer().getID()==2)
	{
	deltaID1 = fabs( firstHit.getScintillator().getID() - secondHit.getScintillator().getID() );
	deltaID2 = 48 -  fabs( firstHit.getScintillator().getID() - secondHit.getScintillator().getID() );
		if(deltaID1 < deltaID2)
		  deltaID = deltaID1;
		else 
		  deltaID = deltaID2;
	}
	else
	{
	deltaID1 = fabs( firstHit.getScintillator().getID() - secondHit.getScintillator().getID() );
	deltaID2 = 96 -  fabs( firstHit.getScintillator().getID() - secondHit.getScintillator().getID() );
		if(deltaID1 < deltaID2)
		  deltaID = deltaID1;
		else 
		  deltaID = deltaID2;
	}
	//~ cout << firstHit.getBarrelSlot().getLayer().getID() << "  " <<  deltaID << endl;		   
	return deltaID;
}

int EventCategorizer::DeltaAngle(const JPetHit& firstHit, const JPetHit& secondHit)
{
	int deltaAngle1=-999, deltaAngle2=-999, deltaAngle = -999;
		
	deltaAngle1 =  fabs(firstHit.getBarrelSlot().getTheta() - secondHit.getBarrelSlot().getTheta());
	deltaAngle2 = 360 - deltaAngle1;
	
		if(deltaAngle1 < deltaAngle2)
		  deltaAngle = deltaAngle1;
		else 
		  deltaAngle = deltaAngle2;
	
	//~ cout << firstHit.getBarrelSlot().getLayer().getID() << "  " <<  deltaID << endl;		   
	return deltaAngle;
}

void EventCategorizer::saveEvents(const vector<JPetEvent>& events)
{
  for (const auto& event : events) {
    fOutputEvents->add<JPetEvent>(event);
  }
}
