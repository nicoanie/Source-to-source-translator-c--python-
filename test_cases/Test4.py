#Source generato dal frontend C->Python

def func( x):
	 print("Inserire un valore intero: ")
	 case = input()
	 try:
	 	 x = int(case)
	 except ValueError: 
	 	 x = x

	 print ( x )
	 return x
def main( ):
	 z = 0 
	 func( z )
	 return 0

main()