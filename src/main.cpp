#include "system.h"
#include <iostream.h>

extern int userMain(int argc, char* argv[]);

int main(int argc, char* argv[]) {

	cout << "Pokrecem jezgro!" << endl;
	System::load();

	int value = userMain(argc, argv);

	System::unload();
	return value;
}
