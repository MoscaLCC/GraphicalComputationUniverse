#define _USE_MATH_DEFINES

#include <vector>
#include <tinystr.h>
#include <tinyxml.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

struct Modelo {
	string nome; 
	vector <float> pontos; 
};

struct Grupo {
	float xT, yT, zT;
	float xR, yR, zR, angR;
	float xS, yS, zS;
	int ordem[3];
	vector <Modelo*> modelo;
	vector <Grupo*> grupo;
};

struct Cena {
	vector<Grupo*> filhos;
};

Cena* cena;
float alt_cam = 5, alt = 2, translate_x = 0, rotate = 0;
float raio = 150, cam_h = 0, cam_v = 0.5, camh_x = 0, camh_y = 0, h = 2.0;

void changeSize(int w, int h) {
	if (h == 0)
		h = 1;

	float ratio = w * 1.0 / h;

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	glViewport(0, 0, w, h);

	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
}

Modelo* lerPontos(const char* ficheiro) {
	Modelo* modelo = new Modelo();
	modelo->nome = ficheiro;
	double a, b, c;
	ifstream input_file(ficheiro);
	while (input_file >> a >> b >> c) {
		modelo->pontos.push_back(a);
		modelo->pontos.push_back(b);
		modelo->pontos.push_back(c);
	}
	return modelo;
}

void desenharfilhos(Grupo* g) {
	vector<float> vet;
	size_t i, k;
	glPushMatrix();
	for (int j = 0; j < 3; j++) {
		if (g->ordem[j] == 1) {//translaçao
			glTranslatef(g->xT, g->yT, g->zT);
		}
		else if (g->ordem[j] == 2) {//rotaçao
			glRotatef(g->angR, g->xR, g->yR, g->zR);
		}
		else if (g->ordem[j] == 3) {//escala
			glScalef(g->xS, g->yS, g->zS);
		}
	}
	for (i = 0; i < g->modelo.size(); i++) {
		//controlar cores
		if (g->modelo[i]->nome == "esfera.3d")
			glColor3f(1, 1, 0);
		if (g->modelo[i]->nome == "cilindro.3d")
			glColor3f(1, 0.15, 0);
		vet = g->modelo[i]->pontos;
		glBegin(GL_TRIANGLES);
		for (k = 0; k < vet.size() / 3; k++) {
			glVertex3f(vet[3 * k], vet[3 * k + 1], vet[3 * k + 2]);
		}
		glEnd();
	}

	for (k = 0; k< g->grupo.size(); k++) {
		desenharfilhos(g->grupo[k]);
	}
	glPopMatrix();
}

void desenhar(Cena* cena) {
	size_t i;
	for (i = 0; i < cena->filhos.size(); i++) {
		desenharfilhos(cena->filhos[i]);
	}
}


void renderScene(void) {
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// set the camera
	glLoadIdentity();
	gluLookAt(raio*sin(cam_h + camh_x)*cos(cam_v + camh_y), raio*sin(cam_v + camh_y), raio*cos(cam_h + camh_x)*cos(cam_v + camh_y),
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);
	glPolygonMode(GL_FRONT, GL_LINE);
	//End of frame
	if (cena != NULL)
		desenhar(cena);
	glutSwapBuffers();
}


Grupo* aux1(TiXmlElement* grupo) {
	string elemnome;
	Grupo* g1 = new Grupo();
	const char* ficheiro;
	const char* atributo;
	//iniciar as variaveis de transiçao e rotaçao a zero caso nao existam, e escala a 1
	g1->xT = g1->yT = g1->zT = g1->xR = g1->yR = g1->zR = g1->angR = 0.0; 
	g1->xS = g1->yS = g1->zS = 1.0;
	int i = 0;
	int t =0, r =0, s = 0,mo=0;//conta o numero de translaçoes, rotaçoes e escalas 
	for(TiXmlElement* e = grupo->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
		elemnome = e->Value();
		if (elemnome == "grupo") {
			g1->grupo.push_back(aux1(e));//caso tenha filho
		}
		else if (elemnome == "translacao") {
			t++;
			if (t > 1)return NULL;//impedir casos invalidos
			g1->ordem[i] = 1;//translaçao
			atributo = e->Attribute("x");
			if (atributo != NULL) g1->xT = atof(atributo);
			atributo = e->Attribute("y");
			if (atributo != NULL) g1->yT = atof(atributo);
			atributo = e->Attribute("z");
			if (atributo != NULL) g1->zT = atof(atributo);
		}
		else if (elemnome == "rotacao")
		{
			r++;
			if (r>1)return NULL;
			g1->ordem[i] = 2;//rotaçao
			atributo = e->Attribute("angulo");
			if (atributo != NULL) g1->angR = atof(atributo);
			atributo = e->Attribute("eixox");
			if (atributo != NULL) g1->xR = atof(atributo);
			atributo = e->Attribute("eixoy");
			if (atributo != NULL) g1->yR = atof(atributo);
			atributo = e->Attribute("eixoz");
			if (atributo != NULL) g1->zR = atof(atributo);
		}
		else if (elemnome == "escala")
		{
			s++;
			if (s>1)return NULL;
			g1->ordem[i] = 3;//escala
			atributo = e->Attribute("x");
			if (atributo != NULL) g1->xS = atof(atributo);
			atributo = e->Attribute("y");
			if (atributo != NULL) g1->yS = atof(atributo);
			atributo = e->Attribute("z");
			if (atributo != NULL) g1->zS = atof(atributo);
		}
		else if (elemnome == "modelos")
		{
			mo++;
			if (mo>1)
				return NULL;
			for (TiXmlElement* e1 = e->FirstChildElement(); e1 != NULL; e1 = e1->NextSiblingElement())
			{
				ficheiro = e1->Attribute("ficheiro"); // vai buscar os atributos de ficheiro
				if (ficheiro != NULL) {
					g1->modelo.push_back(lerPontos(ficheiro));
				}

			}
		}
		i++;
	}

	return g1;
}





