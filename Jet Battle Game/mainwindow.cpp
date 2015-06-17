#include "mainwindow.h"
#include "gameboard.h"
#include "ui_mainwindow.h"

/**@file mainwindow.cpp*/
/**Function creates introductory display window
 * @param parent is what to display widgets on
 */
MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent), ui(new Ui::MainWindow)
{
   QMediaPlayer * bgmusic = new QMediaPlayer();
   bgmusic->setMedia(QUrl("qrc:/images/bgmusic.mp3"));
   bgmusic->play();
    ui->setupUi(this);
}


/**Function is called to close the game and clean up the widget*/
void MainWindow::game_over() {
    // Take the current board game out of the central widget of MainWindow
    QWidget* wid = this->centralWidget();
    wid->setParent(nullptr);
    // Line above will delete the widget gracefully (no memory errors)

}

/**Function creates gameboard*/
void MainWindow::new_game(){
    ui->setupUi(this);
}

/**Destructor to manage heap memory*/
MainWindow::~MainWindow()
{
    delete ui;
}

/**Function creates gameboard*/
void MainWindow::on_start_button_clicked()
{
    this->setStyleSheet("border-image:transparent;");
    if(difficulty_selection==true)
        board = new GameBoard(this,difficulty_speed);
    else
        board = new GameBoard(this,250);
    this->setCentralWidget(board);
}

/**Function called upon clicking instruction button and displays text instructions for gameplay*/
void MainWindow::on_instruction_button_clicked()
{
    QWidget *instructions = new QWidget;
   instructions->setWindowTitle("Instructions");
    QVBoxLayout *vlay = new QVBoxLayout(instructions);
    QLabel* arrows = new QLabel("Use the arrow keys to move your player");
    QLabel* space = new QLabel("Use spacebar to shoot missiles");
    QLabel* music = new QLabel("PLEASE Be sure to turn on your speakers for music and notifications");
    QLabel* score = new QLabel("Every 10,000 points increases level and difficulty/enemy speed");
    QLabel* life = new QLabel("Touching an enemy jet will cause you to lose a life(3)");

    QPushButton* close_instructions = new QPushButton("Continue");
    QObject::connect(close_instructions, SIGNAL(clicked()),instructions, SLOT(close()));

    vlay->addWidget(arrows);
    vlay->addWidget(space);
    vlay->addWidget(music);
    vlay->addWidget(score);
    vlay->addWidget(life);
    vlay->addWidget(close_instructions);
    instructions->show();
}

/**Function sets difficulty/enemy jet speed for the easy mode
 * @param checked is whether or not the radio button is checked
 */
void MainWindow::on_easy_button_clicked(bool checked)
{
    if(checked) {
        difficulty_speed = 350;
        difficulty_selection = true;
    }
}

/**Function sets difficulty/enemy jet speed for the medium mode
 * @param checked is whether or not the radio button is checked
 */
void MainWindow::on_medium_button_clicked(bool checked)
{
   if (checked) {
       difficulty_selection = true;
       difficulty_speed = 250;
    }
}

/**Function sets difficulty/enemy jet speed for the hard mode
 * @param checked is whether or not the radio button is checked
 */
void MainWindow::on_hard_button_clicked(bool checked)
{
    if(checked){
        difficulty_speed = 150;
       difficulty_selection = true;
    }
}
