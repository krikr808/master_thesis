#ifndef MAINWIN_H
#define MAINWIN_H

////////////////////////////////////////////////////////////////
// INCLUDE FILES
////////////////////////////////////////////////////////////////

// Standard includes
#include <time.h>

// Qt includes
#include <QMainWindow>

// Own widget includes
#include "viswidget.h"

////////////////////////////////////////////////////////////////
// CLASS DEFINITION
////////////////////////////////////////////////////////////////

namespace Ui {
class mainwin;
}

class mainwin : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit mainwin(QWidget *parent = 0);
    ~mainwin();

private slots:
    // Overload the close event slot
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *e);

    void start_simulation();
    bool toggle_pause_simulation();
    void save_screen_as_tikz_picture(QString file_name = "screenshot.tex");

    // Slots connected to UI items
    void on_actionAbout_rtocean_triggered();

private:
    /* Private methods */
    void initialize_scalar_propery_names();
    void run_simulation();
    void do_events();
    void take_printscreens();
    static void do_events_callback(void* mainwin_object);
    static void take_printscreens_callback(void* mainwin_object);

private:
    Ui::mainwin *ui;
    watersystem system;
    QString scalar_propery_names[NUM_SCALAR_PROPERTIES];
};

#endif // MAINWIN_H
