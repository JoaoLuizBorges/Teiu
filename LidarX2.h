#ifndef LidarX2_h
#define LidarX2_h

#include <Arduino.h>

class LidarMeasure {
public:
    float angle;
    float distance;
    int sumSilsiM;

    LidarMeasure();
    LidarMeasure(float a, float d, int sum);

    void print();
};

class LidarX2 {
private:
    HardwareSerial& serial;
    bool connected;
    LidarMeasure* measureList;
    int measureCapacity;
    int measureCount;

public:
    LidarX2(HardwareSerial& serial);
    bool open();
    void close();
    void lidarMeasureThread();
    LidarMeasure* readMeasures(int& measureCount, int& measureCapacity);
};

#endif // LidarX2_h
