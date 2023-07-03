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
 
int main(int argc, char *argv[]){
 
    ofstream outputFile;
    string aux;
     
 
    int q = 1;
 
    cout << argv[1];
 
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
            q++;
            float raio = atof(argv[q]);
            q++;
            int camadas = atoi(argv[q]);
            q++;
            int fatias = atoi(argv[q]);
            q++;
            string nmf = argv[q];
            double beta = -M_PI / 2, alpha = 0.0;
            double betaLinha = 0.0, alphaLinha = 0.0;
 
            outputFile.open(nmf);
            for (int i = 0; i < fatias; i++) {
                alpha = 0.0;
                for (int j = 0; j < camadas; j++) {
                    betaLinha = beta + M_PI / fatias;
 
                    outputFile << raio*cos(betaLinha)*sin(alpha) << " " << raio*sin(betaLinha) << " " << raio*cos(betaLinha)*cos(alpha) << " " << raio*cos(beta)*sin(alpha) << " " << raio*sin(beta) << " " << raio*cos(beta)*cos(alpha) << " ";
                    alpha = alpha + 2 * M_PI / camadas;
                    outputFile << raio*cos(beta)*sin(alpha) << " " << raio*sin(beta) << " " << raio*cos(beta)*cos(alpha) << " " << endl;
                    outputFile << raio*cos(beta)*sin(alpha) << " " << raio*sin(beta) << " " << raio*cos(beta)*cos(alpha) << " " << raio*cos(betaLinha)*sin(alpha) << " " << raio*sin(betaLinha) << " " << raio*cos(betaLinha)*cos(alpha) << " ";
                    alpha = alpha - 2 * M_PI / camadas;
                    outputFile << raio*cos(betaLinha)*sin(alpha) << " " << raio*sin(betaLinha) << " " << raio*cos(betaLinha)*cos(alpha) << " " << endl;
                    alpha = alpha + 2 * M_PI / camadas;
                }
                beta = beta + M_PI / fatias;
            }
            outputFile.close();
            q++;
        }
 
    }
 
    return 0;
}