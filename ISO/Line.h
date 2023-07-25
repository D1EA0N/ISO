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

    public:
        Line(Point start, Point end, bool curve)
            :startPoint(start), endPoint(end), IsCurve(curve)
        {
            color = Color::DarkRed;
            thickness = 2;
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
            // Define control points for the Bezier curve
            Point controlPoint1(midX - 50, midY - 50); // Adjust these values to control the curve's shape
            Point controlPoint2(midX + 50, midY - 50); // Adjust these values to control the curve's shape

            GraphicsPath^ path = gcnew GraphicsPath();
            path->StartFigure();
            path->AddBezier(startPoint, controlPoint1, controlPoint2, endPoint);

            g->DrawPath(pen, path);
            delete pen;
        }
        void DrawLineCurve(Graphics^ g, int midX, int midY) {
            if (IsCurve == false) {
                Pen^ pen = gcnew Pen(color, thickness);
                g->DrawLine(pen, startPoint, endPoint);
                delete pen;
            }
            else {
                Pen^ pen = gcnew Pen(color, thickness);

                int midX = (startPoint.X + endPoint.X) / 2;
                int midY = (startPoint.Y + endPoint.Y) / 2;

                // Calculate the radius of the circle
                int radius = (int)Math::Round(Math::Sqrt(Math::Pow(startPoint.X - midX, 2) + Math::Pow(startPoint.Y - midY, 2)));

                // Calculate the start and sweep angles for the arc
                int startAngle = (int)Math::Round(Math::Atan2(startPoint.Y - midY, startPoint.X - midX) * (180 / Math::PI));
                int endAngle = (int)Math::Round(Math::Atan2(endPoint.Y - midY, endPoint.X - midX) * (180 / Math::PI));
                int sweepAngle = endAngle - startAngle;

                // Create a bounding rectangle for the circle
                System::Drawing::Rectangle rect(midX - radius, midY - radius, 2 * radius, 2 * radius);

                g->DrawArc(pen, rect, startAngle, sweepAngle);
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