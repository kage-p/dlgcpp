#include "drawing_demo.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

enum class DrawObjectType
{
    None = 0,
    Line,
    Rectangle,
    Ellipse
};

struct DrawObject
{
    DrawObjectType type = DrawObjectType::None;
    int strokeWidth;
    Color strokeColor;
    Color fillColor;
    Point start;
    Point end;
};

struct DrawConfig
{
    DrawObjectType type = DrawObjectType::Line;
    int strokeWidth = 5;
    Color strokeColor = Color::Blue;
    Color fillColor = Color::Green;
    Point mouseStart;
    DrawObject currentObject;
    std::vector<DrawObject> drawObjects;
};

std::shared_ptr<Menu> createMenu(DrawConfig& drawConfig, std::shared_ptr<Label> canvas);


/// <summary>
/// Show the drawing demo, the mouse can be used to draw shapes on a canvas.
/// A context menu is also provided.
/// </summary>
void advanced_drawing_demo(ISharedDialog parent)
{
    DrawConfig drawConfig;

    auto dlg = std::make_shared<Dialog>(DialogType::Application, parent);
    dlg->title("Drawing Demo");
    dlg->resize({ 500, 350 });
    dlg->center();

    auto canvas = std::make_shared<Label>();
    canvas->cursor(Cursor::Cross);
    canvas->wantMouseEvents(true);
    canvas->wantPaintEvents(true);
    dlg->add(canvas);

    auto menu = createMenu(drawConfig, canvas);

    canvas->MouseDownEvent() += [canvas, menu, &drawConfig](ISharedControl control, MouseEvent event)
        {
            if (event.button == MouseButton::Left)
            {
                canvas->mouseCapture(true);
                drawConfig.mouseStart = event.point;
            }

            // show the menu on right click
            if (event.button == MouseButton::Right)
            {
                menu->popup(control, event.point);
            }
        };

    canvas->MouseMoveEvent() +=
        [dlg, canvas, &drawConfig](ISharedControl control, MouseEvent event)
        {
            if (!canvas->mouseCapture())
                return;

            auto point1 = dlg->toPixels(drawConfig.mouseStart);
            auto point2 = dlg->toPixels(event.point);

            drawConfig.currentObject = DrawObject
            {
                drawConfig.type,
                drawConfig.strokeWidth,
                drawConfig.strokeColor,
                drawConfig.fillColor,
                point1,
                point2
            };
            canvas->redraw();
        };

    canvas->MouseUpEvent() +=
        [dlg, canvas, &drawConfig](ISharedControl control, MouseEvent event)
        {
            if (!canvas->mouseCapture())
                return;

            // draw from point a - b
            drawConfig.drawObjects
                .push_back(drawConfig.currentObject);

            canvas->redraw();
            canvas->mouseCapture(false);
        };

    dlg->MouseCaptureLostEvent() += [canvas, &drawConfig](auto)
        {
            // clear the 'current' object
            drawConfig.currentObject = DrawObject{};
            canvas->redraw();
        };

    dlg->SizeEvent() +=
        [canvas](ISharedDialog dlg)
        {
            auto canvasPos =
                Position(
                    0,
                    0,
                    dlg->p().size().width(),
                    dlg->p().size().height());

            canvas->p(canvasPos);
            canvas->redraw();
        };

    canvas->PaintEvent() +=
        [dlg, &drawConfig](ISharedControl control, ISharedDrawingContext ctx)
        {
            auto size = ctx->canvasSize();
            //ctx->handled(true);

            int squareSize = 22;

            for (int y = 0; y < size.height(); y += squareSize)
            {
                for (int x = 0; x < size.width(); x += squareSize)
                {
                    bool odd = ((x / squareSize) + (y / squareSize)) % 2 == 0;
                    auto color = odd ? Color::White : Color::LtGray;
                    ctx->fill(Rect(x, y, squareSize, squareSize), color);
                }
            }

            auto drawTheObject = [ctx](const DrawObject& obj)
                {
                    if (obj.type == DrawObjectType::Line)
                    {
                        // draw a line
                        ctx->drawLine(
                            obj.start,
                            obj.end,
                            obj.strokeColor,
                            obj.strokeWidth);
                    }
                    else if (obj.type == DrawObjectType::Rectangle)
                    {
                        // draw a rectangle

                        Rect r{
                            std::min(obj.start.x(), obj.end.x()),
                            std::min(obj.start.y(), obj.end.y()),
                            std::abs(obj.end.x() - obj.start.x()),
                            std::abs(obj.end.y() - obj.start.y())
                        };

                        ctx->drawRect(
                            r,
                            obj.strokeColor,
                            obj.strokeWidth,
                            obj.fillColor);
                    }
                    else if (obj.type == DrawObjectType::Ellipse)
                    {
                        // draw a ellipse/circle

                        Rect r{
                            std::min(obj.start.x(), obj.end.x()),
                            std::min(obj.start.y(), obj.end.y()),
                            std::abs(obj.end.x() - obj.start.x()),
                            std::abs(obj.end.y() - obj.start.y())
                        };

                        ctx->drawEllipse(
                            r,
                            obj.strokeColor,
                            obj.strokeWidth,
                            obj.fillColor);
                    }
                };

            for (auto& obj : drawConfig.drawObjects)
            {
                drawTheObject(obj);
            }

            if (drawConfig.currentObject.type != DrawObjectType::None)
            {
                drawTheObject(drawConfig.currentObject);
            }
        };

    dlg->exec();
}

