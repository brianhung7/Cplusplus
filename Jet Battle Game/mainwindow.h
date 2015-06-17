#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gameboard.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QMediaPlayer>



namespace Ui {
class MainWindow;
}
/** @class MainWindow
 * @brief Creates the initial window which displays several options to start the game.
 * MainWindow is primarily created/modeled through the .ui form. Presents an introductory
 * window to present the game and provides several options to begin the game. Includes an option
 * to open up instructions and also creates background music for the game.
 * @author Brian Pham
 * @date June 03,2015
 *
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:

    void game_over();
    void new_game();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_start_button_clicked();

    void on_instruction_button_clicked();

    void on_easy_button_clicked(bool checked);

    void on_medium_button_clicked(bool checked);

    void on_hard_button_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    GameBoard* board;
    size_t difficulty_speed;
    bool difficulty_selection;
};

#endif // MAINWINDOW_H
