#ifndef SNAKEDLG_H
#define SNAKEDLG_H
#include <QDialog>
#include <QImage>
#include <QList>
#include <QLabel>
#include <QTimer>
namespace Ui {
class SnakeDlg;
}
class SnakeDlg : public QDialog
{
Q_OBJECT
public:
explicit SnakeDlg(QWidget *parent = 0);
~SnakeDlg();
private:
void showEvent (QShowEvent*);
void paintEvent (QPaintEvent*);
void keyPressEvent (QKeyEvent* event);
void initGame (void);
QLabel* makeFood (int x = -1, int y = -1);
int rand (int x, int y);
int round (int x, int y);
bool nextStep (QPoint& ptStep) const;
bool validPos (QPoint const& ptPos) const;
void updateScoreboard (void) const;
private slots:
void crawlExpired (void);
private:
Ui::SnakeDlg *ui;
typedef enum tag_Direction {
EDIR_UP,
EDIR_DOWN,
EDIR_LEFT,
EDIR_RIGHT
} EDIR;
QImage m_imgAction;
QList<QLabel*> m_lstSnake;
QLabel* m_labFood;
EDIR m_eDir;
static const int s_nStep = 20;
QTimer m_tmrCrawl;
unsigned int m_uScore;
};
#endif // SNAKEDLG_H