std::shared_ptr<Menu> createMenu(
    DrawConfig& drawConfig,
    std::shared_ptr<Label> canvas)
{
    auto menu = std::make_shared<Menu>();
    std::shared_ptr<IMenuItem> item;
    std::shared_ptr<IMenuItem> childItem;

    //------------------

    item = std::make_shared<MenuItem>("Undo");
    menu->add(item);
    item->ClickEvent() += [&drawConfig, canvas](auto)
        {
            if (drawConfig.drawObjects.empty())
                return;
            drawConfig.drawObjects.pop_back();
            canvas->redraw();
        };

    item = std::make_shared<MenuItem>("");
    menu->add(item);

    //------------------

    item = std::make_shared<MenuItem>("Shapes");
    menu->add(item);

    childItem = std::make_shared<MenuItem>("Line");
    item->add(childItem);
    childItem->ClickEvent() += [&drawConfig](auto)
        {
            drawConfig.type = DrawObjectType::Line;
        };

    childItem = std::make_shared<MenuItem>("Rectangle");
    item->add(childItem);
    childItem->ClickEvent() += [&drawConfig](auto)
        {
            drawConfig.type = DrawObjectType::Rectangle;
        };

    childItem = std::make_shared<MenuItem>("Ellipse");
    item->add(childItem);
    childItem->ClickEvent() += [&drawConfig](auto)
        {
            drawConfig.type = DrawObjectType::Ellipse;
        };

    //------------------

    item = std::make_shared<MenuItem>("Line color");
    menu->add(item);

    childItem = std::make_shared<MenuItem>("Black");
    item->add(childItem);
    childItem->ClickEvent() += [&drawConfig](auto)
        {
            drawConfig.strokeColor = Color::Black;
        };

    childItem = std::make_shared<MenuItem>("White");
    item->add(childItem);
    childItem->ClickEvent() += [&drawConfig](auto)
        {
            drawConfig.strokeColor = Color::White;
        };

    childItem = std::make_shared<MenuItem>("Red");
    item->add(childItem);
    childItem->ClickEvent() += [&drawConfig](auto)
        {
            drawConfig.strokeColor = Color::Red;
        };

    childItem = std::make_shared<MenuItem>("Green");
    item->add(childItem);
    childItem->ClickEvent() += [&drawConfig](auto)
        {
            drawConfig.strokeColor = Color::Green;
        };

    childItem = std::make_shared<MenuItem>("Blue");
    item->add(childItem);
    childItem->ClickEvent() += [&drawConfig](auto)
        {
            drawConfig.strokeColor = Color::Blue;
        };

    //------------------

    item = std::make_shared<MenuItem>("Line width");
    menu->add(item);

    childItem = std::make_shared<MenuItem>("1 Pixel");
    item->add(childItem);
    childItem->ClickEvent() += [&drawConfig](auto)
        {
            drawConfig.strokeWidth = 1;
        };

    childItem = std::make_shared<MenuItem>("3 Pixels");
    item->add(childItem);
    childItem->ClickEvent() += [&drawConfig](auto)
        {
            drawConfig.strokeWidth = 3;
        };

    childItem = std::make_shared<MenuItem>("5 Pixels");
    item->add(childItem);
    childItem->ClickEvent() += [&drawConfig](auto)
        {
            drawConfig.strokeWidth = 5;
        };

    //------------------

    item = std::make_shared<MenuItem>("Fill color");
    menu->add(item);

    childItem = std::make_shared<MenuItem>("Black");
    item->add(childItem);
    childItem->ClickEvent() += [&drawConfig](auto)
        {
            drawConfig.fillColor = Color::Black;
        };

    childItem = std::make_shared<MenuItem>("White");
    item->add(childItem);
    childItem->ClickEvent() += [&drawConfig](auto)
        {
            drawConfig.fillColor = Color::White;
        };

    childItem = std::make_shared<MenuItem>("Red");
    item->add(childItem);
    childItem->ClickEvent() += [&drawConfig](auto)
        {
            drawConfig.fillColor = Color::Red;
        };

    childItem = std::make_shared<MenuItem>("Green");
    item->add(childItem);
    childItem->ClickEvent() += [&drawConfig](auto)
        {
            drawConfig.fillColor = Color::Green;
        };

    childItem = std::make_shared<MenuItem>("Blue");
    item->add(childItem);
    childItem->ClickEvent() += [&drawConfig](auto)
        {
            drawConfig.fillColor = Color::Blue;
        };

    //------------------

    item = std::make_shared<MenuItem>("");
    menu->add(item);

    item = std::make_shared<MenuItem>("Reset");
    menu->add(item);
    item->ClickEvent() += [&drawConfig, canvas](auto)
        {
            if (drawConfig.drawObjects.empty())
                return;
            drawConfig.drawObjects.clear();
            canvas->redraw();
        };

    return menu;
}
