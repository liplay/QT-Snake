#include <QPainter>
#include <QKeyEvent>
#include <QMessageBox>
#include "SnakeDlg.h"
#include "ui_SnakeDlg.h"
SnakeDlg::SnakeDlg(QWidget *parent) :QDialog(parent),ui(new Ui::SnakeDlg),
m_imgAction (":/Res/Action.gif")
{
ui->setupUi(this);
connect (&m_tmrCrawl, SIGNAL (timeout (void)), this, SLOT (crawlExpired (void)));
}
SnakeDlg::~SnakeDlg()
{
delete ui;
}
// 显示事件处理
void SnakeDlg::showEvent (QShowEvent*) {
initGame ();
m_labFood = makeFood ();
m_tmrCrawl.start (105);
}
// 绘制事件处理
void SnakeDlg::paintEvent (QPaintEvent*) {
if (! m_imgAction.isNull ()) {
QPainter painter (this);
QRect rcAction = ui->m_frmAction->frameRect ();
rcAction.translate (ui->m_frmAction->pos ());
painter.setClipRect (rcAction);
for (int x = rcAction.left ();x < rcAction.right ();x += m_imgAction.width ())
for (int y = rcAction.top ();y < rcAction.bottom ();y += m_imgAction.height ())
painter.drawImage (x, y,m_imgAction);
}
}
// 键盘事件处理
void SnakeDlg::keyPressEvent (QKeyEvent* event) {
switch (event->key ()) {
case Qt::Key_Up:
case Qt::Key_W:
if (m_eDir == EDIR_LEFT || m_eDir == EDIR_RIGHT)
m_eDir = EDIR_UP;
break;
case Qt::Key_Down:
case Qt::Key_S:
if (m_eDir == EDIR_LEFT || m_eDir == EDIR_RIGHT)
m_eDir = EDIR_DOWN;
break;
case Qt::Key_Left:
case Qt::Key_A:
if (m_eDir == EDIR_UP || m_eDir == EDIR_DOWN)
m_eDir = EDIR_LEFT;
break;
case Qt::Key_Right:
case Qt::Key_D:
if (m_eDir == EDIR_UP || m_eDir == EDIR_DOWN)
m_eDir = EDIR_RIGHT;
break;
}
}
// 初始化游戏
void SnakeDlg::initGame (void) {
foreach (QLabel* labNode, m_lstSnake)
delete labNode;
m_lstSnake.clear ();
for (int i = 0; i < 3; ++i)
m_lstSnake.push_back (makeFood (s_nStep, 0));
m_eDir = EDIR_DOWN;
m_uScore = 0;
updateScoreboard ();
}
// 制造食物
QLabel* SnakeDlg::makeFood (int x /* = -1 */, int y /* = -1 */) {
QPoint ptFood (x, y);
if (x == -1 && y == -1) {
QRect rcAction =ui->m_frmAction->frameRect ();
ptFood.setX (round (rand (rcAction.left (),rcAction.right () - s_nStep),s_nStep));
ptFood.setY (round (rand (rcAction.top (),rcAction.bottom () - s_nStep),s_nStep));
    //若位置无效
    if (! validPos (ptFood))
    //递归，重新生成
    return makeFood ();
}
QLabel* labFood = new QLabel (ui->m_frmAction);
labFood->setFrameStyle (QFrame::Panel);
labFood->setFrameShadow (QFrame::Raised);
labFood->setAutoFillBackground (true);
labFood->setPalette (QPalette (QColor (rand (0, 255),rand (0, 255),rand (0, 255))));
labFood->resize (s_nStep, s_nStep);
labFood->move (ptFood);
labFood->show ();
return labFood;
}
// 生成给定值域[x,y]内的伪随机数
int SnakeDlg::rand (int x, int y) {
    //x <=r <= y;
    //0 <= r-x <= y=x;
    return qrand () % (y - x + 1) + x;
}
// 对齐圆整
int SnakeDlg::round (int x, int y) {
return x / y * y;
}
// 计算下一步
bool SnakeDlg::nextStep (QPoint& ptStep) const {
ptStep = m_lstSnake.back ()->pos ();
switch (m_eDir) {
case EDIR_UP:
ptStep -= QPoint (0, s_nStep);
break;
case EDIR_DOWN:
ptStep += QPoint (0, s_nStep);
break;
case EDIR_LEFT:
ptStep -= QPoint (s_nStep, 0);
break;
case EDIR_RIGHT:
ptStep += QPoint (s_nStep, 0);
break;
}
return validPos (ptStep);
}
// 判断有效位置
bool SnakeDlg::validPos (QPoint const& ptPos) const {
QRect rcAction =ui->m_frmAction->frameRect ();
if (! rcAction.contains (ptPos))
return false;
foreach (QLabel* labNode, m_lstSnake)
if (labNode->pos () == ptPos)
return false;
return true;
}
// 更新记分板
void SnakeDlg::updateScoreboard (void) const {
ui->m_labScoreboard->setText (tr ("得分：") + QString::number (m_uScore));
}
// 爬行定时器到期信号处理
void SnakeDlg::crawlExpired (void) {
    QPoint ptStep;
    //若下一步是有效位置...
    if (nextStep (ptStep))
        //若下一步是食物
        if (ptStep == m_labFood->pos ()) {
            //生长
            m_lstSnake.push_back (m_labFood);
            m_labFood = makeFood ();
            //加分
            m_uScore += 100;
            updateScoreboard ();
        }
    else { //否则...
        //爬到下一步
        QLabel* labPrev = NULL;
        foreach (QLabel* labNext,m_lstSnake) {
        if (labPrev)
        labPrev->move (labNext->pos ());
        labPrev = labNext;
        }
        labPrev->move (ptStep);
    }
else {
QMessageBox msgBox (QMessageBox::Question,windowTitle (),tr ("蛇已死，继续否？"),
QMessageBox::Yes | QMessageBox::No, this);
if (msgBox.exec () ==QMessageBox::Yes) {
initGame ();
delete m_labFood;
m_labFood = makeFood ();
}
else
close ();
}
}
