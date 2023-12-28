#include "LidarX2.h"

LidarMeasure::LidarMeasure() : angle(0), distance(0), sumSilsiM(0) {}

LidarMeasure::LidarMeasure(float a, float d, int sumSilsiM) : angle(a), distance(d), sumSilsiM(sumSilsiM) {}


//LidarMeasure::LidarMeasure(float a, float d,int sumSilsiM) {
//    angle = a;
//    distance = d;
//    this->sumSilsiM = sumSilsiM;   
//}

void LidarMeasure::print() {
    Serial.print("Angle: ");
    Serial.print(angle);
    Serial.print(", Distance: ");
    Serial.println(distance);
}

LidarX2::LidarX2(HardwareSerial& serial) : serial(serial) {
    connected = false;
    measureList = nullptr;
    measureCapacity = 100;
    measureCount = 0;
}

bool LidarX2::open() {
    if (!connected) {
        serial.begin(115200);
        connected = true;
        return true;
    }
    return false;
}

void LidarX2::close() {
    if (connected) {
        serial.end();
        connected = false;
    }
}

void LidarX2::lidarMeasureThread() {
    float startAngle = 0;

    while (true) {

        LidarMeasure* measures = readMeasures(measureCount, measureCapacity);
        if (!measures) {
            continue;
        }
        float endAngle = measures[measureCount - 1].angle;
        int i = 0;
        while (i < measureCount) {
            LidarMeasure m = measures[i];
            bool inRange = false;
            if (endAngle > startAngle) {
                inRange = startAngle <= m.angle && m.angle <= endAngle;
            } else {
                inRange = (startAngle <= m.angle && m.angle <= 360) || (0 <= m.angle && m.angle <= endAngle);
            }
            if (inRange) {
                for (int j = i; j < measureCount - 1; ++j) {
                    measures[j] = measures[j + 1];
                }
                measureCount--;
                i--;
            }
            i++;
        }
        for (int i = 0; i < measureCount; ++i) {
            measures[i].print();
        }
        startAngle = endAngle;
        delay(10);
        delete[] measures;
        
    }
}

LidarMeasure* LidarX2::readMeasures(int& measureCount, int& measureCapacity) {
  
    LidarMeasure* result = new LidarMeasure[measureCapacity];

    if (!connected) {
        return result;
    }

    bool found = false;
    uint16_t checksum = 0x55AA;
    
    while (!found) {
 
        while (serial.available() > 0 && (serial.read()) != 0xAA){
        }
        if (serial.available() > 0 && (serial.read()) == 0x55) {
            found = true;
        }
    }

    int ct = serial.available() > 0 ? serial.read() : 0;
    if (ct != 0) {
        return result;
    }

    int ls = serial.available() > 0 ? serial.read() : 0;
    int sampleCount = ls;
    if (sampleCount == 0) {
        return result;
    }

    int fsaL = serial.available() > 0 ? serial.read() : 0;
    int fsaM = serial.available() > 0 ? serial.read() : 0;
    int fsa = fsaL + fsaM * 256;
    
    checksum ^= fsa;
    float startAngle = static_cast<float>(fsa >> 1) / 64;
    int lsaL = serial.available() > 0 ? serial.read() : 0;
    int lsaM = serial.available() > 0 ? serial.read() : 0;
    int lsa = lsaL + lsaM * 256;
    
    float endAngle = static_cast<float>(lsa >> 1) / 64;
    float aDiff = endAngle - startAngle;
    
    if (aDiff < 0) {
        aDiff += 360;
    }

    int csL = serial.available() > 0 ? serial.read() : 0;
    int csM = serial.available() > 0 ? serial.read() : 0;
    uint16_t cs = csL + csM * 256;

    uint16_t checksumComputed = 0;

    byte* dataRaw = new byte[sampleCount * 2];
    for (int i = 0; i < sampleCount * 2; ++i) {
        dataRaw[i] = serial.available() > 0 ? serial.read() : 0;
    }
    
    uint16_t* data = reinterpret_cast<uint16_t*>(dataRaw);
    
    for (int i = 0; i < sampleCount * 2; i += 2) {
        int siL = data[i];
        int siM = data[i + 1];
        int sumSilsiM = siL + siM;
        
        checksumComputed ^= (siL + siM * 256);
        float distance = static_cast<float>(siL + siM) / 9.65;
        float angle = startAngle + (aDiff / sampleCount) * i / 2;
        float angleCorrection = 0;
        
        if (distance > 0) {
            angleCorrection = (atan(21.8 * ((155.3 - distance) / (155.3 * distance))) * (180 / PI));
        }
        angle += angleCorrection;
        if (angle > 360) {
            angle -= 360;
        }
        result[i] = LidarMeasure(angle,distance, sumSilsiM);// Insira seu código para processar a medida aqu
    }

    return result;

    checksumComputed ^= (ct + ls * 256);
    checksumComputed ^= lsa;
    if (checksumComputed == cs) {
        return result;
    }
    delete[] result;
    return nullptr;
}