Cena* aux2(TiXmlElement* cena) {
	Cena* c1 = new Cena();
for (TiXmlElement* e = cena->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
		if (aux1(e) == NULL)
			return NULL;
		else
			c1->filhos.push_back(aux1(e));
	}
	return c1;
}

void teclado_normal(unsigned char tecla, int x, int y) {
	switch (tecla) {
	case 'a':
		raio -= 5;
		break;
	case 'd':
		raio += 5;
		break;
	case 'w':
		h += 0.1;
		break;
	case 's':
		h -= 0.1;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}
void teclado_especial(int tecla, int x, int y) {
	switch (tecla) {
	case GLUT_KEY_UP:
		if (cam_v + 0.05<M_PI_2)   //Para câmera não virar ao contrário
			cam_v += 0.05;
		break;
	case GLUT_KEY_DOWN:
		if (cam_v - 0.05>-M_PI_2)  //Para câmera não virar ao contrário
			cam_v -= 0.05;
		break;
	case GLUT_KEY_LEFT:
		cam_h -= 0.05;
		break;
	case GLUT_KEY_RIGHT:
		cam_h += 0.05;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}


void menu(int op) {


}

void front_menu(int op) {
	switch (op) {
	case 1:
		glPolygonMode(GL_FRONT, GL_POINT);
		break;
	case 2:
		glPolygonMode(GL_FRONT, GL_LINE);
		break;
	case 3:
		glPolygonMode(GL_FRONT, GL_FILL);
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void back_menu(int op) {
	switch (op) {
	case 1:
		glPolygonMode(GL_BACK, GL_POINT);
		break;
	case 2:
		glPolygonMode(GL_BACK, GL_LINE);
		break;
	case 3:
		glPolygonMode(GL_BACK, GL_FILL);
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char **argv) {
	TiXmlDocument doc;

	if (!doc.LoadFile("figura.xml")) 
	{
		cerr << doc.ErrorDesc() << endl;
		return EXIT_FAILURE;
	}


	TiXmlElement* cenaxml = doc.FirstChildElement();
	if (cenaxml == NULL) // se documento nao tem nada
	{
		cerr << "Failed to load file: No root element." << endl;
		doc.Clear();
		return EXIT_FAILURE; // retorna o erro
	}

	cena = new Cena();
	cena = aux2(cenaxml);

	if (cena == NULL)
		cerr << "ERRO" << endl;



	// put init here

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("CG@DI");
	// put callback registration here
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

	glutKeyboardFunc(teclado_normal);
	glutSpecialFunc(teclado_especial);

	int  id_frontmenu,id_backmenu ;

	id_frontmenu = glutCreateMenu(front_menu);
	glutAddMenuEntry("GL POINT", 1);
	glutAddMenuEntry("GL LINE", 2);
	glutAddMenuEntry("GL FILL", 3);

	id_backmenu = glutCreateMenu(back_menu);
	glutAddMenuEntry("GL POINT", 1);
	glutAddMenuEntry("GL LINE", 2);
	glutAddMenuEntry("GL FILL", 3);

	glutCreateMenu(menu);
	glutAddSubMenu("GL FRONT", id_frontmenu);
	glutAddSubMenu("GL BACK", id_backmenu);

	glutAttachMenu(GLUT_RIGHT_BUTTON);


	// OpenGL settings 
	glEnable(GL_DEPTH_TEST);
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// enter GLUT's main loop
	glutMainLoop();

	return 1;
}
