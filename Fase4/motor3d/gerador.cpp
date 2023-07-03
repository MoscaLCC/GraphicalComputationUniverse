#define _USE_MATH_DEFINES
 
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <queue>
#include <string>
#include <set>
#include <fstream>
#include <math.h>
 
using namespace std;

struct Ponto{
float x,y,z;
};

float ** mult(float **a, float **b)
{
   int i, j;
    
    float ** mr = new float*[1];
    
    for (int i = 0; i < 1; ++i)
        mr[i] = new float[4];
    
    for (int i = 0; i < 4; ++i)
        mr[0][i] = 0;
        
    for (i = 0 ; i < 4; i++ )
      for (j = 0; j < 4; j++)
            mr[0][i] += a[0][j]*b[j][i];
              
    return mr; 
 
}
float mult_14_41(float **a, float **b)
{
   float t = 0.0;
   
   for(int i = 0; i < 4; i++)
        t += a[0][i] * b[i][0];
 
   return t;
}

void reduzParaUnitario(Ponto* vector){ 
        float length;

        // Calculate the length of the vector 
        length = (float) sqrt((vector->x * vector->x) + 
                                (vector->y * vector->y) + 
                                (vector->z * vector->z));

        // Keep the program from blowing up by providing an exceptable 
        // value for vectors that may calculated too close to zero. 
        if (length == 0.0f) 
            length =  1.0f;

        // Dividing each element by the length will result in a 
        // unit normal vector. 
        vector->x = vector->x / length; 
        vector->y = vector->y / length; 
        vector->z = vector->z / length; 
}

Ponto* calculateNormal(Ponto* p1, Ponto* p2 ,Ponto* p3){
    Ponto* normal = new Ponto();
    Ponto* v1 = new Ponto();
    Ponto*v2 = new Ponto();

    v1->x = p2->x - p1->x;
    v1->y = p2->y - p1->y;
    v1->z = p2->z - p1->z;

    v2->x = p3->x - p1->x;
    v2->y = p3->y - p1->y;
    v2->z = p3->z - p1->z;

     normal->x = v1->y * v2->z - v1->z * v2->y;
     normal->y = v1->z * v2->x - v1->x * v2->z;
     normal->z = v1->x * v2->y - v1->y * v2->x;

    (reduzParaUnitario(normal));    
    return normal;
}

float getPonto( float** U ,float** M , float** P ,float** V){
    
    float ** aux = new float*[1];
    
    for (int i = 0; i < 1; ++i)
        aux[i] = new float[4];
    
    for (int i = 0; i < 4; ++i)
        aux[0][i] = 0;
        
    aux = mult(U,M);
    aux = mult(aux,P);
    aux = mult(aux,M);
    
    float t = mult_14_41(aux,V);
    
    // libertar memoria
    delete [] aux[0];       
    delete [] aux;
    
    return t;
    
}
 
