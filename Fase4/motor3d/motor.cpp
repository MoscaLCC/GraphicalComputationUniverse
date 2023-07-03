#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "DevIL.lib")

#define _USE_MATH_DEFINES

#include <vector>
#include <tinystr.h>
#include <tinyxml.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <IL/il.h>

extern long currentTime = 0;
using namespace std;
float alt_cam = 5, alt = 2, translate_x = 0, rotate = 0;
float raio = 150, cam_h = 0, cam_v = 0.5, camh_x = 0, camh_y = 0, h = 2.0;
float t = 0.0;

GLfloat amb[3] = { 0.2f, 0.2f, 0.2f };
GLfloat diff[3] = { 1.0f, 1.0f, 1.0f };
GLfloat specularLight[3] = { 0.0f, 0.0f, 0.0f };

struct Modelo {
	string nome;
	GLuint vbo; //identificador do vbo
	GLuint norm;
	int nvertices;
	vector <float> pontos;
	vector <float> normal;
};

struct Luzes{
	string tipo;
	float lx,ly,lz;
};

struct Ponto {
	float pontos[3];
};

struct Grupo {
	int tempoR, tempoT;
	float t, tempoA;
	float xR, yR, zR, angR;
	float xS, yS, zS;
	int npontos;
	vector <Ponto*> pontos;
	vector <Modelo*> modelo;
	vector <Grupo*> grupo;
};

struct Cena {
	vector<Luzes*> luzes;
	vector<Grupo*> filhos;
};

Cena* cena;

//introduzir os pontos em cada modelo

Modelo* lerPontos(const char* ficheiro) {
	Modelo* modelo = new Modelo();
	modelo->nome = ficheiro;
	float a, b, c;
	int n,i = 0,j=0;
	ifstream input_file(ficheiro);
	input_file >> n;
	modelo->nvertices = n;
	while (i<n) {
		input_file >> a >> b >> c;
		modelo->pontos.push_back(a); i++;
		modelo->pontos.push_back(b); i++;
		modelo->pontos.push_back(c); i++;
	}
	while (j<n) {
		input_file >> a >> b >> c;
		modelo->normal.push_back(a); j++;
		modelo->normal.push_back(b); j++;
		modelo->normal.push_back(c); j++;
	}
	return modelo;
}

//criar o VBO do modelo atrasves dos vertices inseridos da lerPontos

void modeloVBO(Modelo *modelo) {
	int k = 0,h=0,l=0;
	size_t i;
	float *vertexB;
	float *normalB;
	int numpontos = modelo->nvertices;
	normalB = (float*)malloc(numpontos*sizeof(float));
	vertexB = (float*)malloc(numpontos*sizeof(float));
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	for (i = 0; i<modelo->pontos.size(); i++) {
		vertexB[k++] = modelo->pontos[i];
	}
	for(i =0;i<modelo->normal.size();i++){
		normalB[l++] = modelo->normal[i];
	}
	glGenBuffers(1, &(modelo->vbo));//criar um novo vbo , e usar a variavel criada para armazenar o identificador do vbo
	glBindBuffer(GL_ARRAY_BUFFER, modelo->vbo);//ativar o vbo
	glBufferData(GL_ARRAY_BUFFER, numpontos*sizeof(float), vertexB, GL_STATIC_DRAW);//carregar os vertices do vertexb para o 
	glGenBuffers(2,&(modelo->norm));
	glBindBuffer(GL_ARRAY_BUFFER, modelo->norm);
	glBufferData(GL_ARRAY_BUFFER,numpontos*sizeof(float) ,normalB,GL_STATIC_DRAW);
	
	free(vertexB);
	free(normalB);
}

//Desenhar a VBO do modelo

void modeloCarregar(Modelo* modelo) {
	glBindBuffer(GL_ARRAY_BUFFER, modelo->vbo);//activar o vbo
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER,modelo->norm);
	glNormalPointer(GL_FLOAT,0,0);
	glDrawArrays(GL_TRIANGLES, 0, (modelo->nvertices)/2);
}

void modeloCarregar2(Modelo* modelo) {
	glBindBuffer(GL_ARRAY_BUFFER, modelo->vbo);//activar o vbo
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, modelo->norm);
	glNormalPointer(GL_FLOAT, 0, 0);
	float white[4] = { 1,0,0,0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glDrawArrays(GL_TRIANGLES, 0, (modelo->nvertices) / 2);
}

//Funçoes Catmull

