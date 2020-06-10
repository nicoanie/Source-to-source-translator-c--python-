#Source generato dal frontend C->Python

def funzione( x,y,z):
	 a = 4
	 b = 6.35
	 b = (a + (b * 4))
	 print ( x )
	 print ( y )
	 print ( chr( z ) )
	 return b
def main( ):
	 x = 5
	 y = 3.65
	 z = ord('g')
	 x = int( funzione( int(y), z, x ) )
	 print ( x )
	 return 0

main()