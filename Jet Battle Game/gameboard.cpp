 #  include "gameboard.h"
#include "mainwindow.h"
#include "ui_gameboard.h"
#include <QPushButton>
#include <iostream>

/**@file gameboard.cpp */

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine generator (seed);

/**The default constructor for the GameBoard
 * @param parent is the widget upon which everything will be drawn
 * @param difficulty is how fast the enemy jets will move around
 */
GameBoard::GameBoard(QWidget *parent, size_t difficulty) : QWidget(parent), ui(new Ui::GameBoard), enemy_speed(difficulty){
ui->setupUi(this);

//Creating widget that is on top of the main playing space
Top = new QWidget;
bar = new QHBoxLayout(Top);

//Creating image objects
QString fileName(":/images/jet.gif");
QString fileNameEnemyJet(":/images/enemy_jet.png");
QString fileNameBG(":/images/bg.png");
QString fileNamePM(":/images/player_missile.png");
QString fileNameExplode(":/images/explosion.png");
player_jet_image = new QPixmap(fileName);
enemy_jet_image = new QPixmap(fileNameEnemyJet);
bg_image = new QPixmap(fileNameBG);
player_missile_image = new QPixmap(fileNamePM);
explosion_image = new QPixmap(fileNameExplode);
QMediaPlayer * letsgo = new QMediaPlayer();
letsgo->setMedia(QUrl("qrc:/images/letsgo.wav"));
letsgo->play();

lives_remaining = 4;
lives = new QLabel*[lives_remaining-1];


// Creating lives property with player jet image which is added to the top of the window
for(size_t i=0;i<lives_remaining-1;i++) {
    lives[i] = new QLabel;
    lives[i]->setPixmap(*player_jet_image);
    lives[i]->setMinimumSize(40,40);
    lives[i]->setMaximumSize(40,40);
    lives[i]->setScaledContents(true); //Needed to fit entire image into square
    lives[i]->setStyleSheet("QLabel{border-style:outset; border-width:2px;border-color:blue;}");
    bar->addWidget(lives[i]);
}

//Creating Level label and adds to top portion
level_value = 1;
QLabel* level_text = new QLabel("Level: ");
level_text->setStyleSheet("QLabel{background-color: beige; color:black; font:bold; border-style:outset; border-width:2px;}");
level_value_box = new QLabel(QString::number(level_value));
level_value_box->setStyleSheet("QLabel{background-color: beige; color:black; border-style:outset; border-width:2px;}");
bar->addWidget(level_text);
bar->addWidget(level_value_box);

//Creating Score label and score box to add to top
score = 0;
score_between_levels = 0;
score_text = new QLabel("Score: ");
score_text->setStyleSheet("QLabel{background-color: beige; color:black; font:bold; border-style:outset; border-width:2px;}");
score_box = new QLabel(QString::number(score));
score_box->setStyleSheet("QLabel{background-color: beige; color:black; border-style:outset; border-width:2px;}");
bar->addWidget(score_text);
bar->addWidget(score_box);

Board = new QWidget;
Board->setStyleSheet("border-image: url(:/images/bg.png);" "border-color: purple;");

QGridLayout *RectGrid = new QGridLayout(Board);
RectGrid->setGeometry(QRect());
RectGrid->setSpacing(0);

//Creating game size, adjust to increase/decrease how many possible locations to move
grid_pos = new QLabel*[100];
std::uniform_int_distribution<int> distribution(1,10);

for(size_t i=0;i<10;i++) {
    for(size_t j=0;j<10;j++) {
        // Create grids in game
        grid_pos[i*10+j] = new QLabel;
        grid_pos[i*10+j]->setMinimumSize(80,80);
        grid_pos[i*10+j]->setMaximumSize(80,80);
        //grid_pos[i*10+j]->setStyleSheet("QLabel { background-color : black; color : white; }");
        grid_pos[i*10+j]->setFrameStyle(3);
        grid_pos[i*10+j]->setAlignment(Qt::AlignCenter);

        // Add grids to the layout
        RectGrid -> addWidget(grid_pos[i*10+j] ,i,j);
    }
}

//Generate player location spawn
player_pos = new QPoint(5,5);

//Max player missiles 5
player_missile_pos = new QPoint[5];
player_missile_number = 0;

//Explosion map
explosion_pos = new QPoint[4];

//Generating enemy locations,
enemy_jet_pos = new QPoint[4];
jet_number = 4;

enemy_jet_pos[0].setX(-1);
enemy_jet_pos[0].setY(distribution(generator)%10);
enemy_jet_pos[1].setX(10);
enemy_jet_pos[1].setY(distribution(generator)%10);
enemy_jet_pos[2].setY(-1);
enemy_jet_pos[2].setX(distribution(generator)%10);
enemy_jet_pos[3].setY(10);
enemy_jet_pos[3].setX(distribution(generator)%10);

QTimer *etimer = new QTimer(this);
connect(etimer, SIGNAL(timeout()), this, SLOT(update_enemy_jet()));
etimer->start(enemy_speed);

QTimer *mtimer = new QTimer(this);
connect(mtimer, SIGNAL(timeout()), this, SLOT(update_missile()));
mtimer->start(missile_speed);
Board->setFixedSize(800,800);

//Combining all widgets together onto primary layout.
QVBoxLayout *unify = new QVBoxLayout;
unify->addWidget(Top,0,Qt::AlignCenter);
unify->addWidget(Board,0,Qt::AlignCenter);

//Creating quit button
QPushButton* quit = new QPushButton("Quit");
QObject::connect(quit, SIGNAL(clicked()),this->parent(), SLOT(close()));
unify->addWidget(quit);

this->setLayout(unify);
this->setStyleSheet("QLabel{background-color:black; color:white;}");

}

