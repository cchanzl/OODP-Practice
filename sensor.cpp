#include <functional>
#include <string>
#include <iostream>
void ingestEvent(double value, double latitude, double longitude, int timestamp);
void addObserverToSensorAtPosition(std::function<void()> observer, double latitude,
                                   double longitude);

int main() {
  // Example for event ingestions
  ingestEvent(1709.88, 988.456, 3470.3, 1);
  ingestEvent(4856.02, 4687.31, 216.378, 2);
  ingestEvent(183.283, 780.31, 1854.87, 3);
  ingestEvent(1046.07, 3325.91, 2326.46, 4);
  ingestEvent(3768.23, 106.577, 978.304, 5);
  ingestEvent(2834.78, 40.3523, 3343.87, 6);
  ingestEvent(3690.92, 578.758, 2952.94, 7);
  ingestEvent(4051.8, 1484.58, 2824.1, 8);
  std::cout << "Test" << std::endl;
  
  ///////////////////////////////////////////////
  // Q1 b): Your code to add observers goes here: //
  void observer();
  addObserverToSensorAtPosition(observer, 988.456, 3470.3);

  return 0;
}

//////////////////////////////////////////
// Your implementation for Q1 a) goes here //
class Region; // forward declaration
class Sensor;

class Measurement{
private:
  Sensor* sensor = nullptr;
  double value;
  long int timestamp;
public:
  Measurement(){};
  Measurement(Sensor* sensor, double value, long int timestamp)
    : sensor(sensor), value(value), timestamp(timestamp){};

  Measurement* ptr_to_next = nullptr;
  Sensor* getSensor(){return sensor;}
  double getValue(){return value;}
  long int getTimestamp(){return timestamp;}

  void setSensor(Sensor* sensor){this->sensor=sensor;}
  void setValue(double value){this->value=value;}
  void setTimestamp(long int timestamp){this->timestamp=timestamp;}
};


class Sensor{
private:
  Region* region;
  double latitude;   // in degrees
  double longitude;  // in degrees
  Measurement* measurementList[1000]; // sensor may not have measurement
  int count = 0;
  int overwrite_count = 0;
  std::function<void()> observer;
  
public:
  Sensor(){};
  
  Sensor(Region* region, double latitude, double longitude)
    : region(region), latitude(latitude), longitude(longitude){};
  
  bool operator==(Sensor& sensor){
    int threshold = 0.001;
    if ( (this->latitude - sensor.getLatitude()) > threshold ) return false;
    if ( (this-> longitude - sensor.getLongitude()) > threshold ) return false;
    return true;
  }

  Sensor& operator=(const Sensor& sensor2){
    // copy assignment
    return *this;
  }
  
  double getLatitude(){
    return latitude;
  }

  double getLongitude(){
    return longitude;
  }

  void addMeasurement(Measurement& measurement){
    if ( count == 0 ) {
      measurementList[count] = &measurement;
      count++;
      return;
    }

    // if count is more than 0 but less than 1000
    else{
      // go to oldest measurement in list
      int toreplace = 0;
      if ( overwrite_count > 0 ) toreplace = overwrite_count;
      else toreplace = count;

      measurementList[toreplace] = &measurement;
      
      if ( count < 1000 ) {
	count++;
	return;
      }
      if ( overwrite_count == 1000 ) overwrite_count = 0;
      if ( count == 1000 ) overwrite_count++;
    }
  }

  void addObserver(std::function<void()> observer){
    observer = observer;
  }  

  int getAverage(){
    if ( count == 0 ) return 0;
    int sum = 0;
    for ( int i = 0; i < count; i++){
      sum += measurementList[i]->getValue();
    }
    return sum/count;
  }

  bool checkDivergence(){
    if ( count == 0 ) return 0;
    int average =  getAverage();
    for ( int i = 0; i < count; i++){
      if((measurementList[i]->getValue() / average) > 1.2 ) return true;
      if((measurementList[i]->getValue() / average) < 0.8 ) return true;
    }
    return false;
  }
  
  ~Sensor(){
    // to implement delete of linked list
  }
  
}; // end sensor class

class Region{
private:
  std::string region;
  double lowerLeft;
  double upperRight;
  Sensor* sensorList[32]; // region may not have sensor
  int count = 0;
  
public:
  Region(){};
  Region(double lowerLeft, double upperRight)
    : lowerLeft(lowerLeft), upperRight(upperRight){};
  
  void addSensor(Sensor& sensor){
    if ( count == 32 ) {
      std::cout << "A region cannot have more than 32 sensors" << std::endl;
      return;
    }
    sensorList[count] = &sensor;
    count++;
  }

  Sensor getSensor(int index){return *sensorList[index];}

  int findSensor(double latitude, double longitude){
    int threshold = 0.001;

    // add sensor if no sensor in the list
    if ( count == 0 ){
      Sensor* sensor = new Sensor(this, latitude, longitude);
      addSensor(*sensor);
      return count;
    }    

    // search through list
    for ( int i = 0; i < count; i++){
      if ( (latitude - sensorList[i]->getLatitude()) > threshold ) continue;
      if ( (longitude - sensorList[i]->getLongitude()) > threshold ) continue;
      return i;
    }
    
    // add sensor if not found in region
    if ( count <= 32 ){
      Sensor* sensor = new Sensor(this, latitude, longitude);
      addSensor(*sensor);
      return count;
    }

    return -1;
  }
  
  int getCount(){return count;};

  ~Region(){
    delete[] sensorList;
  }
  
};

Region* regionArray[32];

// to find region
Region* findRegion(double latitude, double longitude){
  double lowerLeft = 0;
  double upperRight = 0;
  // find lowerLeft and upperRight based on lat long
  // implement for loop to find region based on lowerLeft and upperRight
  // to add region if not found
  // to initialise region
  for ( int i = 0; i < 32; i++){
    regionArray[i] = new Region();
  };
  int index = 0;
  return regionArray[index];
}


void ingestEvent (double value, double latitude, double longitude, int timestamp){
  Region* region = findRegion(latitude, longitude);
  int sensorIndex = region->findSensor(latitude, longitude);
  if ( sensorIndex > 0 ){
    Sensor sensor = region->getSensor(sensorIndex-1);
    Measurement* newMeasurement = new Measurement(&sensor, value, timestamp);
    sensor.addMeasurement(*newMeasurement);
  } 
}


void addObserverToSensorAtPosition(std::function<void()> observer, double latitude,
                                   double longitude){
  
  Region* region = findRegion(latitude, longitude);
  int sensorIndex = region->findSensor(latitude, longitude);
  int average = 0;
  if ( sensorIndex > 0 ){
    Sensor sensor = region->getSensor(sensorIndex-1);
    sensor.addObserver(observer);
    if ( sensor.checkDivergence() ) observer();
  }


}

void observer(){
  std::cout << "Extraodinary event occurred" << std::endl;
  exit(-1);
}
