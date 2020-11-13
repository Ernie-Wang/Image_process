#include "MyForm.h"
#include <string>
#include <math.h>

using namespace IPHW;


int mean_f[3][3] = { {1, 1, 1}, {1, 1, 1}, {1, 1, 1} };
int v_sob_f[3][3] = { {1, 0, -1}, {2, 0, -2}, {1, 0, -1} };
int h_sob_f[3][3] = { {-1, -2, -1}, {0, 0, 0}, {1, 2, 1} };
int upper_bound_1 = 1330, lower_bound_1 = -1330, diff_1 = 2660;
int upper_bound_2 = 1881, lower_bound_2 = -1881, diff_2 = 3762;

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
		dim[0] = 2; 
		dim[1] = 2;				// origin, result, Histogram
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
		dim[0] = 2;
		dim[1] = 2;				// target, origin, result
		break;

	default:			// Not an option
		break;
	}
}

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
	imgArr[1] = R_image;
	imgArr[2] = G_image;
	imgArr[3] = B_image;
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
	imgArr[1] = gray_image;
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
			int value = conv_kernel(orgImg, i, j, mean_f, KERNEL_SIZE);
			value = value / KERNEL_SIZE / KERNEL_SIZE;
			f_image->SetPixel(j, i, Color::FromArgb(value, value, value));
		}
	}
	imgArr[1] = f_image;
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
	imgArr[1] = f_image;
}

System::Void MyForm::Histo(System::Object^  sender, System::EventArgs^  e) {								// Get histogram
	int histo[256] = {};

	for (int i = 0; i < orgImg->Height; i++) {
		for (int j = 0; j < orgImg->Width; j++) {
			histo[((Color^)orgImg->GetPixel(j, i))->R]++;
		}
	}

	Bitmap^ histo_i = gcnew Bitmap(pic_box_size[1], pic_box_size[0]);
	DataVisualization::Charting::Chart^  h_chart = gcnew DataVisualization::Charting::Chart();
	DataVisualization::Charting::Series^ series1 = gcnew DataVisualization::Charting::Series("histogram");
	DataVisualization::Charting::ChartArea^ area = h_chart->ChartAreas->Add("area");

	for (int i = 0; i < 256; i++) {
		series1->Points->AddXY(i, histo[i]);
	}
	series1->ChartType = DataVisualization::Charting::SeriesChartType::Column;
	series1->Color = Color::Blue;
	series1->ChartArea = area->Name;
	h_chart->Series->Add(series1);
	h_chart->Size = System::Drawing::Size(pic_box_size[1], pic_box_size[0]);
	h_chart->ChartAreas[0]->AxisX->Minimum = 0;
	h_chart->ChartAreas[0]->AxisX->Maximum = 255;
	h_chart->ChartAreas[0]->AxisX->Minimum = 0;
	h_chart->DrawToBitmap(histo_i, h_chart->Bounds);

	imgArr[2] = histo_i;
	delete series1;
	delete area;
	delete h_chart;

	int sum = 0, h_max = -100, h_min = 10000;
	for (int i = 0; i < 256; i++)
	{
		sum += histo[i];
		if (i != 0)
			histo[i] += histo[i - 1];
		if (h_max < histo[i])
			h_max = histo[i];
		if (histo[i] != 0 && h_min > histo[i])
			h_min = histo[i];
	}


	int h_diff = h_max - h_min;
	int histo_e[256] = {};

	Bitmap^ histo_eq_i = gcnew Bitmap(orgImg->Width, orgImg->Height);
	for (int i = 0; i < orgImg->Height; i++) {
		for (int j = 0; j < orgImg->Width; j++) {
			int value = 255 * (histo[((Color^)orgImg->GetPixel(j, i))->R] - h_min) / h_diff;
			value = round(value);

			histo_eq_i->SetPixel(j, i, Color::FromArgb(value, value, value));
			histo_e[value]++;
		}
	}

	imgArr[1] = histo_eq_i;
	Bitmap^ histo_e_i = gcnew Bitmap(pic_box_size[1], pic_box_size[0]);
	h_chart = gcnew DataVisualization::Charting::Chart();
	series1 = gcnew DataVisualization::Charting::Series("histogram");
	area = h_chart->ChartAreas->Add("area");

	for (int i = 0; i < 256; i++) {
		series1->Points->AddXY(i, histo_e[i]);
	}
	series1->ChartType = DataVisualization::Charting::SeriesChartType::Column;
	series1->Color = Color::Blue;
	series1->ChartArea = area->Name;
	h_chart->Series->Add(series1);
	h_chart->Size = System::Drawing::Size(pic_box_size[1], pic_box_size[0]);
	h_chart->ChartAreas[0]->AxisX->Minimum = 0;
	h_chart->ChartAreas[0]->AxisX->Maximum = 255;
	h_chart->ChartAreas[0]->AxisX->Minimum = 0;
	h_chart->DrawToBitmap(histo_e_i, h_chart->Bounds);

	imgArr[3] = histo_e_i;
	delete series1;
	delete area;
	delete h_chart;

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
	imgArr[1] = f_image;
}

