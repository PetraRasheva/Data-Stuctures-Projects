#include <iostream>
#include <vector>
#include "interface.h"
#include "Application.hpp"
using namespace std;

int main() {

	Application* app = new Application();
	app->start();

	return 0;
}