/**Function draws the player, enemies, and weapons in their respective locations on the board.
Is called for repaint() or update()
@param e is the paint event
*/
void GameBoard::paintEvent(QPaintEvent *e) {
    //Drawing player jet position
    int x = player_pos->rx();
    int y = player_pos->ry();
    grid_pos[y*10+x]->setPixmap(*player_jet_image);
    grid_pos[y*10+x]->setScaledContents(true);

    //Drawing enemy jet locations
    for(size_t i=0;i<jet_number;i++) {
        int px = enemy_jet_pos[i].rx();
        int py = enemy_jet_pos[i].ry();

        if(px >= 0 && py >= 0 && px <10 && py < 10) {
            grid_pos[py*10+px]->setPixmap(*enemy_jet_image);
            grid_pos[py*10+px]->setScaledContents(true);
        }
    }

    //Drawing player missile positions
    for(size_t i=0;i<5;i++) {
        int mx = player_missile_pos[i].rx();
        int my = player_missile_pos[i].ry();

        if(my >= 0) {
            grid_pos[my*10+mx]->setPixmap(*player_missile_image);
            grid_pos[my*10+mx]->setScaledContents(true);
        }
        grid_pos[mx]->clear();
    }

    //Drawing explosion if contact with missile.
    for(size_t i = 0;i<player_missile_number;++i){
        for(size_t j = 0; j<jet_number;++j){
            int px = enemy_jet_pos[j].rx();
            int py = enemy_jet_pos[j].ry();
            int mx = player_missile_pos[i].rx();
            int my = player_missile_pos[i].ry();
            if(px==mx && py==my){
                //Clearing previous explosion location
                grid_pos[explosion_pos[j].ry()*10+explosion_pos[j].rx()]->clear();
                //Storing explosion location (missile+enemy contact)
                explosion_pos[j].setX(px);
                explosion_pos[j].setY(py);
                //Respawning enemy jet
                enemy_jet_pos[j].setY(-1);
                enemy_jet_pos[j].setX(distribution(generator)%10);
                //Updating Score
                update_top_bar();
            }
            grid_pos[explosion_pos[j].ry()*10+explosion_pos[j].rx()]->setPixmap(*explosion_image);
            grid_pos[explosion_pos[j].ry()*10+explosion_pos[j].rx()]->setScaledContents(true);
        }
    }
}

/** Displays the board and actions
 * @param e is the event that activates and is displayed
 */
void GameBoard::showEvent(QShowEvent *e) {
    this->activateWindow();
    this->setFocus();
    QWidget::showEvent(e);
}

/**Function is called whenever a keypress event occurs to register events
@param event is the key that was pressed
*/
void GameBoard::keyPressEvent(QKeyEvent *event){

    int x = player_pos->rx();
    int y = player_pos->ry();

    switch (event->key()) {
    case Qt::Key_Left:
        if(player_pos->rx() != 0)
            move_player(x,y,x-1,y);
        break;

    case Qt::Key_Right:
        if(player_pos->rx() != 9) //0 -> boardsize-1
            move_player(x,y,x+1,y);
        break;

    case Qt::Key_Up:
        if(player_pos->ry() != 0)
            move_player(x,y,x,y-1);
        break;

    case Qt::Key_Down:
        if(player_pos->ry() != 9)//0 -> boardsize-1
            move_player(x,y,x,y+1);
        break;

    case Qt::Key_Space:
        missile = new QMediaPlayer();
        missile->setMedia(QUrl("qrc:/images/missile.mp3"));
        missile->play();
        shoot_weapon();
        break;

    default:
        QWidget::keyPressEvent(event);
    }

    QCoreApplication::processEvents();
    repaint();
}

/**Function is called to move player location
@param px is the previous x location of the player
@param py is the previous y location of the player
@param new_x is the new x location of the player
@param new_y is the new y location of the player
*/
void GameBoard::move_player(int px, int py, int new_x, int new_y){
    grid_pos[py*10+px]->clear();
    player_pos->setX(new_x);
    player_pos->setY(new_y);

}

