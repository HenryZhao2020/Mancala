#include "PCH.h"
#include "Game.h"
#include "Attr.h"

void restoreWindow() {
    HWND hwnd = (HWND) File::readAll("WinID").toInt();
    SendMessage(hwnd, WM_SYSCOMMAND, SC_RESTORE, 0);
    SetForegroundWindow(hwnd); 
    SetActiveWindow(hwnd); 
    SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
    RedrawWindow(hwnd, NULL, 0, RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QDir::setCurrent(QApplication::applicationDirPath());    
    QDir::setCurrent("Mancala_Files");

    RunGuard guard("Mancala");
    if (!guard.tryToRun()) {
        restoreWindow();
        return 0;
    }

    QFontDatabase::addApplicationFont("../Fonts/Ubuntu_Bold.ttf");

    app.setStyle("Fusion");
    app.setStyleSheet(File::readAll("Styles.qss"));
    QObject::connect(&app, &QApplication::aboutToQuit, &app, &Attr::save);

    Game game;
    game.restore();
    game.show();

    File::write("WinID", QString::number(game.winId()));
    return app.exec();
}
