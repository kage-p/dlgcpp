#include "graph_demo.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

struct GraphObject
{
    int value = 0;
    int year = 0;
};

/// <summary>
/// Show the graph demo, which draws a simple line chart/graph with some sample data.
/// </summary>
void advanced_graph_demo(ISharedDialog parent)
{
    auto dlg = std::make_shared<Dialog>(DialogType::Application, parent);
    dlg->title("Graph Demo");
    dlg->resize({ 480, 200 });
    dlg->center();

    auto canvas = std::make_shared<Label>();
    canvas->wantMouseEvents(true);
    canvas->wantPaintEvents(true);
    dlg->add(canvas);

    dlg->SizeEvent() += [canvas](ISharedDialog dlg)
        {
            canvas->resize(dlg->p());
            canvas->redraw();
        };

    auto items = std::vector<GraphObject>
    {
        GraphObject {1,2002}, GraphObject {3,2003}, GraphObject{8,2004}, GraphObject{12,2005}, GraphObject{18,2006},
        GraphObject {50,2007}, GraphObject {12,2008}, GraphObject{6,2009}, GraphObject{9,2010}
    };

    canvas->PaintEvent() += [dlg, &items](ISharedControl control, ISharedDrawingContext ctx)
        {
            // draw a simple line graph
            auto size = ctx->canvasSize();

            int lineWidth = 3;
            int margin = 50;

            auto font = Font("Arial", 8);

            // draw horz/vert lines and labels
            ctx->drawLine(
                Point(margin, margin),
                Point(margin, size.height() - margin),
                Color::Gray,
                lineWidth);

            // TODO: transform vertical
            ctx->drawText(
                "Growth (%)",
                font,
                Color::Blue,
                Point(margin, (size.height() - margin) / 2));

            ctx->drawLine(
                Point(margin, size.height() - margin),
                Point(size.width() - margin, size.height() - margin),
                Color::Gray, lineWidth);

            ctx->drawText(
                "Year",
                font,
                Color::Blue,
                Point(((size.width() - margin) / 2), (size.height() - margin) + 25),
                HorizontalAlign::Center);

            // get max value of all items
            int maxValue = 0;
            for (const auto& item : items)
                if (item.value > maxValue)
                    maxValue = item.value;

            int itemWidth = (size.width() - (margin * 2)) / items.size();
            int itemValueHeight = (size.height() - (margin * 2)) / maxValue;

            int xOff = 0;
            int yOff = (size.height() - margin);
            for (const auto& item : items)
            {
                int yThis = (size.height() - margin) - (itemValueHeight * item.value);
                ctx->drawLine(
                    Point(margin + xOff, yOff),
                    Point(margin + xOff + itemWidth, yThis),
                    Color::Blue,
                    lineWidth);

                // text is centered
                ctx->drawText(
                    std::to_string(item.year),
                    font,
                    Color::Blue,
                    Point(margin + xOff + (itemWidth / 2), (size.height() - margin) + 10),
                    HorizontalAlign::Center);

                xOff += itemWidth;
                yOff = yThis;
            }
        };

    dlg->exec();
}