System::Void MyForm::V_Sob(System::Object^  sender, System::EventArgs^  e) {								// Vertical Sobel Filter
	Bitmap^ f_image = gcnew Bitmap(orgImg->Width, orgImg->Height);
	for (int i = 0; i < orgImg->Height; i++) {
		for (int j = 0; j < orgImg->Width; j++) {
			int value = conv_kernel(orgImg, i, j, v_sob_f, KERNEL_SIZE);
			value = abs(value);
			if (value < 0)
				value = 0;
			if (value > 255)
				value = 255;
			f_image->SetPixel(j, i, Color::FromArgb(value, value, value));
		}
	}
	/*value = round((value - lower_bound_1) * 255 / diff_1);*/
	imgArr[1] = f_image;
}

System::Void MyForm::H_Sob(System::Object^  sender, System::EventArgs^  e) {								// Herizontal Sobel Filter
	Bitmap^ f_image = gcnew Bitmap(orgImg->Width, orgImg->Height);
	for (int i = 0; i < orgImg->Height; i++) {
		for (int j = 0; j < orgImg->Width; j++) {
			int value = conv_kernel(orgImg, i, j, h_sob_f, KERNEL_SIZE);
			value = abs(value);
			if (value < 0)
				value = 0;
			if (value > 255)
				value = 255;
			f_image->SetPixel(j, i, Color::FromArgb(value, value, value));
		}
	}
	imgArr[1] = f_image;
}

System::Void MyForm::C_Sob(System::Object^  sender, System::EventArgs^  e) {								// Combined Filter
	Bitmap^ f_image = gcnew Bitmap(orgImg->Width, orgImg->Height);
	for (int i = 0; i < orgImg->Height; i++) {
		for (int j = 0; j < orgImg->Width; j++) {
			int v_value = conv_kernel(orgImg, i, j, v_sob_f, KERNEL_SIZE);
			int h_value = conv_kernel(orgImg, i, j, h_sob_f, KERNEL_SIZE);
			int value = sqrt(pow(v_value, 2) + pow(h_value, 2));

			//value = round((value - lower_bound_2) * 255 / diff_2);
			if (value < 0)
				value = 0;
			if (value > 255)
				value = 255;
			f_image->SetPixel(j, i, Color::FromArgb(value, value, value));
		}
	} 
	imgArr[1] = f_image;
}

System::Void MyForm::ovelap(System::Object^  sender, System::EventArgs^  e) {								// Overlap original image with combine sobel result
	Bitmap^ f_image = gcnew Bitmap(orgImg->Width, orgImg->Height);
	Bitmap^ mask = gcnew Bitmap(orgImg->Width, orgImg->Height);
	int thres = (Int16)threshold->Value;
	for (int i = 0; i < orgImg->Height; i++) {
		for (int j = 0; j < orgImg->Width; j++) {
			Color RGB = orgImg->GetPixel(j, i);
			int v_value = conv_kernel(orgImg, i, j, v_sob_f, KERNEL_SIZE);
			int h_value = conv_kernel(orgImg, i, j, h_sob_f, KERNEL_SIZE);
			int value = sqrt(pow(v_value, 2) + pow(h_value, 2));

			if (value >= thres) {
				value = 255;
				f_image->SetPixel(j, i, Color::FromArgb(0, value, 0));
				mask->SetPixel(j, i, Color::FromArgb(value, value, value));
			}
			else {
				f_image->SetPixel(j, i, Color::FromArgb(RGB.R, RGB.G, RGB.B));
				mask->SetPixel(j, i, Color::FromArgb(0, 0, 0));
			}
		}
	}
	imgArr[1] = mask;
	imgArr[2] = f_image;
}

