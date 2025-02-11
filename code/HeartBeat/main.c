
#include <msp430.h>

//void Software_Trim();                       // Software Trim to get the best DCOFTRIM value
//#define MCLK_FREQ_MHZ 8                     // MCLK = 8MHz

unsigned int ADCCOMPLETE_FLAG = 0;
int Adata;
int main(void)
{

    WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT

        // Configure one FRAM waitstate as required by the device datasheet for MCLK
        // operation beyond 8MHz _before_ configuring the clock system.
        FRCTL0 = FRCTLPW | NWAITS_1;





        __bis_SR_register(SCG0);                     // disable FLL
        CSCTL3 |= SELREF__XT1CLK;                    // Set XT1 as FLL reference source
        CSCTL0 = 0;                                  // clear DCO and MOD registers
        CSCTL1 &= ~(DCORSEL_7);                      // Clear DCO frequency select bits first
        CSCTL1 |= DCORSEL_5;                         // Set DCO = 16MHz
        CSCTL2 = FLLD_0 + 487;                       // DCOCLKDIV = 16MHz
        __delay_cycles(3);
        __bic_SR_register(SCG0);                     // enable FLL
        while(CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1));   // FLL locked




  /*
        __bis_SR_register(SCG0);                // disable FLL
       CSCTL3 |= SELREF__REFOCLK;              // Set REFO as FLL reference source
       CSCTL1 = DCOFTRIMEN_1 | DCOFTRIM0 | DCOFTRIM1 | DCORSEL_3;// DCOFTRIM=3, DCO Range = 8MHz
       CSCTL2 = FLLD_0 + 243;                  // DCODIV = 8MHz
       __delay_cycles(3);
       __bic_SR_register(SCG0);                // enable FLL
       Software_Trim();                        // Software Trim to get the best DCOFTRIM value


       CSCTL4 = SELMS__DCOCLKDIV | SELA__REFOCLK; // set default REFO(~32768Hz) as ACLK source, ACLK = 32768Hz
                                               // default DCODIV as MCLK and SMCLK source
*/



    // GPIO config

       P1DIR |= BIT1;
       P2DIR |= BIT0 + BIT1;                    // P2.0 and P2.1 output
       P2SEL0 |= BIT0 + BIT1;                   // P2.0 and P2.1 options select
       P1SEL0 |= BIT5;                          // Configure ADC A1 pin
       P1SEL1 |= BIT5;

       // timer config
      const unsigned int TimerPeriod = 26666;
      const unsigned int AGC_duty = 14500;
      const unsigned int AmpRef_duty = 6060;


          // Disable the GPIO power-on default high-impedance mode to activate
          // previously configured port settings
          PM5CTL0 &= ~LOCKLPM5;
          SYSCFG3 |= TBRMP;                         //0b = P1.x is selected P1.6 and P1.7  1b = P2.0 and P2.1
          TB0CCR0 = TimerPeriod-1;                  // PWM Period
          TB0CCTL1 = OUTMOD_7;                      // CCR1 reset/set
          TB0CCR1 = AmpRef_duty;                       // CCR1 PWM duty cycle
          TB0CCTL2 = OUTMOD_7;                      // CCR2 reset/set
          TB0CCR2 = AGC_duty;                       // CCR2 PWM duty cycle
          TB0CTL = TBSSEL__SMCLK | MC__UP | TBCLR;  // SMCLK, up mode, clear TBR
          TB0CCTL0 |= CCIE;




          // Disable the GPIO power-on default high-impedance mode to activate
             // previously configured port settings
             PM5CTL0 &= ~LOCKLPM5;

             // Configure ADC10
             ADCCTL0 |= ADCSHT_2 | ADCON;                              // ADCON, S&H=16 ADC clks
             ADCCTL1 |= ADCSHP;                                        // ADCCLK = MODOSC; sampling timer
             ADCCTL2 |= ADCRES;                                        // 10-bit conversion results
             ADCIE |= ADCIE0;                                          // Enable ADC conv complete interrupt
             ADCMCTL0 |= ADCINCH_5 | ADCSREF_1;                        // A1 ADC input select; Vref=1.5V

             // Configure reference
             PMMCTL0_H = PMMPW_H;                                      // Unlock the PMM registers
             PMMCTL2 |= INTREFEN;                                      // Enable internal reference
             __delay_cycles(400);                                      // Delay for reference settling



        unsigned int k=0;                           //declare unsigned integers
        unsigned int LEDTIMER=0;
        //int BassBoundryNow = 0;
        long long int BassBoundry = 0;
        int filtered = 0;
        int CenterTap=0;
        long unsigned int filteredScaled;
        int LEDon;


       // int Peak_search_Arr[25]; // Declare Arrays


        //PID Variables

         int INT_LIMIT = 5;
         int setpoint = 700;                        //PID controller input
         int peakHold_Now;
         int peakHold_Last;
         float error;
         float errSum;
         float dErr;
         float PID_out;
         float kp = 0.02;
         float ki = 0.0001;
         float kd = 0.01;
         int peakHold_decay = 1;
         P1OUT |= BIT1;








                  int RegX1[4],RegX2[4],RegY1[4],RegY2[4];

                  const float a1[]={-1.169925255,-1.402167657,-1.120723799,-1.649948614};
                  const float a2[]={0.6360640657,0.6962444294,0.8186383078,0.8846651141}; //Set the Digital filter coefficients
                  const float b0[]={-0.173675154,-0.1901072,-0.1910775392,-0.2064887892};
                  const float b2[]={0.173675154,0.1901072,0.1910775392,0.2064887892};


                  for (k=0; k<3; ++k)

                      {
                      RegX1[k]=0;
                      RegX2[k]=0;             //Clear the Digital filter multiply\accumulate registers
                      RegY1[k]=0;
                      RegY2[k]=0;
                      }

                          //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------




              while(1) // endless loop

              {
                  __bis_SR_register(LPM0_bits | GIE);



                  if (ADCCOMPLETE_FLAG == 1) // check to see if P2OUT is equal to 00001000 and excute code if true.
                                             // Wait for timer to finish, then begin processing next data point.
                                             // A Timer is Set at 1000hz intervals and is used to set the sample rate of the ADC. P2OUT bit 4 is set by the timer.

                  {

                     // P1OUT ^=BIT1;
                      ADCCOMPLETE_FLAG = 0;
                      ADCCTL0 |= ADCENC | ADCSC;             // Sampling and conversion start
                      __bis_SR_register(LPM0_bits | GIE);

                     if  (Adata > peakHold_Now) {                  // get and hold the peak to be used for PID controller

                      peakHold_Now += Adata/10;

                     }

                      peakHold_Now -=peakHold_decay;                         //slowly discharge the peak


                      error =  setpoint - peakHold_Now;
                      errSum += error;

                      if (errSum > 100)
                      {
                          errSum = INT_LIMIT;
                      }

                      if (errSum < -INT_LIMIT){

                          errSum = -INT_LIMIT;
                      }

                      dErr = peakHold_Now - peakHold_Last;

                      PID_out = (kp * error) + (ki *errSum) + (kd* dErr);

                      peakHold_Last = peakHold_Now;


                      TB0CCR2 += PID_out;

                      if (TB0CCR2 > 14000){

                          TB0CCR2 = 14000;
                      }

                      if (TB0CCR2 < 10500){

                          TB0CCR2 = 10500;
                      }

                      if ( LEDon == 1 )

                          {
                                  if (LEDTIMER < 200)
                                  {
                                  LEDTIMER++;
                                  }
                                  else
                                  {
                                  LEDTIMER = 0;
                                  P1OUT &=~BIT1;
                                  LEDon = 0;
                                  }

                          }






                             for(k=0; k<3; k++) /* 3 + 0 is the number of sections - stop when 4 is reached*/

                                   {

                                   CenterTap = ((Adata * b0[k]) + (b2[k] * RegX2[k]));
                                   filtered = (CenterTap - (a1[k] * RegY1[k]) - (a2[k] * RegY2[k]));

                                         RegX2[k] = RegX1[k];
                                         RegX1[k] = Adata;
                                         RegY2[k] = RegY1[k];
                                         RegY1[k] = filtered;
                                         Adata = filtered;
                                   }



                             if (filtered < 0)
                             {
                                 filtered *=-1;
                             }



                             filteredScaled = filtered;
                             filteredScaled *= 100;



                             if (filteredScaled > BassBoundry)
                                   {

                                       P1OUT |= BIT1;
                                       LEDon = 1;

                                   }

                             if (LEDon == 0)

                              {

                              BassBoundry-= 5;

                              }



                                 if (BassBoundry <3000)

                                   {
                                       BassBoundry = 3000;
                                   }




                                if (filteredScaled > BassBoundry)

                                {
                                    BassBoundry = filtered;
                                    BassBoundry *= 90;

                                }



                  }
              }
          }

// ADC interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC_VECTOR))) ADC_ISR (void)
#else
#error Compiler not supported!
#endif

{
    switch(__even_in_range(ADCIV,ADCIV_ADCIFG))
    {

        case ADCIV_ADCIFG:
            Adata = ADCMEM0;
            __bic_SR_register_on_exit(LPM0_bits);            // Clear CPUOFF bit from LPM0
            break;
        default:
            break;
    }
}

              // Timer B0 interrupt service routine
              #if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
              #pragma vector = TIMER0_B0_VECTOR
              __interrupt void Timer_B (void)
              #elif defined(__GNUC__)
              void __attribute__ ((interrupt(TIMER0_B0_VECTOR))) Timer_B (void)
              #else
              #error Compiler not supported!
              #endif
{
         ADCCOMPLETE_FLAG = 1;
         __bic_SR_register_on_exit(LPM0_bits);              // Clear CPUOFF bit from LPM0
}

