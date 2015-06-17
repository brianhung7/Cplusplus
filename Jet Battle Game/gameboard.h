#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QLabel>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <random>
#include <chrono>
#include <QTimer>
#include <QMediaPlayer>

namespace Ui {
class GameBoard;
}

/**
 * @class GameBoard
 * @brief Creates gameboard upon which the entire game is played and presented
 * Class is used to create the primary gameboard which includes creating and managing the player, enemies,
 * scoring, and drawing all of the aformentioned. Score and level is kept until the player runs out of lives
 * after which the class object is cleaned up.
 * @author Brian Pham
 * @date June 03,2015
 *
 */
class GameBoard: public QWidget
{

    Q_OBJECT

public slots:
    void update_enemy_jet();
    void update_missile();

public:
   explicit GameBoard(QWidget *parent = 0, size_t difficulty = 350);
    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *e);
    void showEvent(QShowEvent *e);

    void move_player(int px, int py, int new_x, int new_y);
    void shoot_weapon();
    void game_over();

    void update_top_bar();

    ~GameBoard();

private:
    Ui::GameBoard *ui;
    std::uniform_int_distribution<int> distribution;
    QPoint *player_pos;
    QPoint *enemy_jet_pos;
    QPoint *explosion_pos;

    QPoint *player_missile_pos;

    QWidget *Board;
    QWidget* Top;
    QHBoxLayout *bar;

    unsigned int lives_remaining;
    QLabel** lives;
    QLabel* level_value_box;
    QLabel* score_box;
    QLabel* score_text;

    QPixmap* player_jet_image;
    QPixmap* enemy_jet_image;
    QPixmap* bg_image;
    QPixmap* player_missile_image;
    QPixmap* explosion_image;
    QMediaPlayer * missile;

    QLabel** grid_pos;

   size_t enemy_speed; //Lower is faster
   static const size_t missile_speed = 100; //Lower is faster
   size_t jet_number;
   size_t player_missile_number;
   static const int min_to_level_up = 9000; //Lower to increase levels sooner
   int level_value;
   int score;
   int score_between_levels;


};

#endif // GAMEBOARD_H
