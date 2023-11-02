#include <CController.hpp>

int main()
{
    CController controller;
    uint64_t documentHandler;

    controller.openDocument(documentHandler);
    controller.addGraphicPrimitive(documentHandler, "Квадрат");
    controller.closeDocument(documentHandler);

    controller.openDocument(documentHandler);
    controller.addGraphicPrimitive(documentHandler, "Овал");

    controller.openDocument(documentHandler, "filePath");
    controller.addGraphicPrimitive(documentHandler, "Круг");
    controller.removeGraphicPrimitive(documentHandler, 2);

    return 0;
}