#include<fstream>
#include<iostream>
#include<string>
#include<cstring>
#include<vector>
using namespace std;

struct Alumno{
    char codigo [5];
    char nombre [11];
    char apellidos [20];
    char carrera [15];
    int ciclo;
    float mensualidad;
};

ostream& operator<<(ostream& out, Alumno a){
    out.write((char*) &a, sizeof(Alumno));
    return out;
}

istream& operator>>(istream& is, Alumno& a){
    is.read((char*) &a, sizeof(Alumno));
    return is;
}

class FixedRecord{
    public:
    FixedRecord(string _path): path(_path){
        ofstream archivo;
        archivo.open(path, ios::out|ios::app |ios::binary);
        archivo.seekp(0, ios::end);
        if(archivo.tellp()==0) {
            int firstDel= -1;
            archivo.write((char*) &firstDel, sizeof(int));
        }
        archivo.close();
    }
    void add(Alumno a){
        ifstream archivo_in(path, ios::in|ios::binary);
        int ptr_pos_head, ptr_pos_next;
        archivo_in.read((char*) &ptr_pos_head, sizeof(int));
        if(ptr_pos_head != -1){
            archivo_in.seekg((ptr_pos_head-1)*sizeof(int)+ptr_pos_head*sizeof(Alumno),ios::cur);
            archivo_in.read((char*) &ptr_pos_next, sizeof(int));
        }
        archivo_in.close();
        ofstream archivo_out;
        if(ptr_pos_head == -1)
            archivo_out.open(path, ios::out|ios::app | ios::binary);
        else{
            archivo_out.open(path, ios::in|ios::binary|ios::out);
            archivo_out.write((char*) &ptr_pos_next, sizeof(int));
            archivo_out.seekp((ptr_pos_head-1)*(sizeof(Alumno)+sizeof(int)), ios::cur);
        }
        archivo_out<<a;
        int pos_default = 0;
        archivo_out.write((char*) &pos_default, sizeof(int));
        archivo_out.close();
    }
    vector<Alumno> load(){
        ifstream archivo;
        archivo.open(path, ios::in | ios::binary);
        archivo.seekg(sizeof(int));
        vector<Alumno> result;
        Alumno atemp;
        while(archivo>>atemp){
            int del;
            archivo.read((char*) &del, sizeof(int));
            if(del == 0 )
                result.push_back(atemp);
        }
        archivo.close();
        return result;
    }
    Alumno readRecord(int pos){
        if(pos < 0) throw("No valores negativos");
        pos++;
        Alumno result;
        int del;
        ifstream archivo(path, ios::in | ios::binary);
        archivo.seekg(sizeof(int), ios::beg);
        for (int i = 1; i < pos; i++){
            archivo.seekg(sizeof(Alumno), ios::cur);
            archivo.read((char*) &del, sizeof(int));
            if(del != 0) i--;
        }
        do{
            archivo>>result;
            archivo.read((char*) &del, sizeof(int));
        }while(del != 0);
        archivo.close();
        return result;
    }
    bool delete_reg(int pos){
        if(pos < 0) throw("No valores negativos");
        pos++;
        ifstream f_in(path, ios::in|ios::binary);
        int last_ptr;
        f_in.read((char*) &last_ptr, sizeof(int));
        if(last_ptr != -1){
            int free_ptr;
            for (int i = 1; i <= pos; i++){
                f_in.seekg(sizeof(Alumno), ios::cur);
                f_in.read((char*) &free_ptr, sizeof(int));
                if(free_ptr != 0) pos++;
            }
        }
        f_in.close();

        ofstream f_out(path, ios::in|ios::binary|ios::out);
        f_out.write((char*) &pos, sizeof(int));
        f_out.seekp(pos*sizeof(Alumno) + (pos-1)*sizeof(int), ios::cur);
        f_out.write((char*) &last_ptr, sizeof(int));
        f_out.close();
        return true;;
    }
    private:
    string path;
};

void pA(Alumno a){
    cout<<a.codigo<<" "<<a.nombre<<" "<<a.apellidos<<" "<<a.carrera<<" "<<a.ciclo<<" "<<a.mensualidad;
}

int main(){
    // Crear archivos
    FixedRecord fr("datos2.dat");

    // Agregar registros
    fr.add(Alumno{"a101", "cristian", "cueva vargas", "ciencia", 5, 9832.12});
    fr.add(Alumno{"a102", "carlos", "lopez marques", "comunicacion", 5, 245.2});
    fr.add(Alumno{"a103", "gianmarco", "ramirez", "tecnologia", 7, 34});
    fr.add(Alumno{"a104", "pablo", "nolberto solano", "futbol", 25, 0.12});
    fr.add(Alumno{"a105", "joaquin", "nolberto solano", "arte", 3, -32.12}); 

    // Seleccionar registros
    auto rvector = fr.load();
    for(auto i: rvector) {pA(i); cout<<endl;}
    cout<<endl;

    // Seleccionar registro
    auto r = fr.readRecord(2);
    pA(r);
    cout<<"\n\n";

    // Borrar registro
    fr.delete_reg(2);
    fr.delete_reg(3);

    // Seleccionar registros
    rvector = fr.load();
    for(auto i: rvector) {pA(i); cout<<endl;}
    cout<<"\n";

    // Seleccionar registro
    auto r2 = fr.readRecord(2);
    pA(r2);
    cout<<"\n\n";

    // Agregar registros
    fr.add(Alumno{"a111", "joaquin", "nolberto solano", "arte", 3, -32.12}); 
    fr.add(Alumno{"a150", "joaquin", "nolberto solano", "arte", 3, -32.12}); 

    // Seleccionar registros
    rvector = fr.load();
    for(auto i: rvector) {pA(i); cout<<endl;}

    return 0;
}