void getCatmullRomPoint(float t, int *indices, float *res, vector<Ponto*> p1) {
	float res_aux[4];
	int i;
	// catmull-rom matrix
	float m[4][4] = { { -0.5f,  1.5f, -1.5f,  0.5f },
	{ 1.0f, -2.5f,  2.0f, -0.5f },
	{ -0.5f,  0.0f,  0.5f,  0.0f },
	{ 0.0f,  1.0f,  0.0f,  0.0f } };

	for (i = 0; i<4; i++)
		res_aux[i] = pow(t, 3) * m[0][i] + pow(t, 2) * m[1][i] + t * m[2][i] + m[3][i];
	//Calculo do RES
	for (i = 0; i<3; i++) {
		res[i] = res_aux[0] * p1[indices[0]]->pontos[i] + res_aux[1] * p1[indices[1]]->pontos[i] + res_aux[2] * p1[indices[2]]->pontos[i] + res_aux[3] * p1[indices[3]]->pontos[i];
	}
}

void getGlobalCatmullRomPoint(Grupo* g, float gt, float *res) {

	float t = gt * g->npontos; // this is the real global t
	int index = floor(t);  // which segment
	t = t - index; // where within  the segment

				   // indices store the points
	int indices[4];
	indices[0] = (index + g->npontos - 1) % g->npontos;	indices[1] = (indices[0] + 1) % g->npontos;
	indices[2] = (indices[1] + 1) % g->npontos; indices[3] = (indices[2] + 1) % g->npontos;

	getCatmullRomPoint(t, indices, res, g->pontos);
}

void renderCatmullRomCurve(Grupo* g) {
	float  gtt;
	float res[3];
	// desenhar a curva usando segmentos de reta - GL_LINE_LOOP

	glBegin(GL_LINE_LOOP);
	for (gtt = 0; gtt<1; gtt += 0.01) {
		getGlobalCatmullRomPoint(g, gtt, res);
		glVertex3fv(res);
	}
	glEnd();
}

//Num grupo, desenhar os seus modelos, com as suas orbitas, rotaçoes e translaçoes

void desenharfilhos(Grupo* g) {
	size_t i, k;
	glPushMatrix();
	for (i = 0; i<g->modelo.size(); i++) {
		float res[3];
		if (g->tempoT != 0) {
			renderCatmullRomCurve(g);
			getGlobalCatmullRomPoint(g, (g->t), res);
			glTranslatef(res[0], res[1], res[2]);
		}
		glPushMatrix();
		if (g->tempoR != 0) {
			glRotatef(360 * (currentTime%g->tempoR) / (g->tempoR), g->xR, g->yR, g->zR);
		}
		glPushMatrix();
		if (g->xS != 1 || g->yS != 1 || g->zS != 1)
			glScalef(g->xS, g->yS, g->zS);
		if ((g->modelo[i]->nome) == "novo.3d") {
			glPushMatrix();
			glRotatef(-90, 1, 0, 0);
			modeloCarregar2(g->modelo[i]);
			glPopMatrix();
		}
		else 
			modeloCarregar(g->modelo[i]);
		
		glPopMatrix();
		glPopMatrix();
		//se o tempo anterior estiver a 0 actualize com o tempo currente
		if (g->tempoA == 0)
			g->tempoA = currentTime;
		else {//actualiza a variavel t de modo que demora o tempo de translacao
			g->t += ((currentTime - (g->tempoA)) / (g->tempoT * 1000));
			g->tempoA = currentTime;
		}
	}
	for (k = 0; k< g->grupo.size(); k++){
		desenharfilhos(g->grupo[k]);
	}
	glPopMatrix();
}