int main(int argc, char *argv[]){
 
    vector<Ponto*> pontosFinal;
    ofstream outputFile;
    string aux,nome;
    list<float> med;
    list<float> ::iterator it;
    float x, y;
    int w, v;
    double d, a, b, c;
    list<double> p;
    list<double> ::iterator it2;
	int q = 1;
 
    while (q < argc) {
        string aux = argv[q];
        if (aux == "cone") {
            q++;
            float raio = atof(argv[q]);
            q++;
            float height = atof(argv[q]);
            q++;
            float stacks = atof(argv[q]);
            double iter = M_PI*2.0;
            double i = 0.0;
            double m = 2 * M_PI / stacks;
            q++;
            string nmf = argv[q];
            cout << nmf;
 
            outputFile.open(nmf);
 
            while (i < iter) {
                outputFile << "0 " << height / 2 << " " << "0 " << sin(i)*raio << " " << -height / 2 << " " << cos(i)*raio << " ";
                i += m;
                outputFile << sin(i)*raio << " " << -height / 2 << " " << cos(i)*raio << endl;
 
                outputFile << sin(i)*raio << " " << -height / 2 << " " << cos(i)*raio << " ";
                i -= m;
                outputFile << sin(i)*raio << " " << -height / 2 << " " << cos(i)*raio << " " << "0 " << -height / 2 << " 0" << endl;
                i += m;
            }
            outputFile.close();
            q++;
        }
 
 
        else if (aux == "caixa") {
            q++;
            float comprimento = atof(argv[q]);
            q++;
            float largura = atof(argv[q]);
            q++;
            float altura = atof(argv[q]);
            q++;
            string nmf = argv[q];
            cout << nmf;
            outputFile.open(nmf);
            outputFile << largura / 2 << " " << -altura / 2 << " " << -comprimento / 2 << " " << -largura / 2 << " " << -altura / 2 << " " << -comprimento / 2 << " " << -largura / 2 << " " << -altura / 2 << " " << comprimento / 2 << endl;
            outputFile << -largura / 2 << " " << -altura / 2 << " " << comprimento / 2 << " " << largura / 2 << " " << -altura / 2 << " " << comprimento / 2 << " " << largura / 2 << " " << -altura / 2 << -comprimento / 2 << endl;
            outputFile << -largura / 2 << " " << altura / 2 << " " << comprimento / 2 << " " << -largura / 2 << " " << altura / 2 << " " << -comprimento / 2 << " " << largura / 2 << " " << altura / 2 << " " << -comprimento / 2 << endl;
            outputFile << largura / 2 << " " << altura / 2 << " " << -comprimento / 2 << " " << largura / 2 << " " << altura / 2 << " " << comprimento / 2 << " " << -largura / 2 << " " << altura / 2 << " " << comprimento / 2 << endl;
            outputFile << largura / 2 << " " << -altura / 2 << " " << -comprimento / 2 << " " << -largura / 2 << " " << altura / 2 << " " << -comprimento / 2 << " " << -largura / 2 << " " << -altura / 2 << " " << -comprimento / 2 << endl;
            outputFile << largura / 2 << " " << -altura / 2 << " " << -comprimento / 2 << " " << largura / 2 << " " << altura / 2 << " " << -comprimento / 2 << " " << -largura / 2 << " " << altura / 2 << " " << -comprimento / 2 << endl;
            outputFile << -largura / 2 << " " << -altura / 2 << " " << -comprimento / 2 << " " << -largura / 2 << " " << altura / 2 << " " << -comprimento / 2 << " " << -largura / 2 << " " << -altura / 2 << " " << comprimento / 2 << endl;
            outputFile << -largura / 2 << " " << altura / 2 << " " << -comprimento / 2 << " " << -largura / 2 << " " << altura / 2 << " " << comprimento / 2 << " " << -largura / 2 << " " << -altura / 2 << " " << comprimento / 2 << endl;
            outputFile << largura / 2 << " " << altura / 2 << " " << -comprimento / 2 << " " << largura / 2 << " " << -altura / 2 << " " << -comprimento / 2 << " " << largura / 2 << " " << -altura / 2 << " " << comprimento / 2 << endl;
            outputFile << largura / 2 << " " << altura / 2 << " " << -comprimento / 2 << " " << largura / 2 << " " << -altura / 2 << " " << comprimento / 2 << " " << largura / 2 << " " << altura / 2 << " " << comprimento / 2 << endl;
            outputFile << largura / 2 << " " << altura / 2 << " " << comprimento / 2 << " " << largura / 2 << " " << -altura / 2 << " " << comprimento / 2 << " " << -largura / 2 << " " << -altura / 2 << " " << comprimento / 2 << endl;
            outputFile << -largura / 2 << " " << -altura / 2 << " " << comprimento / 2 << " " << -largura / 2 << " " << altura / 2 << " " << comprimento / 2 << " " << largura / 2 << " " << altura / 2 << " " << comprimento / 2 << endl;
            outputFile.close();
            q++;
        }
 
 
        else if (aux == "plano") {
            q++;
            float comprimento = atof(argv[q]);
            q++;
            float largura = atof(argv[q]);
            q++;
            string nmf = argv[q];
 
            outputFile.open(nmf);
            outputFile << largura / 2 << " " << 0.0 << " " << -comprimento / 2 << " " << -largura / 2 << " " << 0.0 << " " << -comprimento / 2 << " " << -largura / 2 << " " << 0.0 << " " << comprimento / 2 << endl;
            outputFile << -largura / 2 << " " << 0.0 << " " << comprimento / 2 << " " << largura / 2 << " " << 0.0 << " " << comprimento / 2 << " " << largura / 2 << " " << 0.0 << " " << -comprimento / 2 << endl;
            outputFile << -largura / 2 << " " << 0.0 << " " << comprimento / 2 << " " << -largura / 2 << " " << 0.0 << " " << -comprimento / 2 << " " << largura / 2 << " " << 0.0 << " " << -comprimento / 2 << endl;
            outputFile << largura / 2 << " " << 0.0 << " " << -comprimento / 2 << " " << largura / 2 << " " << 0.0 << " " << comprimento / 2 << " " << -largura / 2 << " " << 0.0 << " " << comprimento / 2 << endl;
            outputFile.close();
            q++;
        }
         else if (aux == "esfera") {

            int k=0,l=0;
            Ponto*p1 = new Ponto();
            Ponto*p2 = new Ponto();
            Ponto*p3 = new Ponto();
            Ponto*norm = new Ponto();   
            q++;
            float raio = atof(argv[q]);
            q++;
            int camadas = atoi(argv[q]);
            q++;
            int fatias = atoi(argv[q]);
            q++;
            string nmf = argv[q];
            float *v=(float*)malloc(sizeof(float)* 6 * fatias*camadas * 3);
            float *n=(float*)malloc(sizeof(float)* 6 * fatias*camadas * 3);
            double beta = -M_PI / 2, alpha = 0.0;
            double betaLinha = 0.0, alphaLinha = 0.0;
 
            
            for (int i = 0; i < fatias; i++) {
                alpha = 0.0;
                for (int j = 0; j < camadas; j++) {
                    betaLinha = beta + M_PI / fatias;
 
                    v[k++]= raio*cosf(betaLinha)*sinf(alpha);
            p1->x = raio*cosf(betaLinha)*sinf(alpha);
            v[k++]= raio*sinf(betaLinha);
            p1->y = raio*sinf(betaLinha);
            v[k++] = raio*cosf(betaLinha)*cosf(alpha);
            p1->z = raio*cosf(betaLinha)*cosf(alpha);

            v[k++]= raio*cosf(beta)*sinf(alphaLinha);
            p2->x = raio*cosf(beta)*sinf(alphaLinha);
            v[k++]= raio*sinf(beta);
            p2->y = raio*sinf(beta);
            v[k++]= raio*cosf(beta)*cosf(alphaLinha);
            p2->z = raio*cosf(beta)*cosf(alphaLinha);

            v[k++] = raio*cosf(betaLinha)*sinf(alphaLinha);
            p3->x = raio*cosf(betaLinha)*sinf(alphaLinha);
            v[k++] = raio*sinf(betaLinha);
            p3->y = raio*sinf(betaLinha);
            v[k++] = raio*cosf(betaLinha)*cosf(alphaLinha);
            p3->z = raio*cosf(betaLinha)*cosf(alphaLinha);
            
            norm = calculateNormal(p1,p2,p3);

            n[l++]=norm->x;// raio*cosf(betaLinha)*sinf(alpha);
            n[l++]=norm->y;// raio*sinf(betaLinha);
            n[l++]= norm->z;//raio*cosf(betaLinha)*cosf(alpha);


            
            n[l++]= norm->x;//cosf(beta)*sinf(alphaLinha);
            n[l++]= norm->y;//sinf(beta);
            n[l++]= norm->z;//cosf(beta)*cosf(alphaLinha);


                    
            n[l++]= norm->x;//cosf(betaLinha)*sinf(alphaLinha);
            n[l++]= norm->y;//sinf(betaLinha);
            n[l++]= norm->z;//cosf(betaLinha)*cosf(alphaLinha);
                    
            

            v[k++] = raio*cosf(beta)*sinf(alphaLinha);
            p1->x = raio*cosf(beta)*sinf(alphaLinha);
            v[k++] = raio*sinf(beta);
            p1->y = raio*sinf(beta);
            v[k++] = raio*cosf(beta)*cosf(alphaLinha);
            p1->z = raio*cosf(beta)*cosf(alphaLinha);

            v[k++] = raio*cosf(betaLinha)*sinf(alpha);
            p2->x = raio*cosf(betaLinha)*sinf(alpha);
            v[k++] = raio*sinf(betaLinha);
            p2->y = raio*sinf(betaLinha);
            v[k++] = raio*cosf(betaLinha)*cosf(alpha);
            p2->z = raio*cosf(betaLinha)*cosf(alpha);

            v[k++] = raio*cosf(beta)*sinf(alpha);
            p3->x = raio*cosf(beta)*sinf(alpha);
            v[k++] = raio*sinf(beta);
            p3->y = raio*sinf(beta);
            v[k++] = raio*cosf(beta)*cosf(alpha);
            p3->z = raio*cosf(beta)*cosf(alpha);


            norm = calculateNormal(p1,p2,p3);
            n[l++]= norm->x;//(beta)*sinf(alphaLinha);
            n[l++]= norm->y;//(beta);
            n[l++]= norm->z;//(beta)*cosf(alphaLinha);
            
            n[l++]= norm->x;//cosf(betaLinha)*sinf(alpha);
            n[l++]= norm->y;//sinf(betaLinha);
            n[l++]= norm->z;//cosf(betaLinha)*cosf(alpha);

            n[l++]= norm->x;//cosf(beta)*sinf(alpha);
            n[l++]= norm->y;//sinf(beta);
            n[l++]= norm->z;//cosf(beta)*cosf(alpha);
                }
                beta = beta + M_PI / fatias;
            }
            
            q++;

        outputFile.open(nmf); 
        outputFile << 6*camadas*fatias*3 << endl;
        for (int auxx = 0; auxx < k; auxx++){
            outputFile << v[auxx] << " ";
        }
        for ( int auxx = 0; auxx < l; auxx++){
            outputFile << n[auxx] << " ";
        }
        outputFile.close();   
        }
        else if (aux == "patch"){
        float** Px = new float*[4];
        for (int i = 0; i < 4; ++i)
            Px[i] = new float[4];
        float** Py = new float*[4];
        for (int i = 0; i < 4; ++i)
          Py[i] = new float[4];
        float** Pz = new float*[4];
        for (int i = 0; i < 4; ++i)
            Pz[i] = new float[4];
        vector<Ponto*> pontos;
        float** M = new float*[4];
        for (int i = 0; i < 4; ++i)
            M[i] = new float[4];
        float** U = new float*[1];
        
        for (int i = 0; i < 1; ++i)
            U[i] = new float[4];
        float** V = new float*[4];
        for (int i = 0; i < 4; ++i)
            V[i] = new float[4];
        
        int numeropatches;
        int numeropontos;
        int m;
        float x1,y1,z1;
        FILE* nome = fopen("teapot.patch","r");
        fscanf(nome, "%d\n", &numeropatches);
        float** patch = new float*[numeropatches];
        for(int i = 0;i<numeropatches;i++){
            patch[i] = new float[16];
        }
        for (int i = 0; i < numeropatches; i++){
            for (int j = 0; j < 16; j++){
                fscanf(nome, "%d, ", &m);
                patch[i][j] = m;
            }
        }
        fscanf(nome, "%d\n", &numeropontos);
        
        int k = 0;
        for (int i = 0; i < numeropontos; i++){
            Ponto* p = new Ponto();
            fscanf(nome, "%f, %f, %f\n", &x1,&y1,&z1);
            p->x = x1;
            p->y = y1;
            p->z = z1;
            pontos.push_back(p);
            
        }
        
        M[0][0] = -1;M[0][1] = 3;M[0][2] = -3;M[0][3] = 1;
        M[1][0] = 3;M[1][1] = -6;M[1][2] = 3;M[1][3] = 0;
        M[2][0] = -3;M[2][1] = 3;M[2][2] = 0;M[2][3] = 0;
        M[3][0] = 1;M[3][1] = 0;M[3][2] = 0;M[3][3] = 0;

        int pontosI;
        
        for(pontosI = 0;pontosI<numeropatches;pontosI++){
                
                
                Px[0][0] = pontos[patch[pontosI][0]]->x;Px[0][1] = pontos[patch[pontosI][1]]->x;Px[0][2] = pontos[patch[pontosI][2]]->x;Px[0][3] = pontos[patch[pontosI][3]]->x;
                Px[1][0] = pontos[patch[pontosI][4]]->x;Px[1][1] = pontos[patch[pontosI][5]]->x;Px[1][2] = pontos[patch[pontosI][6]]->x;Px[1][3] = pontos[patch[pontosI][7]]->x;
                Px[2][0] = pontos[patch[pontosI][8]]->x;Px[2][1] = pontos[patch[pontosI][9]]->x;Px[2][2] = pontos[patch[pontosI][10]]->x;Px[2][3] = pontos[patch[pontosI][11]]->x;
                Px[3][0] = pontos[patch[pontosI][12]]->x;Px[3][1] = pontos[patch[pontosI][13]]->x;Px[3][2] = pontos[patch[pontosI][14]]->x;Px[3][3] = pontos[patch[pontosI][15]]->x;
                
                Py[0][0] = pontos[patch[pontosI][0]]->y;Py[0][1] = pontos[patch[pontosI][1]]->y;Py[0][2] = pontos[patch[pontosI][2]]->y;Py[0][3] = pontos[patch[pontosI][3]]->y;
                Py[1][0] = pontos[patch[pontosI][4]]->y;Py[1][1] = pontos[patch[pontosI][5]]->y;Py[1][2] = pontos[patch[pontosI][6]]->y;Py[1][3] = pontos[patch[pontosI][7]]->y;
                Py[2][0] = pontos[patch[pontosI][8]]->y;Py[2][1] = pontos[patch[pontosI][9]]->y;Py[2][2] = pontos[patch[pontosI][10]]->y;Py[2][3] = pontos[patch[pontosI][11]]->y;
                Py[3][0] = pontos[patch[pontosI][12]]->y;Py[3][1] = pontos[patch[pontosI][13]]->y;Py[3][2] = pontos[patch[pontosI][14]]->y;Py[3][3] = pontos[patch[pontosI][15]]->y;

                Pz[0][0] = pontos[patch[pontosI][0]]->z;Pz[0][1] = pontos[patch[pontosI][1]]->z;Pz[0][2] = pontos[patch[pontosI][2]]->z;Pz[0][3] = pontos[patch[pontosI][3]]->z;
                Pz[1][0] = pontos[patch[pontosI][4]]->z;Pz[1][1] = pontos[patch[pontosI][5]]->z;Pz[1][2] = pontos[patch[pontosI][6]]->z;Pz[1][3] = pontos[patch[pontosI][7]]->z;
                Pz[2][0] = pontos[patch[pontosI][8]]->z;Pz[2][1] = pontos[patch[pontosI][9]]->z;Pz[2][2] = pontos[patch[pontosI][10]]->z;Pz[2][3] = pontos[patch[pontosI][11]]->z;
                Pz[3][0] = pontos[patch[pontosI][12]]->z;Pz[3][1] = pontos[patch[pontosI][13]]->z;Pz[3][2] = pontos[patch[pontosI][14]]->z;Pz[3][3] = pontos[patch[pontosI][15]]->z;    

        float u,v;
        for(u=0.0;u<1.0;){
            for(v=0.0;v<1.0;){
                U[0][0] = pow(u,3);U[0][1] = pow(u,2);U[0][2] = u;U[0][3] = 1;
                V[0][0] = pow(v,3);V[1][0] = pow(v,2);V[2][0] = v;V[3][0] = 1;

                Ponto* pf = new Ponto();
                float x2,y2,z2;
                x2 = getPonto(U,M,Px,V);
                y2 = getPonto(U,M,Py,V);
                z2 = getPonto(U,M,Pz,V);
                pf->x = x2;
                pf->y = y2;
                pf->z = z2;
                pontosFinal.push_back(pf);
                v= v+0.015;
            }
            u =u+0.015;
        }
}
        
        outputFile.open("teapot.3d");
        for (size_t i = 0; i < pontosFinal.size(); i++){
                outputFile << pontosFinal[i]->x << " ";
                outputFile << pontosFinal[i]->y << " ";
                outputFile << pontosFinal[i]->z << endl;
        }
        outputFile.close();


		q++;
}
        
    }
 
    return 0;
}