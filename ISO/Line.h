#pragma once
#include "Graph.h"

using namespace System;
using namespace Graphs;
using namespace System::Drawing;
using namespace System::Collections::Generic;
using namespace System::Drawing::Drawing2D;
namespace Lines {

    public ref class Line
    {
    public:
        Point startPoint;
        Point endPoint;
        Point controlPoints1; // New member variable for the first control point
        Point controlPoints2; // New member variable for the second control point
        Point controlPoint1; // New member variable for the first control point
        Point controlPoint2; // New member variable for the second control point
        Color color;
        float thickness;
        bool IsCurve;

    public:
        Line(Point start, Point end, bool curve, Point cp1, Point cp2)
            :startPoint(start), endPoint(end), IsCurve(curve), controlPoints1(cp1), controlPoints2(cp2)
        {
            color = Color::Cyan;
            thickness = 2;

            // Set default control points at a distance of 50 units above the midpoint
            int midX = (start.X + end.X) / 2;
            int midY = (start.Y + end.Y) / 2;
            controlPoint1 = Point(midX - 50, midY - 50);
            controlPoint2 = Point(midX + 50, midY - 50);
        }
        Line() {}
        void Draw(Graphics^ g)
        {
            Pen^ pen = gcnew Pen(color, thickness);
            g->DrawLine(pen, startPoint, endPoint);
            delete pen;
        }
        void LoopLine(Graphics^ g, int midX, int midY) {
            Pen^ pen = gcnew Pen(color, thickness);

            GraphicsPath^ path = gcnew GraphicsPath();
            path->StartFigure();
            path->AddBezier(startPoint, controlPoints1, controlPoints2, endPoint);

            g->DrawPath(pen, path);
            delete pen;
        }
        void DrawLineCurve(Graphics^ g, int cp1, int cp2, Color color, float thickness) {

            if (IsCurve == false) {
                Pen^ pen = gcnew Pen(color, thickness);
                g->DrawLine(pen, startPoint, endPoint);
                delete pen;
            }
            else {
                Pen^ pen = gcnew Pen(color, thickness);

                GraphicsPath^ path = gcnew GraphicsPath();
                path->StartFigure();
                path->AddBezier(startPoint, controlPoints1, controlPoints2, endPoint);

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