#include "mapscene.hpp"

#include <QPainter>
#include <iostream>


const int scaleFactor = 5;

MapScene::MapImage::MapImage(std::vector<std::vector<int> > _map, QGraphicsItem * parent) : QGraphicsItem(parent)
{
    map = _map;
    width = map.size();
    height = map.at(0).size();
}

QRectF MapScene::MapImage::boundingRect() const
{
    return QRectF(0, 0, width * scaleFactor, height * scaleFactor);
}

void MapScene::MapImage::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    (void) option;
    (void) widget;

    painter->setPen(Qt::black);
    for(size_t i = 0; i < map.size(); i++)
        for(size_t j = 0; j < map.at(i).size(); j++)
        {
            QColor color(50, map.at(i).at(j), 50);
            painter->setBrush(color);
            painter->drawRect(i * scaleFactor, j * scaleFactor, scaleFactor, scaleFactor);
        }
}

//----------------------------------------------------------------

MapScene::BaseStation::BaseStation(int _x, int _y, bool _empty, QGraphicsItem * parent) : QGraphicsItem(parent)
{
    x = _x;
    y = _y;
    isEmpty = _empty;
}

QRectF MapScene::BaseStation::boundingRect() const
{
    return QRectF(x * scaleFactor, y * scaleFactor, scaleFactor, scaleFactor);
}

void MapScene::BaseStation::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    (void) option;
    (void) widget;

    painter->setBrush(isEmpty ? Qt::blue : Qt::red);
    painter->drawRect(x * scaleFactor, y * scaleFactor, scaleFactor, scaleFactor);
}

//----------------------------------------------------------------

MapScene::Mask::Mask(std::set<std::pair<int, int> > _mask, QGraphicsItem * parent) : QGraphicsItem(parent)
{
    mask = _mask;
}

QRectF MapScene::Mask::boundingRect() const
{
    return QRectF(0, 0, 500, 500);
}

void MapScene::Mask::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    (void) option;
    (void) widget;

    painter->setPen(Qt::blue);
    painter->setBrush(QColor(0, 0, 255, 10));
    for(std::set<std::pair<int, int> >::iterator mask_iter = mask.begin(); mask_iter != mask.end(); ++mask_iter)
    {
        painter->drawRect((*mask_iter).first * scaleFactor, (*mask_iter).second * scaleFactor, scaleFactor, scaleFactor);
    }
}

//----------------------------------------------------------------

MapScene::MapScene(QObject * parent) : QGraphicsScene(parent)
{
    img = NULL;
    mask = NULL;
}

void MapScene::fillMap(std::vector<std::vector<int> > map)
{
    clear();
    mask = NULL;
    img = new MapImage(map);
    addItem(img);
}

bool MapScene::addRealStation(int _x, int _y)
{
    if(!img)
        return false;

    if((_x < 0 || _x >= img->width)
        || (_y < 0 || _y >= img->height))
            return false;

    BaseStation * station = new BaseStation(_x, _y);
    addItem(station);
    return true;
}

bool MapScene::addEmptyStation(int _x, int _y)
{
    if(!img)
        return false;

    if((_x < 0 || _x >= img->width)
        || (_y < 0 || _y >= img->height))
            return false;

    BaseStation * station = new BaseStation(_x, _y, true);
    addItem(station);
    return true;
}

void MapScene::addMask(std::set<std::pair<int, int> > _mask)
{
    if(_mask.empty())
        return;
    if(mask)
    {
        removeItem(mask);
        delete mask;
    }
    mask = new Mask(_mask);
    addItem(mask);
}
