#ifndef _GRAPHEZONE_
#define _GRAPHEZONE_

#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QWidget>
#include <QLabel>
#include <vector>
#include <iostream>

class grapheZone : public QWidget
{
   Q_OBJECT

   public:

      grapheZone(QWidget *parent = 0);
      QSize minimumSizeHint() const;
      QSize sizeHint() const;

   public slots:

      void setPen(const QPen &pen);
      void setBrush(const QBrush &brush);
      void drawLine(double a, double b, double c, double d);
      void drawPoint(double a, double b);
      void eraseGraph();

   protected:

      void paintEvent(QPaintEvent * );

   private:

      QPen _pen;
      QBrush _brush;
      std::vector<std::pair<double, double> > _lines;
      std::vector<std::pair<double, double> > _points;
};

#endif
