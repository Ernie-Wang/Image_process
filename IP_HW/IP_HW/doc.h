#pragma once
#ifndef DOC_H
#define DOC_H


using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
public ref class doc
{
private:
	int choice;
	Bitmap^ orgImg;

public:
	doc() {
		choice = 0;
	}
	~doc() {
		//delete orgImg;
	}

	// Set the original image of the documentory
	void set_org_img(Bitmap^);

	// Remove origin image
	void del_org_img();

	// Set the last choosen operation
	void set_chioce(int);

	// Return the original image of the documentory
	Bitmap^ get_org_img();

	// Return the last choosen operation
	int get_chioce();
};

#endif 
