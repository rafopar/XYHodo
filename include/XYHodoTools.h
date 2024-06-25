/* 
 * File:   XYHodoTools.h
 * Author: rafopar
 *
 * Created on June 11, 2024, 11:19 AM
 */

#ifndef XYHODOTOOLS_H
#define XYHODOTOOLS_H


// ===== Hipo headers =====
#include <reader.h>
namespace XYHodoTools {

    struct DetElement {
        short det; // The detector ID, 0, 1, etc
        short layer; // The layer of the detector
        short barID; // ID of the scintillator bar
        short pmt; // PMT, 0 or 1

        bool operator<(const DetElement& t) const {

            return ( this->det < t.det) || (!(t.det < this->det) && (this->layer < t.layer)) || (!(t.det < this->det) && !(t.layer < this->layer)
                    && (this->barID < t.barID)) || (!(t.det < this->det) && !(t.layer < this->layer) && !(t.barID < this->barID) && (this->pmt < t.pmt));
        }

    };

    class XYHodoHit {
    public:
        XYHodoHit();
        XYHodoHit(DetElement det_el);
        XYHodoHit(DetElement det_el, int tdc_rise, int tdc_fall);

        void SetTDC(int edge, int tdc);
        void SetRiseTDC(int tdc);
        void SetFallTDC(int tdc);

        DetElement DetectorElement() const {
            return fDetElement;
        }

        int RiseTime() const {
            return fTDC_Rise;
        }

        int FallTime() const {
            return fTDC_Fall;
        }

        bool IsInitialized() const {
            return fInitialized;
        }


    private:
        DetElement fDetElement;
        int fTDC_Rise;
        int fTDC_Fall;
        bool fInitialized;

    };

    class XYHodoCross {
    public:
        XYHodoCross(std::shared_ptr<XYHodoHit>, std::shared_ptr<XYHodoHit>);

        std::shared_ptr<XYHodoHit> ShortBarHit()const {
            return fShortBarHit;
        }
        std::shared_ptr<XYHodoHit> LongBarHit()const {
            return fShortBarHit;
        }
        
        short ShortBarId()const;             // points to the id of the short bar
        short LongBarId()const;              // points to the id of the long bar
        int ShortBarRiseTime()const;         // Rise Time TDC of the short Bar
        int LongBarRiseTime()const;          // Rise Time TDC of the long Bar
        int ShortBarFallTime()const;         // Fall Time TDC of the short Bar
        int LongBarFallTime()const;          // Fall Time TDC of the long Bar
        
        /* 
         * The plan is to form crosses from hits that belong to the same PMT, 
         * however in the future this might be reconsidered and we make crosses
         * from hits of two separate PMTs, that is why more general methods
         * are created below to access the PMT.
         */
        short PMTShort()const{
            return fShortBarHit->DetectorElement().pmt;    
        }
        short PMTLong()const{
            return fLongBarHit->DetectorElement().pmt;    
        }
        

    private:
        std::shared_ptr<XYHodoHit> fShortBarHit; // Points to the short Bar Hit
        std::shared_ptr<XYHodoHit> fLongBarHit; // Points to the long Bar Hit
    };

    class XYHodoAnalyzer {
    public:
        XYHodoAnalyzer();
        XYHodoAnalyzer(hipo::bank &HodoBank);
        void PrintEvent() const;
        
    /* 
     * Return methods for PMT1 and PMT2 crosses
     */
    const std::vector< XYHodoCross > *PMT1_Crosses(){
        return &v_PMT1_Crosses;
    }
    const std::vector< XYHodoCross > *PMT2_Crosses(){
        return &v_PMT2_Crosses;
    }

    /*
     *  Return methods from PMT1 and PMT2
     */
    
    const std::vector<std::shared_ptr< XYHodoHit> > *PMT1_Hits(){
        return &v_PMT1_Hits;
    }
    const std::vector<std::shared_ptr< XYHodoHit> > *PMT2_Hits(){
        return &v_PMT2_Hits;
    }
    
    private:
        std::vector< std::shared_ptr<XYHodoHit> > v_allHits;                    // Vector of all XYHodoHits
        std::vector< std::shared_ptr<XYHodoHit> > v_PMT1_Hits;                  // Vector of all XYHodoHits from PMT1 only
        std::vector< std::shared_ptr<XYHodoHit> > v_PMT2_Hits;                  // Vector of all XYHodoHits from PMT2 only
        
        std::vector< std::shared_ptr<XYHodoHit> > v_PMT1_ShortBar_Hits;         // Vector of short Bar XYHodoHits from PMT1 only
        std::vector< std::shared_ptr<XYHodoHit> > v_PMT1_LongBar_Hits;          // Vector of long  Bar XYHodoHits from PMT1 only
        std::vector< std::shared_ptr<XYHodoHit> > v_PMT2_ShortBar_Hits;         // Vector of short Bar XYHodoHits from PMT2 only
        std::vector< std::shared_ptr<XYHodoHit> > v_PMT2_LongBar_Hits;          // Vector of long  Bar XYHodoHits from PMT2 only
        
        std::vector< XYHodoCross > v_PMT1_Crosses;                              // Crosses formed out of PMT1 hits
        std::vector< XYHodoCross > v_PMT2_Crosses;                              // Crosses formed out of PMT2 hits
        
        std::vector< std::pair< std::shared_ptr<XYHodoHit>, std::shared_ptr<XYHodoHit> > > v_LR_Matches; // Vector of all matched XYHodoHits from both PMTs
        //std::map< DetElement, XYHodoHit* > m_allHits;
        std::map< DetElement, std::shared_ptr<XYHodoHit> > m_allHits;

        int fBankSize; // The number of hits in the bank;
        int fnAllHits; // The number of hits, i.e. the size of v_allHits, this number almost always is less than the BankSize.
        int fn_PMT1Hits; // The number of hits from PMT1
        int fn_PMT2Hits; // The number of hits from PMT2

        hipo::bank *fHodoBank;

        bool IsAnalyzed;
        bool IsBankSet;

        bool AnalyzeEvent();



    };


}

#endif /* XYHODOTOOLS_H */