#define ADC_MISO 7  // DRDY_DOUT 1
#define ADC_SCK  8  // CLCK 2
#define ADC_PDN  6  // PDWN
#define nADC_POWER 9 // low enables the 5V regulator for gauge and ADC analog part

#define ADC_MUX0 A2 // A0 on ADS1232 chip
#define ADC_MUX1 A3 // TEMP

#define ADC_GAIN0 11
#define ADC_GAIN1 10

#define PWR_BTN 5

// connection without tensiometric sensor
//
// X3-1 – wire – X3-3
// X3-2 – wire – X3-4
// X3-1 – odpor 2k2 – X3-6 – odpor 1R0 – X3-5 – odpor 2k2 – X3-2
// spotreba sice neodpovida, ale readout by nejaky mel byt. Pokud chcete cist readout opacneho znamenka:
// X3-1 – odpor 2k2 – X3-5 – odpor 1R0 – X3-6 – odpor 2k2 – X3-2
// nulovy readout:
// X3-1 – odpor 2k2 – X3-6 – zkrat  – X3-5 – odpor 2k2 – X3-2

void wait50ns()
// wait at least 50ns
{
  // probably no need to wait 20Mhz -> one instruction is 50ns (??)
  asm volatile ( 
    "nop"   "\n\t" 
  ::);
}

long readADC()
{
  // typical 100ms for slow (speed = 0) reading
  while( digitalRead(ADC_MISO) == HIGH )
    ;
  int i;
  long ret = 0;
  for( i = 0; i < 24 + 1; i++ ) // force extra high bit
  {
    ret <<= 1;
    // pulse min length (t3) min = 100ns
    digitalWrite( ADC_SCK, HIGH );
    // rise time (t4) max = 50ns
    wait50ns();
    if( digitalRead(ADC_MISO) == HIGH )
      ret += 1;    
    wait50ns();
    digitalWrite( ADC_SCK, LOW );    
    wait50ns();
    wait50ns();
  }
  // TODO signed value and remove extra 25th bit
  return ret;
}

void setup()
{
  // CPU takes-over the power controll
  pinMode( PWR_BTN, OUTPUT);
  digitalWrite( PWR_BTN, HIGH);
  //RTC seq:
  // nastavit datum, cas
  // nastavit kdy ma budit
  // nastavit mod RTC pinu#5
  // povolit RTC Interrupt Output

  // power up the ADC module
  pinMode( nADC_POWER, OUTPUT);
  digitalWrite( nADC_POWER, LOW);
  pinMode( ADC_PDN, OUTPUT );
  digitalWrite( ADC_PDN, LOW );	//LOW aby se resetoval, nechat po dobu pripravy pinu
  
  pinMode( ADC_MISO, INPUT );  
  pinMode( ADC_SCK, OUTPUT );
  digitalWrite( ADC_SCK, LOW ); //presunout pred digitalWrite( ADC_PDN, HIGH )

  pinMode( ADC_GAIN0, OUTPUT );
  pinMode( ADC_GAIN1, OUTPUT );
  digitalWrite( ADC_GAIN0, LOW ); // set Gain=1 (for temperature test)
  digitalWrite( ADC_GAIN1, LOW );
  
  pinMode( ADC_MUX0, OUTPUT );
  pinMode( ADC_MUX1, OUTPUT );
  digitalWrite( ADC_MUX0, LOW ); // only AINP1/AINN1 are connected
  digitalWrite( ADC_MUX1, HIGH ); // test - read temperature
    
  wait50ns();
  digitalWrite( ADC_PDN, HIGH ); //povolit ADC
    
  Serial.begin( 9600 );
  Serial.println( "ADC convertor output ... (temperature test)" ); 
}

void loop()
{
  Serial.println( readADC() );
}
