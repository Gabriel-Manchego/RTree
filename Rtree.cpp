#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <sstream>
#include <utility>
#include <stack>
#include <math.h>
#include <queue>
using namespace std;

struct Pagina;
struct Entrada;
struct Rectangulo{
    string identificador;
	int minx,miny,maxx,maxy;
	Rectangulo(string identificador, int minx, int miny, int maxx, int maxy){ 
		this -> identificador = identificador;
		this -> minx = minx;
		this -> miny = miny;
		this -> maxx = maxx;
		this -> maxy = maxy;
	}
	Rectangulo(){
        identificador = ""; 
    }
};

struct Entrada{ 
	Rectangulo rec; 
	Pagina *ptr;
	Entrada(){
        rec.identificador = "";
    }
};

struct Pagina{
	Entrada ent[4]; 
	bool esHoja; 
	Pagina(){
        esHoja = false;
    }
};

struct Par{
	Pagina *vP;
	int index;
};

class RTree{
	Pagina* root;
    bool ArbolVacio;
	stack<Par> pila;//Par visitado
	ostringstream SMBR;
public:
	RTree(){
        root = new Pagina;
	    root -> ent[0].ptr = new Pagina;//puntero a una hoja
	    root -> ent[0].ptr -> esHoja = true;
	    bool ArbolVacio = true;
    }
	void BuscarParaInsertar(string identificador, int minx, int miny, int maxx, int maxy);
	Rectangulo CalcMBRdePagina(Entrada *ent);
	Rectangulo CalcMBRde2Rec(Rectangulo A, Rectangulo B);
	int CalcArea(int minx, int miny, int maxx, int maxy);
	int Diferencia(Rectangulo R1, Rectangulo R2);
	void Insertar(Pagina *temp, Rectangulo Rect, Pagina *ptr);
	Rectangulo* Sort(Rectangulo *rectangles);
	bool EstaLlena(Pagina *temp);
	void Split(Pagina *temp, Rectangulo newNode);	
};

//BuscarParaInsertar
void RTree::BuscarParaInsertar(string identificador, int minx, int miny, int maxx, int maxy){
	Rectangulo R = Rectangulo(identificador,minx,miny,maxx,maxy);
	if(ArbolVacio){
		root->ent[0].rec = R;
		root->ent[0].rec.identificador = SMBR.str();
		ArbolVacio = false;
	}
	Pagina *temp;
	temp = root;
	int Dif[4];
	while(temp->esHoja==false){ //mientras no sea hoja
		cout << "no es hoja" << endl;
		int Dif[4] = { 0 };
		int stop;
		Rectangulo nRect, oRect;
		for(int i =0; i<4; i++){
			if (!temp->ent[i].rec.identificador.empty()){
				nRect = CalcMBRde2Rec(temp->ent[i].rec, R);
				oRect = temp->ent[i].rec;
				Dif[i]=Diferencia(nRect,oRect);
				cout << "Dif["<<i<<"]: " << Dif[i] << endl;
				stop = i;
			}
		}
		//para encontrar el que menor area produzca
		int act = Dif[0];
		int index = 0;
		for(int i=0;i<=stop;i++){
			if(Dif[i]<act){
				cout<<"Dif["<<i<<"] es el elegido!" << endl;
				act = Dif[i];
				index = i;
				cout<< index << endl;
			}
		}
		Par visitado;
		visitado.vP = temp;
		visitado.index=index;
		pila.push(visitado);
		//para ir bajando por las ramas
		temp=temp->ent[index].ptr;
		//hasta encontrar una hoja
	}
	cout << "es hoja" << endl;
	//ahora temp estaria apuntando a la hoja
	Insertar(temp, R, NULL);
}

Rectangulo RTree::CalcMBRdePagina(Entrada *ent){
	Rectangulo R;
	int cmbr[4]={ent -> ptr -> ent[0].rec.minx,ent -> ptr -> ent[0].rec.miny,0,0};
	for(int i=0;i<4;i++){
		if (ent->ptr -> ent[i].rec.minx < cmbr[0] && ent -> ptr -> ent[i].rec.minx !=0) 
            cmbr[0] = ent -> ptr->ent[i].rec.minx;
		if (ent->ptr -> ent[i].rec.miny < cmbr[1] && ent -> ptr -> ent[i].rec.miny !=0) 
            cmbr[1] = ent -> ptr->ent[i].rec.miny;
		if (ent->ptr -> ent[i].rec.maxx > cmbr[2] ) 
            cmbr[2] = ent -> ptr -> ent[i].rec.maxx;
		if (ent->ptr -> ent[i].rec.maxy > cmbr[3] ) 
            cmbr[3] = ent -> ptr -> ent[i].rec.maxy;
	}
	R.minx = cmbr[0];
	R.miny = cmbr[1];
	R.maxx = cmbr[2];
	R.maxy = cmbr[3];
	R.identificador = ent -> ptr -> ent[0].rec.identificador;
	return R;
}


Rectangulo RTree::CalcMBRde2Rec(Rectangulo A, Rectangulo B){
	Rectangulo R;
	R.minx = A.minx;
	R.miny = A.miny;
	R.maxx = A.maxx;
	R.maxy = A.maxy;
	if(A.minx>B.minx) 
        R.minx=B.minx;
	if(A.maxx<B.maxx) 
        R.maxx=B.maxx;
	if(A.miny>B.miny) 
        R.miny=B.miny;
	if(A.maxy<B.maxy) 
        R.maxy=B.maxy;
	
	return R;
}