/**Function spawns a weapon at the location in front of the player*/
void GameBoard::shoot_weapon(){
    if(player_missile_number<6){
        player_missile_pos[player_missile_number].setX(player_pos->rx());
        player_missile_pos[player_missile_number].setY(player_pos->ry()-1);
        player_missile_number++;
    }
    if(player_missile_number==6){
        player_missile_number=0;
    }

    repaint();
    QCoreApplication::processEvents();
};


/**Function is continually called to randomly relocate the enemy jets to adjacent grid locations */
void GameBoard::update_enemy_jet(){
    std::uniform_real_distribution<double> p(0,1);
    std::uniform_int_distribution<int> x_or_y(0,1);
    std::uniform_int_distribution<int> increment(0,1);

    for(size_t i = 0; i<jet_number;++i){
        int px = enemy_jet_pos[i].rx();
        int py = enemy_jet_pos[i].ry();

        if(px>=0 && py>=0 && px<10 && py<10){
            grid_pos[py*10 + px]->clear();
        }

        int nx = px;
        int ny = py;

        if(p(generator) <.5){
            if(x_or_y(generator)==0){
                nx=px+2*increment(generator)-1;
                if(nx>=0 && nx <=10)  //If program crashes for whatever reason, change nx>=0 to nx>=-1
                    enemy_jet_pos[i].setX(nx);
            }
            else{
                ny = py + 2*increment(generator)-1;
                if(ny>=0 && ny<=10) //If program crashes for whatever reason, change ny>=0 to ny>=-1
                    enemy_jet_pos[i].setY(ny);
            }
        }

        //Checking for impact with another enemy jet then remove life
        int x = player_pos->rx();
        int y = player_pos->ry();
        if(px==x && py==y && lives_remaining>1 ){
            lives_remaining--;
            lives[lives_remaining-1]->clear();
            if(lives_remaining==1){//Change to 3 for quick testing
                lives_remaining--;
                game_over();
                break;
            }
            QMediaPlayer * damaged = new QMediaPlayer();
            damaged->setMedia(QUrl("qrc:/images/damagetaken.wav"));
            damaged->play();
        }
    }
    repaint();
    QCoreApplication::processEvents();
};

/**Function moves the player's missile forward(up) the board/grid*/
void GameBoard::update_missile(){
    for(size_t i = 0; i<player_missile_number;++i){
            int x = player_missile_pos[i].rx();
            int y = player_missile_pos[i].ry();
            if(y>=1){
                player_missile_pos[i].setY(y-1);
            }
            grid_pos[y*10+x]->clear();
        }

        repaint();
        QCoreApplication::processEvents();
}

/**Function updates the data located on the top bar: score and level*/
void GameBoard::update_top_bar(){
    //If certain points are achieved at a level, increase level
    if (score_between_levels >=min_to_level_up){
        level_value_box->clear();
        level_value++;
        level_value_box->setText(QString::number(level_value));
        bar->addWidget(level_value_box);
        score_between_levels = 0;
        enemy_speed-=50; //Speed up enemies
        QMediaPlayer * levelup = new QMediaPlayer();
        levelup->setMedia(QUrl("qrc:/images/levelup.wav"));
        levelup->play();
    }

    bar->addWidget(score_text);

    score_box ->clear();
    score+=1000;
    score_between_levels +=1000;
    score_box->setText(QString::number(score));
    bar->addWidget(score_box);
}

/**Function removes content from main window and displays a new game over window with final score and level reached*/
void GameBoard::game_over(){
    QMediaPlayer * loser = new QMediaPlayer();
    loser->setMedia(QUrl("qrc:/images/gameover.mp3"));
    loser->play();
    QWidget *game_over = new QWidget;
    game_over->setWindowTitle("New"); //Used to call connect function later
    QVBoxLayout *vlay = new QVBoxLayout(game_over);
    QLabel* score_box_quit = new QLabel("Your final score was: ");
    QLabel* level_box_quit = new QLabel("You reached level: ");

    score_box->setText(QString::number(score));
    level_value_box->setText(QString::number(level_value));

    QPushButton* quit = new QPushButton("Continue");
    QObject::connect(quit, SIGNAL(clicked()),game_over, SLOT(close()));
    QObject::connect(quit, SIGNAL(clicked()),this->parent(), SLOT(new_game()));
    //QObject::connect(game_over,SIGNAL(windowTitleChanged(QString)),this->parent(), SLOT(game_over())); //Only works in newer Qt Version
    game_over->setWindowTitle("Game Over");
    QObject::connect(game_over,SIGNAL(objectNameChanged(QString)),this->parent(),SLOT(game_over()));
    game_over->setObjectName("game_over2"); //Used to activate previous connect function

    vlay->addWidget(score_box_quit);
    vlay->addWidget(score_box);
    vlay->addWidget(level_box_quit);
    vlay->addWidget(level_value_box);
    vlay->addWidget(quit);
    game_over->setFixedSize(300,250);
    game_over->show();
}

/**Destructor to manage memory*/
GameBoard::~GameBoard(){
    delete ui;
}

