#include "MyForm.h"
#include <string>
#include <math.h>

using namespace IPHW;

void set_dim(int choice, unsigned int* dim) {
	// Need to Initial Image boxes for different function
	switch (choice) {
	case 0:				// RGB Extraction
		dim[0] = 2;				// origin, R, 
		dim[1] = 2;				// G, B, 
		break;
	case 1:				// Gray Scale
		dim[0] = 1;
		dim[1] = 2;				// origin, result
		break;
	case 2:				// Mean Filter
		dim[0] = 1;
		dim[1] = 2;				// origin, result
		break;
	case 3:				// Median Filter
		dim[0] = 1;
		dim[1] = 2;				// origin, result
		break;
	case 4:				// Histogram Equalization
		dim[0] = 1;
		dim[1] = 3;				// origin, result, Histogram
		break;
	case 5:				// Defined Threshold
		dim[0] = 1;
		dim[1] = 2;				// origin, result
		break;
	case 6:				// V-Sobel Edge
		dim[0] = 1;
		dim[1] = 2;				// origin, result
		break;
	case 7:				// H-Sobel Edge
		dim[0] = 1;
		dim[1] = 2;				// origin, result
		break;
	case 8:				// C-Sobel Edge
		dim[0] = 1;
		dim[1] = 2;				// origin, result
		break;
	case 9:				// Overlap to Origin
		dim[0] = 1;
		dim[1] = 3;				// origin, result, cover
		break;
	case 10:			// Image Registration
		dim[0] = 1;
		dim[1] = 3;				// target, origin, result
		break;

	default:			// Not an option
		break;
	}
}

int mean_f[3][3] = { {1, 1, 1}, {1, 1, 1}, {1, 1, 1} };
int v_sob_f[3][3] = { {1, 0, -1}, {2, 0, -2}, {1, 0, -1} };
int h_sob_f[3][3] = { {-1, -2, -1}, {0, 0, 0}, {1, 2, 1} };

//		OPERATIONS START

System::Void MyForm::RGB(System::Object^  sender, System::EventArgs^  e) {   // RGB Extraction

	Bitmap^ R_image = gcnew Bitmap(orgImg->Width, orgImg->Height);
	Bitmap^ G_image = gcnew Bitmap(orgImg->Width, orgImg->Height);
	Bitmap^ B_image = gcnew Bitmap(orgImg->Width, orgImg->Height);
	for (int i = 0; i < orgImg->Height; i++) {
		for (int j = 0; j < orgImg->Width; j++)
		{
			Color RGB = orgImg->GetPixel(j, i);

			// Get RGB Extraction
			R_image->SetPixel(j, i, Color::FromArgb(RGB.R, RGB.R, RGB.R));
			G_image->SetPixel(j, i, Color::FromArgb(RGB.G, RGB.G, RGB.G));
			B_image->SetPixel(j, i, Color::FromArgb(RGB.B, RGB.B, RGB.B));
		}
	}
	pictureBox[1]->Image = R_image;
	pictureBox[2]->Image = G_image;
	pictureBox[3]->Image = B_image;
}

System::Void MyForm::Gray(System::Object^  sender, System::EventArgs^  e) {			// Gray Scale
	Bitmap^ gray_image = gcnew Bitmap(orgImg->Width, orgImg->Height);
	for (int i = 0; i < orgImg->Height; i++) {
		for (int j = 0; j < orgImg->Width; j++)
		{
			Color RGB = orgImg->GetPixel(j, i);

			// Change to gray scale using lightness method
			int c_max = RGB.R;
			int c_min = RGB.G;
			if (RGB.G > RGB.R) {
				c_max = RGB.G;
				c_min = RGB.R;
			}
			else if (RGB.B > c_max)
				c_max = RGB.B;
			else if (c_min > RGB.B)
				c_min = RGB.B;

			int gray_l = (c_max + c_min) / 2;

			gray_image->SetPixel(j, i, Color::FromArgb(gray_l, gray_l, gray_l));
		}
	}
	pictureBox[1]->Image = gray_image;
}

