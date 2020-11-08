#pragma once

#include <stdio.h>
#include "doc.h"

const int BOX_MARGIN = 10;
const int Q_SIZE = 20;
const int KERNEL_SIZE = 3;
int pic_box_size[2];											// [row, col], [height, width]

namespace IPHW {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Collections::Generic;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// MyForm 的摘要
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO:  在此加入建構函式程式碼
			//
		}

	protected:
		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ComboBox^  comboBox1;

	private: System::Windows::Forms::Button^  button1;



	private: System::Windows::Forms::SaveFileDialog^  saveFileDialog1;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  undo_b;

	private: System::Windows::Forms::ComboBox^  comboBox2;
	private: System::Windows::Forms::NumericUpDown^  threshold;

	private: System::Windows::Forms::GroupBox^  crtlBox;



	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::GroupBox^  resultBox;


	private: array < System::Windows::Forms::PictureBox^ >^  pictureBox;		// Used to store picturebox array
	private: array < System::Windows::Forms::Label^ >^  picBoxLab;				// Used to store picBoxLab array
	private: array < Bitmap^ >^  imgArr;										// Used to store picBoxLab array, 0 for origin
	private: Bitmap^ orgImg;													// Used to store origin image
	private: System::Collections::Generic::List< doc^ >  docs;					// Used to store picBoxLab array, 0 for origin





	private: System::ComponentModel::IContainer^  components;
	protected:

	private:
		/// <summary>
		/// 設計工具所需的變數。
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改
		/// 這個方法的內容。
		/// </summary>
		void InitializeComponent(void)
		{
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->undo_b = (gcnew System::Windows::Forms::Button());
			this->comboBox2 = (gcnew System::Windows::Forms::ComboBox());
			this->threshold = (gcnew System::Windows::Forms::NumericUpDown());
			this->crtlBox = (gcnew System::Windows::Forms::GroupBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->resultBox = (gcnew System::Windows::Forms::GroupBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->threshold))->BeginInit();
			this->crtlBox->SuspendLayout();
			this->SuspendLayout();
			// 
			// comboBox1
			// 
			this->comboBox1->AccessibleName = L"";
			this->comboBox1->Font = (gcnew System::Drawing::Font(L"Arial", 20.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Items->AddRange(gcnew cli::array< System::Object^  >(11) {
				L"RGB Extraction", L"Gray Scale", L"Mean Filter",
					L"Median Filter", L"Histogram Equalization", L"Defined Threshold", L"V-Sobel Edge", L"H-Sobel Edge", L"C-Sobel Edge", L"Overlap to Origin",
					L"Image Registration"
			});
			this->comboBox1->Location = System::Drawing::Point(17, 57);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(213, 40);
			this->comboBox1->TabIndex = 0;
			this->comboBox1->Text = L"Operation";
			this->comboBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::comboBox1_SelectedIndexChanged);
			// 
			// button1
			// 
			this->button1->Font = (gcnew System::Drawing::Font(L"Arial", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button1->Location = System::Drawing::Point(17, 115);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(103, 34);
			this->button1->TabIndex = 0;
			this->button1->Text = L"Load Image";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			this->openFileDialog1->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &MyForm::openFileDialog1_FileOk);
			// 
			// button2
			// 
			this->button2->Font = (gcnew System::Drawing::Font(L"Arial", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button2->Location = System::Drawing::Point(126, 115);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(104, 34);
			this->button2->TabIndex = 4;
			this->button2->Text = L"Save Image";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
			// 
			// undo_b
			// 
			this->undo_b->Font = (gcnew System::Drawing::Font(L"Arial", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->undo_b->Location = System::Drawing::Point(17, 288);
			this->undo_b->Name = L"undo_b";
			this->undo_b->Size = System::Drawing::Size(64, 39);
			this->undo_b->TabIndex = 5;
			this->undo_b->Text = L"Undo";
			this->undo_b->UseVisualStyleBackColor = true;
			this->undo_b->Click += gcnew System::EventHandler(this, &MyForm::Undo_Click);
			// 
			// comboBox2
			// 
			this->comboBox2->Font = (gcnew System::Drawing::Font(L"Arial", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->comboBox2->FormattingEnabled = true;
			this->comboBox2->Items->AddRange(gcnew cli::array< System::Object^  >(7) {
				L"Origin Image", L"1", L"2", L"3", L"4", L"5",
					L"6"
			});
			this->comboBox2->Location = System::Drawing::Point(17, 232);
			this->comboBox2->Name = L"comboBox2";
			this->comboBox2->Size = System::Drawing::Size(148, 32);
			this->comboBox2->TabIndex = 6;
			this->comboBox2->Text = L"Hold Image";
			this->comboBox2->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::comboBox2_SelectedIndexChanged);
			// 
			// threshold
			// 
			this->threshold->Font = (gcnew System::Drawing::Font(L"Arial", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->threshold->Location = System::Drawing::Point(145, 166);
			this->threshold->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 255, 0, 0, 0 });
			this->threshold->Name = L"threshold";
			this->threshold->Size = System::Drawing::Size(69, 32);
			this->threshold->TabIndex = 7;
			this->threshold->Tag = L"";
			this->threshold->ValueChanged += gcnew System::EventHandler(this, &MyForm::NumericUpDown1_ValueChanged);
			// 
			// crtlBox
			// 
			this->crtlBox->BackColor = System::Drawing::SystemColors::Control;
			this->crtlBox->Controls->Add(this->label1);
			this->crtlBox->Controls->Add(this->comboBox1);
			this->crtlBox->Controls->Add(this->undo_b);
			this->crtlBox->Controls->Add(this->comboBox2);
			this->crtlBox->Controls->Add(this->button2);
			this->crtlBox->Controls->Add(this->threshold);
			this->crtlBox->Controls->Add(this->button1);
			this->crtlBox->Font = (gcnew System::Drawing::Font(L"Arial", 20.25F, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Italic)),
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->crtlBox->ForeColor = System::Drawing::SystemColors::ControlText;
			this->crtlBox->Location = System::Drawing::Point(12, 12);
			this->crtlBox->Name = L"crtlBox";
			this->crtlBox->Size = System::Drawing::Size(249, 837);
			this->crtlBox->TabIndex = 9;
			this->crtlBox->TabStop = false;
			this->crtlBox->Text = L"Control Panel";
			this->crtlBox->Enter += gcnew System::EventHandler(this, &MyForm::groupBox1_Enter);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Arial", 18, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(19, 171);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(120, 27);
			this->label1->TabIndex = 10;
			this->label1->Text = L"Threshold";
			this->label1->Click += gcnew System::EventHandler(this, &MyForm::label1_Click);
			// 
			// resultBox
			// 
			this->resultBox->Font = (gcnew System::Drawing::Font(L"Arial", 20.25F, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Italic)),
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->resultBox->Location = System::Drawing::Point(267, 12);
			this->resultBox->Name = L"resultBox";
			this->resultBox->Size = System::Drawing::Size(1192, 837);
			this->resultBox->TabIndex = 9;
			this->resultBox->TabStop = false;
			this->resultBox->Text = L" Result";
			// 
			// MyForm
			// 
			this->AccessibleDescription = L"";
			this->AccessibleRole = System::Windows::Forms::AccessibleRole::MenuBar;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoScroll = true;
			this->ClientSize = System::Drawing::Size(1484, 861);
			this->Controls->Add(this->resultBox);
			this->Controls->Add(this->crtlBox);
			this->Location = System::Drawing::Point(34, 36);
			this->Name = L"MyForm";
			this->Tag = L"";
			this->Text = L"Display";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->threshold))->EndInit();
			this->crtlBox->ResumeLayout(false);
			this->crtlBox->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e);

	private: System::Void comboBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void openFileDialog1_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e) {}
	private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e) {}
	private: System::Void Undo_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void comboBox2_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {}
	private: System::Void textBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) {}
	private: System::Void groupBox1_Enter(System::Object^  sender, System::EventArgs^  e) {}
	private: System::Void label1_Click(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void NumericUpDown1_ValueChanged(Object^ sender, EventArgs^ e);

	private: System::Void addHistory(System::Object^  sender, System::EventArgs^  e);
	private: System::Void resetInterface(System::Object^  sender, System::EventArgs^  e, int choice);			// Reset Interface
	private: System::Void operation(System::Object^  sender, System::EventArgs^  e, int choice);				// Do Operation

	// Operations
	private: System::Void RGB(System::Object^  sender, System::EventArgs^  e);									// RGB Extraction
	private: System::Void Gray(System::Object^  sender, System::EventArgs^  e);									// Gray Extraction
	private: System::Void Mean_F(System::Object^  sender, System::EventArgs^  e);								// Mean Filter
	private: System::Void Median_F(System::Object^  sender, System::EventArgs^  e);								// Median Filter
	private: System::Void Thres_F(System::Object^  sender, System::EventArgs^  e);								// Threshold Filter
	private: System::Void Histo(System::Object^  sender, System::EventArgs^  e);								// Get histogram
	private: System::Void Histo_Eq(System::Object^  sender, System::EventArgs^  e);								// Calculate histogram equalization
	private: System::Void V_Sob(System::Object^  sender, System::EventArgs^  e);								// Vertical Sobel Filter
	private: System::Void H_Sob(System::Object^  sender, System::EventArgs^  e);								// Herizontal Sobel Filter
	private: System::Void C_Sob(System::Object^  sender, System::EventArgs^  e);								// Combined Filter
	private: System::Void ovelap(System::Object^  sender, System::EventArgs^  e);								// Overlap original image with combine sobel result
	private: System::Void regist(System::Object^  sender, System::EventArgs^  e);								// Registration of two image
}; //public

}