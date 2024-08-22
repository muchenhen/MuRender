#include "Application.h"

#define SAFE_DELETE(p) if (p) { delete (p); (p) = nullptr; }

int main(int argc, char* argv[])
{
    Application app;
    app.Run();
    return 0;
}