int conv_kernel(Bitmap^ img, int c_x, int c_y, int filter[3][3], int size) {
	int result = 0;

	for (int i = (c_x - (int)size / 2); i <= (c_x + (int)size / 2); i++) {
		for (int j = (c_y - (int)size / 2); j <= (c_y + (int)size / 2); j++) {
			int w_id = j;
			if (j < 0 || j >= img->Width)
				w_id = c_y;
			int h_id = i;
			if (i < 0 || i >= img->Height)
				h_id = c_x;
			int tmmp = filter[c_x + size - ((int)size / 2) - i - 1][c_y + size - ((int)size / 2) - j - 1];
			//int tmmp = filter[i - (c_x - (int)size / 2)][j - (c_y - (int)size / 2)];
			result += tmmp * ((Color^)img->GetPixel(w_id, h_id))->R;
		}
	}
	return result;
}

System::Void MyForm::Mean_F(System::Object^  sender, System::EventArgs^  e) {
	Bitmap^ f_image = gcnew Bitmap(orgImg->Width, orgImg->Height);
	for (int i = 0; i < orgImg->Height; i++) {
		for (int j = 0; j < orgImg->Width; j++) {
			int value = conv_kernel(orgImg, j, i, mean_f, KERNEL_SIZE);
			value = value / KERNEL_SIZE / KERNEL_SIZE;
			f_image->SetPixel(j, i, Color::FromArgb(value, value, value));
		}
	}
	pictureBox[1]->Image = f_image;
}

