#include "MyForm.h"
#include <string>

using namespace IPHW;

System::Void MyForm::comboBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
	int choice = comboBox1->SelectedIndex;
	unsigned int dim[] = { 1, 1 };		//Dimention of the picture boxs in results, [rows, cols]

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
			this->pictureBox[p_index]->Location = System::Drawing::Point(1 * BOX_MARGIN + j * (BOX_MARGIN + pic_box_size[1]), (i+1) * (3*BOX_MARGIN + pic_box_size[0]) - pic_box_size[0]);
			this->pictureBox[p_index]->Name = L"pictureBox";
			this->pictureBox[p_index]->Size = System::Drawing::Size(pic_box_size[1], pic_box_size[0]);
			this->pictureBox[p_index]->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pictureBox[p_index]->TabIndex = 0;
			this->pictureBox[p_index]->TabStop = false;
			this->resultBox->Controls->Add(this->pictureBox[p_index]);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox[p_index]))->EndInit();

			// Set picture label variables
			this->picBoxLab[p_index]->AutoSize = false;
			this->picBoxLab[p_index]->Font = (gcnew System::Drawing::Font(L"Arial", 18, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->picBoxLab[p_index]->TextAlign = ContentAlignment::MiddleCenter;
			this->picBoxLab[p_index]->Location = System::Drawing::Point(1 * BOX_MARGIN + j * (BOX_MARGIN + pic_box_size[1]) + 0.5 *pic_box_size[1] - 60, (i + 1) * (3 * BOX_MARGIN + +pic_box_size[0])  );
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

	// Print out combo box status
	std::string tmp = std::to_string(comboBox1->SelectedIndex);
	System::String^ tmp2 = gcnew String(tmp.c_str());
	MessageBox::Show("Change to " + tmp2 + ": " + comboBox1->Text, "Change");

}

System::Void MyForm::button1_Click(System::Object^  sender, System::EventArgs^  e) {

	std::string tmp = std::to_string(comboBox1->SelectedIndex);
	System::String^ tmp2 = gcnew String(tmp.c_str());
	MessageBox::Show("Change to " + tmp2 + ": " + comboBox1->Text, "Change");
	if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) { // 如果成功開檔
		orgImg = gcnew Bitmap(openFileDialog1->FileName);
		this->imgArr[0] = orgImg;
		Bitmap^ temp = gcnew Bitmap(this->imgArr[0]->Width, this->imgArr[0]->Height);
		//宣告暫存的 Bitmap
		pictureBox[0]->Image = this->imgArr[0];
		for (int i = 0; i < temp->Height; i++)
			for (int j = 0; j < temp->Width; j++) {
				Color RGB = this->imgArr[0]->GetPixel(j, i);
				//讀取影像平面上 (j,i) 的 RGB 資訊
				//這邊要注意因為縱軸座標是 i 橫軸座標是 j 所以要寫成 (j,i) ;
				int invR = Convert::ToInt32(255 - RGB.R);
				int invG = Convert::ToInt32(255 - RGB.G);
				int invB = Convert::ToInt32(255 - RGB.B);
				//RGB 是VS內建的class 可以直接讀取影像的色彩資訊 R = Red G = Green B =Blue

				temp->SetPixel(j, i, Color::FromArgb(invR, invG, invB));
			}
		this->imgArr[1] = temp;
		pictureBox[1]->Image = temp;
		pictureBox[2]->Image = this->imgArr[1];
	}
} //button

System::Void MyForm::button2_Click(System::Object^  sender, System::EventArgs^  e) {

	SaveFileDialog ^ saveFileDialog1 = gcnew SaveFileDialog();
	saveFileDialog1->Filter = "JPeg Image|*.jpg|Bitmap Image|*.bmp|Gif Image|*.gif";
	saveFileDialog1->Title = "Save an Image File";

	if (saveFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
		String^ sfd = saveFileDialog1->FileName;
		//pictureBox2->Image->Save(sfd, System::Drawing::Imaging::ImageFormat::Jpeg);
	} //button
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
