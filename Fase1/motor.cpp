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

vector <string> vet;
vector <string>::iterator it;
float alt_cam = 5, alt = 2, translate_x = 0, rotate = 0;
float raio = 20, cam_h = 0, cam_v = 0.5, camh_x = 0, camh_y = 0, h = 6.0;

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

void desenha() {
	double a, b, c;
	for (it = vet.begin(); it != vet.end(); ++it) {
		if (*it == "cone.3d") {
			glBegin(GL_TRIANGLES);
			ifstream input_file("cone.3d");
			while (input_file >> a >> b >> c) {
				glColor3f(0.0f, 1.0f, 1.0f);
				glVertex3f(a, b, c);

			}
			glEnd();
		}
		else if (*it == "caixa.3d") {
			glBegin(GL_TRIANGLES);
			ifstream input_file("caixa.3d");
			while (input_file >> a >> b >> c) {
				glColor3f(1.0f,0.0f, 0.0f);
				glVertex3f(a, b, c);

			}
			glEnd();
		}
		else if (*it == "plano.3d") {
			glBegin(GL_TRIANGLES);
			ifstream input_file("plano.3d");
			while (input_file >> a >> b >> c) {
				glColor3f(0.0f, 1.0f, 0.0f);
				glVertex3f(a, b, c);

			}
			glEnd();
		}
		else if (*it == "esfera.3d") {
			glBegin(GL_TRIANGLES);
			ifstream input_file("esfera.3d");
			while (input_file >> a >> b >> c) {
				glColor3f(0.0f, 0.0f, 1.0f);
				glVertex3f(a, b, c);

			}
			glEnd();
		}
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
	desenha();
	glPolygonMode(GL_FRONT, GL_LINE);
	//End of frame
	glutSwapBuffers();
}

void teclado_normal(unsigned char tecla, int x, int y) {
	switch (tecla) {
	case 'a':
		raio -= 1;
		break;
	case 'd':
		raio += 1;
		break;
	case 'w':
		h += 1;
		break;
	case 's':
		h -= 1;
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

	if (!doc.LoadFile("figura.xml")) // seleciona o ficheior xml
	{
		cerr << doc.ErrorDesc() << endl;
		return EXIT_FAILURE;
	}


	TiXmlElement* cena = doc.FirstChildElement();
	if (cena == NULL) // se documento nao tem nada
	{
		cerr << "Failed to load file: No root element." << endl;
		doc.Clear();
		return EXIT_FAILURE; // retorna o erro
	}

	const char* ficheiro;

	for (TiXmlElement* elem = cena->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
	{
		string elemName = elem->Value();

		if (elemName == "modelo")
		{
			ficheiro = elem->Attribute("ficheiro"); // procura o nome do ficheiro e adiciona ao vetor
			if (ficheiro != NULL) {
				vet.push_back(ficheiro);
			}
		}
	}



	// put init here

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("CG@DI");
	// put callback registration here
	glutDisplayFunc(renderScene);
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
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// enter GLUT's main loop
	glutMainLoop();

	return 1;
}