int median(int* arr, int len) {
	int ans = -1;
	for (int i = 0; i < len; i++) {
		for (int j = (i+1); j < len; j++) {
			if (arr[i] > arr[j]) {
				int tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
		}
	}
	if (len % 2 == 1) {
		int a = arr[((int)len / 2)];
		for (int i = 0; i < len; i++)
		{
			int b = arr[i];
			b = 0;
		}
		return a;
	}
	else {
		return (arr[((int)len / 2 - 1)] + arr[(int)len / 2]) / 2;
	}
}

System::Void MyForm::Median_F(System::Object^  sender, System::EventArgs^  e) {
	Bitmap^ f_image = gcnew Bitmap(orgImg->Width, orgImg->Height);
	for (int c_x = 0; c_x < orgImg->Height; c_x++) {
		for (int c_y = 0; c_y < orgImg->Width; c_y++) {
			int l_val[KERNEL_SIZE*KERNEL_SIZE] = {};
			int count = 0;
			for (int i = (c_x - (int)KERNEL_SIZE / 2); i <= (c_x + (int)KERNEL_SIZE / 2); i++) {
				for (int j = (c_y - (int)KERNEL_SIZE / 2); j <= (c_y + (int)KERNEL_SIZE / 2); j++) {
					int w_id = j;
					if (j < 0 || j >= orgImg->Width)
						w_id = c_y;
					int h_id = i;
					if (i < 0 || i >= orgImg->Height)
						h_id = c_x;
					l_val[count] = ((Color^)orgImg->GetPixel(w_id, h_id))->R;
					count++;
				}
			}
			int value = median(l_val, KERNEL_SIZE*KERNEL_SIZE);
			f_image->SetPixel(c_y, c_x, Color::FromArgb(value, value, value));
		}
	}
	pictureBox[1]->Image = f_image;
}

System::Void MyForm::Histo(System::Object^  sender, System::EventArgs^  e) {								// Get histogram
}

System::Void MyForm::Histo_Eq(System::Object^  sender, System::EventArgs^  e) {								// Calculate histogram equalization
}

System::Void MyForm::Thres_F(System::Object^  sender, System::EventArgs^  e) {
	Bitmap^ f_image = gcnew Bitmap(orgImg->Width, orgImg->Height);
	int thres = (Int16)threshold->Value;
	for (int i = 0; i < orgImg->Height; i++) {
		for (int j = 0; j < orgImg->Width; j++) {
			int value = ((Color^)orgImg->GetPixel(j, i))->R;
			if (value >= thres)
				value = 255;
			else
				value = 0;
			f_image->SetPixel(j, i, Color::FromArgb(value, value, value));
		}
	}
	pictureBox[1]->Image = f_image;
}

System::Void MyForm::V_Sob(System::Object^  sender, System::EventArgs^  e) {								// Vertical Sobel Filter
	Bitmap^ f_image = gcnew Bitmap(orgImg->Width, orgImg->Height);
	for (int i = 0; i < orgImg->Height; i++) {
		for (int j = 0; j < orgImg->Width; j++) {
			int value = conv_kernel(orgImg, i, j, v_sob_f, KERNEL_SIZE);
			if (value < 0)
				value = 0;
			if (value > 255)
				value = 255;
			f_image->SetPixel(j, i, Color::FromArgb(value, value, value));
		}
	}
	pictureBox[1]->Image = f_image;
}

System::Void MyForm::H_Sob(System::Object^  sender, System::EventArgs^  e) {								// Herizontal Sobel Filter
	Bitmap^ f_image = gcnew Bitmap(orgImg->Width, orgImg->Height);
	for (int i = 0; i < orgImg->Height; i++) {
		for (int j = 0; j < orgImg->Width; j++) {
			int value = conv_kernel(orgImg, i, j, h_sob_f, KERNEL_SIZE);
			if (value < 0)
				value = 0;
			if (value > 255)
				value = 255;
			f_image->SetPixel(j, i, Color::FromArgb(value, value, value));
		}
	}
	pictureBox[1]->Image = f_image;
}

System::Void MyForm::C_Sob(System::Object^  sender, System::EventArgs^  e) {								// Combined Filter
	Bitmap^ f_image = gcnew Bitmap(orgImg->Width, orgImg->Height);
	for (int i = 0; i < orgImg->Height; i++) {
		for (int j = 0; j < orgImg->Width; j++) {
			int v_value = conv_kernel(orgImg, i, j, v_sob_f, KERNEL_SIZE);
			int h_value = conv_kernel(orgImg, i, j, h_sob_f, KERNEL_SIZE);
			int value = sqrt(pow(v_value, 2) + pow(h_value, 2));
			if (value < 0)
				value = 0;
			if (value > 255)
				value = 255;
			f_image->SetPixel(j, i, Color::FromArgb(value, value, value));
		}
	}
	pictureBox[1]->Image = f_image;
}

System::Void MyForm::ovelap(System::Object^  sender, System::EventArgs^  e) {								// Overlap original image with combine sobel result
}
System::Void MyForm::regist(System::Object^  sender, System::EventArgs^  e) {								// Registration of two image
}
//		OPERATIONS END

System::Void MyForm::operation(System::Object^  sender, System::EventArgs^  e, int choice) {
	// Decide the correct operation
	switch (choice) {
	case 0:				// RGB Extraction
		RGB(sender, e);
		break;
	case 1:				// Gray Scale
		Gray(sender, e);
		break;
	case 2:				// Mean Filter
		Mean_F(sender, e);
		break;
	case 3:				// Median Filter
		Median_F(sender, e);
		break;
	case 4:				// Histogram Equalization

		break;
	case 5:				// Defined Threshold
		Thres_F(sender, e);
		break;
	case 6:				// V-Sobel Edge
		V_Sob(sender, e);
		break;
	case 7:				// H-Sobel Edge
		H_Sob(sender, e);
		break;
	case 8:				// C-Sobel Edge
		C_Sob(sender, e);
		break;
	case 9:				// Overlap to Origin

		break;
	case 10:			// Image Registration

		break;

	default:			// Not an option
		break;
	}
}

System::Void MyForm::resetInterface(System::Object^  sender, System::EventArgs^  e, int choice) {			// Reset Interface
	unsigned int dim[] = { 1, 1 };		//Dimention of the picture boxs in results, [rows, cols]

	set_dim(choice, dim);				// Set the box diameter to correct ones

	int total_box = dim[0] * dim[1];

	// Delete all newed pointers
	if (pictureBox != nullptr) {
		for (int i = pictureBox->Length - 1; i >= 0; i--) {
			if (this->resultBox->Controls->Contains(this->pictureBox[i])) {
				this->resultBox->Controls->Remove(this->pictureBox[i]);
				delete this->pictureBox[i];
			}
		}
	}
	if (picBoxLab != nullptr) {
		for (int i = picBoxLab->Length - 1; i >= 0; i--) {
			if (this->resultBox->Controls->Contains(this->picBoxLab[i])) {
				this->resultBox->Controls->Remove(this->picBoxLab[i]);
				delete this->picBoxLab[i];
			}
		}
	}
	if (imgArr != nullptr) {
		for (int i = imgArr->Length - 1; i > 0; i--) {
			delete this->imgArr[i];
		}
	}

	// Clear all the pointer array
	delete[] pictureBox;
	delete[] picBoxLab;
	delete[] imgArr;

	// Reconstruct pointer array
	this->pictureBox = gcnew array < System::Windows::Forms::PictureBox^>(total_box);
	this->picBoxLab = gcnew array < System::Windows::Forms::Label^>(total_box);
	this->imgArr = gcnew array < Bitmap^>(total_box);
	this->imgArr[0] = orgImg;

	// Calculate every picture box size
	pic_box_size[0] = (this->resultBox->Size.Height - (3 * dim[0] + 3) * BOX_MARGIN) / dim[0];
	pic_box_size[1] = (this->resultBox->Size.Width - (dim[1] + 1) * BOX_MARGIN) / dim[1];

	int p_index = 0;
	for (int i = 0; i < dim[0]; i++) {
		for (int j = 0; j < dim[1]; j++) {

			std::string tmp3 = std::to_string(p_index);
			System::String^ tmp4 = gcnew String(tmp3.c_str());
			if (p_index == 0)
				tmp4 = L"Origin";

			// Initial picture box and label
			this->pictureBox[p_index] = gcnew System::Windows::Forms::PictureBox();
			this->picBoxLab[p_index] = gcnew System::Windows::Forms::Label();

			// Set picture box variables
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox[p_index]))->BeginInit();
			//this->pictureBox[p_index]->BackColor = System::Drawing::SystemColors::ActiveCaption;			// FOR DEBUG
			this->pictureBox[p_index]->Location = System::Drawing::Point(1 * BOX_MARGIN + j * (BOX_MARGIN + pic_box_size[1]), (i + 1) * (3 * BOX_MARGIN + pic_box_size[0]) - pic_box_size[0]);
			this->pictureBox[p_index]->Name = L"pictureBox";
			this->pictureBox[p_index]->Size = System::Drawing::Size(pic_box_size[1], pic_box_size[0]);
			this->pictureBox[p_index]->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pictureBox[p_index]->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pictureBox[p_index]->TabIndex = 0;
			this->pictureBox[p_index]->TabStop = false;
			this->resultBox->Controls->Add(this->pictureBox[p_index]);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox[p_index]))->EndInit();

			// Set picture label variables
			this->picBoxLab[p_index]->AutoSize = false;
			this->picBoxLab[p_index]->Font = (gcnew System::Drawing::Font(L"Arial", 18, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->picBoxLab[p_index]->TextAlign = ContentAlignment::MiddleCenter;
			this->picBoxLab[p_index]->Location = System::Drawing::Point(1 * BOX_MARGIN + j * (BOX_MARGIN + pic_box_size[1]) + 0.5 *pic_box_size[1] - 60, (i + 1) * (3 * BOX_MARGIN + +pic_box_size[0]));
			this->picBoxLab[p_index]->Name = L"label";
			this->picBoxLab[p_index]->Size = System::Drawing::Size(120, 27);
			this->picBoxLab[p_index]->TabIndex = 10;
			this->picBoxLab[p_index]->Text = tmp4;
			this->resultBox->Controls->Add(this->picBoxLab[p_index]);

			p_index++;
		}
	}

	this->resultBox->ResumeLayout(false);
	this->ResumeLayout(false);
	if (imgArr[0] != nullptr)
		pictureBox[0]->Image = orgImg;		// Give first picture box the original image

}