void transfer(int *before, int *after) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			after[i] += trans_m[j][i] * before[j];
		}
	}
}

System::Void MyForm::regist(System::Object^  sender, System::EventArgs^  e) {								// Registration of two image
	// Calculate transformation matrix, Find the matrix transform the need-to-transform image back to origin image
	// Pseudo matrix
	float p_m_des[3][3] = {};				// pseudo matrix for transformed matrix
	for (int i = 0; i < 4; i++) {
		p_m_des[0][0] += reg_pt[1][i][0] * reg_pt[1][i][0]; 
		p_m_des[0][1] += reg_pt[1][i][0] * reg_pt[1][i][1];
		p_m_des[0][2] += reg_pt[1][i][0];
		p_m_des[1][1] += reg_pt[1][i][1] * reg_pt[1][i][1];
		p_m_des[1][2] += reg_pt[1][i][1];
	}
	p_m_des[1][0] = p_m_des[0][1];
	p_m_des[2][0] = p_m_des[0][2];
	p_m_des[2][1] = p_m_des[1][2];
	p_m_des[2][2] = 4;


	float a_t_b[3][3] = {};				// pseudo matrix for transformed matrix
	for (int i = 0; i < 4; i++) {
		a_t_b[0][0] += reg_pt[1][i][0] * reg_pt[0][i][0];
		a_t_b[0][1] += reg_pt[1][i][0] * reg_pt[0][i][1];
		a_t_b[0][2] += reg_pt[1][i][0];
		a_t_b[1][0] += reg_pt[0][i][0] * reg_pt[1][i][1];
		a_t_b[1][1] += reg_pt[0][i][1] * reg_pt[1][i][1];
		a_t_b[1][2] += reg_pt[1][i][1];
		a_t_b[2][0] += reg_pt[0][i][0];
		a_t_b[2][1] += reg_pt[0][i][1];
	}
	a_t_b[2][2] = 4;

	float inv_mat[3][3] = {};
	float det_a = 0;
	det_a += (p_m_des[0][0] * p_m_des[1][1] * p_m_des[2][2] + p_m_des[0][1] * p_m_des[1][2] * p_m_des[2][0] + p_m_des[0][2] * p_m_des[1][0] * p_m_des[2][1]);
	det_a -= (p_m_des[0][2] * p_m_des[1][1] * p_m_des[2][0] + p_m_des[0][0] * p_m_des[1][2] * p_m_des[2][1] + p_m_des[0][1] * p_m_des[1][0] * p_m_des[2][2]);
	inv_mat[0][0] = p_m_des[1][1] * p_m_des[2][2] - p_m_des[2][1] * p_m_des[1][2];
	inv_mat[0][1] = -(p_m_des[0][1] * p_m_des[2][2] - p_m_des[0][2] * p_m_des[2][1]);
	inv_mat[0][2] = p_m_des[0][1] * p_m_des[1][2] - p_m_des[1][1] * p_m_des[0][2];
	inv_mat[1][0] = -(p_m_des[1][0] * p_m_des[2][2] - p_m_des[1][2] * p_m_des[2][0]);
	inv_mat[1][1] = p_m_des[0][0] * p_m_des[2][2] - p_m_des[2][0] * p_m_des[0][2];
	inv_mat[1][2] = -(p_m_des[0][0] * p_m_des[1][2] - p_m_des[1][0] * p_m_des[0][2]);
	inv_mat[2][0] = p_m_des[1][0] * p_m_des[2][1] - p_m_des[1][1] * p_m_des[2][0];
	inv_mat[2][1] = -(p_m_des[0][0] * p_m_des[2][1] - p_m_des[2][0] * p_m_des[0][1]);
	inv_mat[2][2] = p_m_des[1][1] * p_m_des[0][0] - p_m_des[0][1] * p_m_des[1][0];

	for (int i = 0; i < 3; i++)	{
		for (int j = 0; j < 3; j++) {
			trans_m[i][j] = 0;
		}
	}

	for (int i = 0; i < 3; i++)	{
		trans_m[0][0] += inv_mat[0][i] * a_t_b[i][0] / det_a;
		trans_m[0][1] += inv_mat[0][i] * a_t_b[i][1] / det_a;
		trans_m[0][2] += inv_mat[0][i] * a_t_b[i][2] / det_a;
		trans_m[1][0] += inv_mat[1][i] * a_t_b[i][0] / det_a;
		trans_m[1][1] += inv_mat[1][i] * a_t_b[i][1] / det_a;
		trans_m[1][2] += inv_mat[1][i] * a_t_b[i][2] / det_a;
		trans_m[2][0] += inv_mat[2][i] * a_t_b[i][0] / det_a;
		trans_m[2][1] += inv_mat[2][i] * a_t_b[i][1] / det_a;
		trans_m[2][2] += inv_mat[2][i] * a_t_b[i][2] / det_a;
	}

	// Extract variables
	// Use average to calculate theta
	theta = 0;
	theta += atan(-trans_m[1][0] / trans_m[0][0]);
	theta += atan(trans_m[0][1] / trans_m[1][1]);
	theta /= 2;
	float theta_op[] = { sin(theta), cos(theta) };		// [0] -> sine theta, [1] -> cosine theta
	theta = theta * 180 / 3.14159;						// theta * 360 / 2 / 3.14159

	shear[0] = (theta_op[0] * trans_m[0][0] + theta_op[1] * trans_m[1][0]) / (theta_op[1] * trans_m[0][0] - theta_op[0] * trans_m[1][0]);
	shear[1] = (theta_op[1] * trans_m[0][1] - theta_op[0] * trans_m[1][1]) / (theta_op[1] * trans_m[1][1] + theta_op[0] * trans_m[0][1]);

	scale[0] = trans_m[0][0] / (theta_op[1] + shear[0] * theta_op[0]);
	scale[1] = trans_m[0][1] / (theta_op[0] + shear[1] * theta_op[1]);

	trans[0] = trans_m[2][0];
	trans[1] = trans_m[2][1];

	// Print out result image, after the tansformation matrix is calculated.  pictureBox
	//Bitmap^ result_img = gcnew Bitmap(pictureBox[0]->Image);
	Bitmap^ result_img = gcnew Bitmap(pictureBox[0]->Image->Width,pictureBox[0]->Image->Height);
	for (int i = 0; i < pictureBox[1]->Image->Height; i++) {
		for (int j = 0; j < pictureBox[1]->Image->Width; j++) {
			int org_vec[] = { j, i, 1 };
			int cal_vec[] = { 0, 0, 0 };		// [0] -> width, [1] -> height, [2] -> 1
			transfer(org_vec, cal_vec);			// Calculate the translated image using transformation matrix
			if (cal_vec[0] >= 0 && cal_vec[1] >= 0 && cal_vec[0] < pictureBox[0]->Image->Width && cal_vec[1] < pictureBox[0]->Image->Height) {
				//Color orgRGB = result_img->GetPixel(cal_vec[0], cal_vec[1]);
				Color transRGB = ((Bitmap^)pictureBox[1]->Image)->GetPixel(j, i);
				//int result_R = round((orgRGB.R + transRGB.R *alpha) / 2);
				//int result_G = round((orgRGB.G + transRGB.G *alpha) / 2);
				//int result_B = round((orgRGB.B + transRGB.B *alpha) / 2);
				int result_R = round(transRGB.R *alpha);
				int result_G = round(transRGB.G *alpha);
				int result_B = round(transRGB.B *alpha);
				result_img->SetPixel(cal_vec[0], cal_vec[1], Color::FromArgb(result_R, result_G, result_B));
			}
		}
	}
	imgArr[2] = result_img;

	// Print out the matrix, angle, scale, sheer, etc.
	Color BackColor = Color::Transparent;
	String^ FontName = gcnew String("Times New Roman");
	int FontSize = 16;

	Bitmap^ bitmap = gcnew Bitmap(pictureBox[3]->Size.Width, pictureBox[3]->Size.Height);
	Graphics^ graphics = System::Drawing::Graphics::FromImage(bitmap);
	Color color = Color::Transparent;
	System::Drawing::Font^ font = gcnew System::Drawing::Font(FontName, FontSize);

	SolidBrush^ BrushBackColor = gcnew SolidBrush(BackColor);
	Pen^ BorderPen = gcnew Pen(color);

	Rectangle displayRectangle = Rectangle( System::Drawing::Point(0, 0),  System::Drawing::Size(Width - 1, Height - 1));

	graphics->FillRectangle(BrushBackColor, displayRectangle);
	graphics->DrawRectangle(BorderPen, displayRectangle);

	std::string tmp = "| ";

	// Print transformation matrix
	tmp = tmp + std::to_string(trans_m[0][0]) + ", ";
	tmp = tmp + std::to_string(trans_m[0][1]) + ", ";
	tmp = tmp + std::to_string(trans_m[0][2]) + "|\r\n| ";
	tmp = tmp + std::to_string(trans_m[1][0]) + ", ";
	tmp = tmp + std::to_string(trans_m[1][1]) + ", ";
	tmp = tmp + std::to_string(trans_m[1][2]) + "|\r\n| ";
	tmp = tmp + std::to_string(trans_m[2][0]) + ", ";
	tmp = tmp + std::to_string(trans_m[2][1]) + ", ";
	tmp = tmp + std::to_string(trans_m[2][2]) + "|\r\n";
	tmp = tmp + "\r\n";

	// Print Rotation
	tmp = tmp + "Rotation: " + std::to_string(theta) + "\r\n\r\n";
	// Print Scale
	tmp = tmp + "Scale x: " + std::to_string(scale[0]) + "    Scale y: " + std::to_string(scale[1]) + "\r\n\r\n";
	// Print Translation
	tmp = tmp + "Trans x: " + std::to_string(trans[0]) + "    Trans y: " + std::to_string(trans[1]) + "\r\n\r\n";
	// Print Shear
	tmp = tmp + "Shear v: " + std::to_string(shear[0]) + "    Shear h: " + std::to_string(shear[1]) + "\r\n\r\n";

	
	//sprintf(tmp, "| %f,\t%f,\t%f |\r\n| %f,\t%f,\t%f |\r\n| %f,\t%f,\t%f |", trans_m[0][0], trans_m[0][1], trans_m[0][2], trans_m[1][0], trans_m[1][1], trans_m[1][2], trans_m[2][0], trans_m[2][1], trans_m[2][2]);
	System::String^ tmp2 = gcnew String(tmp.c_str());
	graphics->DrawString(tmp2, font, Brushes::Black, 0, 0);
	pictureBox[3]->Image = bitmap;
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
		Histo(sender, e);
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
		ovelap(sender, e);
		break;
	case 10:			// Image Registration
		if (rec_pt == 8) {
			regist(sender, e);
			MessageBox::Show("Operation done");
		}
		break;

	default:			// Not an option
		break;
	}

	for (int i = 0; i < pictureBox->Length; i++) {
		if (imgArr[i] != nullptr)
			pictureBox[i]->Image = imgArr[i];
	}
}

