#include <gmock/gmock.h>
#include <util/BasicFactory.h>
#include <JobMock.h>

using JobFactory = BasicFactory<Job>;

class JobFactoryMock : public JobFactory
{
  public:
  ~JobFactoryMock() {};

  MOCK_METHOD(JobMock*, create, ());
};
