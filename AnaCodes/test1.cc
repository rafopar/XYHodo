#include <iostream>
#include <TH2D.h>
#include <TH1D.h>
#include <TMath.h>
#include <TFile.h>

// ===== Hipo headers =====
#include <reader.h>
#include <writer.h>
#include <dictionary.h>

// ===== Hodo Tools ======

#include <XYHodoTools.h>

using namespace std;

int main() {

    cout << "Kuku" << endl;

    const int NL1bars = 33;
    const int NL2bars = 12;
    const int NL3 = 19;

    TFile *file_out = new TFile("Test1.root", "Recreate");
    TH1D h_NHits1("h_NHits1", "", 51, -0.5, 50.5);
    TH2D h_TDC_vsPMT1("h_TDC_vsPMT1", "", 3, -0.5, 2.5, 200, 0., 1050.);
    TH2D h_TDC_vsPMT2("h_TDC_vsPMT2", "", 3, -0.5, 2.5, 200, 0., 1050.);
    TH1D h_TDC_PMT1("h_TDC_PMT1", "", 200, 0., 1050.);
    TH1D h_TDC_PMT2("h_TDC_PMT2", "", 200, 0., 1050.);

    TH1D h_bar_layer1_PMT1("h_bar_layer1_PMT1", "", NL1bars + 1, -0.5, NL1bars + 0.5);
    TH1D h_bar_layer1_PMT2("h_bar_layer1_PMT2", "", NL1bars + 1, -0.5, NL1bars + 0.5);
    TH1D h_bar_layer2_PMT1("h_bar_layer2_PMT1", "", NL2bars + 1, -0.5, NL2bars + 0.5);
    TH1D h_bar_layer2_PMT2("h_bar_layer2_PMT2", "", NL2bars + 1, -0.5, NL2bars + 0.5);
    TH1D h_bar_layer3_PMT1("h_bar_layer3_PMT1", "", NL3 + 1, 99.5, 100 + NL3 + 0.5);
    TH1D h_bar_layer3_PMT2("h_bar_layer3_PMT2", "", NL3 + 1, 99.5, 100 + NL3 + 0.5);

    TH1D h_PMT1_N_PMT1_Crosses("h_PMT1_N_PMT1_Crosses", "", 19, -0.5, 18.5);
    TH1D h_PMT1_N_PMT2_Crosses("h_PMT1_N_PMT2_Crosses", "", 19, -0.5, 18.5);
    
    TH2D h_PMT1_Cross_coors1("h_PMT1_Cross_coors1", "", 34, -0.5, 33.5, 13, -0.5, 12.5);
    TH2D h_PMT2_Cross_coors1("h_PMT2_Cross_coors1", "", 34, -0.5, 33.5, 13, -0.5, 12.5);
    
    TH1D h_PMT1_Cross_DeltaT1("h_PMT1_Cross_DeltaT1", "", 200, -500., 500.);
    TH1D h_PMT2_Cross_DeltaT1("h_PMT2_Cross_DeltaT1", "", 200, -500., 500.);
    
    TH2D h_PMT1_TimtOverThr_uniqueChan1("h_PMT1_TimtOverThr_uniqueChan1", "", 121, -0.5, 120.5, 200, 0., 1000.);
    TH2D h_PMT2_TimtOverThr_uniqueChan1("h_PMT2_TimtOverThr_uniqueChan1", "", 121, -0.5, 120.5, 200, 0., 1000.);
    
    TH2D h_PMT1_TRise_UniqueChan1("h_PMT1_TRise_UniqueChan1", "", 121, -0.5, 120.5, 200, 0., 1050.);
    TH2D h_PMT2_TRise_UniqueChan1("h_PMT2_TRise_UniqueChan1", "", 121, -0.5, 120.5, 200, 0., 1050.);
    TH2D h_PMT1_TFall_UniqueChan1("h_PMT1_TFall_UniqueChan1", "", 121, -0.5, 120.5, 200, 0., 1050.);
    TH2D h_PMT2_TFall_UniqueChan1("h_PMT2_TFall_UniqueChan1", "", 121, -0.5, 120.5, 200, 0., 1050.);
    
    TH2D h_PMT1_TimeOverThr_TRise1("h_PMT1_TimeOverThr_TRise1", "", 400, 500., 1050., 200, 0., 200);
    TH2D h_PMT2_TimeOverThr_TRise1("h_PMT2_TimeOverThr_TRise1", "", 400, 500., 1050., 200, 0., 200);
    
    hipo::reader reader;
    //reader.open("Data/decoded_2067_16.hipo");
    reader.open("Data/decoded_2067_All.hipo");

    hipo::dictionary factory;

    reader.readDictionary(factory);

    factory.show();
    hipo::event event;
    int evCounter = 0;

    hipo::bank bXYHodoTDC(factory.getSchema("XYHODO::tdc"));

    try {

        while (reader.next() == true) {
            reader.read(event);

            evCounter = evCounter + 1;

            //if( evCounter > 2000 ){break;}
            if (evCounter % 1000 == 0) {
                cout.flush() << "Processed " << evCounter << " events \r";
            }

            event.getStructure(bXYHodoTDC);
            
            XYHodoTools::XYHodoAnalyzer analyzer(bXYHodoTDC);
            
            const std::vector<XYHodoTools::XYHodoCross> *v_PMT1Crosses = analyzer.PMT1_Crosses();
            const std::vector<XYHodoTools::XYHodoCross> *v_PMT2Crosses = analyzer.PMT2_Crosses();
            
            const std::vector< std::shared_ptr< XYHodoTools::XYHodoHit> > *v_PMT1_Hits = analyzer.PMT1_Hits();
            const std::vector< std::shared_ptr< XYHodoTools::XYHodoHit> > *v_PMT2_Hits = analyzer.PMT2_Hits();
            
            h_PMT1_N_PMT1_Crosses.Fill( v_PMT1Crosses->size() );
            h_PMT1_N_PMT2_Crosses.Fill( v_PMT2Crosses->size() );
                        
            for( auto cur_Cross : *v_PMT1Crosses ){
                h_PMT1_Cross_coors1.Fill( cur_Cross.ShortBarId(), cur_Cross.LongBarId() );
                h_PMT1_Cross_DeltaT1.Fill( cur_Cross.ShortBarRiseTime() - cur_Cross.LongBarRiseTime() );
            }

            for( auto cur_Cross : *v_PMT2Crosses ){
                h_PMT2_Cross_coors1.Fill( cur_Cross.ShortBarId(), cur_Cross.LongBarId() );
                h_PMT2_Cross_DeltaT1.Fill( cur_Cross.ShortBarRiseTime() - cur_Cross.LongBarRiseTime() );
            }
            
            
            for( auto curHit : *v_PMT1_Hits ){  
                int unique_chan = 50*(curHit->DetectorElement().layer%2) + curHit->DetectorElement().barID;
                h_PMT1_TimtOverThr_uniqueChan1.Fill(unique_chan, curHit->FallTime() - curHit->RiseTime() );
                h_PMT1_TRise_UniqueChan1.Fill( unique_chan, curHit->RiseTime() );
                h_PMT1_TFall_UniqueChan1.Fill( unique_chan, curHit->FallTime() );
                
                h_PMT1_TimeOverThr_TRise1.Fill( curHit->RiseTime(), curHit->FallTime() - curHit->RiseTime() );
            }

            for( auto curHit : *v_PMT2_Hits ){
                int unique_chan = 50*(curHit->DetectorElement().layer%2) + curHit->DetectorElement().barID;
                h_PMT2_TimtOverThr_uniqueChan1.Fill(unique_chan, curHit->FallTime() - curHit->RiseTime() );
                h_PMT2_TRise_UniqueChan1.Fill( unique_chan, curHit->RiseTime() );
                h_PMT2_TFall_UniqueChan1.Fill( unique_chan, curHit->FallTime() );
                
                h_PMT2_TimeOverThr_TRise1.Fill( curHit->RiseTime(), curHit->FallTime() - curHit->RiseTime() );
            }
            
            
//            int nHodoHits = bXYHodoTDC.getRows();
//            h_NHits1.Fill(nHodoHits);


            //cout<<"*****NHits = "<<nHodoHits<<endl;
//            for (int ihit = 0; ihit < nHodoHits; ihit++) {
//
//                int sector = bXYHodoTDC.getInt("sector", ihit);
//                int layer = bXYHodoTDC.getInt("layer", ihit);
//                int component = bXYHodoTDC.getInt("component", ihit);
//                int edge = bXYHodoTDC.getInt("order", ihit);
//                int tdc = bXYHodoTDC.getInt("TDC", ihit);
//
//
//                int pmt = component % 2;
//                int barID = component / 2;
//
//                //cout<<"PMT = "<<pmt<<"   sec = "<<sector<<"    layer = "<<layer<<"   component = "<<component<<"    edge = "<<edge<<"    tdc = "<<tdc<<endl;
//
//                h_TDC_vsPMT1.Fill(pmt, tdc);
//
//                if (edge == 0) {
//                    h_TDC_vsPMT2.Fill(pmt, tdc);
//
//                    if (pmt == 0) {
//                        h_TDC_PMT1.Fill(tdc);
//
//                        if (layer == 0) {
//                            h_bar_layer1_PMT1.Fill(barID);
//                        } else if (layer == 1) {
//                            h_bar_layer2_PMT1.Fill(barID);
//                        } else if (layer == 2) {
//                            h_bar_layer3_PMT1.Fill(barID);
//                        }
//
//                    } else {
//                        h_TDC_PMT2.Fill(tdc);
//
//                        if (layer == 0) {
//                            h_bar_layer1_PMT2.Fill(barID);
//                        } else if (layer == 1) {
//                            h_bar_layer2_PMT2.Fill(barID);
//                        } else if (layer == 2) {
//                            h_bar_layer3_PMT2.Fill(barID);
//                        }
//
//                    }
//                }
//            }

            //cin.ignore();


        }
    } catch (const char msg) {
        cerr << msg << endl;
    }


    gDirectory->Write();

    return 0;

}