void RTree::Insertar(Pagina *temp, Rectangulo Rect, Pagina *ptr){
	Rectangulo R;
	if(!EstaLlena(temp)){
		cout << "no esta llena" << endl;
		
		for (int j=0;j<4;j++){
			if (temp->ent[j].rec.identificador.empty()){
				temp->ent[j].rec = Rect;
				temp->ent[j].ptr = ptr;
				SMBR.str("");SMBR.clear();
				SMBR << "MBR" << j+1;
				if(temp->esHoja==false)
					temp->ent[j].rec.identificador = SMBR.str();
				
				cout<<"Fue insertado: "<<temp->ent[j].rec.identificador << endl;
				break;
			}
		}
		//mientras existan niveles
		if(!pila.empty()){
			cout<<"pila no esta vacia" << endl;
			int entryIndex = pila.top().index;
			cout << "Index: " << entryIndex << endl;
			R = CalcMBRdePagina(&pila.top().vP->ent[entryIndex]);
			pila.top().vP->ent[entryIndex].rec=R;
			SMBR.str("");SMBR.clear();
			SMBR << "MBR" << entryIndex+1;
			pila.top().vP->ent[entryIndex].rec.identificador=SMBR.str();
			pila.pop();
		}
	}
    else{ // si la pila no esta vacia
		cout << "Pagina llena" << endl;
		Split(temp,Rect);
	}
}

bool RTree::EstaLlena(Pagina *temp){ //para ver si la pagina esta llena
	for (int i=0;i<4;i++)
		if ( temp->ent[i].rec.identificador.empty())
			return false;
	return true;
}

void RTree::Split(Pagina *temp, Rectangulo newNode){
	cout<<"Split iniciando "<< endl;
	Pagina* PaginaN = new Pagina;// new Pagina;
	PaginaN->esHoja = true;
	Rectangulo rec[5];
	
	for (int i=0;i<4;i++){
		rec[i]=temp->ent[i].rec;
	}
	rec[4]=newNode;
	Rectangulo *sorted;
	sorted = Sort(rec);
	for (int i=0;i<4;i++){//vaciar la antigua pagina
		temp->ent[i].rec.identificador= "";
	}
	//para redistribuir
	temp->ent[0].rec=sorted[0];
	temp->ent[1].rec=sorted[1];
	PaginaN->ent[0].rec=sorted[2];
	PaginaN->ent[1].rec=sorted[3];
	PaginaN->ent[2].rec=sorted[4];
	
	int entryIndex = pila.top().index;
	//necesito calcular un nuevo MBR
	pila.top().vP->ent[entryIndex].rec= CalcMBRdePagina(&pila.top().vP->ent[entryIndex]);
	
	Rectangulo tmp;
	tmp = CalcMBRde2Rec(sorted[2], CalcMBRde2Rec(sorted[3], sorted[4]));
	tmp.identificador = SMBR.str();
	
	//Para insertar en la nueva pagina
	if(!EstaLlena(pila.top().vP)){
		Insertar(pila.top().vP,tmp,PaginaN);	
	}
    else{
		cout<<"La pagina esta llena \n";
		Pagina* newRoot = new Pagina;
		newRoot->ent[0].ptr=pila.top().vP;//para conectar con la pagina antigua
		newRoot->ent[0].rec=CalcMBRdePagina(&newRoot->ent[0]);
		newRoot->ent[0].rec.identificador= "MBR5";
		pila.pop();
		root = newRoot;//reasirnar la raiz
		Par visitado;
		visitado.vP = newRoot;
		visitado.index=0;
		pila.push(visitado);
		Insertar(newRoot,tmp,PaginaN);
	}
}


//diferencia entre las areas
int RTree::Diferencia(Rectangulo newMBR, Rectangulo oldMBR){
	int newArea = CalcArea(newMBR.minx, newMBR.miny, newMBR.maxx, newMBR.maxy);
	int oldArea = CalcArea(oldMBR.minx, oldMBR.miny, oldMBR.maxx, oldMBR.maxy);
	return newArea-oldArea;
}

//se explica solo
int RTree::CalcArea(int minx, int miny, int maxx, int maxy){
	return (maxx-minx)*(maxy-miny);
}

Rectangulo* RTree::Sort(Rectangulo *rectangles){
	Rectangulo temp;
	cout<<"Sort iniciando "<< " \n";
	for(int i=0;i<5;i++){
		for(int j=i+1;j<5;j++){
			if (rectangles[i].minx >= rectangles[j].minx){
				temp = rectangles[i];
				rectangles[i] = rectangles[j];
				rectangles[j] = temp;
			}
		}
	}
	return rectangles;
}

int main(){
	RTree R;
	R.BuscarParaInsertar("A",7,5,12,9);
	R.BuscarParaInsertar("B",2,5,8,12);
	R.BuscarParaInsertar("C",20,25,22,26);
	R.BuscarParaInsertar("D",20,55,30,65);
	R.BuscarParaInsertar("E",1,6,5,12);
	return 0;
}