System::Void MyForm::comboBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
	int choice = comboBox1->SelectedIndex;
	resetInterface(sender, e, choice);						// Reset Interface

	// Print out combo box status
	std::string tmp = std::to_string(comboBox1->SelectedIndex);
	System::String^ tmp2 = gcnew String(tmp.c_str());
	MessageBox::Show("Change to " + tmp2 + ": " + comboBox1->Text, "Change");

	// Specialties
	if(choice == 5)
		threshold->Value = 127;

	if (orgImg != nullptr) {
		operation(sender, e, choice);						// Select the operation
		if(docs[docs.Count - 1]->get_chioce() != choice || docs[docs.Count - 1]->get_org_img() != orgImg)
			addHistory(sender, e);
	}
}

System::Void MyForm::button1_Click(System::Object^  sender, System::EventArgs^  e) { 
	if (comboBox1->SelectedIndex < 0)
		MessageBox::Show("Choose a operation first!!", "Error");
	else if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {	// Successfully open files
		orgImg = gcnew Bitmap(openFileDialog1->FileName);									// Store as origin image
		this->imgArr[0] = orgImg;															
		pictureBox[0]->Image = this->imgArr[0];												// Print out the image
		operation(sender, e, comboBox1->SelectedIndex);										// Select the operation
		addHistory(sender, e);
	}
}

