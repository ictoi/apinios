#define ADC_MISO 7  // DRDY_DOUT 1
#define ADC_SCK  8  // CLCK 2
#define ADC_PDN  6  // PDWN

void wait50ns()
// wait at least 50ns
{
  // probably no need to wait 20Mhz -> one instruction is 50ns (??)
  asm volatile ( 
    "nop"   "\n\t" 
  ::);
}

int readADC()
{
  // typical 100ms for slow (speed = 0) reading
  while( digitalRead(ADC_MISO) == HIGH )
    ;
  int i, ret = 0;
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
  // power up the ADC module
  pinMode( ADC_PDN, OUTPUT );
  digitalWrite( ADC_PDN, HIGH );
  
  pinMode( ADC_MISO, INPUT );  
  pinMode( ADC_SCK, OUTPUT );
  digitalWrite( ADC_SCK, LOW );
  
  Serial.begin( 9600 );
  Serial.println( "ADC convertor output ..." ); 
}

void loop()
{
  Serial.println( readADC() );
}

