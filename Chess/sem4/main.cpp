#include "ApplicationManager.h"
#include <locale>

int main() {
    // Ustawienie lokalizacji na polsk�
    setlocale(LC_CTYPE, "Polish");

    // Utworzenie i uruchomienie mened�era aplikacji
    ApplicationManager app;
    app.initialize();
    app.run();

    return 0;
}