System::Void MyForm::comboBox_Coord(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
	if (rec_pt == 8) {
		rec_pt = 0;
		MessageBox::Show("Re-select all reference points");
	}
	else if (comboBox1->SelectedIndex == 10) {
		int boxw = ((PictureBox^)sender)->Size.Width;
		int boxh = ((PictureBox^)sender)->Size.Height;
		int imgw = ((PictureBox^)sender)->Image->Size.Width;
		//int img_x_off = ((PictureBox^)sender)->Image->;
		int imgh = ((PictureBox^)sender)->Image->Size.Height;
		int newx = e->X - boxw / 2 + imgw / 2;
		int newy = e->Y - boxh / 2 + imgh / 2;
		reg_pt[(int)rec_pt / 4][(int)rec_pt % 4][0] = newx;
		reg_pt[(int)rec_pt / 4][(int)rec_pt % 4][1] = newy;

		//std::string tmp = std::to_string(e->X);
		//System::String^ tmp2 = gcnew String(tmp.c_str());
		//std::string tmp3 = std::to_string(e->Y);
		//System::String^ tmp4 = gcnew String(tmp3.c_str());
		// MessageBox::Show("x: " + tmp2 + ", y: " + tmp4);

		//std::string tmp = std::to_string(reg_pt[(int)rec_pt / 4][(int)rec_pt % 4][0]);
		//System::String^ tmp2 = gcnew String(tmp.c_str());
		//std::string tmp3 = std::to_string(reg_pt[(int)rec_pt / 4][(int)rec_pt % 4][1]);
		//System::String^ tmp4 = gcnew String(tmp3.c_str());
		//MessageBox::Show("x: " + tmp2 + ", y: " + tmp4, "Point " + rec_pt);
		rec_pt++;
	}

	if (rec_pt == 8 )
		operation(sender, e, comboBox1->SelectedIndex);						// Select the operation
	
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
			if(i != comboBox2->SelectedIndex)
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
			this->pictureBox[p_index]->SizeMode = System::Windows::Forms::PictureBoxSizeMode::CenterImage;
			this->pictureBox[p_index]->TabIndex = 0;
			this->pictureBox[p_index]->TabStop = false;
			this->pictureBox[p_index]->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::comboBox_Coord);
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
		pictureBox[0]->Image = imgArr[0];			// Give first picture box the original image

}

