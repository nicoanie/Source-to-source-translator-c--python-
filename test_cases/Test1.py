#Source generato dal frontend C->Python

a = 0 
d = 0 
def main( ):
	 global d
	 a = 5
	 b = 4
	 c = 0 
	 x = 3.5
	 y = ord('z')
	 z = False 
	 while( int( (a > 0) ) ):
	 	 if( int( (a == 3) )):
	 	 	 print ( x )
	 	 	 a = int( (a - 1) )
	 	 	 z = True
	 	 else:
	 	 	 print ( chr( y ) )
	 	 	 a = int( (a - 1) )
	 	 	 z = False

	 	 print ( int( z ) )

	 print("Inserire variabile intera: ")
	 case = input()
	 try:
	 	 c = int(case)
	 except ValueError: 
	 	 c = c

	 c = int( ((a + (b * 4)) - c) )
	 print ( c )
	 return 0

main()