#include "doctest.h"
#include <cstring>
#include <cmath>

const double EPS = 1E-5;

void CHECK_DOUBLE(double d1, double d2) {
  CHECK(std::abs(d1 - d1) < EPS);
}

bool operator==(Measurement const& m1, Measurement const& m2) {
  return
    strcmp(m1.getLocation(), m2.getLocation()) == 0 &&
    strcmp(m1.getDate(), m2.getDate()) == 0 &&
    m1.getTemperature() == m2.getTemperature();
}

bool operator!=(Measurement const& m1, Measurement const& m2) {
  return !(m1 == m2);
}

TEST_CASE("Default Measurements have the same values for all attributes") {
  const Measurement m1, m2;
  CHECK(m1 == m2);
}

TEST_CASE("Constructed Measurements set values correctly") {
  const char LOCATION1[] = "Sofia";
  const char DATE1[] = "26/03/19";
  const char LOCATION2[] = "Varna";
  const char DATE2[] = "01/07/18";
  char LOCATION[100] = "";
  strcpy(LOCATION, LOCATION1);
  char DATE[100] = "";
  strcpy(DATE, DATE1);
  const double TEMPERATURE = 20.5;
  const Measurement si(LOCATION, DATE, TEMPERATURE);
  CHECK(strcmp(si.getLocation(), LOCATION1) == 0);
  CHECK(strcmp(si.getDate(), DATE1) == 0);
  CHECK(si.getTemperature() == TEMPERATURE);
  // check if sharing is avoided
  strcpy(LOCATION, LOCATION2);
  strcpy(DATE, DATE2);
  CHECK(strcmp(si.getLocation(), LOCATION1) == 0);
  CHECK(strcmp(si.getDate(), DATE1) == 0);
}

TEST_CASE("Measurement is copied correctly and does not share memory") {
  const char LOCATION[] = "Sofia";
  const char DATE[] = "26/03/19";
  const int TEMPERATURE = 20.5;
  const char LOCATION2[] = "Varna";
  const char DATE2[] = "01/07/18";
  const int TEMPERATURE2 = 26.8;
  const Measurement* m1 = new Measurement(LOCATION, DATE, TEMPERATURE);
  const Measurement m2 = *m1;
  CHECK(*m1 == m2);
  CHECK(m1->getLocation() != m2.getLocation());
  CHECK(m1->getDate() != m2.getDate());
  delete m1;
  m1 = new Measurement(LOCATION2, DATE2, TEMPERATURE2);
  CHECK(strcmp(LOCATION, m2.getLocation()) == 0);
  CHECK(strcmp(DATE, m2.getDate()) == 0);
  CHECK(TEMPERATURE == m2.getTemperature());
  delete m1;
}

TEST_CASE("Measurement is assigned correctly and does not share memory") {
  const char LOCATION[] = "Sofia";
  const char DATE[] = "26/03/19";
  const int TEMPERATURE = 20.5;
  const char LOCATION2[] = "Varna";
  const char DATE2[] = "01/07/18";
  const int TEMPERATURE2 = 26.8;
  const Measurement* m1 = new Measurement(LOCATION, DATE, TEMPERATURE);
  Measurement m2(LOCATION2, DATE2, TEMPERATURE2);
  m2 = *m1;
  CHECK(*m1 == m2);
  CHECK(m1->getLocation() != m2.getLocation());
  CHECK(m1->getDate() != m2.getDate());
  delete m1;
  m1 = new Measurement(LOCATION2, DATE2, TEMPERATURE2);
  CHECK(strcmp(LOCATION, m2.getLocation()) == 0);
  CHECK(strcmp(DATE, m2.getDate()) == 0);
  CHECK(TEMPERATURE == m2.getTemperature());
  delete m1;
}

double testTemperature(int i) { return 0.1 * i + 15; }

Measurement createTestMeasurement(int i, bool unique = true) {
   char LOCATION[] = "XX Test Location";
   char DATE[] = "14/03/XX";
   if (unique)
     DATE[6] = LOCATION[0] = '0' + i / 10;
   DATE[7] = LOCATION[1] = '0' + i % 10;
   const double TEMPERATURE = testTemperature(i);
   return Measurement(LOCATION, DATE, TEMPERATURE);
 }