System::Void MyForm::comboBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
	int choice = comboBox1->SelectedIndex;
	resetInterface(sender, e, choice);						// Reset Interface

	// Print out combo box status
	//std::string tmp = std::to_string(comboBox1->SelectedIndex);
	//System::String^ tmp2 = gcnew String(tmp.c_str());
	//MessageBox::Show("Change to " + tmp2 + ": " + comboBox1->Text, "Change");

	// Specialties
	if(choice == 5 || choice == 9)
		threshold->Value = 127;
	else if (choice == 10)
		rec_pt = 0;

	if (orgImg != nullptr) {
		imgArr[0] = orgImg;
		operation(sender, e, choice);						// Select the operation
		if(docs[docs.Count - 1]->get_chioce() != choice || docs[docs.Count - 1]->get_org_img() != orgImg)
			addHistory(sender, e);
	}
}

System::Void MyForm::comboBox2_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
	int choice = comboBox2->SelectedIndex;
	orgImg = imgArr[choice];
}

System::Void MyForm::button1_Click(System::Object^  sender, System::EventArgs^  e) { 
	if (comboBox1->SelectedIndex < 0)
		MessageBox::Show("Choose a operation first!!", "Error");
	else if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {	// Successfully open files
		orgImg = gcnew Bitmap(openFileDialog1->FileName);									// Store as origin image
 		if (comboBox1->SelectedIndex == 10 && this->imgArr[0] != nullptr) {
			if (this->imgArr[1] == nullptr) {
				this->imgArr[1] = orgImg;
				pictureBox[1]->Image = orgImg;
				orgImg = imgArr[0];
				operation(sender, e, comboBox1->SelectedIndex);									// Select the operation
			}
			else {
				this->imgArr[0] = orgImg;
				pictureBox[0]->Image = orgImg;
				delete this->imgArr[1];
				imgArr[1] = nullptr;
				addHistory(sender, e);
			}
		}
		else {
			this->imgArr[0] = orgImg;
			pictureBox[0]->Image = orgImg;													// Print out the image
			operation(sender, e, comboBox1->SelectedIndex);									// Select the operation
			addHistory(sender, e);
		}
	}
}

System::Void MyForm::button2_Click(System::Object^  sender, System::EventArgs^  e) {
	if (comboBox2->SelectedIndex < 0)
		MessageBox::Show("Choose a image first!!", "Error");
	else {
		SaveFileDialog ^ saveFileDialog1 = gcnew SaveFileDialog();
		saveFileDialog1->Filter = "Png Image|*.png|JPeg Image|*.jpg|Bitmap Image|*.bmp|Gif Image|*.gif";
		saveFileDialog1->Title = "Save an Image File";

		if (saveFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			String^ sfd = saveFileDialog1->FileName;
			pictureBox[comboBox2->SelectedIndex]->Image->Save(sfd, System::Drawing::Imaging::ImageFormat::Png);
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
	if ((comboBox1->SelectedIndex == 5 || comboBox1->SelectedIndex == 9) && orgImg != nullptr)
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