System::Void MyForm::button2_Click(System::Object^  sender, System::EventArgs^  e) {
	if (comboBox2->SelectedIndex < 0)
		MessageBox::Show("Choose a image first!!", "Error");
	else {
		SaveFileDialog ^ saveFileDialog1 = gcnew SaveFileDialog();
		saveFileDialog1->Filter = "JPeg Image|*.jpg|Bitmap Image|*.bmp|Gif Image|*.gif";
		saveFileDialog1->Title = "Save an Image File";

		if (saveFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			String^ sfd = saveFileDialog1->FileName;
			pictureBox[0]->Image->Save(sfd, System::Drawing::Imaging::ImageFormat::Jpeg);
		}
	}
}

System::Void MyForm::Undo_Click(System::Object^  sender, System::EventArgs^  e) {
	if(docs.Count >= 2) {
		doc ^tmp = docs[docs.Count - 2];
		delete docs[docs.Count - 1];
		docs.RemoveAt(docs.Count - 1);
		
		orgImg = tmp->get_org_img();
		comboBox1->SelectedIndex = tmp->get_chioce();
		resetInterface(sender, e, tmp->get_chioce());						// Reset Interface
		operation(sender, e, tmp->get_chioce());						// Select the operation
	}
}

System::Void MyForm::NumericUpDown1_ValueChanged(Object^ sender, EventArgs^ e) {
	if (comboBox1->SelectedIndex == 5 && orgImg != nullptr)
		operation(sender, e, comboBox1->SelectedIndex);
}

System::Void MyForm::addHistory(System::Object^  sender, System::EventArgs^  e) {
	if (docs.Count >= Q_SIZE) {
		docs[0]->del_org_img();
		delete docs[0];
		docs.RemoveAt(0);
	}
	// Print out the image
	doc ^tmp = gcnew doc();
	tmp->set_chioce(comboBox1->SelectedIndex);
	tmp->set_org_img(orgImg);
	docs.Add(tmp);
}

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	// Create the main window and run it
	Application::Run(gcnew MyForm());
	return 0;
}
