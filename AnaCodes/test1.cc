#include <iostream>
#include <TH2D.h>
#include <TH1D.h>
#include <TMath.h>
#include <TFile.h>

// ===== Hipo headers =====
#include <reader.h>
#include <writer.h>
#include <dictionary.h>

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

    hipo::reader reader;
    reader.open("Data/decoded_2067_16.hipo");

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

            int nHodoHits = bXYHodoTDC.getRows();
            h_NHits1.Fill(nHodoHits);


            //cout<<"*****NHits = "<<nHodoHits<<endl;
            for (int ihit = 0; ihit < nHodoHits; ihit++) {

                int sector = bXYHodoTDC.getInt("sector", ihit);
                int layer = bXYHodoTDC.getInt("layer", ihit);
                int component = bXYHodoTDC.getInt("component", ihit);
                int edge = bXYHodoTDC.getInt("order", ihit);
                int tdc = bXYHodoTDC.getInt("TDC", ihit);


                int pmt = component % 2;
                int barID = component / 2;

                //cout<<"PMT = "<<pmt<<"   sec = "<<sector<<"    layer = "<<layer<<"   component = "<<component<<"    edge = "<<edge<<"    tdc = "<<tdc<<endl;

                h_TDC_vsPMT1.Fill(pmt, tdc);

                if (edge == 0) {
                    h_TDC_vsPMT2.Fill(pmt, tdc);

                    if (pmt == 0) {
                        h_TDC_PMT1.Fill(tdc);

                        if (layer == 0) {
                            h_bar_layer1_PMT1.Fill(barID);
                        } else if (layer == 1) {
                            h_bar_layer2_PMT1.Fill(barID);
                        } else if (layer == 2) {
                            h_bar_layer3_PMT1.Fill(barID);
                        }

                    } else {
                        h_TDC_PMT2.Fill(tdc);

                        if (layer == 0) {
                            h_bar_layer1_PMT2.Fill(barID);
                        } else if (layer == 1) {
                            h_bar_layer2_PMT2.Fill(barID);
                        } else if (layer == 2) {
                            h_bar_layer3_PMT2.Fill(barID);
                        }

                    }
                }
            }

            //cin.ignore();


        }
    } catch (const char msg) {
        cerr << msg << endl;
    }


    gDirectory->Write();

    return 0;

}