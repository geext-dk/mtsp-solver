using Xunit;
using mtsp;
using System.Collections.Generic;

namespace Tests
{
    public class TestSolutionClass
    {
        private readonly Solution solution_;
        private readonly int[] array_;
        private readonly int[] car_path_lengths_;
        private readonly int number_of_cars_;

        // данные для работы
        public TestSolutionClass()
        {
            array_ = new int[] { 2, 3, 1, 6, 5, 4, 7, 10, 8, 9 };
            car_path_lengths_ = new int[] { 2, 4, 3, 1 };
            number_of_cars_ = car_path_lengths_.Length;
            solution_ = new Solution(array_, car_path_lengths_);
        }

        // Проверка Foreach
        [Fact]
        public void TestForeach()
        {
            int i = 0, car = 0;
            foreach (int[] car_path in solution_)
            {
                int car_path_length = 0;
                foreach (int shop in car_path)
                {
                    Assert.Equal(array_[i], shop);
                    ++i;
                    ++car_path_length;
                }
                Assert.Equal(car_path_lengths_[car], car_path_length);
                ++car;
            }
        }

        // Проверка GetCarPathLength
        [Fact]
        public void TestGetCarPathLength()
        {
            for (int i = 0; i < number_of_cars_; ++i)
            {
                Assert.Equal(car_path_lengths_[i], solution_.GetCarPathLength(i));
            }
        }

        // Проверка TestGetNumberOfCars
        [Fact]
        public void TestGetNumberOfCars()
        {
            Assert.Equal(number_of_cars_, solution_.GetNumberOfCars());
        }
    }
}
