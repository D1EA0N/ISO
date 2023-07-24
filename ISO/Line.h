#pragma once

using namespace System;
using namespace System::Drawing;
using namespace System::Collections::Generic;
using namespace System::Drawing::Drawing2D;
namespace Lines {

    public ref class Line
    {
    public:
        Point startPoint;
        Point endPoint;
        Color color;
        float thickness;
        bool IsCurve;
        bool IsLoop;

    public:
        Line(Point start, Point end, bool curve, bool loop)
            :startPoint(start), endPoint(end), IsCurve(curve), IsLoop(loop)
        {
            color = Color::DarkRed;
            thickness = 2;
        }
        Line(){}
         void Draw(Graphics^ g)
         {
             Pen^ pen = gcnew Pen(color, thickness);
             g->DrawLine(pen, startPoint, endPoint);
             delete pen;
         }
         void LoopLine(Graphics^ g, int midX, int midY) {
             Pen^ pen = gcnew Pen(color, thickness);
             // Define control points for the Bezier curve
             Point controlPoint1(midX - 50, midY - 50); // Adjust these values to control the curve's shape
             Point controlPoint2(midX + 50, midY - 50); // Adjust these values to control the curve's shape

             GraphicsPath^ path = gcnew GraphicsPath();
             path->StartFigure();
             path->AddBezier(startPoint, controlPoint1, controlPoint2, endPoint);

             g->DrawPath(pen, path);
             delete pen;
        }
         void Curve(Graphics^ g, int midX, int midY) {

             if (IsLoop == true) {
                 LoopLine(g, midX, midY);
             }
             if (IsCurve == false) {
                 Pen^ pen = gcnew Pen(color, thickness);
                 g->DrawLine(pen, startPoint, endPoint);
                 delete pen;
             }
             else{
                 Pen^ pen = gcnew Pen(color, thickness);
                 // Define control points for the Bezier curve
                 Point controlPoint1(midX - 50, midY - 50); // Adjust these values to control the curve's shape
                 Point controlPoint2(midX + 50, midY - 50); // Adjust these values to control the curve's shape

                 GraphicsPath^ path = gcnew GraphicsPath();
                 path->StartFigure();
                 path->AddBezier(startPoint, controlPoint1, controlPoint2, endPoint);

                 g->DrawPath(pen, path);
                 delete pen;
             }
         }

        void PreviewDraw(Graphics^ g)
        {
            Pen^ pen = gcnew Pen(color, thickness);
            pen->DashStyle = System::Drawing::Drawing2D::DashStyle::Dash; // Set the dash style
            g->DrawLine(pen, startPoint, endPoint);
            delete pen;
        }
    };
}
