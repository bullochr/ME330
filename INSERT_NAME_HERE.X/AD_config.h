
#ifndef AD_config
#define	AD_config

#include <xc.h> // include processor files - each processor file is guarded.  

// This function configures the A/D to read from
// multiple channels in scan mode.
void configureAD(void){
    //clear AD registers
    AD1CON1 = 0;
    AD1CON2 = 0;
    AD1CON3 = 0;
    AD1CON5 = 0;
    AD1CSSL = 0; //clear channels to scan
    AD1CSSH = 0;
    
    _ADON = 0;    // AD1CON1<15> -- Turn off A/D during config
    
    // AD1CON1 register
    _ADSIDL = 0;  // AD1CON1<13> -- A/D continues in idle mode
    _MODE12 = 1;  // AD1CON1<10> -- 12-bit A/D operation
    _FORM = 0;    // AD1CON1<9:8> -- Unsigned integer output
    _SSRC = 7;    // AD1CON1<7:4> -- Auto conversion (internal counter)
    _ASAM = 1;    // AD1CON1<2> -- Auto sampling

    // AD1CON2 register
    _PVCFG = 0;   // AD1CON2<15:14> -- Use VDD as positive
                  // ref voltage
    _NVCFG = 0;   // AD1CON2<13> -- Use VSS as negative
                  // ref voltage
    _BUFREGEN = 1;// AD1CON2<11> -- Result appears in buffer
                  // location corresponding to channel
    _CSCNA = 1;   // AD1CON2<10> -- Scans inputs specified
                  // in AD1CSSx registers
    _SMPI = 8;	  // AD1CON2<6:2> -- Every (n+1)th conversion sent (n = number of channels sampled -1)
                  // to buffer, set interrupt flag after n+1 conversions
    _ALTS = 0;    // AD1CON2<0> -- Sample MUXA only
    
    // AD1CON3 register
    _ADRC = 0;    // AD1CON3<15> -- Use system clock
    _SAMC = 0;    // AD1CON3<12:8> -- Auto sample every A/D
                  // period TAD
    _ADCS = 0b00000011;    // AD1CON3<7:0> -- TAD needs to be at least 600 ns. Thus, _ADCS = 0b00000010 will give us the fastest AD clock given a 4 MHz system clock. 0b00000011 is slower than we could go, but still fast enough for our needs.

    // choose A2D channels you'd like to scan here.
    _CSS4 = 1; //AN4 - pin 6 - Left line QRD
    _CSS13 = 1; //AN13 - pin 7 - center line QRD
    _CSS14 = 1; //AN14 - pin 8 - right line QRD
    _CSS0 = 1; //pin 2 - AN0 - satellite sensor
    _CSS1 = 1; //pin 3 - AN1 - ball QRD
    _CSS12 = 1; //pin 15 - AN12 - Sample Collection IR
    _CSS11 = 1; //pin 16 - AN11 - Front Sharp
    _CSS10 = 1; //pin 17 - AN10 - Right Sharp
    _CSS9 = 1; // pin 18 - AN9 - Left Sharp
    
    //AD1 interrupt
    _AD1IP = 4; //priority 
    _AD1IF = 0; //clear interrupt flag
    _AD1IE = 1; //enable interrupt
    
    _ADON = 1;    // AD1CON1<15> -- Turn on A/D
}

#endif	/* XC_HEADER_TEMPLATE_H */

