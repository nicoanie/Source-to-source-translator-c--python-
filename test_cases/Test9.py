#Source generato dal frontend C->Python
def struct(x):
	 if(x == 0):
	 	 return False
	 else:
	 	 return True

def main( ):
	 x = 0
	 y = 5
	 while( struct(x) ):
	 	 x = x
	 	 print ( x )

	 while( struct(y) ):
	 	 x = y
	 	 x = int( (x + 1) )
	 	 print ( x )

	 return 0

main()