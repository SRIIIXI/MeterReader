#ifndef GRAPH_DATA
#define GRAPH_DATA

#include <QtCore>

class BarData
{
public:
    Q_GADGET

public:
    QString x_;
public:
    float y1_;
public:
    float y2_;

public:
    Q_PROPERTY(QString x MEMBER x_)
    Q_PROPERTY(float y1 MEMBER y1_)
    Q_PROPERTY(float y2 MEMBER y2_)

public:
    ~BarData()
    {

    }

    BarData()
    {
        x_ = "";
        y1_ = 0;
        y2_ = 0;
    }

    BarData(QString  ix, float iy1, float iy2)
    {
        x_ = ix;
        y1_ = iy1;
        y2_ = iy2;
    }

    BarData(const BarData& other)
    {
        x_ = other.x_;
        y1_ = other.y1_;
        y2_ = other.y2_;
    }

    BarData& operator = (const BarData& other)
    {
        x_ = other.x_;
        y1_ = other.y1_;
        y2_ = other.y2_;
        return *this;
    }
};

class LineData
{
public:
    Q_GADGET

public:
    float x1_;
public:
    float y1_;
public:
    float x2_;
public:
    float y2_;

public:
    Q_PROPERTY(float x1 MEMBER x1_)
    Q_PROPERTY(float y1 MEMBER y1_)
    Q_PROPERTY(float x2 MEMBER x2_)
    Q_PROPERTY(float y2 MEMBER y2_)

public:
    ~LineData()
    {

    }

    LineData()
    {
        x1_ = 0;
        y1_ = 0;
        x2_ = 0;
        y2_ = 0;
    }

    LineData(float  ix1, float iy1, float  ix2, float iy2)
    {
        x1_ = ix1;
        y1_ = iy1;
        x2_ = ix2;
        y2_ = iy2;
    }

    LineData(const LineData& other)
    {
        x1_ = other.x1_;
        y1_ = other.y1_;
        x2_ = other.x2_;
        y2_ = other.y2_;
    }

    LineData& operator = (const LineData& other)
    {
        x1_ = other.x1_;
        y1_ = other.y1_;
        x2_ = other.x2_;
        y2_ = other.y2_;
        return *this;
    }
};

#endif
