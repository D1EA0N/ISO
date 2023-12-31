#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>
#include <Windows.h>
#include "Line.h"
#include "Graph.h"


namespace ISO {

	using namespace System;
	using namespace Lines;
	using namespace Graphs;
	using namespace System::ComponentModel;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Drawing::Drawing2D;
	using namespace System::Collections::Generic;

	/// <summary>
	/// Summary for mainform
	/// </summary>
	public ref class mainform : public System::Windows::Forms::Form
	{
	private:
		//Varaible declaration
		//Point mouse_location, Line-class variables
		Line^ previewLine;
		Line^ line;
		//lists
		Dictionary<int, int>^ dotDegrees = gcnew Dictionary<int, int>();
		Dictionary<int, int>^ gen1Degrees = gcnew Dictionary<int, int>();
		Dictionary<int, int>^ gen2Degrees = gcnew Dictionary<int, int>();
		List<Line^>^ lines = gcnew List<Line^>();
		List<Point>^ graph1Dots = gcnew List<Point>();  // Dots for graph 1
		List<Point>^ graph2Dots = gcnew List<Point>();  // Dots for graph 2
		List<Line^>^ graph1Lines = gcnew List<Line^>();  // Lines for graph 1
		List<Line^>^ graph2Lines = gcnew List<Line^>();  // Lines for graph 2
		List<Line^>^ linesList = gcnew List<Line^>();
		Random^ random = gcnew Random();  // Random number generator
		//Points
		Point startPoint = Point();
		Point endPoint = Point();
		Point controlPointloops1;
		Point controlPointloops2;
		Point Pointloop1;
		Point Pointloop2;
		// Calculate the control points to create the letter "C" shaped curve
		int controlPointOffsetY = 15; 
		int controlPointOffsetX = 15; 
		int midX = (startPoint.X + endPoint.X) / 2;
		int midY = (startPoint.Y + endPoint.Y) / 2;
		Point controlPoint1 = Point(midX + controlPointOffsetX + 200, midY + controlPointOffsetY);
		Point controlPoint2 = Point(midX + controlPointOffsetX + 200, midY + controlPointOffsetY);
		Point controlPoints1 = Point(midX + controlPointOffsetX + 200, midY + controlPointOffsetY);
		Point controlPoints2 = Point(midX + controlPointOffsetX + 200, midY + controlPointOffsetY);
		//variables
		float angle;
		bool isDrawing = false;
		bool showGridLines = false;
		bool showDot = true;
		bool showline = true;
		bool completedot = false;
		bool completeline = false;
		bool clickedOnDot = false; // Flag to check if a dot was clicked
		bool ctrlKeyPressed = false;
		bool zKeyPressed = false;
		bool BackKeyPressed = false;
		bool temp = true;
		int vertcount;
		int edgecount;
		int clicknum = 1;
		int dotSize = 24;
		int dotnum;
		int DrawMode = 2;
		int startDotIndex;
		int endDotIndex;
		int numLinesDrawn = 0;
		String^ edgeLabel;
		Graph^ graph = gcnew Graph(dotnum);
		//Stack
		Stack<Point>^ validDots = gcnew Stack<Point>();
		Stack<Point>^ dotHistory = gcnew Stack<Point>();
		Stack<Point>^ lineHistory = gcnew Stack<Point>();
		//Modifications
		Color dotFillColor = Color::DarkSlateGray;
		Color dotOutlineColor = Color::Cyan;
		Drawing::Font^ letterFont = gcnew System::Drawing::Font("Comic Sans MS", 8);
		//Dot drawing variables
		ref class PointAndIndex
		{
		public:
			Point position;
			int index;

			PointAndIndex(Point pos, int idx)
			{
				position = pos;
				index = idx;
			}
		};
		List<PointAndIndex^>^ dots = gcnew List<PointAndIndex^>();
		//Undo variables
		ref class DrawObject {
		public:
			int Type;  // 0 for dot, 1 for line
		};
		List<DrawObject^>^ objects = gcnew List<DrawObject^>();
		DrawObject^ dotundo = gcnew DrawObject();
	private: System::Windows::Forms::RichTextBox^ txtdegree2;
	private: System::Windows::Forms::RichTextBox^ txtdegree1;
		   DrawObject^ lineundo = gcnew DrawObject();
		// Helper function to calculate the distance between two points
		double Distance(Point p1, Point p2) {
			int dx = p2.X - p1.X;
			int dy = p2.Y - p1.Y;
			return Math::Sqrt(dx * dx + dy * dy);
		}
		// Helper function to check for overlap with existing dots
		bool HasOverlap(List<Point>^ dots, Point newDot) {
			int dotSpacing = 16;
			for each (Point dot in dots) {
				if (Distance(dot, newDot) < dotSpacing) {
					return true;
				}
			}
			return false;
		}
		//Degree counter for drawing, graph 1, graph 2
		void UpdateDotDegrees(int dotIndex) {
			if (dotDegrees->ContainsKey(dotIndex)) {
				dotDegrees[dotIndex]++;
			}
			else {
				dotDegrees->Add(dotIndex, 1);
			}
		}
		void Gen1DotDegrees(int dotIndex) {
			if (gen1Degrees->ContainsKey(dotIndex)) {
				gen1Degrees[dotIndex]++;
			}
			else {
				gen1Degrees->Add(dotIndex, 1);
			}
		}
		void Gen2DotDegrees(int dotIndex) {
			if (gen2Degrees->ContainsKey(dotIndex)) {
				gen2Degrees[dotIndex]++;
			}
			else {
				gen2Degrees->Add(dotIndex, 1);
			}
		}
		//------------------------------------- 
	private: System::Windows::Forms::Panel^ panel3;
	private: System::Windows::Forms::Panel^ panel5;
	private: System::Windows::Forms::RichTextBox^ txtdegree;
	private: System::Windows::Forms::ErrorProvider^ errorValidator;
	private: System::Windows::Forms::ToolTip^ toolTip1;
	private: System::Windows::Forms::Button^ undobtn;

	private: System::Windows::Forms::CheckBox^ CBgrid;

	private: System::Windows::Forms::Button^ generatebtn;

	private: System::Windows::Forms::Timer^ timer_move;

	private: System::Windows::Forms::Panel^ panel2;
	private: System::Windows::Forms::Panel^ panelSidemenu;

	private: System::Windows::Forms::Button^ Clearbtn;
	private: System::Windows::Forms::PictureBox^ isognrt2;
	private: System::Windows::Forms::PictureBox^ isognrt1;
	private: System::Windows::Forms::Button^ addvertexbtn;
	private: System::Windows::Forms::Button^ connectbtn;
	private: System::Windows::Forms::Label^ instructlbl;
	private: System::Windows::Forms::Panel^ panel1;
	private: System::Windows::Forms::Button^ adbtb;






	public:
		mainform(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~mainform()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::TextBox^ Verticestb;
	private: System::Windows::Forms::TextBox^ Edgetb;
	private: System::Windows::Forms::Button^ enterbtn;
	private: System::Windows::Forms::PictureBox^ PBdraw;




	private: System::ComponentModel::IContainer^ components;
	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(mainform::typeid));
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->Verticestb = (gcnew System::Windows::Forms::TextBox());
			this->Edgetb = (gcnew System::Windows::Forms::TextBox());
			this->enterbtn = (gcnew System::Windows::Forms::Button());
			this->PBdraw = (gcnew System::Windows::Forms::PictureBox());
			this->toolTip1 = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->undobtn = (gcnew System::Windows::Forms::Button());
			this->Clearbtn = (gcnew System::Windows::Forms::Button());
			this->addvertexbtn = (gcnew System::Windows::Forms::Button());
			this->connectbtn = (gcnew System::Windows::Forms::Button());
			this->CBgrid = (gcnew System::Windows::Forms::CheckBox());
			this->generatebtn = (gcnew System::Windows::Forms::Button());
			this->timer_move = (gcnew System::Windows::Forms::Timer(this->components));
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->panelSidemenu = (gcnew System::Windows::Forms::Panel());
			this->adbtb = (gcnew System::Windows::Forms::Button());
			this->isognrt2 = (gcnew System::Windows::Forms::PictureBox());
			this->isognrt1 = (gcnew System::Windows::Forms::PictureBox());
			this->instructlbl = (gcnew System::Windows::Forms::Label());
			this->errorValidator = (gcnew System::Windows::Forms::ErrorProvider(this->components));
			this->panel3 = (gcnew System::Windows::Forms::Panel());
			this->panel5 = (gcnew System::Windows::Forms::Panel());
			this->txtdegree = (gcnew System::Windows::Forms::RichTextBox());
			this->txtdegree1 = (gcnew System::Windows::Forms::RichTextBox());
			this->txtdegree2 = (gcnew System::Windows::Forms::RichTextBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->PBdraw))->BeginInit();
			this->panel2->SuspendLayout();
			this->panel1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->isognrt2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->isognrt1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->errorValidator))->BeginInit();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->BackColor = System::Drawing::Color::Transparent;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Uighur", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label1->ForeColor = System::Drawing::SystemColors::ActiveCaptionText;
			this->label1->Location = System::Drawing::Point(56, 10);
			this->label1->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(58, 28);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Vertices";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->BackColor = System::Drawing::Color::Transparent;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Uighur", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label2->ForeColor = System::Drawing::SystemColors::ActiveCaptionText;
			this->label2->Location = System::Drawing::Point(69, 50);
			this->label2->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(41, 28);
			this->label2->TabIndex = 0;
			this->label2->Text = L"Edge";
			// 
			// Verticestb
			// 
			this->Verticestb->Location = System::Drawing::Point(118, 13);
			this->Verticestb->Margin = System::Windows::Forms::Padding(4, 3, 4, 3);
			this->Verticestb->Name = L"Verticestb";
			this->Verticestb->Size = System::Drawing::Size(60, 20);
			this->Verticestb->TabIndex = 2;
			this->toolTip1->SetToolTip(this->Verticestb, L"\r\n");
			this->Verticestb->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &mainform::Verticestb_KeyDown);
			this->Verticestb->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &mainform::Verticestb_KeyPress);
			this->Verticestb->MouseEnter += gcnew System::EventHandler(this, &mainform::Verticestb_MouseEnter);
			this->Verticestb->Validating += gcnew System::ComponentModel::CancelEventHandler(this, &mainform::Verticestb_Validating);
			// 
			// Edgetb
			// 
			this->Edgetb->Location = System::Drawing::Point(118, 53);
			this->Edgetb->Margin = System::Windows::Forms::Padding(4, 3, 4, 3);
			this->Edgetb->Name = L"Edgetb";
			this->Edgetb->Size = System::Drawing::Size(60, 20);
			this->Edgetb->TabIndex = 3;
			this->toolTip1->SetToolTip(this->Edgetb, L"\r\n");
			this->Edgetb->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &mainform::Edgestb_KeyDown);
			this->Edgetb->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &mainform::Edgetb_KeyPress);
			this->Edgetb->MouseEnter += gcnew System::EventHandler(this, &mainform::Edgetb_MouseEnter);
			this->Edgetb->Validating += gcnew System::ComponentModel::CancelEventHandler(this, &mainform::Edgetb_Validating);
			// 
			// enterbtn
			// 
			this->enterbtn->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->enterbtn->BackColor = System::Drawing::Color::Transparent;
			this->enterbtn->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"enterbtn.BackgroundImage")));
			this->enterbtn->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->enterbtn->Cursor = System::Windows::Forms::Cursors::Hand;
			this->enterbtn->FlatAppearance->BorderColor = System::Drawing::Color::LightCyan;
			this->enterbtn->FlatAppearance->BorderSize = 0;
			this->enterbtn->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Transparent;
			this->enterbtn->FlatAppearance->MouseOverBackColor = System::Drawing::Color::Transparent;
			this->enterbtn->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->enterbtn->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->enterbtn->Location = System::Drawing::Point(186, 47);
			this->enterbtn->Margin = System::Windows::Forms::Padding(4, 3, 4, 3);
			this->enterbtn->Name = L"enterbtn";
			this->enterbtn->Padding = System::Windows::Forms::Padding(5, 0, 0, 0);
			this->enterbtn->Size = System::Drawing::Size(38, 34);
			this->enterbtn->TabIndex = 1;
			this->enterbtn->TextAlign = System::Drawing::ContentAlignment::BottomCenter;
			this->toolTip1->SetToolTip(this->enterbtn, L"Click to Enter");
			this->enterbtn->UseVisualStyleBackColor = false;
			this->enterbtn->Click += gcnew System::EventHandler(this, &mainform::enterbtn_Click);
			// 
			// PBdraw
			// 
			this->PBdraw->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(51)),
				static_cast<System::Int32>(static_cast<System::Byte>(51)));
			this->PBdraw->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->PBdraw->Cursor = System::Windows::Forms::Cursors::Cross;
			this->PBdraw->Location = System::Drawing::Point(29, 143);
			this->PBdraw->Margin = System::Windows::Forms::Padding(4, 3, 4, 3);
			this->PBdraw->Name = L"PBdraw";
			this->PBdraw->Size = System::Drawing::Size(502, 437);
			this->PBdraw->TabIndex = 0;
			this->PBdraw->TabStop = false;
			this->PBdraw->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &mainform::PBdraw_Paint);
			this->PBdraw->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &mainform::PBdraw_VertexClick);
			this->PBdraw->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &mainform::PBdraw_MouseDown);
			this->PBdraw->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &mainform::PBdraw_MouseMove);
			// 
			// toolTip1
			// 
			this->toolTip1->AutomaticDelay = 0;
			this->toolTip1->AutoPopDelay = 5000;
			this->toolTip1->InitialDelay = 500;
			this->toolTip1->ReshowDelay = 100;
			// 
			// undobtn
			// 
			this->undobtn->BackColor = System::Drawing::Color::Transparent;
			this->undobtn->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"undobtn.BackgroundImage")));
			this->undobtn->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->undobtn->Cursor = System::Windows::Forms::Cursors::Hand;
			this->undobtn->FlatAppearance->BorderColor = System::Drawing::Color::LightCyan;
			this->undobtn->FlatAppearance->BorderSize = 0;
			this->undobtn->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Transparent;
			this->undobtn->FlatAppearance->MouseOverBackColor = System::Drawing::Color::Transparent;
			this->undobtn->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->undobtn->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->undobtn->Location = System::Drawing::Point(232, 47);
			this->undobtn->Margin = System::Windows::Forms::Padding(4, 3, 4, 3);
			this->undobtn->Name = L"undobtn";
			this->undobtn->Size = System::Drawing::Size(38, 34);
			this->undobtn->TabIndex = 5;
			this->undobtn->TextAlign = System::Drawing::ContentAlignment::BottomCenter;
			this->toolTip1->SetToolTip(this->undobtn, L"Click to undo the recent vertex/edge");
			this->undobtn->UseVisualStyleBackColor = false;
			this->undobtn->Click += gcnew System::EventHandler(this, &mainform::undobtn_Click);
			this->undobtn->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &mainform::mainform_KeyDown);
			this->undobtn->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &mainform::mainform_KeyUp);
			// 
			// Clearbtn
			// 
			this->Clearbtn->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->Clearbtn->BackColor = System::Drawing::Color::Transparent;
			this->Clearbtn->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"Clearbtn.BackgroundImage")));
			this->Clearbtn->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->Clearbtn->Cursor = System::Windows::Forms::Cursors::Hand;
			this->Clearbtn->FlatAppearance->BorderColor = System::Drawing::Color::LightCyan;
			this->Clearbtn->FlatAppearance->BorderSize = 0;
			this->Clearbtn->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Transparent;
			this->Clearbtn->FlatAppearance->MouseOverBackColor = System::Drawing::Color::Transparent;
			this->Clearbtn->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->Clearbtn->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Clearbtn->ImageAlign = System::Drawing::ContentAlignment::MiddleLeft;
			this->Clearbtn->Location = System::Drawing::Point(278, 47);
			this->Clearbtn->Margin = System::Windows::Forms::Padding(4, 3, 4, 3);
			this->Clearbtn->Name = L"Clearbtn";
			this->Clearbtn->Padding = System::Windows::Forms::Padding(4, 0, 0, 0);
			this->Clearbtn->Size = System::Drawing::Size(38, 34);
			this->Clearbtn->TabIndex = 5;
			this->Clearbtn->TextAlign = System::Drawing::ContentAlignment::BottomCenter;
			this->Clearbtn->TextImageRelation = System::Windows::Forms::TextImageRelation::TextBeforeImage;
			this->toolTip1->SetToolTip(this->Clearbtn, L"Click to clear the content of the textbox and drawing box");
			this->Clearbtn->UseVisualStyleBackColor = false;
			this->Clearbtn->Click += gcnew System::EventHandler(this, &mainform::Clearbtn_Click);
			// 
			// addvertexbtn
			// 
			this->addvertexbtn->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->addvertexbtn->BackColor = System::Drawing::Color::Transparent;
			this->addvertexbtn->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"addvertexbtn.BackgroundImage")));
			this->addvertexbtn->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->addvertexbtn->Cursor = System::Windows::Forms::Cursors::Hand;
			this->addvertexbtn->FlatAppearance->BorderColor = System::Drawing::Color::LightCyan;
			this->addvertexbtn->FlatAppearance->BorderSize = 0;
			this->addvertexbtn->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Transparent;
			this->addvertexbtn->FlatAppearance->MouseOverBackColor = System::Drawing::Color::LightCyan;
			this->addvertexbtn->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->addvertexbtn->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->addvertexbtn->Location = System::Drawing::Point(370, 47);
			this->addvertexbtn->Margin = System::Windows::Forms::Padding(4, 3, 4, 3);
			this->addvertexbtn->Name = L"addvertexbtn";
			this->addvertexbtn->Padding = System::Windows::Forms::Padding(5, 0, 0, 0);
			this->addvertexbtn->Size = System::Drawing::Size(38, 34);
			this->addvertexbtn->TabIndex = 1;
			this->addvertexbtn->TextAlign = System::Drawing::ContentAlignment::BottomCenter;
			this->toolTip1->SetToolTip(this->addvertexbtn, L"Click here to add vertices");
			this->addvertexbtn->UseVisualStyleBackColor = false;
			this->addvertexbtn->Click += gcnew System::EventHandler(this, &mainform::addvertexbtn_Click);
			this->addvertexbtn->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &mainform::mainform_KeyDown);
			this->addvertexbtn->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &mainform::mainform_KeyUp);
			// 
			// connectbtn
			// 
			this->connectbtn->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->connectbtn->BackColor = System::Drawing::Color::Transparent;
			this->connectbtn->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"connectbtn.BackgroundImage")));
			this->connectbtn->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->connectbtn->Cursor = System::Windows::Forms::Cursors::Hand;
			this->connectbtn->FlatAppearance->BorderColor = System::Drawing::Color::LightCyan;
			this->connectbtn->FlatAppearance->BorderSize = 0;
			this->connectbtn->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Transparent;
			this->connectbtn->FlatAppearance->MouseOverBackColor = System::Drawing::Color::LightCyan;
			this->connectbtn->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->connectbtn->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->connectbtn->Location = System::Drawing::Point(324, 47);
			this->connectbtn->Margin = System::Windows::Forms::Padding(4, 3, 4, 3);
			this->connectbtn->Name = L"connectbtn";
			this->connectbtn->Padding = System::Windows::Forms::Padding(5, 0, 0, 0);
			this->connectbtn->Size = System::Drawing::Size(38, 34);
			this->connectbtn->TabIndex = 1;
			this->connectbtn->TextAlign = System::Drawing::ContentAlignment::BottomCenter;
			this->toolTip1->SetToolTip(this->connectbtn, L"Click here to connect edges");
			this->connectbtn->UseVisualStyleBackColor = false;
			this->connectbtn->Click += gcnew System::EventHandler(this, &mainform::connectbtn_Click);
			this->connectbtn->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &mainform::mainform_KeyDown);
			this->connectbtn->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &mainform::mainform_KeyUp);
			// 
			// CBgrid
			// 
			this->CBgrid->AutoSize = true;
			this->CBgrid->BackColor = System::Drawing::Color::Transparent;
			this->CBgrid->Cursor = System::Windows::Forms::Cursors::Hand;
			this->CBgrid->ForeColor = System::Drawing::SystemColors::ActiveCaptionText;
			this->CBgrid->Location = System::Drawing::Point(29, 120);
			this->CBgrid->Margin = System::Windows::Forms::Padding(4, 3, 4, 3);
			this->CBgrid->Name = L"CBgrid";
			this->CBgrid->Size = System::Drawing::Size(75, 17);
			this->CBgrid->TabIndex = 8;
			this->CBgrid->Text = L"Show Grid";
			this->CBgrid->UseVisualStyleBackColor = false;
			this->CBgrid->CheckedChanged += gcnew System::EventHandler(this, &mainform::CBgrid_CheckedChanged);
			// 
			// generatebtn
			// 
			this->generatebtn->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->generatebtn->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->generatebtn->ForeColor = System::Drawing::SystemColors::ActiveCaptionText;
			this->generatebtn->Location = System::Drawing::Point(1082, 451);
			this->generatebtn->Name = L"generatebtn";
			this->generatebtn->Size = System::Drawing::Size(252, 58);
			this->generatebtn->TabIndex = 10;
			this->generatebtn->Text = L"GENERATE";
			this->generatebtn->UseVisualStyleBackColor = false;
			this->generatebtn->Click += gcnew System::EventHandler(this, &mainform::generatebtn_Click);
			// 
			// timer_move
			// 
			this->timer_move->Enabled = true;
			this->timer_move->Interval = 500;
			// 
			// panel2
			// 
			this->panel2->BackColor = System::Drawing::Color::Black;
			this->panel2->Controls->Add(this->panel1);
			this->panel2->Dock = System::Windows::Forms::DockStyle::Top;
			this->panel2->Location = System::Drawing::Point(0, 0);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(1384, 109);
			this->panel2->TabIndex = 12;
			// 
			// panel1
			// 
			this->panel1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->panel1->BackColor = System::Drawing::Color::SaddleBrown;
			this->panel1->Controls->Add(this->connectbtn);
			this->panel1->Controls->Add(this->addvertexbtn);
			this->panel1->Controls->Add(this->label1);
			this->panel1->Controls->Add(this->Verticestb);
			this->panel1->Controls->Add(this->Clearbtn);
			this->panel1->Controls->Add(this->enterbtn);
			this->panel1->Controls->Add(this->undobtn);
			this->panel1->Controls->Add(this->Edgetb);
			this->panel1->Controls->Add(this->label2);
			this->panel1->Location = System::Drawing::Point(22, 12);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(1340, 84);
			this->panel1->TabIndex = 15;
			// 
			// panelSidemenu
			// 
			this->panelSidemenu->AutoScroll = true;
			this->panelSidemenu->BackColor = System::Drawing::Color::Gray;
			this->panelSidemenu->Dock = System::Windows::Forms::DockStyle::Left;
			this->panelSidemenu->Location = System::Drawing::Point(0, 109);
			this->panelSidemenu->Name = L"panelSidemenu";
			this->panelSidemenu->Size = System::Drawing::Size(22, 502);
			this->panelSidemenu->TabIndex = 13;
			// 
			// adbtb
			// 
			this->adbtb->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->adbtb->BackColor = System::Drawing::Color::Transparent;
			this->adbtb->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->adbtb->Cursor = System::Windows::Forms::Cursors::Hand;
			this->adbtb->FlatAppearance->BorderColor = System::Drawing::Color::LightCyan;
			this->adbtb->FlatAppearance->BorderSize = 0;
			this->adbtb->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Transparent;
			this->adbtb->FlatAppearance->MouseOverBackColor = System::Drawing::Color::Transparent;
			this->adbtb->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->adbtb->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->adbtb->Location = System::Drawing::Point(100, 116);
			this->adbtb->Margin = System::Windows::Forms::Padding(4, 3, 4, 3);
			this->adbtb->Name = L"adbtb";
			this->adbtb->Padding = System::Windows::Forms::Padding(5, 0, 0, 0);
			this->adbtb->Size = System::Drawing::Size(116, 21);
			this->adbtb->TabIndex = 1;
			this->adbtb->Text = L"Adjacency Matrix";
			this->adbtb->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			this->adbtb->UseVisualStyleBackColor = false;
			this->adbtb->Click += gcnew System::EventHandler(this, &mainform::adbtb_Click);
			// 
			// isognrt2
			// 
			this->isognrt2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->isognrt2->BackColor = System::Drawing::Color::DarkSlateGray;
			this->isognrt2->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->isognrt2->Cursor = System::Windows::Forms::Cursors::Cross;
			this->isognrt2->Location = System::Drawing::Point(986, 143);
			this->isognrt2->Margin = System::Windows::Forms::Padding(4, 3, 4, 3);
			this->isognrt2->Name = L"isognrt2";
			this->isognrt2->Size = System::Drawing::Size(349, 302);
			this->isognrt2->TabIndex = 0;
			this->isognrt2->TabStop = false;
			this->isognrt2->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &mainform::isognrt2_Paint);
			// 
			// isognrt1
			// 
			this->isognrt1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->isognrt1->BackColor = System::Drawing::Color::DarkSlateGray;
			this->isognrt1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->isognrt1->Cursor = System::Windows::Forms::Cursors::Cross;
			this->isognrt1->Location = System::Drawing::Point(626, 143);
			this->isognrt1->Margin = System::Windows::Forms::Padding(4, 3, 4, 3);
			this->isognrt1->Name = L"isognrt1";
			this->isognrt1->Size = System::Drawing::Size(352, 302);
			this->isognrt1->TabIndex = 0;
			this->isognrt1->TabStop = false;
			this->isognrt1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &mainform::isognrt1_Paint);
			// 
			// instructlbl
			// 
			this->instructlbl->AutoSize = true;
			this->instructlbl->BackColor = System::Drawing::Color::Transparent;
			this->instructlbl->Font = (gcnew System::Drawing::Font(L"Comic Sans MS", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->instructlbl->ForeColor = System::Drawing::Color::DarkSlateGray;
			this->instructlbl->Location = System::Drawing::Point(216, 116);
			this->instructlbl->Name = L"instructlbl";
			this->instructlbl->Size = System::Drawing::Size(20, 23);
			this->instructlbl->TabIndex = 14;
			this->instructlbl->Text = L"4";
			this->instructlbl->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// errorValidator
			// 
			this->errorValidator->ContainerControl = this;
			// 
			// panel3
			// 
			this->panel3->AutoScroll = true;
			this->panel3->BackColor = System::Drawing::Color::Gray;
			this->panel3->Dock = System::Windows::Forms::DockStyle::Right;
			this->panel3->Location = System::Drawing::Point(1362, 109);
			this->panel3->Name = L"panel3";
			this->panel3->Size = System::Drawing::Size(22, 502);
			this->panel3->TabIndex = 15;
			// 
			// panel5
			// 
			this->panel5->AutoScroll = true;
			this->panel5->BackColor = System::Drawing::Color::Gray;
			this->panel5->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->panel5->Location = System::Drawing::Point(22, 586);
			this->panel5->Name = L"panel5";
			this->panel5->Size = System::Drawing::Size(1340, 25);
			this->panel5->TabIndex = 17;
			// 
			// txtdegree
			// 
			this->txtdegree->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtdegree->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->txtdegree->Cursor = System::Windows::Forms::Cursors::Hand;
			this->txtdegree->Location = System::Drawing::Point(626, 452);
			this->txtdegree->Name = L"txtdegree";
			this->txtdegree->ReadOnly = true;
			this->txtdegree->Size = System::Drawing::Size(92, 96);
			this->txtdegree->TabIndex = 18;
			this->txtdegree->Text = L"";
			// 
			// txtdegree1
			// 
			this->txtdegree1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtdegree1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->txtdegree1->Cursor = System::Windows::Forms::Cursors::Hand;
			this->txtdegree1->Location = System::Drawing::Point(724, 452);
			this->txtdegree1->Name = L"txtdegree1";
			this->txtdegree1->ReadOnly = true;
			this->txtdegree1->Size = System::Drawing::Size(92, 96);
			this->txtdegree1->TabIndex = 18;
			this->txtdegree1->Text = L"";
			// 
			// txtdegree2
			// 
			this->txtdegree2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtdegree2->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->txtdegree2->Cursor = System::Windows::Forms::Cursors::Hand;
			this->txtdegree2->Location = System::Drawing::Point(822, 452);
			this->txtdegree2->Name = L"txtdegree2";
			this->txtdegree2->ReadOnly = true;
			this->txtdegree2->Size = System::Drawing::Size(92, 96);
			this->txtdegree2->TabIndex = 18;
			this->txtdegree2->Text = L"";
			// 
			// mainform
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoScroll = true;
			this->AutoSize = true;
			this->BackColor = System::Drawing::Color::DarkGray;
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->ClientSize = System::Drawing::Size(1384, 611);
			this->Controls->Add(this->txtdegree2);
			this->Controls->Add(this->txtdegree1);
			this->Controls->Add(this->txtdegree);
			this->Controls->Add(this->panel5);
			this->Controls->Add(this->panel3);
			this->Controls->Add(this->CBgrid);
			this->Controls->Add(this->instructlbl);
			this->Controls->Add(this->adbtb);
			this->Controls->Add(this->panelSidemenu);
			this->Controls->Add(this->panel2);
			this->Controls->Add(this->generatebtn);
			this->Controls->Add(this->isognrt1);
			this->Controls->Add(this->isognrt2);
			this->Controls->Add(this->PBdraw);
			this->DoubleBuffered = true;
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->Margin = System::Windows::Forms::Padding(4, 3, 4, 3);
			this->Name = L"mainform";
			this->SizeGripStyle = System::Windows::Forms::SizeGripStyle::Hide;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Load += gcnew System::EventHandler(this, &mainform::mainform_Load);
			this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &mainform::mainform_KeyDown);
			this->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &mainform::mainform_KeyUp);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->PBdraw))->EndInit();
			this->panel2->ResumeLayout(false);
			this->panel1->ResumeLayout(false);
			this->panel1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->isognrt2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->isognrt1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->errorValidator))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		// Loading Mainform
		private: System::Void mainform_Load(System::Object^ sender, System::EventArgs^ e) {
			PBdraw->Enabled = false;
			CBgrid->Enabled = false;
			generatebtn->Enabled = false;
			instructlbl->Text = "";
			addvertexbtn->Enabled = false;
			connectbtn->Enabled = false;
			Verticestb->Focus();
		}

		//Dots in Click
		private: System::Void PBdraw_VertexClick(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
			int dotsradius = 30;
			bool IsOverLap = false;
			if (Int32::TryParse(Verticestb->Text, vertcount) && DrawMode == 0) {
				if (isDrawing == true && dots->Count < vertcount) {
					for each (PointAndIndex ^ dot in dots) {
						if (e->X >= dot->position.X - dotsradius && e->X <= dot->position.X + dotsradius && e->Y >= dot->position.Y - dotsradius && e->Y <= dot->position.Y + dotsradius) {
							MessageBox::Show("Please do not overlap the vertices", "ERROR", MessageBoxButtons::OK, MessageBoxIcon::Stop);
							IsOverLap = true;
						}
					}
					if (IsOverLap == false) {
						Point mousePos = PBdraw->PointToClient(Control::MousePosition);
						dotundo->Type = 0;
						graph->ResizeAdjacencyMatrix(dotnum);
						dots->Add(gcnew PointAndIndex(mousePos, dotnum));
						objects->Add(dotundo);
						validDots->Push(mousePos);
						dotHistory->Push(mousePos);
						PBdraw->Invalidate();
					}
				}
			}
			//Notice for vertices
			if (showDot == false && DrawMode == 0 && isDrawing == true) {
				MessageBox::Show("You've already drawn " + vertcount + " vertices", "NO MORE VERTICES TO DRAW!", MessageBoxButtons::OK, MessageBoxIcon::Stop);
			}
			//change draw mode to edges
			else if (isDrawing == true && dots->Count == vertcount && DrawMode == 0) {
				showDot = false;
				completedot = true;
			}
		}
	
		private: System::Void PBdraw_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
			Point currentPoint = e->Location;
			int dotsradius = 16;
			int gridSize = 24;
			array<array<int>^>^ adjacencyMatrix1 = graph->GetAdjacencyMatrix();

			if (Int32::TryParse(Edgetb->Text, edgecount)) {
				if (lines->Count < edgecount) {
					// Check if the click is on any of the dots
					for each (PointAndIndex ^ dot in dots) {
						int cellX = dot->position.X / gridSize;
						int cellY = dot->position.Y / gridSize;
						int dotX = cellX * gridSize + gridSize / 2 - dotSize / 2;
						int dotY = cellY * gridSize + gridSize / 2 - dotSize / 2;
						
						if (e->X >= dot->position.X - dotsradius && e->X <= dot->position.X + dotsradius && e->Y >= dot->position.Y - dotsradius && e->Y <= dot->position.Y + dotsradius) {
							clickedOnDot = true;
							//toolTip1->Show(dotNumber.ToString(), PBdraw, e->Location);  // Replace 'PBdraw' with the actual PictureBox control name
							if (clickedOnDot && DrawMode == 1 && dots->Count > 1) {
								instructlbl->Text = "Select the second point of the edge";

								switch (clicknum)
								{
								case 1:
									startPoint = Point(dotX + dotSize / 2, dotY + dotSize / 2); 
									startDotIndex = dot->index - 1;
									UpdateDotDegrees(dot->index);
									clicknum++;
									break;
								case 2:
									instructlbl->Text = "Select the first point of the edge";
									endPoint = Point(dotX + dotSize / 2, dotY + dotSize / 2); // Use dot center as end point
									UpdateDotDegrees(dot->index);
									endDotIndex = dot->index - 1;
									lineundo->Type = 1;
									int deltaY = 10; // Adjust this value to increase the arc height
								
									if (adjacencyMatrix1[startDotIndex][endDotIndex] == 1 && adjacencyMatrix1[endDotIndex][startDotIndex] == 1)
									{
										if (startDotIndex == endDotIndex) {
											Pointloop1.X -= 3;
											Pointloop2.Y -= 12;
											lines->Add(gcnew Line(startPoint, endPoint, false, Pointloop1, Pointloop2));
										}
										else {
											lines->Add(gcnew Line(startPoint, endPoint, true, controlPoint1, controlPoint2));
											controlPoint1.X += 7;
											controlPoint2.Y += 9;
										}
									}
									else {
										if (startDotIndex == endDotIndex) {
											int midX = (startPoint.X + endPoint.X) / 2;
											int midY = (startPoint.Y + endPoint.Y) / 2;
											Pointloop1 = Point(midX - 50, midY - 50); // Adjust these values to control the curve's shape
											Pointloop2 = Point(midX + 50, midY - 50); // Adjust these values to control the curve's shape

											lines->Add(gcnew Line(startPoint, endPoint, false, Pointloop1, Pointloop2));
										}
										else {
											lines->Add(gcnew Line(startPoint, endPoint, false, controlPoint1, controlPoint2));
										}
									}
									objects->Add(lineundo);
									PBdraw->Invalidate();
									clicknum = 1;
									graph->AddEdge(startDotIndex, endDotIndex); // Add the edge to the graph
									graph->AddCount(startDotIndex, endDotIndex);
									break;
								}
								previewLine = gcnew Line(startPoint, endPoint, true, controlPoint1, controlPoint2);
								PBdraw->Invalidate();
								break;
							}
						}
					}
				}
			}
			//Notice for edges
			for each (PointAndIndex ^ dot in dots) {
				if (e->X >= dot->position.X - dotsradius && e->X <= dot->position.X + dotsradius && e->Y >= dot->position.Y - dotsradius && e->Y <= dot->position.Y + dotsradius) {
					clickedOnDot = true;
					if (clickedOnDot == true && DrawMode == 1 && showline == false) {
						MessageBox::Show("You've already drawn " + edgecount + " edges", "NO MORE EDGES TO DRAW!", MessageBoxButtons::OK, MessageBoxIcon::Stop);
					}
					else if (clickedOnDot == true && DrawMode == 1 && lines->Count == edgecount) {
						showline = false;
						completeline = true;
					}
				}
			}
		}
		//adjacency matrix
		private: System::Void adbtb_Click(System::Object^ sender, System::EventArgs^ e) {
			array<array<int>^>^ adjacencyMatrix = graph->GetAdjacencyMatrix();
			int vertexCount = adjacencyMatrix->GetLength(0);

			// Create a string representation of the adjacency matrix with vertex labels
			String^ matrixString = "Adjacency Matrix:\n";

			// Add column labels (vertex indices)
			matrixString += "   ";
			for (int i = 0; i < vertexCount; i++) {
				matrixString += i + 1 + " ";  // Vertex indices start from 1
			}
			matrixString += "\n";

			// Add row labels and matrix values
			for (int i = 0; i < vertexCount; i++) {
				// Add row label (vertex index)
				matrixString += i + 1 + " ";

				// Add matrix values
				for (int j = 0; j < vertexCount; j++) {
					matrixString += adjacencyMatrix[i][j] + ",";
				}
				matrixString += "\n";
			}
			MessageBox::Show(matrixString, "Adjacency Matrix", MessageBoxButtons::OK, MessageBoxIcon::Information);

			array<array<int>^>^ lineCount = graph->GetLineCount();
			int vertexCount1 = lineCount->GetLength(0);

			// Create a string representation of the lineCount array with vertex labels
			String^ lineString = "Line Count Matrix:\n";

			// Add column labels (vertex indices)
			lineString += "   ";
			for (int i = 0; i < vertexCount1; i++) {
				lineString += i + 1 + " ";  // Vertex indices start from 1
			}
			lineString += "\n";

			// Add row labels and matrix values
			for (int i = 0; i < vertexCount1; i++) {
				// Add row label (vertex index)
				lineString += i + 1 + " ";

				// Add matrix values
				for (int j = 0; j < vertexCount1; j++) {
					lineString += lineCount[i][j] + ",";
				}
				lineString += "\n";
			}
			MessageBox::Show(lineString, "Line Count", MessageBoxButtons::OK, MessageBoxIcon::Information);
		}

		//Preview Line
		private: System::Void PBdraw_MouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
			if (clicknum == 1)
				return;
			if (DrawMode = 1) {
				// Update the end point of the preview line
				previewLine->endPoint = e->Location;
				// Redraw the PictureBox control
				PBdraw->Invalidate();
			}
		}
		// Picture Box
		private: System::Void PBdraw_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
			Graphics^ g = e->Graphics;
			g->SmoothingMode = System::Drawing::Drawing2D::SmoothingMode::AntiAlias;

			if (isDrawing == true) {
				//Declaration
				int gridSize = 24; // Adjust the size of each grid cell
				dotnum = 1;
				Pen^ gridPen = gcnew Pen(Color::FromArgb(0, 0, 0)); // Adjust the color of the grid line
				List<Line^> prevLines;

				showGridLines = CBgrid->Checked;
				//grid line
				if (showGridLines) {
					// Draw vertical grid lines
					for (int x = 0; x <= PBdraw->Width; x += gridSize) {
						g->DrawLine(gridPen, x, 0, x, PBdraw->Height);
					}

					// Draw horizontal grid lines
					for (int y = 0; y <= PBdraw->Height; y += gridSize) {
						g->DrawLine(gridPen, 0, y, PBdraw->Width, y);
					}
					delete gridPen;
				}
				for each (Line ^ line in lines)
				{
					// Calculate the percentage of the distance to determine control point positions
					int midX = (line->startPoint.X + line->endPoint.X) / 2;
					int midY = (line->startPoint.Y + line->endPoint.Y) / 2;

					if (line->startPoint != line->endPoint) {
						line->DrawLineCurve(g, midX, midY, Color::Cyan, 2.0);

						SizeF labelSize = g->MeasureString(edgeLabel, letterFont);
						PointF labelPosition(midX - labelSize.Width / 2, midY - labelSize.Height / 2);
						g->DrawString(edgeLabel, letterFont, Brushes::Black, labelPosition);
					}
					if (line->startPoint == line->endPoint) {
						line->LoopLine(g, line->startPoint.X, line->startPoint.Y);
					}
				}
				if (clicknum == 2) {
					previewLine->PreviewDraw(g);
				}

				//drawing dots
				for each (PointAndIndex ^ p in dots) {
					// alignment and size
					int cellX = p->position.X / gridSize;
					int cellY = p->position.Y / gridSize;
					int dotX = cellX * gridSize + gridSize / 2 - dotSize / 2;
					int dotY = cellY * gridSize + gridSize / 2 - dotSize / 2;

					g->FillEllipse(gcnew SolidBrush(dotFillColor), dotX, dotY, dotSize, dotSize);
					g->DrawEllipse(gcnew Pen(dotOutlineColor), dotX, dotY, dotSize, dotSize);

					String^ letter = dotnum.ToString();  // convert dotnum to string
					SizeF size = g->MeasureString(letter, letterFont);
					PointF position(dotX + (dotSize - size.Width) / 2, dotY + (dotSize - size.Height) / 2);
					g->DrawString(letter, letterFont, Brushes::Cyan, position);
					p->index = dotnum;
					dotnum++;
				}
			}
		}
		// graph 1
		private: System::Void isognrt1_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
			Graphics^ g = e->Graphics;
			g->SmoothingMode = System::Drawing::Drawing2D::SmoothingMode::AntiAlias;

			// Draw the lines for graph 1
			for each (Line ^ line in graph1Lines) {
				// Calculate the percentage of the distance to determine control point positions
				int midX = (line->startPoint.X + line->endPoint.X) / 2;
				int midY = (line->startPoint.Y + line->endPoint.Y) / 2;

				if (line->startPoint != line->endPoint) {
					line->DrawLineCurve(g, midX, midY, Color::AliceBlue, 2);
				}
				if (line->startPoint == line->endPoint) {
					line->LoopLine(g, line->startPoint.X, line->startPoint.Y);
				}
			}

			// Draw the dots and numbers for graph 1
			for (int i = 0; i < graph1Dots->Count; i++) {
				Point p = graph1Dots[i];
				int dotSize = 16;
				int dotX = p.X - dotSize / 2;
				int dotY = p.Y - dotSize / 2;

				// Draw the dot
				g->FillEllipse(Brushes::Red, dotX, dotY, dotSize, dotSize);
				g->DrawEllipse(Pens::Black, dotX, dotY, dotSize, dotSize);

				// Draw the number in the middle of the dot
				String^ dotNumber = (i + 1).ToString();
				SizeF textSize = g->MeasureString(dotNumber, letterFont);
				PointF textLocation = PointF(p.X - textSize.Width / 2, p.Y - textSize.Height / 2);
				g->DrawString(dotNumber, letterFont, Brushes::Black, textLocation);
			}
		// graph 2
		}
		private: System::Void isognrt2_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
			Graphics^ g = e->Graphics;
			g->SmoothingMode = System::Drawing::Drawing2D::SmoothingMode::AntiAlias;
			array<array<int>^>^ adjacencyMatrix1 = graph->GetAdjacencyMatrix();

			// Draw the lines for graph 2
			for each (Line ^ line in graph2Lines) {
				// Calculate the percentage of the distance to determine control point positions
				int midX = (line->startPoint.X + line->endPoint.X) / 2;
				int midY = (line->startPoint.Y + line->endPoint.Y) / 2;

				if (line->startPoint != line->endPoint) {
					line->DrawLineCurve(g, midX, midY, Color::Aquamarine, 2);
				}
				if (line->startPoint == line->endPoint) {
					line->LoopLine(g, line->startPoint.X, line->startPoint.Y);
				}
			}

			// Draw the dots and numbers for graph 2
			for (int i = 0; i < graph2Dots->Count; i++) {
				Point p = graph2Dots[i];
				int dotSize = 16;
				int dotX = p.X - dotSize / 2;
				int dotY = p.Y - dotSize / 2;

				// Draw the dot
				g->FillEllipse(Brushes::Blue, dotX, dotY, dotSize, dotSize);
				g->DrawEllipse(Pens::Black, dotX, dotY, dotSize, dotSize);


				// Draw the number in the middle of the dot
				String^ dotNumber = (i + 1).ToString();
				SizeF textSize = g->MeasureString(dotNumber, letterFont);
				PointF textLocation = PointF(p.X - textSize.Width / 2, p.Y - textSize.Height / 2);
				g->DrawString(dotNumber, letterFont, Brushes::Black, textLocation);
			}
		}
		//generate button
		private: System::Void generatebtn_Click(System::Object^ sender, System::EventArgs^ e) {
			if (completedot == true && completeline == true) {
				// Clear existing dots and lines
				graph1Dots->Clear();
				graph1Lines->Clear();
				graph2Dots->Clear();
				graph2Lines->Clear();
				Point controlPoints1 = Point(midX + controlPointOffsetX + 200, midY + controlPointOffsetY);
				Point controlPoints2 = Point(midX + controlPointOffsetX + 200, midY + controlPointOffsetY);;

				int vertexCount;
				if (Int32::TryParse(Verticestb->Text, vertexCount)) {

					// Generate random positions for the dots
					int dotSpacing = 50;
					int maxX = isognrt1->Width - dotSpacing;
					int maxY = isognrt1->Height - dotSpacing;

					// Get the adjacency matrix for the first graph
					array<array<int>^>^ adjacencyMatrix1 = graph->GetAdjacencyMatrix();
					array<array<int>^>^ linecount = graph->GetLineCount();

					// Create dots for the first graph
					for (int i = 0; i < vertexCount; i++) {
						int x, y;
						do {
							x = random->Next(dotSpacing, maxX);
							y = random->Next(dotSpacing, maxY);
						} while (HasOverlap(graph1Dots, Point(x, y)));

						graph1Dots->Add(Point(x, y));
					}

					// Generate a random permutation of indices
					array<int>^ permutation1 = gcnew array<int>(vertexCount);
					for (int i = 0; i < vertexCount; i++) {
						permutation1[i] = i;
					}
					for (int i = 0; i < vertexCount - 1; i++) {
						int j = random->Next(i, vertexCount);
						int temp = permutation1[i];
						permutation1[i] = permutation1[j];
						permutation1[j] = temp;
					}
					// Create dots for the second graph
					for (int i = 0; i < vertexCount; i++) {
						int x, y;
						do {
							x = random->Next(dotSpacing, maxX);
							y = random->Next(dotSpacing, maxY);
						} while (HasOverlap(graph1Dots, Point(x, y)));

						graph2Dots->Add(Point(x, y));
					}
					
					// Generate a random permutation of indices
					array<int>^ permutation = gcnew array<int>(vertexCount);
					for (int i = 0; i < vertexCount; i++) {
						permutation[i] = i;
					}
					for (int i = 0; i < vertexCount - 1; i++) {
						int j = random->Next(i, vertexCount);
						int temp = permutation[i];
						permutation[i] = permutation[j];
						permutation[j] = temp;
					}

					// straight line 1
					for (int i = 0; i < vertexCount; i++) {
						for (int j = 0; j < vertexCount; j++) {
							Point p1 = graph1Dots[i];
							Point p2 = graph1Dots[j];
							if (adjacencyMatrix1[permutation1[i]][permutation1[j]] == 1) {
								if (i != j) {
									graph1Lines->Add(gcnew Line(p1, p2, false, controlPoint1, controlPoint2));
									Gen1DotDegrees(permutation1[i]);
								}
							}
						}
					}
					// straight line 2
					for (int i = 0; i < vertexCount; i++) {
						for (int j = 0; j < vertexCount; j++) {
							Point p1 = graph2Dots[i];
							Point p2 = graph2Dots[j];
							if (adjacencyMatrix1[permutation[i]][permutation[j]] == 1) {
								if (i != j) {
									graph2Lines->Add(gcnew Line(p1, p2, false, controlPoint1, controlPoint2));
									Gen2DotDegrees(permutation[i]);
								}
							}
						}
					}
					//curve lines 1
					for (int i = 0; i < vertexCount; i++) {
						for (int j = i + 1; j < vertexCount; j++) {
							Point p1 = graph1Dots[i];
							Point p2 = graph1Dots[j];
							if (linecount[permutation1[i]][permutation1[j]] > 1) {
								for (int k = 0; k < linecount[permutation1[i]][permutation1[j]] - 1; k++) {
									graph1Lines->Add(gcnew Line(p1, p2, true, controlPoints1, controlPoints2));
									controlPoints1.X += 7;
									controlPoints2.Y += 9;
								}
							}
						}
					}
					for (int i = 0; i < vertexCount; i++) {
						for (int j = 0; j < vertexCount; j++) {
							if (linecount[permutation1[i]][permutation1[j]] > 1 && i != j) {
								Gen1DotDegrees(permutation1[i]);
							}
						}
					}
					//curve lines 2
					for (int i = 0; i < vertexCount; i++) {
						for (int j = i + 1; j < vertexCount; j++) {
							Point p1 = graph2Dots[i];
							Point p2 = graph2Dots[j];
							if (linecount[permutation[i]][permutation[j]] > 1) {
								for (int k = 0; k < linecount[permutation[i]][permutation[j]] - 1; k++) {
									graph2Lines->Add(gcnew Line(p1, p2, true, controlPoints1, controlPoints2));
									controlPoints1.X += 7;
									controlPoints2.Y += 9;
								}
							}
						}
					}
					for (int i = 0; i < vertexCount; i++) {
						for (int j = 0; j < vertexCount; j++) {
							if (linecount[permutation[i]][permutation[j]] > 1 && i != j) {
								Gen2DotDegrees(permutation[i]);
							}
						}
					}
					//loop line 1 
					for (int i = 0; i < vertexCount; i++) {
						for (int j = 0; j < vertexCount; j++) {
							Point p1 = graph1Dots[i];
							Point p2 = graph1Dots[j];
							int midX = (p1.X + p2.X) / 2;
							int midY = (p1.Y + p2.Y) / 2;
							controlPointloops1 = Point(midX - 30, midY - 30); 
							controlPointloops2 = Point(midX + 30, midY - 30); 
							if (i == j) {
								for (int k = 0; k < linecount[permutation1[i]][permutation1[j]]; k += 2) {
									graph1Lines->Add(gcnew Line(p1, p2, false, controlPointloops1, controlPointloops2));
									Gen1DotDegrees(permutation1[i]);
									Gen1DotDegrees(permutation1[i]);
									controlPointloops1.X -= 8;
									controlPointloops2.Y -= 20;
								}
							}
						}
					}
					//loop line 2
					for (int i = 0; i < vertexCount; i++) {
						for (int j = 0; j < vertexCount; j++) {
							Point p1 = graph2Dots[i];
							Point p2 = graph2Dots[j];
							int midX = (p1.X + p2.X) / 2;
							int midY = (p1.Y + p2.Y) / 2;
							controlPointloops1 = Point(midX - 30, midY - 30);
							controlPointloops2 = Point(midX + 30, midY - 30);
							if (i == j) {
								for (int k = 0; k < linecount[permutation[i]][permutation[j]]; k += 2) {
									graph2Lines->Add(gcnew Line(p1, p2, false, controlPointloops1, controlPointloops2));
									Gen2DotDegrees(permutation[i]);
									Gen2DotDegrees(permutation[i]);
									controlPointloops1.X -= 8;
									controlPointloops2.Y -= 20;

								}
							}
						}
					}

					// Redraw the PictureBox controls
					isognrt1->Invalidate();
					isognrt2->Invalidate();

					undobtn->Enabled = false;
					generatebtn->Enabled = false;
				 
					// Diplay the adjacency matrix in a message box
					txtdegree->Clear();
					txtdegree1->Clear();
					txtdegree2->Clear();

					txtdegree->Text += "User's Drawing:\n";
					for each (PointAndIndex ^ dot in dots) {
						int vertexIndex = dot->index;
						if (dotDegrees->ContainsKey(vertexIndex)) {
							txtdegree->Text += "Vertex " + vertexIndex + ": " + dotDegrees[vertexIndex] + "\n";
						}
						else {
							dotDegrees[vertexIndex] = 0;
							txtdegree->Text += "Vertex " + vertexIndex + ": " + dotDegrees[vertexIndex] + "\n";
						}
					}
					txtdegree1->Text += "First Graph:\n";
					for (int i = 0; i < vertexCount; i++) {
						if (gen1Degrees->ContainsKey(permutation1[i])) {
							txtdegree1->Text += "Vertex " + (i + 1) + ": " + gen1Degrees[permutation1[i]] + "\n";
						}
						else {
							gen1Degrees[permutation1[i]] = 0;
							txtdegree1->Text += "Vertex " + (i + 1) + ": " + gen1Degrees[permutation1[i]] + "\n";
						}
					}
					txtdegree2->Text += "Second Graph:\n";
					for (int i = 0; i < vertexCount; i++) {
						if (gen2Degrees->ContainsKey(permutation[i])) {
							txtdegree2->Text += "Vertex " + (i + 1) + ": " + gen2Degrees[permutation[i]] + "\n";
						}
						else {
							gen2Degrees[permutation[i]] = 0;
							txtdegree2->Text += "Vertex " + (i + 1) + ": " + gen2Degrees[permutation[i]] + "\n";
						}
					}
					//Checking
					bool isIsomorphic = false;
					/*for each (KeyValuePair<int, int> ^ kvp in dotDegrees) {
						int vertexIndex = kvp->Key;
						int dotDegree = kvp->Value;

						if (!gen1Degrees->ContainsKey(vertexIndex) || !gen2Degrees->ContainsKey(vertexIndex)) {
							// If the vertex is missing in either graph, they can't be isomorphic.
							isIsomorphic = false;
							break;
						}

						int gen1Degree = gen1Degrees[vertexIndex];
						int gen2Degree = gen2Degrees[vertexIndex];

						if (dotDegree != gen1Degree || dotDegree != gen2Degree || gen1Degree != gen2Degree) {
							// If the degrees don't match, the graphs aren't isomorphic.
							isIsomorphic = false;
							break;
						}
					}*/
					for each (PointAndIndex ^ dot in dots) {
						int vertexIndex = dot->index;
						for (int i = 0; i < vertexCount; i++) {
							if (dotDegrees[vertexIndex] == gen1Degrees[permutation1[i]] || dotDegrees[vertexIndex] == gen2Degrees[permutation1[i]]) {
								isIsomorphic = true;
								//break;
							}
						}
					}
					if (isIsomorphic == true){
						MessageBox::Show("IT IS ISOMORPHIC!", "CHECKER", MessageBoxButtons::OK, MessageBoxIcon::Information);
					}
					else {
						MessageBox::Show("IT IS NOT ISOMORPHIC!", "CHECKER", MessageBoxButtons::OK, MessageBoxIcon::Information);
					}
				}
			}
			else {
				MessageBox::Show("Complete the drawing first", "ERROR", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
			}
		}
		// Clear Button 
		private: System::Void Clearbtn_Click(System::Object^ sender, System::EventArgs^ e) {
			//clear drawing box
			dots->Clear();
			lines->Clear();
			//clear generate box
			graph1Dots->Clear();
			graph1Lines->Clear();
			graph2Dots->Clear();
			graph2Lines->Clear();
			isognrt1->Invalidate();
			isognrt2->Invalidate();
			//clear textbox
			Verticestb->Text = "";
			Edgetb->Text = "";
			Verticestb->Enabled = true;
			Edgetb->Enabled = true;
			//making the setting default
			DrawMode = 0;
			showDot = true;
			showline = true;
			completeline = false;
			completedot = false;
			isDrawing = false;
			clickedOnDot = false;
			clicknum = 1;
			vertcount = 0;
			edgecount = 0;
			Verticestb->Focus();
			addvertexbtn->BackColor = Color::Transparent;
			connectbtn->BackColor = Color::Transparent;
			addvertexbtn->Enabled = false;
			connectbtn->Enabled = false;
			undobtn->Enabled = true;
			generatebtn->Enabled = true;
			previewLine = gcnew Line();
			PBdraw->Invalidate();
			instructlbl->Text = "";
			dotnum = 1;
			graph->ResizeAdjacencyMatrix(0);
			Point startPoint = Point();
			Point endPoint = Point();
			int controlPointOffsetY = 30; // Adjust this value to control the curve's highest point
			int controlPointOffsetX = 15; // Adjust this value to control the curve's width
			int midX = (startPoint.X + endPoint.X) / 2;
			int midY = (startPoint.Y + endPoint.Y) / 2;
			Point controlPoint1 = Point(midX + controlPointOffsetX + 200, midY + controlPointOffsetY);
			Point controlPoint2 = Point(midX + controlPointOffsetX + 200, midY + controlPointOffsetY);
			dotDegrees->Clear();
			gen1Degrees->Clear();
			gen2Degrees->Clear();
			txtdegree->Clear();
			txtdegree1->Clear();
			txtdegree2->Clear();
			generatebtn->Enabled = true;
			PBdraw->Enabled = false;
			CBgrid->Enabled = false;
			generatebtn->Enabled = false;
			temp = true;
		}
		//Undo the recent dot 
		private: System::Void undobtn_Click(System::Object^ sender, System::EventArgs^ e) {
			undobtn->Focus();
			addvertexbtn->BackColor = Color::Transparent;
			connectbtn->BackColor = Color::Transparent;
			array<array<int>^>^ linecount = graph->GetLineCount();

			if (objects->Count > 0) {
				int obj = objects[objects->Count - 1]->Type;
				
				if (obj == 0) {
					if (dots->Count > 0) {
						dots->RemoveAt(dots->Count - 1);	
						graph->RemoveDisconnectedVertices();
						PBdraw->Invalidate();
						showline = true;
						showDot = true;
					}
				}
				else if (obj == 1) {
					if (lines->Count > 0) {
						previewLine = gcnew Line();
						lines->RemoveAt(lines->Count - 1);	
						PBdraw->Invalidate();
						if (linecount[startDotIndex][endDotIndex] == 1 && linecount[endDotIndex][startDotIndex] == 1)
						{
							graph->Undo();
						}
						graph->UndoLine();
						if (startDotIndex == endDotIndex && linecount[startDotIndex][endDotIndex] == 0)
						{
							graph->Undo();
						}
						showDot = true;
						showline = true;
					}
				}
				objects->RemoveAt(objects->Count - 1); // Update the objects collection
			}
		}
		// undo shortcut
		private: System::Void mainform_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
			if (e->KeyCode == Keys::ControlKey)
			{
				ctrlKeyPressed = true;
			}
			else if (e->KeyCode == Keys::Z && ctrlKeyPressed)
			{
				zKeyPressed = true;
			}
		}
		private: System::Void mainform_KeyUp(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
			if (e->KeyCode == Keys::ControlKey)
			{
				ctrlKeyPressed = false;
			}
			else if (e->KeyCode == Keys::Z && ctrlKeyPressed && zKeyPressed)
			{
				// Perform the undo action when "Ctrl+Z" is pressed
				undobtn_Click(sender, e);
				// Reset the "Z" key state
				zKeyPressed = false;
				addvertexbtn->Focus();
				connectbtn->Focus();
			}
		}
			   // Exit Button
		private: System::Void exitBtn_Click(System::Object^ sender, System::EventArgs^ e) {
			this->Close();
		}
			   // Minimize button
		private: System::Void hideBtn_Click(System::Object^ sender, System::EventArgs^ e) {
			this->WindowState = FormWindowState::Minimized;
		}
		//draw vertex
		private: System::Void addvertexbtn_Click(System::Object^ sender, System::EventArgs^ e) {
			DrawMode = 0;
			instructlbl->Text = "Click on the picturebox to draw the vertices";
			addvertexbtn->BackColor = Color::LightCyan;
			connectbtn->BackColor = Color::Transparent;
			Verticestb->Enabled = false;
			Edgetb->Enabled = false;
		}
		//draw edge
		private: System::Void connectbtn_Click(System::Object^ sender, System::EventArgs^ e) {
			instructlbl->Text = "Select the first point of the edge";
			DrawMode = 1;
			connectbtn->BackColor = Color::LightCyan;
			addvertexbtn->BackColor = Color::Transparent;
			Verticestb->Enabled = false;
			Edgetb->Enabled = false;
		}
		//Show grid
		private: System::Void CBgrid_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
			showGridLines = CBgrid->Checked;
			PBdraw->Refresh();
		}
		// Enter function of vertices textbox
		private: System::Void Verticestb_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
			if (e->KeyCode == Keys::Enter) {
				Edgetb->Focus();
			}
		}
		private: System::Void Edgestb_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
			if (e->KeyCode == Keys::Enter) {
				enterbtn->PerformClick(); // Call the enterbtn_Click event handler
			}
			else if (e->KeyCode == Keys::Back && Edgetb->Text == "") {
				e->SuppressKeyPress = true; // Prevents the backspace from being entered in the empty textbox
				BackKeyPressed = true;
				Verticestb->Focus(); // Focus on the "Verticestb" textbox
			}
		}
		// Enter button function 
		private: System::Void enterbtn_Click(System::Object^ sender, System::EventArgs^ e) {

			String^ verticesInput = this->Verticestb->Text;
			String^ edgesInput = this->Edgetb->Text;
			int vertices, edges;
			bool isVerticesValid = Int32::TryParse(verticesInput, vertices);
			bool isEdgesValid = Int32::TryParse(edgesInput, edges);

			// Validation of input
			if (!isVerticesValid || !isEdgesValid)
			{
				isDrawing = false;
				Verticestb->Focus();
				return;
			}
			else if (vertices < 4 || Verticestb->Text == "")
			{
				MessageBox::Show("Please input vertices greater than or equal to 4.", "INVALID INPUT!", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
				isDrawing = false;
				Verticestb->Text = "";
				return;
			}
			else if (edges < 4 || Edgetb->Text == "") {
				MessageBox::Show("Please input edges greater than or equal to 4.", "INVALID INPUT!", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
				isDrawing = false;
				Edgetb->Text = "";
				return;
			}
			else {
				isDrawing = true;
				PBdraw->Enabled = true;
				CBgrid->Enabled = true;
				generatebtn->Enabled = true;
				addvertexbtn->Enabled = true;
				connectbtn->Enabled = true;
			}

		}
		// Length limitation and diigit validation for vertex textbox
		private: System::Void Verticestb_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e) {
			
			if (!Char::IsDigit(e->KeyChar) && e->KeyChar != '\b') // Check if the input is number and if backspace key is not pressed
			{
				e->Handled = true; // Cancel the key press event
			}
			if (Char::IsDigit(e->KeyChar)) {
				if (Verticestb->Text->Length == 2) // Check if length exceeds 2
				{
					e->Handled = true; // Cancel the key press event
				}
			}
		}
		// Length limitation and digit validation for edge textbox
		private: System::Void Edgetb_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e) {

			if (!Char::IsDigit(e->KeyChar) && e->KeyChar != '\b') // Check if the input is number and if backspace key is not pressed
			{
				e->Handled = true; // Cancel the key press event
			}
			if (Char::IsDigit(e->KeyChar)) {
				if (Edgetb->Text->Length == 2) // Check if length exceeds 2
				{
					e->Handled = true; // Cancel the key press event
				}
			}
		}
		// validation of verticestb while typing 
		private: System::Void Verticestb_Validating(System::Object^ sender, System::ComponentModel::CancelEventArgs^ e) {
			String^ verticesInput = this->Verticestb->Text;
			int vertices;
			bool isVerticesValid = Int32::TryParse(verticesInput, vertices);

			if ((verticesInput->Length > 0 && vertices < 4) || Verticestb->Text == "")
			{
				isDrawing = false;
				Verticestb->Clear();
				Verticestb->Focus();
				errorValidator->SetError(Verticestb, "Please input vertices greater than or equal to 4.");
				//MessageBox::Show("Please input vertices greater than or equal to 4.", "INVALID INPUT!", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
			}
			else {
				errorValidator->SetError(Verticestb, nullptr);
			}
		}
		private: System::Void Edgetb_Validating(System::Object^ sender, System::ComponentModel::CancelEventArgs^ e) {
			String^ edgesInput = this->Edgetb->Text;
			if (edgesInput == "" && BackKeyPressed == true) {
				isDrawing = false;
				BackKeyPressed = false;
				e->Cancel = false;
				errorValidator->SetError(Edgetb, nullptr);
				return; // If no input, exit the function without showing an error message

			}
			int edges;
			bool isEdgesValid = Int32::TryParse(edgesInput, edges);

			if (edgesInput->Length > 0 && (edges < 4 || !isEdgesValid) || (edgesInput == "" && BackKeyPressed == false)) {
				//MessageBox::Show("Please input edges greater than or equal to 4.", "INVALID INPUT!", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
				isDrawing = false;
				Edgetb->Clear();
				Edgetb->Focus();
				errorValidator->SetError(Edgetb, "Please input edges greater than or equal to 4.");

			}
			else {
				errorValidator->SetError(Edgetb, nullptr);
			}
		}
		//tooltip vertices and edges
		private: System::Void Verticestb_MouseEnter(System::Object^ sender, System::EventArgs^ e) {
			toolTip1->SetToolTip(Verticestb, "Please enter 4 or more vertices.");
		}
		private: System::Void Edgetb_MouseEnter(System::Object^ sender, System::EventArgs^ e) {
			toolTip1->SetToolTip(Edgetb, "Please enter  4 or more edge.");
		}
	};
}
