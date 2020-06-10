#include <fstream>

//Creo un oggetto della classe ofstream
ofstream trad;

vector<Node*> global_var;

int indentazione;

void indent (int ind){
    for (int i=0; i<ind; i++){
        trad.open("python.py", ios::app);
        trad << "\t " ;
        trad.close();
    }
}

void indent2 (int ind){
    for (int i=0; i<ind; i++){
        trad << "\t " ;
    }
}


string name_variable; // Utile per i controlli quando devo assegnare i valori ad una variabile
string type_variabile;
int assignment_in = 0;
int while_condition = 0;
int if_condition = 0;
string aux_typ;
bool new_file = true;
int Bin_Op = 0;
bool condition = false;
bool utile = false;


//Controllare se le ho utilizzate
string ass_whil_if_name = "";
string ass_whil_if_value = "";