TEST_CASE("MaxTemperatures can hold 100 unique items") {
  const int MAX = 100;
  MaxTemperatures mt(MAX);
  double checkSum = 0;
  for(int i = 0; i < MAX; i++) {
    mt.addMeasurement(createTestMeasurement(i));
    checkSum += testTemperature(i);
    CHECK_DOUBLE(mt.findAverage(), checkSum / (i + 1));
  }
}

TEST_CASE("MaxTemperatures::addMeasurement selects the higher measurement of two on the same location") {
  const int MAX = 100;
  MaxTemperatures mt(MAX);
  double checkSum = 0;
  for(int i = 0; i < MAX; i++) {
    mt.addMeasurement(createTestMeasurement(i, false));
    checkSum += testTemperature(i);
    if (i >= 10)
        checkSum -= testTemperature(i % 10);
    CHECK_DOUBLE(mt.findAverage(), checkSum / (i + 1));
  }
}

TEST_CASE("MaxTemperatures::highestMeasurement finds highest measurement") {
  const int MAX = 100;
  MaxTemperatures mt(MAX);
  double checkHighest = -1;
  int highestMeasurementHash = -1;
  for(int i = 0; i < MAX; i++) {
    int hash = (31 * i + 79) % 101;
    mt.addMeasurement(createTestMeasurement(hash));
    if (testTemperature(hash) > checkHighest) {
      highestMeasurementHash = hash;
      checkHighest = testTemperature(hash);
    }
    CHECK(mt.highestMeasurement() == createTestMeasurement(highestMeasurementHash));
  }
}

TEST_CASE("MaxTemperatures is copied correctly and does not share memory") {
  const int MAX = 10;
  MaxTemperatures* pl1 = new MaxTemperatures(MAX * 2);
  for(int i = 0; i < MAX; i++)
    pl1->addMeasurement(createTestMeasurement(i));
  MaxTemperatures pl2 = *pl1;
  CHECK(pl1->findAverage() == pl2.findAverage());
  CHECK(pl1->highestMeasurement() ==  pl2.highestMeasurement());
  pl2.addMeasurement(createTestMeasurement(MAX));
  CHECK(pl1->findAverage() != pl2.findAverage());
  CHECK(pl1->highestMeasurement() != pl2.highestMeasurement());
  delete pl1;
  pl1 = new MaxTemperatures(MAX * 2);
  for(int i = 0; i < MAX; i++)
    pl1->addMeasurement(createTestMeasurement(i + MAX));
  CHECK(pl1->findAverage() != pl2.findAverage());
  CHECK(pl1->highestMeasurement() != pl2.highestMeasurement());
  delete pl1;
}

TEST_CASE("MaxTemperatures is assigned correctly and does not share memory") {
  const int MAX = 10;
  MaxTemperatures* pl1 = new MaxTemperatures(MAX * 2);
  for(int i = 0; i < MAX; i++)
    pl1->addMeasurement(createTestMeasurement(i));
  MaxTemperatures pl2(MAX);
  for(int i = 0; i < MAX; i++)
    pl2.addMeasurement(createTestMeasurement(i + MAX));
  pl2 = *pl1;
  CHECK(pl1->findAverage() == pl2.findAverage());
  CHECK(pl1->highestMeasurement() == pl2.highestMeasurement());
  pl2.addMeasurement(createTestMeasurement(MAX));
  CHECK(pl1->findAverage() != pl2.findAverage());
  CHECK(pl1->highestMeasurement() != pl2.highestMeasurement());
  delete pl1;
  pl1 = new MaxTemperatures(MAX * 2);
  for(int i = 0; i < MAX; i++)
    pl1->addMeasurement(createTestMeasurement(i + MAX));
  CHECK(pl1->findAverage() != pl2.findAverage());
  CHECK(pl1->highestMeasurement() != pl2.highestMeasurement());
  delete pl1;
}
