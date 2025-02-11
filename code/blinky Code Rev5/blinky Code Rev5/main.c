/*

 ///clock congfig --(ADCtimer)timerA0 600hz (LEDTimer) TimerA1 300ms
 ///GPIO Config --- P1.0 -> output P2.4--->output ----Set the rest to default (input)
 //ADC setup--- clock -> ADC10OSC -->Vref 1.5V ADCInput--> p1.5 - channel 5  Sample and hold time--> 64x10 ADCCLKS
 * ======== Standard MSP430 includes ========
 */
#include <msp430.h>

/*
 * ======== Grace related includes ========
 */
#include <ti/mcu/msp430/Grace.h>




int findaverage(int outavg[150], unsigned int H);
/*
 *  ======== main ========
 */
int main(void)
{
    Grace_init();   // Activate Grace-generated configuration

    
    	unsigned int k;   //declare unsigned integers
    	unsigned int B =0;//

    	int filtered=0; // Declare integers
  		int peak10 = 0;
  		int Adata=0;
  		int CenterTap=0;

  		int outarr[150]; // Declare Arrays

  		int RegX1[4],RegX2[4],RegY1[4],RegY2[4];

  		const float a1[]={-1.169925255,-1.402167657,-1.120723799,-1.649948614};
  		const float a2[]={0.6360640657,0.6962444294,0.8186383078,0.8846651141}; //Set the Digital filter coefficients
  		const float b0[]={-0.173675154,-0.1901072,-0.1910775392,-0.2064887892};
  		const float b2[]={0.173675154,0.1901072,0.1910775392,0.2064887892};


  		for (k=0; k>3; ++k)

  			{
  			RegX1[k]=0;
  			RegX2[k]=0;				//Clear the Digital filter multiply\accumulate registers
  			RegY1[k]=0;
  			RegY2[k]=0;
  			}

  				//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


  	while(1) // endless loop

  	{


  		if (P2OUT && BIT4 == BIT4) // check to see if P2OUT is equal to 00001000 and excute code if true.
  								   // Wait for timer to finish, then begin processing next data point.
  								   // A Timer is Set at 1000hz intervals and is used to set the sample rate of the ADC. P2OUT bit 4 is set by the timer.


  		{

  			ADC10CTL0 |= ENC + ADC10SC; //Start ADC



  						 /* Loop until ADC10IFG is set indicating ADC conversion complete */
  						 while ((ADC10CTL0 & ADC10IFG) == 0);
  						 Adata = ADC10MEM;



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


  			outarr[B]=filtered; //get 150 samples of filter
  			++B; //Set next point in array
  			P2OUT &= ~BIT4; //turn off bit 4 - timer will reset to high state and then the next sample can be processed.

  		}


			if (B > 149) //Check to see if a 150 samples have been gathered


			{

				TA0CTL &= ~BIT4; //Stop sampling and processing audio data through digital filter.
			  	peak10 = findaverage(outarr,149)*2; //find the average of the peaks of the data and multiple by 2
				B=0; // Set B to zero to begin gathering a new set of data points

			}



			if (filtered > peak10) //Check to see if the if there is a filtered data point is higher then the average of the peaks of the last 150 samples of filtered data

				 {

 					P1OUT |= BIT0;  // Turn on LED
					TA1CTL |= BIT4; //Start Timer

				}

  	}


}

	int findaverage(int outavg[], unsigned int H)


		{


		unsigned int X=0; //declare unsigned integers
		unsigned int D = 0;
		unsigned int M = 0;

		int peak= 0;
		int peakavg = 0;//declare integers



			for (X= 0; X < H; ) //run loop for number of samples points-->149

		  	{

				peak = 0; //set peak to zero

//-------------------------------------------------------------------------------------------------------------------------------
	//Find highest point out of every 6 data points for the total number of samples and average them.

				for (M=0;M<6;++M)

		  	    	 {

						if (outavg[X] > 0)
						{
		  	    		  if (outavg[X]> peak)
		  	    		  {
		  	    		    peak = outavg[X];
		  	    		  }
		  	    		}

						++X;
		  	    	 }

			 	 	 peakavg += peak;

			 	 	 ++D;

		  	 }

	peakavg /= D;

//-----------------------------------------------------------------------------------------------------------------------

		  	  if (peakavg < 15) //Min Peak average is 15

		  	  {
		  	  	peakavg = 15;
		  	  }

		  	TA0CTL |= BIT4; // Turn timer A0 back on and begin ADC and digial filter processing.
		  	return (peakavg);
}
