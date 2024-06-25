#include <XYHodoTools.h>

#include <iostream>

namespace XYHodoTools {

    XYHodoHit::XYHodoHit() {

        fInitialized = false;

        fDetElement = DetElement{-1, -1, -1, -1};
        fTDC_Rise = -1;
        fTDC_Fall = -1;
    }

    XYHodoHit::XYHodoHit(DetElement det_el) : fDetElement{det_el}
    {
        fTDC_Rise = -1;
        fTDC_Fall = -1;
        fInitialized = false;
    }

    void XYHodoHit::SetTDC(int edge, int tdc) {
        if (edge < 0 || edge > 1 || tdc < 0) {
            std::cout << "Either edge or tdc values are not real." << std::endl;
            std::cout << "edge = " << edge << std::endl;
            std::cout << "tdc = " << edge << std::endl;
            std::cout << "Exiting." << std::endl;
            exit(1);
        }

        if (edge == 0) {
            fTDC_Rise = tdc;
        } else {
            fTDC_Fall = tdc;
        }
    }

    void XYHodoHit::SetRiseTDC(int tdc) {

        if (tdc < 0) {
            std::cout << "TDC can not be negative" << std::endl;
            std::cout << "TDC = " << tdc << std::endl;
            std::cout << "Exiting..." << std::endl;
            exit(1);
        }
        fTDC_Rise = tdc;
    }

    void XYHodoHit::SetFallTDC(int tdc) {

        if (tdc < 0) {
            std::cout << "TDC can not be negative" << std::endl;
            std::cout << "TDC = " << tdc << std::endl;
            std::cout << "Exiting..." << std::endl;
            exit(1);
        }
        fTDC_Fall = tdc;
    }



    XYHodoHit::XYHodoHit(DetElement det_el, int tdc_rise, int tdc_fall) : fDetElement{det_el}, fTDC_Rise{tdc_rise}, fTDC_Fall{tdc_fall}
    {
        fInitialized = true;
    }

    XYHodoAnalyzer::XYHodoAnalyzer() {

        IsBankSet = false;
        IsAnalyzed = false;
    }

    XYHodoAnalyzer::XYHodoAnalyzer(hipo::bank &HodoBank) {

        fHodoBank = &HodoBank;
        IsBankSet = true;

        IsAnalyzed = AnalyzeEvent();

    }

    bool XYHodoAnalyzer::AnalyzeEvent() {

        fBankSize = fHodoBank->getRows();

        //std::cout<<"The Bank Size is "<<BankSize<<std::endl;

        /* 
         * First we loop over all elements of the TDC bank, then create all
         * XYHodoHits
         */
        for (int i = 0; i < fBankSize; i++) {

            short det = fHodoBank->getShort("sector", i);
            short layer = fHodoBank->getShort("layer", i);
            short component = fHodoBank->getShort("component", i);
            short edge = fHodoBank->getShort("order", i);
            int tdc = fHodoBank->getInt("TDC", i);

            short pmt = component % 2;          // By definition pmt can not have values other than 0 or 1
            short barID = component / 2;

            DetElement detEl{det, layer, barID, pmt};

            /*
             *  Simple check, if the detector element is not in the map, then
             * create a new element in the map.
             */
            if (m_allHits.find(detEl) == m_allHits.end()) {
                m_allHits[detEl] = std::shared_ptr<XYHodoHit>(new XYHodoHit(detEl));
            }

            m_allHits.at(detEl)->SetTDC(edge, tdc);
        }

        /* Now let's loop over m_allHits elements and create different type of
         * containers that will stor pointers to XYHodoHits.
         * As an example PMT1 hits, PMT2 hits etc
         */

        for (auto it = m_allHits.begin(); it != m_allHits.end(); it++) {
            //std::cout<<it->first.layer<<std::endl;
            v_allHits.push_back(it->second);
            if (it->first.pmt == 0) {
                v_PMT1_Hits.push_back(it->second);
                
                if( it->first.layer == 0 ){
                    v_PMT1_ShortBar_Hits.push_back(it->second);
                }else if( it->first.layer == 1 ){
                    v_PMT1_LongBar_Hits.push_back(it->second);
                }
                
            }else{
                v_PMT2_Hits.push_back(it->second);

                if( it->first.layer == 0 ){
                    v_PMT2_ShortBar_Hits.push_back(it->second);
                }else if( it->first.layer == 1 ){
                    v_PMT2_LongBar_Hits.push_back(it->second);
                }
            }
        }
        
        
        /*
         *  Now let's make crosses out of those hits
         */
        
        // ====== PMT1 crosses ======
        for( auto shortBarHit: v_PMT1_ShortBar_Hits ){
            for (auto longBarHit : v_PMT1_LongBar_Hits) {
                v_PMT1_Crosses.push_back( XYHodoCross( shortBarHit, longBarHit ) );
            }
        }
        
        // ====== PMT2 crosses ======
        for( auto shortBarHit: v_PMT2_ShortBar_Hits ){
            for (auto longBarHit : v_PMT2_LongBar_Hits) {
                v_PMT2_Crosses.push_back( XYHodoCross( shortBarHit, longBarHit ) );
            }
        }

        return true;
    }
    
    
    XYHodoCross::XYHodoCross( std::shared_ptr<XYHodoHit> shortBarHit, std::shared_ptr<XYHodoHit> longBarHit) : fShortBarHit{shortBarHit}, fLongBarHit{longBarHit} {
        
    }
    
    short XYHodoCross::ShortBarId()const{
        return fShortBarHit->DetectorElement().barID;
    }

    short XYHodoCross::LongBarId()const{
        return fLongBarHit->DetectorElement().barID;
    }
    
    int XYHodoCross::ShortBarRiseTime()const{
        return fShortBarHit->RiseTime();
    }

    int XYHodoCross::ShortBarFallTime()const{
        return fShortBarHit->FallTime();
    }
    
    int XYHodoCross::LongBarRiseTime()const{
        return fLongBarHit->RiseTime();
    }

    int XYHodoCross::LongBarFallTime()const{
        return fLongBarHit->FallTime();
    }
    
}