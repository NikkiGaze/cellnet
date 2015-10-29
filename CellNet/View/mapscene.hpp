#ifndef MAPSCENE_HPP
#define MAPSCENE_HPP

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <set>

class MapScene : public QGraphicsScene
{
    class MapImage : public QGraphicsItem
    {
    public:
        std::vector<std::vector<int> > map;
        int width;
        int height;

        MapImage(std::vector<std::vector<int> > _map, QGraphicsItem * parent = NULL);
        virtual QRectF boundingRect() const;
        virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    };

    class BaseStation : public QGraphicsItem
    {
    public:
        int x;
        int y;
        bool isEmpty;

        BaseStation(int _x, int _y, bool _empty = false, QGraphicsItem * parent = NULL);
        virtual QRectF boundingRect() const;
        virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    };

    class Mask : public QGraphicsItem
    {
    public:
        std::set<std::pair<int, int> > mask;

        Mask(std::set<std::pair<int, int> > _mask, QGraphicsItem * parent = NULL);
        virtual QRectF boundingRect() const;
        virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    };


    MapImage * img;
    Mask * mask;

public:
    MapScene(QObject *parent = NULL);
    void fillMap(std::vector<std::vector<int> > map);
    bool addRealStation(int _x, int _y);
    bool addEmptyStation(int _x, int _y);
    void addMask(std::set<std::pair<int, int> > _mask);
};

#endif // MAPSCENE_HPP