//ler a informaçao do XML e guardar em cada grupo, modelo
Grupo* aux1(TiXmlElement* g2) {
	string elemnome, elemnome1;
	Grupo* g1 = new Grupo();
	int nump = 0;
	const char* ficheiro;
	const char* atributo;
	g1->tempoR = g1->tempoT = g1->xR = g1->yR = g1->zR = g1->angR = 0.0;
	g1->xS = g1->yS = g1->zS = 1.0;
	int i = 0;
	int t = 0, r = 0, s = 0, mo = 0, f = 0;//conta o numero de translaçoes, rotaçoes e escalas 
	for (TiXmlElement* e = g2->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
		elemnome = e->Value();
		if (elemnome == "grupo") {
			g1->grupo.push_back(aux1(e));//caso tenha filho
		}
		else if (elemnome == "translacao") {
			t++;
			if (t > 1)return NULL;//impedir casos invalidos
			atributo = e->Attribute("tempo");
			if (atributo != NULL) g1->tempoT = atof(atributo);
			g1->tempoA = 0.0;
			g1->t = 0.0;
			for (TiXmlElement* e1 = e->FirstChildElement(); e1 != NULL; e1 = e1->NextSiblingElement()) {
				elemnome1 = e1->Value();
				if (elemnome1 == "ponto") {
					Ponto* ponto1 = new Ponto();
					ponto1->pontos[0] = atof(e1->Attribute("x"));
					ponto1->pontos[1] = atof(e1->Attribute("y"));
					ponto1->pontos[2] = atof(e1->Attribute("z"));
					g1->pontos.push_back(ponto1);
					nump++;
				}
				g1->npontos = nump;
			}
		}
		else if (elemnome == "rotacao")
		{
			r++;
			if (r > 1)return NULL;
			atributo = e->Attribute("tempo");
			if (atributo != NULL) g1->tempoR = atof(atributo);
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
			if (s > 1)return NULL;
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
			if (mo > 1)
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






//Organizar grupos e modelos 
void initGrupo(Grupo* g1) {
	size_t i, j;
	for (i = 0; i<g1->modelo.size(); i++) {
		modeloVBO(g1->modelo[i]);
	}
	for (j = 0; j<g1->grupo.size(); j++) {
		initGrupo(g1->grupo[j]);
	}
}

Cena* aux2(TiXmlElement* c3) {
	Cena* c1 = new Cena();
	const char* atri;
	for (TiXmlElement* elem2 = c3->FirstChildElement("grupo"); elem2 != NULL; elem2 = elem2->NextSiblingElement()){
		
				if(aux1(elem2)==NULL)
					return NULL;	
				else
					(c1->filhos).push_back(aux1(elem2));
				}
	for (TiXmlElement* elem2 = c3->FirstChildElement("luzes"); elem2 != NULL; elem2 = elem2->NextSiblingElement()){
		for (TiXmlElement* elem3 = elem2->FirstChildElement("luz"); elem3 != NULL; elem3 = elem3->NextSiblingElement()){
			Luzes* luz = new Luzes();
			atri = elem3->Attribute("tipo");
			if(atri!=NULL) luz->tipo = atri;
			atri = elem3->Attribute("posX");
			if(atri!=NULL) luz->lx = atof(atri);
			atri = elem3->Attribute("posY");
			if(atri!=NULL) luz->ly = atof(atri);
			atri = elem3->Attribute("posZ");
			if(atri!=NULL) luz->lz = atof(atri);
			c1->luzes.push_back(luz);
	}
	}	
	
	return c1;
}

void desenhar2(Cena* c) {
	size_t i;
		for(int vl = 0;vl<(int)c->luzes.size();vl++){

				glEnable(GL_LIGHT0+vl);
				int define;
				if(c->luzes[vl]->tipo == "POINT") define = 1.0f;
				else if(c->luzes[vl]->tipo == "DIRECIONAL") define = 0.0f;

				GLfloat position[4] = {
										c->luzes[vl]->lx,
										c->luzes[vl]->ly,
										c->luzes[vl]->lz,
										define
				};
				glLightfv(GL_LIGHT0+vl, GL_POSITION,position);
				glLightfv(GL_LIGHT0+vl, GL_AMBIENT,amb);
				glLightfv(GL_LIGHT0+vl, GL_DIFFUSE,diff);
				glLightfv(GL_LIGHT0+vl, GL_SPECULAR,specularLight);

		}
	for (i = 0; i<c->filhos.size(); i++) {
		float white[4] = {1,1,1,1};
		glMaterialfv(GL_FRONT,GL_EMISSION, white);
		desenharfilhos(c->filhos[i]);
	}
}

void initCena(Cena* c1) {
	size_t i;
	for (i = 0; i<c1->filhos.size(); i++) {
		initGrupo(c1->filhos[i]);
	}
}


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

void renderScene(void) {
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// set the camera
	glLoadIdentity();
	gluLookAt(raio*sin(cam_h + camh_x)*cos(cam_v + camh_y), raio*sin(cam_v + camh_y), raio*cos(cam_h + camh_x)*cos(cam_v + camh_y),
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	glPolygonMode(GL_FRONT, GL_FILL);
	//End of frame
	if (cena != NULL)
		desenhar2(cena);
	glutSwapBuffers();
}
//comandos
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

	if (!doc.LoadFile("Sistema.xml"))
	{
		cerr << doc.ErrorDesc() << endl;
		cout << "ooo" << endl;
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

	int  id_frontmenu, id_backmenu;
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
	glEnable(GL_CULL_FACE);
	glewInit();
	initCena(cena);
	glEnable(GL_LIGHTING);

	// enter GLUT's main loop
	glutMainLoop();

	return 1;
}
