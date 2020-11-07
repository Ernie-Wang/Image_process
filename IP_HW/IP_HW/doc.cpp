#include "doc.h"


using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;



// Set the original image of the documentory
void doc::set_org_img(Bitmap^ img) {
	orgImg = img;
}

// Set the last choosen operation
void doc::set_chioce(int choosen) {
	choice = choosen;
}

// Get the original image of the documentory
Bitmap^ doc::get_org_img() {
	return orgImg;
}

// Get the original image of the documentory
int doc::get_chioce() {
	return choice;
}