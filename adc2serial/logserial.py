"""
  Read data from serial line and store it in a file
  usage:
      ./logserial.py <COM> 
"""

import sys
import serial 
import datetime

def logSerial( com, filename ):
    f = open( filename, "wb" )
    while 1:
        ch = com.read(1)
        sys.stdout.write( ch )
        f.write( ch )
        f.flush()

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print __doc__
        sys.exit(0)
    com = serial.Serial( sys.argv[1], 9600 )
    dt = datetime.datetime.now()
    filename = dt.strftime("adc%y%m%d_%H%M%S.log") 
    logSerial( com, filename )

# vim: expandtab sw=4 ts=4

